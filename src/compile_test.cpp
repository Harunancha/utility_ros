#include "utility_ros/geometry_util.hpp"
#include "utility_ros/image_util.hpp"
#include "utility_ros/Timer.hpp"
#include "utility_ros/PlotLib.hpp"

int main(int argc, char **argv)
{
    // geo_u ///////////////////////////
    cv::Point2d a(1., 1.);
    cv::Point2d b(2., 3.);
    double d = geo_u::Distance2d(a, b);
    std::cout << "geo_u::Distance2d: " << d << std::endl;
    // Timer ///////////////////////////
    Timer timer;
    timer.start();
    timer.lap("test");
    timer.end();
    // PlotLib ///////////////////////////
    PlotLib plot;

    return 0;
}