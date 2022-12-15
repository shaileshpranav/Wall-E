/**
 * @file nav.cpp
 * @author Shailesh Pranav Rajendran (spraj@umd.edu)
 * @brief Navigation class
 * @version 0.1
 * @date 2022-12-08
 *
 * @copyright MIT License (c) 2022 Shailesh Pranav Rajendran
 *
 */

#include "../include/wall-e/navigation.hpp"

Navigation::Navigation(ros::NodeHandle* node_handle)
    : det_obj(node_handle), tf_listener(this->tf_Buffer) {
  nh = node_handle;
  nh->getParam("simple_navigation_goals/aruco_lookup_locations/target_1",
               ex_goal_pos[0]);
  nh->getParam("simple_navigation_goals/aruco_lookup_locations/target_2",
               ex_goal_pos[1]);
  nh->getParam("simple_navigation_goals/aruco_lookup_locations/target_3",
               ex_goal_pos[2]);
  nh->getParam("simple_navigation_goals/aruco_lookup_locations/target_4",
               ex_goal_pos[3]);

  cur_pose_sub_ = nh->subscribe("/explorer/amcl_pose", 10,
                                &Navigation::robot_pose_cb, this);

  vel_pub_ = nh->advertise<geometry_msgs::Twist>("explorer/cmd_vel", 10);
  turn_state = TURN_START;
  is_pose_initialized_ = false;
}

void Navigation::set_room_pos(std::array<XmlRpc::XmlRpcValue, 5> room_pos) {
  for (int i = 0; i <= 3; i++) {
    explorer_goal[i].target_pose.header.frame_id = "map";
    explorer_goal[i].target_pose.header.stamp = ros::Time::now();
    explorer_goal[i].target_pose.pose.orientation.w = 1.0;
    explorer_goal[i].target_pose.pose.position.x = room_pos[i][0];
    explorer_goal[i].target_pose.pose.position.y = room_pos[i][1];
    ROS_INFO_STREAM("Explorer: " << ex_goal_pos[i][0] << ","
                                 << ex_goal_pos[i][1]);
  }
}

void Navigation::robot_pose_cb(
    // Receive robot pose parameters
    const geometry_msgs::PoseWithCovarianceStamped& robot_pose) {
  current_pose_ = robot_pose.pose.pose;
  is_pose_initialized_ = true;
}

void Navigation::turn_around() {
  if (turn_state == TURN_START) {
    tf2::fromMsg(current_pose_.orientation, initial_tf_quat_);
    initial_tf_quat_ = initial_tf_quat_.inverse();
    turn_state = TURNING;
  } else {
    tf2::Quaternion tf2_quat;
    tf2::fromMsg(current_pose_.orientation, tf2_quat);
    tf2_quat *= initial_tf_quat_;
    auto angle = tf2::getYaw(tf2_quat);
    set_turning_velocity();
    // ROS_INFO_STREAM("[Navigation] Turning around: " << angle);
    if (angle > -0.1 && angle < 0) {
      turn_state = TURN_COMPLETE;
      ROS_INFO_STREAM("[Navigation] Turning complete");
    }
  }
}

void Navigation::set_turning_velocity() {
  // function for making robot turn around to detect the object
  geometry_msgs::Twist orientation;
  orientation.angular.z = 0.6;
  vel_pub_.publish(orientation);
}

bool Navigation::get_object_pose(std::string wrt) {
  //  Checks if object is within reach of the robot arm.
  if (det_obj.is_object_detected == false) {
    ROS_INFO_STREAM(" object has not been found yet.");
    return false;
  } else {
    geometry_msgs::TransformStamped transformStamped;
    transformStamped = tf_Buffer.lookupTransform(wrt, "trash", ros::Time(0));

    auto x = transformStamped.transform.translation.x;
    auto y = transformStamped.transform.translation.y;

    final.target_pose.header.frame_id = "map";
    final.target_pose.header.stamp = ros::Time::now();
    final.target_pose.pose.position.x = x - 0.25;
    final.target_pose.pose.position.y = y - 0.25;
    final.target_pose.pose.orientation.w = 1.0;
    return true;
  }
}
