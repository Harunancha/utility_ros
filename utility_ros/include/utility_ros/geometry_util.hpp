/*
TODO: 

*/

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

// #include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <tf/tf.h>


#ifndef M_PI
#define M_PI = 3.141592653589793238
#endif

#define DEG2RAD(x) ((x)*M_PI / 180)
#define RAD2DEG(x) ((x)*180 / M_PI)

namespace geo_u
{
    typedef tf::Transform Transform3d;
    typedef Transform3d Pose3d;

    class Pose2d : public cv::Point2d
    {
    public:
        double th; //degree

    public:
        Pose2d(/* args */){};
        ~Pose2d(){};
        Pose2d(double tx, double ty, double a): cv::Point2d(tx,ty), th(a){};
        Pose2d(cv::Point2d p, double a): cv::Point2d(p), th(a){};
        Pose2d(const Pose2d& p): cv::Point2d(p), th(p.th){};
    };

    class Transform2d: public Pose2d
    {
    public:
        // Pose2d pose;
    private:
        cv::Mat mat_p2o; //TODO: should be replaced by Eigen matrix?
        cv::Mat mat_o2p;

    public:
        Transform2d(){};
        ~Transform2d(){};
        Transform2d(double x, double y, double a): Pose2d(x, y, a)
        {
            calc_mat();
        };
        Transform2d(Pose2d p): Pose2d(p)
        {
            calc_mat();
        };

        void calc_mat()
        {
            mat_p2o = (cv::Mat_<double>(3, 3) << cos(DEG2RAD(th)), -sin(DEG2RAD(th)), x, 
            sin(DEG2RAD(th)), cos(DEG2RAD(th)), y, 
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
            Pose2d tf_pose(position, p.th + th);
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
            return Pose2d(position, p.th - th);
        };
    };

    class BoundingBox3d
    {
    public:
        std::string label;
        std::vector<cv::Point3d> points;
        std::vector<cv::Point3d> vertices;
        cv::Point3d center3d;

    public:
        BoundingBox3d(){};
        ~BoundingBox3d(){};
        BoundingBox3d(std::vector<cv::Point3d> &ps)
        {
            points.clear();
            points = ps;
            update();
        };
        void update()
        {
            cv::Point3d min, max;
            for (auto p : points)
            {
                if (p.x < min.x)
                    min.x = p.x;
                if (p.y < min.y)
                    min.y = p.y;
                if (p.z < min.z)
                    min.z = p.z;
                if (p.x > max.x)
                    max.x = p.x;
                if (p.y > max.y)
                    max.y = p.y;
                if (p.z > max.z)
                    max.z = p.z;
            }
            center3d = (min + max) / 2;
            vertices.clear();
            vertices.push_back(min);
            vertices.push_back(cv::Point3d(min.x,min.y,max.z));
            vertices.push_back(cv::Point3d(max.x,min.y,max.z));
            vertices.push_back(cv::Point3d(max.x,min.y,min.z));
            vertices.push_back(cv::Point3d(min.x,max.y,min.z));
            vertices.push_back(cv::Point3d(min.x,max.y,max.z));
            vertices.push_back(cv::Point3d(max.x,max.y,min.z));
            vertices.push_back(max);
        };
    };
    
    cv::Point3d tf_point3d(cv::Point3d &p3d, Transform3d &tf)
    {
        tf::Vector3 p(p3d.x, p3d.y, p3d.z);
        tf::Vector3 tf_p = tf * p;
        return cv::Point3d(tf_p.getX(), tf_p.getY(), tf_p.getZ());
    };

    void tf_bb3d(BoundingBox3d &bb3d, tf::Transform &tf)
    {
        for (auto p : bb3d.points)
        {
            p = tf_point3d(p, tf);
        }
        bb3d.update();
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

    bool is_in(cv::Point2d p, cv::Rect2d rect)
    {
        if ((p.x>rect.x && p.x<rect.x+rect.width
	    	 && p.y>rect.y && p.y<rect.y+rect.height)
	    	 || (p.x<rect.x && p.x>rect.x+rect.width
	    	 && p.y<rect.y && p.y>rect.y+rect.height))
	    	return true;
	    else
	    	return false;
    }
}

#endif