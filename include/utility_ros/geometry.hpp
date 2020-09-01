/*
TODO: 

*/

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifndef M_PI
#define M_PI = 3.141592653589793238
#endif

#define DEG2RAD(x) ((x)*M_PI / 180)
#define RAD2DEG(x) ((x)*180 / M_PI)

class Geometry
{
private:
    /* data */
public:
    Geometry(){};
    ~Geometry(){};
    static double Distance2d(cv::Point2d x1, cv::Point2d x2)
    {
        return (std::sqrt((x1.x - x2.x) * (x1.x - x2.x) + (x1.y - x2.y) * (x1.y - x2.y)));
    };

    static double DistancePixel(cv::Point x1, cv::Point x2)
    {
        return (std::sqrt((x1.x - x2.x) * (x1.x - x2.x) + (x1.y - x2.y) * (x1.y - x2.y)));
    };

    static double Distance3d(cv::Point3d x1, cv::Point3d x2)
    {
        return (std::sqrt((x1.x - x2.x) * (x1.x - x2.x) + (x1.y - x2.y) * (x1.y - x2.y) + (x1.z - x2.z) * (x1.z - x2.z)));
    };

    static int RectArea(cv::Rect &rect)
    {
        return rect.width * rect.height;
    };
};

#endif