/*
TODO: 
*/

#ifndef MARKER_H_
#define MARKER_H_

#include "geometry_msgs/Pose.h"
#include "tf/tf.h"

namespace mrkr_u
{
    geometry_msgs::Pose mrkr_to_cam_pose(geometry_msgs::Pose mrkr_p, tf::Transform &tf) //tf is the output
    {
        tf::poseMsgToTF(mrkr_p, tf); //camera pose in marker frame
        tf = tf.inverse();
        geometry_msgs::Pose cam_p;
        tf::poseTFToMsg(tf, cam_p);
        return cam_p;
    };

    geometry_msgs::PoseStamped mrkr_to_cam_pose_stamped(geometry_msgs::PoseStamped mrkr_p, tf::Stamped<tf::Transform> &tf) //tf is the output
    {
        tf::poseStampedMsgToTF(mrkr_p, tf); //camera pose in marker frame
        tf.setData(tf.inverse());
        geometry_msgs::PoseStamped cam_p;
        tf::poseStampedTFToMsg(tf, cam_p);
        return cam_p;
    };
}

#endif