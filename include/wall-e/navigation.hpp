/**
 * @file navigation.hpp
 * @author Shailesh Pranav Rajendran (spraj@umd.edu)
 * @brief Header file for Navigation class
 * @version 0.1
 * @date 2022-12-08
 *
 * @copyright MIT License (c) 2022 Shailesh Pranav Rajendran
 *
 */

#ifndef INCLUDE_WALL_E_NAVIGATION_HPP_
#define INCLUDE_WALL_E_NAVIGATION_HPP_

#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Twist.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/utils.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <xmlrpcpp/XmlRpcValue.h>

#include <array>
#include <string>
#include <iostream>

#include "detection.hpp"
#include "object_spawner.hpp"

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>
    MoveBaseClient;
class Navigation {
 public:
  /**
   * @brief Construct a new Navigation object
   *
   */
  explicit Navigation(ros::NodeHandle*);
  /**
   * @brief Instance for Detect Object class
   *
   */
  DetectObject det_obj;
  /**
   * @brief Goal positions for wach room
   *
   */
  move_base_msgs::MoveBaseGoal explorer_goal[5];
  /**
   * @brief Position for the detected trash object
   *
   */
  move_base_msgs::MoveBaseGoal final;

  /**
   * @brief Set the goal positions for each room
   *
   */
  void set_room_pos(std::array<XmlRpc::XmlRpcValue, 5>);

  /**
   * @brief Is object detected
   *
   */
  bool det;

  /**
   * @brief Robot pose callback
   *
   */
  void robot_pose_cb(const geometry_msgs::PoseWithCovarianceStamped&);

  /**
   * @brief Get the object pose
   *
   * @param wrt with respect to map
   * @return true Object detected
   * @return false Object not detected
   */
  bool get_object_pose(std::string wrt = "map");

  /**
   * @brief Robot spins to scan the room
   *
   */
  void turn_around();

  /**
   * @brief Pose of the trash object
   *
   */
  geometry_msgs::Pose objectPose;

  /**
   * @brief Enumerations of various states of the robot
   *
   */
  enum turning { TURN_START, TURNING, TURN_COMPLETE };

  turning turn_state;

  /**Goal positions for each room**/
  std::array<XmlRpc::XmlRpcValue, 5> ex_goal_pos;

 private:
  /** Node Handle created*/
  ros::NodeHandle* nh;

  /** TF buffer created*/
  tf2_ros::Buffer tf_Buffer;

  /** TF listener created*/
  tf2_ros::TransformListener tf_listener;

  /** subscriber to get current robot pose  */
  ros::Subscriber cur_pose_sub_;

  /** Publisher to publish the velocity of the robot  */
  ros::Publisher vel_pub_;

  /** Current pose of the robot**/
  geometry_msgs::Pose current_pose_;



  /** stores initial robot orientation for the robot to rotate  */
  tf2::Quaternion initial_tf_quat_;

  /** checks if robot pose is received  */
  bool is_pose_initialized_;

  /**
   * @brief Set the turning velocity of the robot
   *
   */
  void set_turning_velocity();

  /**
   * @brief Get the pos for each room
   *
   */
  void get_room_pos();
};

#endif  // INCLUDE_WALL_E_NAVIGATION_HPP_
