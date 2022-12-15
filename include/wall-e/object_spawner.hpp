/**
 * @file trash_spwaner.hpp
 * @author Shailesh Pranav Rajendran (spraj@umd.edu)
 * @brief
 * @version 0.1
 * @date 2022-12-09
 *
 * @copyright MIT License (c) 2022 Shailesh Pranav Rajendran
 *
 */

#pragma once

// ROS headers
#include <gazebo_msgs/ModelState.h>
#include <gazebo_msgs/SpawnModel.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <ros/ros.h>
#include <std_srvs/SetBool.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>

// Std C++ headers
#include <stdlib.h>

#include <string>

class ObjectSpawner {
 public:
  /**
   * @brief Constructor for the ObjectSpawner class
   *
   * @param node_handle
   */
  explicit ObjectSpawner(ros::NodeHandle*);

  /**
   * @brief Method to spawn the object
   *
   * @return true if the object is spawned
   * @return false if the object is not spawned
   */
  bool spawn_object();

  /**
   * @brief Method to set position of the object in Gazebo
   *
   */
  void set_object_pose(geometry_msgs::Pose);


 private:
  /**
   * @brief Service server for setting the state of the object
   *
   * @param req object starts publishing position if "req" is true
   * @param res
   * @return true
   */
  bool set_object_state_cb(std_srvs::SetBool::Request&,
                           std_srvs::SetBool::Response&);
  /**
   * @brief ROS publisher that publishes position of the object and updates
   *  its frame in tf
   *
   */
  void publish_pose(const ros::TimerEvent&);

  unsigned int seed;
  bool is_spawned;
  int map_range[4];

  ros::NodeHandle* nh_;
  ros::ServiceServer update_state_service_;
  ros::ServiceClient spawn_object_client_;
  ros::Timer object_pose_tf_timer_;
  ros::Publisher pose_pub_;

  std::string urdf_string_;
  geometry_msgs::Pose object_pose_;
  std::string object_name;

  tf2_ros::TransformBroadcaster br_;
  tf2_ros::Buffer tfBuffer_;
  tf2_ros::TransformListener tf_listener_;
};
