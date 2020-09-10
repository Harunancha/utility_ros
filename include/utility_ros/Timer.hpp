/*
TODO:    
*/

/* references
- how to use chrono: http://www.sanko-shoko.net/note.php?id=rnfd
*/
#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <chrono>

class ChronoTimer
{
private:
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point last_time;
    std::chrono::system_clock::time_point end_time;
    double total_duration;
    std::vector<double> lap_time;

public:
    ChronoTimer(/* args */){};
    ~ChronoTimer(){};
    void start()
    {
        start_time = std::chrono::system_clock::now();
        last_time = start_time;
    };
    void lap(std::string process_name)
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        double one_lap = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count() / 1000.0);
        std::cout << "lap("<< process_name << "): " << one_lap << " [ms]"<< std::endl;
        lap_time.push_back(one_lap);
        last_time = now;
    };
    void end()
    {
        end_time = std::chrono::system_clock::now();
        total_duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0);
        std::cout << "total duration: " << total_duration << " [ms]" << std::endl;
    };
    std::vector<double> export_log()
    {
        return lap_time;
    };
};

#include "ros/ros.h"

class RosTimer
{
private:
    ros::Time start_time;
    ros::Time last_time;
    ros::Time end_time;
    ros::Duration total_duration;
    std::vector<ros::Duration> lap_time;

public:
    RosTimer(/* args */){};
    ~RosTimer(){};
    void start()
    {
        start_time = ros::Time::now();
        last_time = start_time;
    };
    void lap(std::string process_name)
    {
        ros::Time now = ros::Time::now();
        ros::Duration one_lap = last_time - now;
        std::cout << "lap(" << process_name << "): " << one_lap.toNSec()/1000 << " [ms]" << std::endl;
        lap_time.push_back(one_lap);
        last_time = now;
    };
    void end()
    {
        end_time = ros::Time::now();
        total_duration = end_time - start_time;
        std::cout << "total duration: " << total_duration.toNSec()/1000000 << " [ms]" << std::endl;
    };
    std::vector<ros::Duration> export_log()
    {
        return lap_time;
    };
};

#endif