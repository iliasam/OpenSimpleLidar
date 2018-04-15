/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2011, Eric Perko, Chad Rockey
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Case Western Reserve University nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/
//Modifided by ILIASAM 2018

#include <lidar_parser.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <ros/ros.h>

//total packet length (2+1+360)*2 bytes - 728
//2 words - header + 1 word status + 1 word - speed

//0 bit - overspeed flag
#define STATUS_WORD_OVERSPEED_MASK     (1<<0)

//15 bit - shows that this is lidar 4
#define STATUS_WORD_LIDAR4_MASK        (1<<15)

//Degrees
#define LIDAR3_ANG_OFFSET        		14

namespace tr_lidar_driver
{
  LidarParser::LidarParser(const std::string& port, uint32_t baud_rate, uint32_t firmware, boost::asio::io_service& io): 
  port_(port),
  baud_rate_(baud_rate),
  firmware_(firmware),
  shutting_down_(false),
  serial_(io, port_) {serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate_));}

  void LidarParser::poll(sensor_msgs::LaserScan::Ptr scan) {
    uint8_t temp_char;
    uint8_t start_count = 0;
    uint16_t pix = 0;
    double dist = 0;
    bool got_scan = false;
    int index;
    
    double dist_scan[360];
    uint16_t offset1;
    int16_t offset2;
    int16_t ang_corr1;
    int16_t ang_corr2;
    int bad_data_flag = 0;
    
    uint16_t i;

    uint8_t byte0;
    uint8_t byte1;

    uint16_t scan_time = 0;
    uint16_t status_word = 0;

    double max_distance = 5.0;//meters
    int16_t static_angular_offset = LIDAR3_ANG_OFFSET;//deg

    boost::array<uint8_t, 724> raw_bytes;
    boost::array<uint8_t, 4> raw_bytes2;
    
    while (!shutting_down_ && !got_scan) 
    {
        // Wait until the start sequence 0xAA, 0xBB,0xCC,0xDD comes around
        boost::asio::read(serial_, boost::asio::buffer(&temp_char,1));
    
        if(start_count == 0) 
        {
            if(temp_char == 0xAA) {start_count = 1;} else {start_count = 0;}
        } 
        else if(start_count == 1) 
        {
            if(temp_char == 0xBB) {start_count = 2;} else {start_count = 0;}
        } 
        else if(start_count == 2)
        {
            if(temp_char == 0xCC) {start_count = 3;} else {start_count = 0;}
        } 
        else if(start_count == 3) 
        {
            if(temp_char == 0xDD) 
            {
                start_count = 0;
                // Now that entire start sequence has been found, read in the rest of the message
                got_scan = true;
                // 728 - 4header = 724bytes
                //724 - 4info = 720bytes (360*2 = 720)
                boost::asio::read(serial_,boost::asio::buffer(&raw_bytes2,4));
                boost::asio::read(serial_,boost::asio::buffer(&raw_bytes,720));

                status_word = (uint16_t)raw_bytes2[0] + ((uint16_t)raw_bytes2[1] << 8);
                scan_time = (uint16_t)raw_bytes2[2] + ((uint16_t)raw_bytes2[3] << 8);

                if ((status_word & STATUS_WORD_LIDAR4_MASK) != 0)
                {
                    //Lidar 4 mode
                    max_distance = 4.0;
                    static_angular_offset = 0;
                }
                else
                {
                    max_distance = 5.0;
                    static_angular_offset = LIDAR3_ANG_OFFSET;
                }

                scan->angle_min = 0.0;
                scan->angle_max = 2.0*M_PI;
                scan->angle_increment = (2.0*M_PI/360.0);
                //scan->time_increment = 1.0/(360.0*3.0);//1 sec
                scan->time_increment = scan_time*0.001/360.0;
                scan->scan_time = scan_time*0.001;//seconds
                scan->range_min = 0.16;
                scan->range_max = max_distance;
                scan->ranges.reserve(360);
                scan->intensities.reserve(360);
                bad_data_flag = 0;
	    

                for(i = 0; i < (360*2); i=i+2)
                {
                  offset1 = i/2; //(0-359)
                  // Two bytes per reading
                  byte0 = raw_bytes[i];
                  byte1 = raw_bytes[i+1];
                  pix = (uint16_t)byte0 + ((uint16_t)byte1 << 8);

                  if ((pix < 20)) {pix = 0;}
                  if (pix > 16383) {pix = 0;}
                  //pix = pix & 16383;
                  
                  //dist = (-0.055) / (tan(((double)pix)*0.00004993 - 0.445)); //lidar3  - OpenLidar
                  dist = base_coef / (tan(((double)pix)*a_coef - b_coef));
                  
                  if ((dist > 0.16) && (dist < max_distance))
                  {
                    double h_base = fabs(base_coef) / 2;
                    ang_corr1 = (int16_t)(atan(h_base/dist) * 180.0 / M_PI);//Angular correction
                    ang_corr2 = (int16_t)(angular_corr * (double)offset1);
                    offset2 = (int16_t)offset1 + ang_corr1 + ang_corr2 + static_angular_offset;
                    if (offset2 > 359) 
                        offset2 = offset2 - 360;
                    if (offset2 < 0) 
                        offset2 = 360 + offset2;
                    dist_scan[(uint16_t)offset2] = dist;  
                  }
                  else
                  {
                    //dist = 0.0;
                    //offset2 = offset1;
                  }
                }

                //dist_scan[359] = 0;
                dist_scan[0] = 0;
                dist_scan[1] = 0;

                if (bad_data_flag == 0)
                {
                    for (i = 0; i < 360; i++)
                    {
                        dist = dist_scan[i];
                        scan->ranges.push_back(dist);
                        scan->intensities.push_back(0);

                        dist_scan[i] = 0;
                        raw_bytes[i*2] = 0;
                        raw_bytes[i*2+1] = 0;
                    }
                }
                else
                {
                    ROS_ERROR("Bad data from LIDAR");
                    for (i = 0; i < 360; i++)
                    {
                        dist_scan[i] = 0;
                        raw_bytes[i*2] = 0;
                        raw_bytes[i*2+1] = 0;
                    }
                }//end of if (bad_data_flag == 0)

            }//end of if(temp_char == 0xDD)
            else 
                start_count = 0;
            
        }//end of if(start_count == 3)
        else 
            start_count = 0;

    }//end while
      
  }//end of LidarParser::poll
};
