using System;

//UINT16_T DATA

namespace LidarScanningTest1
{
	public class UartPacketsParser
	{
		const int BUFFER_LENGTH = 400;
		const int TOTAL_POINTS  = 362;
		int header_counter = 0;
		int packet_bytes_cnt = 0;
		byte prev_byte  = 0;
		public int total_packets = 0;
		
		//number of buffers
		int cur_write_buf = 0;
		int cur_read_buf = 1;
		
		public bool data_pending = false;//new data available
		
		int[] rx_data0 = new int[BUFFER_LENGTH];
		int[] rx_data1 = new int[BUFFER_LENGTH];
		
		public UartPacketsParser()
		{
		}
		
		public bool process_received_data(byte[] data, int length)
		{
			int i;
			byte cur_byte = 0;
			int cur_word = 0;
			bool new_packet = false;
			
			for (i=0;i<length;i++)
			{
				cur_byte = data[i];
				
				if (header_counter == 4)//first byte of new packet
				{
					//System.Diagnostics.Debug.WriteLine("leng: " + packet_bytes_cnt.ToString());
					packet_bytes_cnt = 0;
					header_counter = 0;
					total_packets++;
				}
				else
				{
					switch (cur_byte)
					{
						case 170:  {header_counter = 1; break;}//65 - start
						case 187:  {header_counter = (header_counter == 1)? header_counter+1:  0; break;}
						case 204:  {header_counter = (header_counter == 2)? header_counter+1:  0; break;}
						case 221:  {header_counter = (header_counter == 3)? header_counter+1:  0; break;}
					}
					packet_bytes_cnt++;
				}
				
				if ((packet_bytes_cnt % 2) == 1)//second byte of word received
				{
					int word_position = packet_bytes_cnt/2;
					cur_word = ((int)cur_byte*256) + (int)prev_byte;//new word found
					//System.Diagnostics.Debug.WriteLine("word: " + cur_word.ToString());
					if (word_position < TOTAL_POINTS) 
					{
						if (cur_write_buf == 0) 
							rx_data0[word_position] = cur_word;
						else 
							rx_data1[word_position] = cur_word;
					}
				}
				
				if (packet_bytes_cnt == (TOTAL_POINTS*2))
				{
					new_packet = true;
					data_pending = true;
					cur_read_buf^=1;
					cur_write_buf^=1;
				}
					
				
				prev_byte = cur_byte;
			}//end of for
			return new_packet;
		}//end of "process_received_data"
		
		public int[] get_received_data()
		{
			int[] return_aray = new int[TOTAL_POINTS];
			
			if (cur_read_buf == 0)
				Array.Copy(rx_data0, return_aray, TOTAL_POINTS);
			else
				Array.Copy(rx_data1, return_aray, TOTAL_POINTS);
			
			data_pending = false;			
			return return_aray;
		}
	
		
	}//end of class
}
