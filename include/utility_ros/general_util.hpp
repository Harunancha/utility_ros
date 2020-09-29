/*
TODO: 
*/

#ifndef GENERAL_H_
#define GENERAL_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace util
{
    cv::Point2d cvp_3to2d(cv::Point3d p)
    {
        return cv::Point2d(p.x, p.y);
    }

    cv::Point3d cvp_2to3d(cv::Point2d p)
    {
        return cv::Point3d(p.x, p.y, 0.);
    }

    cv::Point2d cvp_pow2d(cv::Point2d p)
    {
        return cv::Point2d(p.x * p.x, p.y * p.y);
    }
} // namespace util

#endif