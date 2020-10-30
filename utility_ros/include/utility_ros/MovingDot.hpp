/*
TODO: sophisticate the movement
TODO: limit the velocity
*/

#ifndef MOVING_DOT_H_
#define MOVING_DOT_H_

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "utility_ros/general_util.hpp"
#include "utility_ros/probability_util.hpp"

class MovingDot
{
private:
    cv::Point2d position2d;
    cv::Point2d velocity2d;
    cv::Point2d acceleration2d;

    double a_scale;
    double max_velocity;

public:
    MovingDot(/* args */) : position2d(0., 0.), velocity2d(0., 0.), acceleration2d(0.,0.){};
    MovingDot(double scale, double max_v) : position2d(0., 0.), velocity2d(0., 0.), acceleration2d(0.,0.), a_scale(scale), max_velocity(max_v){};
    ~MovingDot(){};
    void update_random(double interval, cv::Rect2d area);
    void bound(double e);

    cv::Point2d get_position()
    {
        return position2d;
    };
};

void MovingDot::update_random(double interval, cv::Rect2d area)
{
    if (!geo_u::is_in(position2d, area))
    {
        bound(1.);
        std::cout << "bound" << std::endl;
    }

    position2d += velocity2d * interval + util::cvp_pow2d(acceleration2d) * interval / 2;
    velocity2d += acceleration2d * interval;
    acceleration2d += pd_u::add_noise_nd2d(pd_u::Mu2d(0.,0.), pd_u::Sigma2d(a_scale, a_scale, 0.));

    if (cv::norm(velocity2d) > max_velocity) //TODO: velocity is limited
    {
        // acceleration2d 
    }
    
    std::cout << "x: " << position2d << ", v: " << velocity2d << ", a: " << acceleration2d << std::endl;
}

void MovingDot::bound(double e)
{
    velocity2d = -velocity2d * e;
    acceleration2d = -acceleration2d * e;
}

#endif