/* Timer */

/* references
  how to use chrono: http://www.sanko-shoko.net/note.php?id=rnfd
*/

#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <chrono>

class Timer
{
protected:
    std::map<std::string, double> lap_time;

public:
    Timer(/* args */)
    {
        lap_time.clear();
    };
    ~Timer(){};
    virtual void start(){};
    virtual double lap(std::string process_name = "unknown"){};
    virtual void pause(){};
    virtual void restart(){};
    virtual double total(){};
    virtual void end(){};
    virtual std::map<std::string, double> export_log() { return lap_time; };
};

class ChronoTimer: public Timer
{
private:
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point last_time;
    std::chrono::system_clock::time_point paused_time;
    std::chrono::system_clock::time_point end_time;
    double total_paused_time;
    double total_duration;

public:
    ChronoTimer(/* args */): total_paused_time(0.), total_duration(0.){};
    ~ChronoTimer(){};
    void start()
    {
        start_time = std::chrono::system_clock::now();
        last_time = start_time;
    };
    double lap(std::string process_name = "unknown")
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        double one_lap = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count() / 1000.0);
        std::cout << "\033[33mlap["<< process_name << "]: " << std::round(one_lap) << " [ms]\033[m"<< std::endl;
        lap_time.insert(std::make_pair(process_name, one_lap));
        last_time = now;
        return one_lap;
    };
    void pause()
    {
        paused_time = std::chrono::system_clock::now();
    }
    void restart()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        double one_pause = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now - paused_time).count() / 1000.0);
        total_paused_time += one_pause;
        last_time = now;
    }
    double total() //get processing time
    {
        return total_duration - total_paused_time;
    }
    void end()
    {
        end_time = std::chrono::system_clock::now();
        total_duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() / 1000.0);
        std::cout << "total duration: " << std::round(total()) << " [ms]" << std::endl;
    };
};

#include "ros/ros.h"

class RosTimer: public Timer
{
private:
    ros::Time start_time;
    ros::Time last_time;
    ros::Time paused_time;
    ros::Time end_time;
    ros::Duration total_duration;
    ros::Duration total_paused_time;
    std::map<std::string, double> lap_time;

public:
    RosTimer(/* args */){};
    ~RosTimer(){};
    void start()
    {
        start_time = ros::Time::now();
        last_time = start_time;
    };
    double lap(std::string process_name)
    {
        ros::Time now = ros::Time::now();
        ros::Duration one_lap = last_time - now;
        double one_lap_ms = one_lap.toNSec() / 1000000;
        std::cout << "lap(" << process_name << "): " << std::round(one_lap_ms) << " [ms]" << std::endl;
        lap_time.insert(std::make_pair(process_name, one_lap_ms));
        last_time = now;
        return one_lap_ms;
    };
    void pause()
    {
        paused_time = ros::Time::now();
    }
    void restart()
    {
        ros::Time now = ros::Time::now();
        ros::Duration one_pause = now - paused_time;
        total_paused_time += one_pause;
        last_time = now;
    }
    double total() //get processing time
    {
        ros::Duration t = total_duration - total_paused_time;
        return t.toNSec()/1000000;
    }
    void end()
    {
        end_time = ros::Time::now();
        total_duration = end_time - start_time;
        std::cout << "total duration: " << std::round(total()) << " [ms]" << std::endl;
    };
};

#endif