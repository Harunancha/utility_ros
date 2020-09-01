#include "utility_ros/Geometry.hpp"
#include "utility_ros/Timer.hpp"
#include "utility_ros/PlotLib.hpp"

int main(int argc, char **argv)
{
    Geometry geo;
    /////////////////////////////
    Timer timer;
    timer.start();
    timer.lap("test");
    timer.end();
    /////////////////////////////
    PlotLib plot;

    return 0;
}