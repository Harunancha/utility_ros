/*
TODO: 
*/

#ifndef IMAGE_H_
#define IMAGE_H_

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace im_u
{
    static double DistancePixel(cv::Point x1, cv::Point x2)
    {
        return (std::sqrt((x1.x - x2.x) * (x1.x - x2.x) + (x1.y - x2.y) * (x1.y - x2.y)));
    };

    bool is_in(cv::Point p, cv::Rect rect)
    {
        if ((p.x>rect.x && p.x<rect.x+rect.width
	    	 && p.y>rect.y && p.y<rect.y+rect.height)
	    	 || (p.x<rect.x && p.x>rect.x+rect.width
	    	 && p.y<rect.y && p.y>rect.y+rect.height))
	    	return true;
	    else
	    	return false;
    }

} // namespace im_u


#endif