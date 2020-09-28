/*
TODO: 
*/

#ifndef MARKER_H_
#define MARKER_H_

#include "geometry_msgs/Pose.h"
#include "tf/tf.h"

namespace mrkr_u
{
    geometry_msgs::Pose marker_to_camera_pose(geometry_msgs::Pose mrkr_p, tf::Transform &tf) //tf is the output
    {
        tf::poseMsgToTF(mrkr_p, tf); //camera pose in marker frame
        tf = tf.inverse();
        geometry_msgs::Pose cam_p;
        tf::poseTFToMsg(tf, cam_p);
        return cam_p;
    };
}

#endif