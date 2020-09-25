/*
TODO: 

*/

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

// #include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifndef M_PI
#define M_PI = 3.141592653589793238
#endif

#define DEG2RAD(x) ((x)*M_PI / 180)
#define RAD2DEG(x) ((x)*180 / M_PI)

namespace geo_u
{
    class Pose2d : public cv::Point2d
    {
    public:
        double th; //degree

    public:
        Pose2d(/* args */){};
        ~Pose2d(){};
        Pose2d(double tx, double ty, double a): cv::Point2d(tx,ty), th(a){};
        Pose2d(cv::Point2d p, double a) : cv::Point2d(p), th(a){};
        Pose2d(const Pose2d& p): cv::Point2d(p), th(p.th){};
    };

    class Transform2d
    {
    private:
        Pose2d pose;
        cv::Mat mat_p2o; //TODO: should be replaced by Eigen matrix?
        cv::Mat mat_o2p;

    public:
        Transform2d(){};
        ~Transform2d(){};
        Transform2d(double x, double y, double a): pose(x, y, a){};
        Transform2d(Pose2d p): pose(p)
        {
            calc_mat();
        };

        void calc_mat()
        {
            mat_p2o = (cv::Mat_<double>(3, 3) << cos(DEG2RAD(pose.th)), -sin(DEG2RAD(pose.th)), pose.x, 
            sin(DEG2RAD(pose.th)), cos(DEG2RAD(pose.th)), pose.y, 
            0, 0, 1);
            mat_o2p = mat_p2o.inv();
        };

        cv::Point2d tf_point_to_o(cv::Point2d p) //transform point pose to origin
        {
            double x = p.x * mat_p2o.at<double>(0, 0) + p.y * mat_p2o.at<double>(0, 1) + mat_p2o.at<double>(0, 2);
            double y = p.x * mat_p2o.at<double>(1, 0) + p.y * mat_p2o.at<double>(1, 1) + mat_p2o.at<double>(1, 2);
            return cv::Point2d(x, y);
        };

        cv::Point2d tf_point_to_p(cv::Point2d p) //transform point origin to pose
        {
            double x = p.x * mat_o2p.at<double>(0, 0) + p.y * mat_o2p.at<double>(0, 1) + mat_o2p.at<double>(0, 2);
            double y = p.x * mat_o2p.at<double>(1, 0) + p.y * mat_o2p.at<double>(1, 1) + mat_o2p.at<double>(1, 2);
            return cv::Point2d(x, y);
        };

        Pose2d tf_pose2d_to_o(Pose2d p)
        {
            cv::Point2d local_p = p;
            cv::Point2d position = tf_point_to_o(local_p);
            Pose2d tf_pose(position, p.th + pose.th);
            // cv::Point2d zero(0., 0.), dx(1.,0.);
            // cv::Point2d zero_ = tf_point_to_o(zero);
            // cv::Point2d dx_ = tf_point_to_o(dx_);
            // double angle = atan2(dx_.y - zero_.y, dx_.x - zero_.x);
            // std::cout << "[DEBUG] angle: " << RAD2DEG(angle) << "," << tf_pose.th << std::endl;
            return tf_pose;
        };
        Pose2d tf_pose2d_to_p(Pose2d p)
        {
            cv::Point2d local_p = p;
            cv::Point2d position = tf_point_to_p(local_p);
            return Pose2d(position, p.th - pose.th);
        };
    };

    static double Distance2d(cv::Point2d x1, cv::Point2d x2)
    {
        return (std::sqrt((x1.x - x2.x) * (x1.x - x2.x) + (x1.y - x2.y) * (x1.y - x2.y)));
    };

    static double square_Distance3d(cv::Point3d x1, cv::Point3d x2)
    {
        return ((x1.x - x2.x) * (x1.x - x2.x) + (x1.y - x2.y) * (x1.y - x2.y) + (x1.z - x2.z) * (x1.z - x2.z));
    };

    static double Distance3d(cv::Point3d x1, cv::Point3d x2)
    {
        return (std::sqrt(square_Distance3d(x1, x2)));
    };

    static int RectArea(cv::Rect &rect)
    {
        return rect.width * rect.height;
    };
}

#endif