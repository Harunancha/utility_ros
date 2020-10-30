/*
    test for both 
    - probability_util NormalDistribution2d random_sampling()
    - matplotlib-cpp scatter, xlim, ylim, and animation (pause())
    
    http://leeswijzer.org/R/R-binormal.html
*/

#include <unistd.h>

#include "utility_ros/probability_util.hpp"

#include <utility_ros/matplotlib-cpp/matplotlibcpp.h>
namespace plt = matplotlibcpp;

int main(int argc, char **argv)
{
    pd_u::Mu2d mu(0., 0.);
    pd_u::Sigma2d sigma(1., 0.4, -0.6);

    pd_u::NormalDistribution2d nd2d(mu, sigma);

    int n = 10000;
    std::vector<double> x(n), y(n);

    for(int i=0; i<n; ++i) {
        cv::Point2d p = nd2d.random_sampling();
        x[i] = p.x;
        y[i] = p.y;

        ////for matplotlib animation test
        // plt::clf();
        // plt::scatter(x, y);
        // plt::xlim(-1., 3.);
        // plt::ylim(-1., 3.);
        // plt::pause(0.001);
        ////
    }
 
    ////for nd2d random sampling test
    plt::scatter(x, y);
    plt::xlim(-4., 4.);
    plt::ylim(-4., 4.);
    plt::show();
    ////
}