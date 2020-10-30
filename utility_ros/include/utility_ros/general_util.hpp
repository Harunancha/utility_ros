/*
TODO: 
*/

#ifndef GENERAL_H_
#define GENERAL_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Eigen/Dense>

#include "geometry_util.hpp"

namespace util
{
    // 3D to 2D //
    cv::Point2d cvp_3to2d(cv::Point3d p)
    {
        return cv::Point2d(p.x, p.y);
    }
    // 2D to 3D //
    cv::Point3d cvp_2to3d(cv::Point2d p)
    {
        return cv::Point3d(p.x, p.y, 0.);
    }

    cv::Point2d cvp_pow2d(cv::Point2d p)
    {
        return cv::Point2d(p.x * p.x, p.y * p.y);
    }

    // 3D to 2D //
    void transform_3to2d(tf::Transform &tf3d, geo_u::Pose2d &p2d, std::string axis="x")
    {
        p2d.x = tf3d.getOrigin().getX();
        p2d.y = tf3d.getOrigin().getY();
        tf::Vector3 unit;
        if (axis == "x")
            unit = tf::Vector3(1, 0, 0);
        else if (axis == "y")
            unit = tf::Vector3(0, 1, 0);
        else if (axis == "z")
            unit = tf::Vector3(0, 0, 1);
        tf::Vector3 projected = tf3d.getBasis() * unit;
        p2d.th = RAD2DEG(atan2(projected.y(), projected.x()));
    }
    geo_u::Pose2d pose_3to2d(geo_u::Pose3d &p3d, std::string axis="x") //TODO: test
    {
        geo_u::Pose2d p2d;
        transform_3to2d(p3d, p2d, axis);
        return p2d;
    }
    geo_u::Transform2d tf_3to2d(geo_u::Transform3d &tf3d, std::string axis="x") //TODO: test
    {
        geo_u::Transform2d tf2d;
        transform_3to2d(tf3d, tf2d, axis);
        tf2d.calc_mat();
        return tf2d;
    }

    // 2D to 3D //
    void transform_2to3d(geo_u::Pose2d &p2d, geo_u::Transform3d &tf3d)
    {
        tf3d.setOrigin(tf::Vector3(p2d.x, p2d.y, 0.));
        tf3d.setRotation(tf::createQuaternionFromRPY(0., 0., p2d.th));
    }
    geo_u::Pose3d pose_2to3d(geo_u::Pose2d &p2d) //TODO: test
    {
        geo_u::Pose3d p3d;
        transform_2to3d(p2d, p3d);
        return p3d;
    };
    geo_u::Transform3d tf_2to3d(geo_u::Pose2d &p2d) //TODO: test
    {
        geo_u::Transform3d tf3d;
        transform_2to3d(p2d, tf3d);
        return tf3d;
    };

    std::string get_time_string()
    {
        time_t t = time(nullptr);
        const tm* lt = localtime(&t);
        std::stringstream s;
        s<<"20";
        s<<lt->tm_year-100; //100を引くことで20xxのxxの部分になる
        s<<"-";
        s<<lt->tm_mon+1; //月を0からカウントしているため
        s<<"-";
        s<<lt->tm_mday; //そのまま
        s<<"-";
        s<<lt->tm_hour; //そのまま
        s<<"-";
        s<<lt->tm_min; //そのまま
        //result = "2015-5-19-8-30" 
        return std::string(s.str());
    }
} // namespace util

#endif