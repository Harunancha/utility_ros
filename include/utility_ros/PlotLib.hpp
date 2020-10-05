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

#include "utility_ros/general_util.hpp"
#include "utility_ros/geometry_util.hpp"
#include "utility_ros/probability_util.hpp"

#include <utility_ros/matplotlib-cpp/matplotlibcpp.h>

namespace plt = matplotlibcpp;


class PlotLib
{
protected:
    std::string window_name;
    std::string time;
    std::string path;

public:
    PlotLib(std::string window="debug"){};
    ~PlotLib(){};

    void set_time(){ time = util::get_time_string(); };
    virtual void save(){};
    virtual void show(){};

    virtual void draw_point(cv::Point2d p){};
    virtual void draw_line(cv::Point2d ps, cv::Point2d pe){};
    virtual void draw_arrow(cv::Point2d ps, cv::Point2d pe){};
    virtual void draw_pose(geo_u::Pose2d p){};
    virtual void draw_curve(std::vector<cv::Point2d> &p_set){};
    virtual void draw_ellipse(pd_u::NormalDistribution2d nd2d){};
};

class CVPlotLib: public PlotLib
{
private:
    cv::Mat image;

    cv::Point2d real_center;
    cv::Point image_center;
    double real_scale, image_scale, scale_ratio, real_interval, image_interval;

public:
    CVPlotLib(std::string window = "debug") : PlotLib(window) { set_time(); };
    ~CVPlotLib(){};

    void set_param(cv::Point2d c, double real_s, double image_s, double real_i)
    {
        real_center = c;
        real_scale = real_s;
        image_scale = image_s;
        real_interval = real_i;
        image_center = cv::Point((int)image_scale / 2, (int)image_scale / 2);
        scale_ratio = image_scale / real_scale;
        image_interval = (int)(scale_ratio * real_interval);
    };
    void save()
    {
        set_time();
        cv::imwrite(path + window_name + "/" + window_name + "-" + time + ".jpg", image);
    };
    void show()
    {
        cv::imshow(window_name, image);
        cv::waitKey(1);
    };

    void init_image()
    {
        image = cv::Mat(image_scale, image_scale, CV_8UC3, cv::Scalar(255, 255, 255));
        int num = (int)(image_scale / image_interval) + 1;
        for (int i = 0; i < num; ++i)
            cv::line(image, cv::Point(0, i * image_interval), cv::Point(image_scale, i * image_interval), cv::Scalar(204, 204, 204));
        for (int i = 0; i < num; ++i)
            cv::line(image, cv::Point(i * image_interval, 0), cv::Point(i * image_interval, image_scale), cv::Scalar(204, 204, 204));
        cv::line(image, cv::Point(0, 0), cv::Point(image_scale, 0), cv::Scalar(0, 0, 0));
        cv::line(image, cv::Point(0, 0), cv::Point(0, image_scale), cv::Scalar(0, 0, 0));
        cv::line(image, cv::Point(image_scale, image_scale), cv::Point(image_scale, 0), cv::Scalar(0, 0, 0));
        cv::line(image, cv::Point(image_scale, image_scale), cv::Point(0, image_scale), cv::Scalar(0, 0, 0));
        cv::circle(image, image_center, 3, cv::Scalar(0, 0, 0), 1, 8, 0);
    }
    cv::Point transform2dtoPix(cv::Point2d &p) //TODO: test
    {
        return cv::Point((p.x-real_center.x) * scale_ratio + image_center.x, image_center.y - (p.y-real_center.y) * scale_ratio);
    }
    void draw_point(cv::Point2d p, double size=0.1, int thickness=0.05, cv::Scalar color=cv::Scalar(0,0,0));
    void draw_line(cv::Point2d ps, cv::Point2d pe, double thickness=0.1, cv::Scalar color=cv::Scalar(0,0,0));
    void draw_arrow(cv::Point2d ps, cv::Point2d pe);
    void draw_pose(geo_u::Pose2d p);
    void draw_curve(std::vector<cv::Point2d> &p_set){};
    void draw_ellipse(pd_u::NormalDistribution2d nd2d);
};

void CVPlotLib::draw_point(cv::Point2d p, double size, int thickness, cv::Scalar color)
{
    cv::Point displayed = transform2dtoPix(p);
    cv::circle(image, displayed, size*scale_ratio, color, thickness*scale_ratio, 8, 0);
}

void CVPlotLib::draw_line(cv::Point2d ps, cv::Point2d pe, double thickness, cv::Scalar color)
{
    cv::Point p_s = transform2dtoPix(ps);
    cv::Point p_e = transform2dtoPix(pe);
    cv::line(image, p_s, p_e, color, thickness*scale_ratio);
}

void CVPlotLib::draw_arrow(cv::Point2d ps, cv::Point2d pe)
{
    cv::Point p_s = transform2dtoPix(ps);
    cv::Point p_e = transform2dtoPix(pe);
    // cv::arrowedLine(image, p_s, p_e, color, thickness*scale_ratio);
}

void CVPlotLib::draw_pose(geo_u::Pose2d p)
{

}

void CVPlotLib::draw_ellipse(pd_u::NormalDistribution2d nd2d)
{

}




#endif