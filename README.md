# OpenSimpleLidar
Open Hardware scanning laser rangefinder.  
Its parameters:  
* 5 scans/second
* 180 measurements per rotation (2 deg anglular resolution)
* Maximum distance - 4 m
* Measured accuracy ~3-5 cm at 3 m (depends upon the color of reflecting surface).

This device is really cheap - its components cost less than $30.  
See "TotalBOM" with price calculations.  
  
Now I'm updating the documentation for this LIDAR.  
Some of it I'm writing to the project wiki: https://github.com/iliasam/OpenSimpleLidar/wiki  

[Please take the simple survey about assembling instructions](https://docs.google.com/forms/d/127PN3ydydF8poReQAPeC-vXDqIst-CIAj8u08tnh1lM)  
  
![Alt text](Misc/Photo_of_LIDAR1.jpg?raw=true "Image")
  
Utility for displaying "radar" scans from the working Lidar:  
![Alt text](/PC_utility/LidarScanningTest/scanning_screenshot.png?raw=true "Image")  

Example of Hector SLAM working with this Lidar:  
![Alt text](Misc/hector_slam_map_test1.png?raw=true "Image")

This Lidar is using **triangulation method** for measuring distance to objects. It can be shown by this picture:  
![Alt text](Misc/optics1.png?raw=true "Image")

Some modeled pictures of the assembled Lidar can be found here: https://github.com/iliasam/OpenSimpleLidar/tree/master/Misc/Pictures/  

Some more info: https://hackaday.io/project/20628-opensimplelidar  

Video of using OpenSimpleLidar for Autonomous Robot Navigation:  
https://www.youtube.com/watch?v=YHt_NTktmlA

See also my previous LIDAR project: https://github.com/iliasam/OpenLIDAR
If you need LIDAR with increased speed and range, see this project: https://github.com/iliasam/OpenTOFLidar  

Please leave your feedback if you will be able to build this Lidar - I need more feedback about assembing instructions.
