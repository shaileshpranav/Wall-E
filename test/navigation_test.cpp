/**
 * @file navigation_test.cpp
 * @author Shailesh Pranav Rajendran (spraj@umd.edu)
 * @brief 
 * @version 0.1
 * @date 2022-12-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <gtest/gtest.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <cv_bridge/cv_bridge.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h>

#include "../include/wall-e/navigation.hpp"

// Global variables
bool message_received;
geometry_msgs::PoseStamped pose_message;
geometry_msgs::Twist velocity_message;

bool wait_for_message(const bool& message_received, double timeout = 5) {
    ros::Time start = ros::Time::now();
    ros::Time now;
    while (!message_received) {
        ros::spinOnce();
        now = ros::Time::now();
        if ((now - start).toSec() > timeout) {
            return false;
        }
    }
    return true;
}

void pose_callback(const geometry_msgs::PoseStampedConstPtr& msg) {
    message_received = true;
    pose_message.header = msg->header;
    pose_message.pose = msg->pose;
}

TEST(test_navigation_class, test_get_object_pose) {

    ros::NodeHandle nh;
    geometry_msgs::PoseStamped expected_msg;
    expected_msg.pose.orientation.w = 0;
    expected_msg.header.frame_id = "";

    // Act
    Navigation navigator(&nh);
    navigator.get_object_pose("map");

    // Assert
    // ASSERT_TRUE(wait_for_message(message_received, 3));
    EXPECT_EQ(pose_message.pose.orientation, expected_msg.pose.orientation);
    EXPECT_EQ(pose_message.pose.position, expected_msg.pose.position);
    EXPECT_EQ(pose_message.header.frame_id, expected_msg.header.frame_id);
}
std::array<XmlRpc::XmlRpcValue, 5> goal_pos =   {(-1.752882, 3.246192),(6.5, 2.0),(-0.289296, -1.282680),(7.710214, -1.716889)};

// TEST(test_navigation_class, test_set_room_pos){
//     ros::NodeHandle nh;
//     Navigation navi(&nh);
//     bool flag{true};
//     navi.set_room_pos(goal_pos);
//     for(int i = 0;i<4;i++)
//     {
//         if(!navi.explorer_goal[i].target_pose.pose.position.x == double(goal_pos[i][0]) || !navi.explorer_goal[i].target_pose.pose.position.y == double(goal_pos[i][1]))
//             flag = false; 
//     }
//     EXPECT_TRUE(flag);
// }

// TEST(test_navigation_class, test_robot_pose_cb){
//     EXPECT_EQ(navi.)
// }