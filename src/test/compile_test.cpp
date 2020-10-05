#include "utility_ros/general_util.hpp"
#include "utility_ros/geometry_util.hpp"
#include "utility_ros/probability_util.hpp"

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
    ChronoTimer timer;
    timer.start();
    timer.lap("test");
    timer.end();
    
    // PlotLib  20200920    ///////////////////////////
    // PlotLib plot;
    // plot.add_point("test", a);
    // plot.add_point("test", b);
    // // plot.draw_point("test", cv::Scalar(0, 255, 0), 1, 1);
    // plot.draw_point("test", "r", 1, 1);
    // plot.show("image");

    // utility  20200921    ///////////////////////////
    cv::Point3d a3d = util::cvp_2to3d(a);
    std::cout << "p3d: (" << a3d << ")" << std::endl;
    return 0;
}