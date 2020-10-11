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

    void set_param(cv::Point2d c, double real_s, int image_s, double real_i)
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
        int num = (int)(real_scale / real_interval + 1);
        for (int i = 0; i < num; ++i)
            cv::line(image, cv::Point(0, i * real_interval*scale_ratio), cv::Point(image_scale, i * real_interval*scale_ratio), cv::Scalar(204, 204, 204));
        for (int i = 0; i < num; ++i)
            cv::line(image, cv::Point(i * real_interval*scale_ratio, 0), cv::Point(i * real_interval*scale_ratio, image_scale), cv::Scalar(204, 204, 204));
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
    void draw_line(cv::Point2d ps, cv::Point2d pe, double thickness=0.05, cv::Scalar color=cv::Scalar(0,0,0));
    void draw_arrow(cv::Point2d ps, cv::Point2d pe, double thickness=0.05, cv::Scalar color=cv::Scalar(0,255,0));
    void draw_pose(geo_u::Pose2d p, double size=0.2, double thickness=0.03, cv::Scalar color_x=cv::Scalar(0,0,255), cv::Scalar color_y=cv::Scalar(0,255,0));
    void draw_curve(std::vector<cv::Point2d> &p_set){};
    void draw_ellipse(pd_u::NormalDistribution2d nd2d, double size=0.1, double thickness=0.05, cv::Scalar color_mu=cv::Scalar(0,255,0), cv::Scalar color_sigma=cv::Scalar(255,0,0));
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

void CVPlotLib::draw_arrow(cv::Point2d ps, cv::Point2d pe, double thickness, cv::Scalar color)
{
    cv::Point p_s = transform2dtoPix(ps);
    cv::Point p_e = transform2dtoPix(pe);
    cv::arrowedLine(image, p_s, p_e, color, thickness*scale_ratio);
}

void CVPlotLib::draw_pose(geo_u::Pose2d p, double size, double thickness, cv::Scalar color_x, cv::Scalar color_y)
{
    double a = DEG2RAD(p.th);
    double b = DEG2RAD(p.th + 90.);
    cv::Point2d t_x(cos(a), sin(a));
    cv::Point2d t_y(cos(b), sin(b));
    // std::cout << "?: " << size / 2 << std::endl;
    cv::Point2d x = p + (size/2) * t_x;
    cv::Point2d x_ = p - (size/2) * t_x;
    cv::Point2d y = p + (size/2) * t_y;
    cv::Point2d y_ = p - (size/2) * t_y;
    // std::cout << "??: " << x_ << ", " << x << ", " << y_ << ", " << y << ", " << thickness * scale_ratio << std::endl;
    draw_arrow(x_, x, thickness, color_x);
    draw_arrow(y_, y, thickness, color_y);
}

void CVPlotLib::draw_ellipse(pd_u::NormalDistribution2d nd2d, double size, double thickness, cv::Scalar color_mu, cv::Scalar color_sigma) //TODO: revise
{
    cv::Point mu = transform2dtoPix(nd2d.mu);
    cv::Size sigma;
    sigma.width = (nd2d.sigma.xx + nd2d.sigma.yy + std::sqrt((nd2d.sigma.xx - nd2d.sigma.yy)*(nd2d.sigma.xx - nd2d.sigma.yy)+4*nd2d.sigma.xy*nd2d.sigma.xy)) / 2;
    sigma.height = (nd2d.sigma.xx + nd2d.sigma.yy - std::sqrt((nd2d.sigma.xx - nd2d.sigma.yy)*(nd2d.sigma.xx - nd2d.sigma.yy)+4*nd2d.sigma.xy*nd2d.sigma.xy)) / 2;
    std::cout << "u: " << sigma.width << ", v: " << sigma.height << ", xx: " << nd2d.sigma.xx << ", yy: " << nd2d.sigma.yy << std::endl;
    double a = atan((sigma.width - nd2d.sigma.xx) / nd2d.sigma.xy);
    double a_pix;
    a_pix = -RAD2DEG(a);
    std::cout << "a[rad]: " << a << ", a_pix[DEG]: " << a_pix << std::endl;
    draw_point(nd2d.mu, size, thickness, color_mu);
    if (sigma.width > 0 && sigma.height > 0)
    {
        sigma.width *= scale_ratio;
        sigma.height *= scale_ratio;
        cv::ellipse(image, mu, sigma, a_pix, a_pix, a_pix + 360., color_sigma, thickness, 8, 0);
    }
    else
        ROS_ERROR("drawing error: ellipse sigma is smaller than 0");
}




#endif