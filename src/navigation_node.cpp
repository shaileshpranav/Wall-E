/**
 * @file navigation_node.cpp
 * @author Shailesh Pranav Rajendran (spraj@umd.edu)
 * @brief Navigation Node
 * @version 0.1
 * @date 2022-12-14
 * 
 * @copyright MIT License (c) 2022 Shailesh Pranav Rajendran
 * 
 */
#include "../include/wall-e/navigation.hpp"

int main(int argc, char** argv) {
  ros::init(argc, argv,
            "simple_navigation_goals");  // simple_navigation_goals node starts
  ros::NodeHandle nh;
  Navigation nav(&nh);
  //   nav.get_room_pos();
  
  nav.set_room_pos(nav.ex_goal_pos);
  MoveBaseClient explorer_client("/explorer/move_base", true);

  // wait for the action server to come up
  while (!explorer_client.waitForServer(ros::Duration(5.0))) {
    ROS_INFO("Waiting for the move_base action server to come up for explorer");
  }
  bool explorer_goal_sent = false;
  bool obj_found{false};
  bool final{false};
  ros::Rate loop_rate(10);
  int cnt_ex = 0;  // Number of goals visited by explorer
  while (ros::ok()) {
    if (cnt_ex < 5) {
      if (!explorer_goal_sent && !obj_found) {
        std::cout << "Sending goal";
        ROS_INFO("Sending goal for explorer");
        explorer_client.sendGoal(
            nav.explorer_goal[cnt_ex]);  // goal sent to explorer
        explorer_goal_sent = true;
      }
      if (explorer_client.getState() ==
          actionlib::SimpleClientGoalState::SUCCEEDED) {
        ROS_INFO("CNT_EX = %d", cnt_ex);
        if (cnt_ex != 4) {
          do {
            nav.turn_around();
            if (nav.get_object_pose("map")) {
              obj_found = true;
              explorer_client.sendGoal(nav.final);
              break;
            }
          } while (!nav.turn_state == nav.TURN_COMPLETE);
          // loop_rate.sleep();
          // ROS_INFO("CALLING LISTENER");
          // loop_rate.sleep();
        }
        nav.det = false;
        ros::spinOnce();
        loop_rate.sleep();

        cnt_ex++;
        explorer_goal_sent = false;
        ROS_INFO("Hooray, Explorer reached goal");
      }
    }
  }
}
