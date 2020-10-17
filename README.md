# utility_ros

???

## DISCRIPTION

A utility library for ROS

- general_util: general utilities about type transform, etc

- image_util: utilities about image

- geometry_util: utilities about geometry
    - Pose2d
    - Transform2d

- probability_util: utilities about probability distributions
    - NormalDistribution1d
    - NormalDistribution2d

- Timer: timer class for measuring the duration of processes
    - ChronoTimer
    - RosTimer

- PlotLib
    - CVPlotLib
    - MatPlotLib

## USAGE

### Eigen
- `sudo apt-get install libeigen3-dev` in terminal
- `include_directories("/usr/include/eigen3")` in CMakeLists.txt
- `#include <Eigen/Dense>` in .cpp 

### matplotlib-cpp
- `git clone https://github.com/lava/matplotlib-cpp.git` in anywhere
- copy "matplotlibcpp.h" to the "include/utility_ros/matplotlib-cpp/" directory
- `#include <utility_ros/matplotlib-cpp/matplotlibcpp.h>` in .cpp

### utility_ros

- git clone `https://github.com/Harunancha/utility_ros.git` in your_ws/src
- catkin_make
- `#include "utility_ros/***.hpp"` in your .cpp file
- `find_package (utility_ros)` in your CMakeLists.txt
- `<depend>utility_ros</depend>` in your package.xml

## TODO

- general_util: 
    * transform Mat type among Eigen, cv, and tf
- image_util: 
    * IoU calculation
- geometry_util: 
    * type transform between Pose2d Transform2d and tf (3d)
- probability_util: 
    * kalman filtering for nd1d
    * kalman filtering for nd2d
- marker_util: 
    * get transform between 2 agents in different viewpoint
- tracking_util: 
    * tracking algorithm combining hungarian algorithm and kalman filtering
- PlotLib:
    * plot library using matplotlib-cpp
    * plot library using opencv
- MovingDot: 
    * moving sophisticately
    * limit velocity and acceleration
- Timer: complete

## REFERENCES

- How to use Git with VScode: https://qiita.com/y-tsutsu/items/2ba96b16b220fb5913be
- How to include a Cpp header from another package: https://roboticsbackend.com/ros-include-cpp-header-from-another-package/