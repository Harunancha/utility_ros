#include "utility_ros/general_util.hpp"
#include "utility_ros/geometry_util.hpp"
#include "utility_ros/probability_util.hpp"

#include "utility_ros/Timer.hpp"
#include "utility_ros/PlotLib.hpp"

#include "unistd.h"

int main(int argc, char **argv)
{
    // geo_u ///////////////////////////
    cv::Point2d a(1., 1.);
    cv::Point2d b(2., 3.);
    // double d = geo_u::Distance2d(a, b);
    // std::cout << "geo_u::Distance2d: " << d << std::endl;

    // Timer ///////////////////////////
    // ChronoTimer timer;
    // timer.start();
    // timer.lap("test");
    // timer.end();
    
    // PlotLib  20200920    ///////////////////////////
    // PlotLib plot;
    // plot.add_point("test", a);
    // plot.add_point("test", b);
    // // plot.draw_point("test", cv::Scalar(0, 255, 0), 1, 1);
    // plot.draw_point("test", "r", 1, 1);
    // plot.show("image");

    // utility  20200921    ///////////////////////////
    // cv::Point3d a3d = util::cvp_2to3d(a);
    // std::cout << "p3d: (" << a3d << ")" << std::endl;

    // pd_u  20201007    ///////////////////////////
    pd_u::Mu2d mu(1., 2.);
    pd_u::Sigma2d sigma(1., 1., 0.6);
    // pd_u::Sigma2d sigma(1., 1., 0.6);
    pd_u::NormalDistribution2d nd2d(mu, sigma);
    
    // CVPlotLib  20201007    ///////////////////////////
    CVPlotLib plt;
    plt.set_param(cv::Point2d(1., 2.), 4., 500, 0.5);
    plt.init_image();
    plt.draw_point(b, 0.3, 0.1, cv::Scalar(255, 0, 0));
    plt.draw_arrow(a, b);
    geo_u::Pose2d p2d(1., 3., 30.);
    plt.draw_pose(p2d);
    plt.draw_ellipse(nd2d);
    int count = 0;
    while (1)
    {
        plt.show();
        sleep(1);
        count++;
    }


    return 0;
}