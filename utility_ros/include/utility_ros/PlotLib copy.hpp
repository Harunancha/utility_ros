/*
TODO: 
*/

#ifndef PLOT_LIB_H_
#define PLOT_LIB_H_

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
// #include "opencv2/video/tracking.hpp"

#include "utility_ros/geometry_util.hpp"
#include "utility_ros/probability_util.hpp"

#include <utility_ros/matplotlib-cpp/matplotlibcpp.h>

namespace plt = matplotlibcpp;


class pltPoints
{
public:
    std::vector<double> x;
    std::vector<double> y;
    pltPoints(){};
    ~pltPoints(){};
    pltPoints(cv::Point2d p)
    {
        x.clear();y.clear();
        x.push_back(p.x);
        y.push_back(p.y);
    };
    void add(cv::Point2d p)
    {
        x.push_back(p.x);
        y.push_back(p.y);
    }
};

class PlotLib
{
private:
    std::map<std::string, pltPoints> points;
    // std::map<std::string, std::vector<cv::Point2d>> curve;
    std::map<std::string, geo_u::Pose2d> pose;
    // std::map<std::string, std::vector<geo_u::Pose2d>> trajectory;
    std::map<std::string, std::vector<pd_u::NormalDistribution2d>> nd2ds;

public:
    PlotLib(/* args */){};
    ~PlotLib(){};

    void clear()
    {
        plt::clf();
    };
    void show(std::string mode="image")
    {
        if (mode == "image")
            plt::show();
        else if (mode == "animation")
            plt::pause(0.001);
    };
    void add_point(std::string name, cv::Point2d p);
    bool draw_point(std::string name, std::string color = "k", double size = 2., int thickness = 1);
};

void PlotLib::add_point(std::string name, cv::Point2d p)
{
    if (points.count(name)==0)
        points.insert(std::make_pair(name, pltPoints(p)));
    else
        points[name].add(p);
}

bool PlotLib::draw_point(std::string name, std::string color, double size, int thickness)
{
    if (points.count(name)==0)
    {
        std::cout << "\033[31mError[PlotLib]:  no data named \"" << name << "\" \033[m" <<std::endl;
        return false;
    }
    plt::scatter(points[name].x, points[name].y, size);
    return true;
}



#endif