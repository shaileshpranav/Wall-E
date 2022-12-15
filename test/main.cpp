/**
 * @file main_test.cpp
 * @author Shailesh Pranav Rajendran (spraj@umd.edu)
 * @brief Main test script
 * @version 0.1
 * @date 2022-12-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <gtest/gtest.h>
#include <ros/ros.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "TestingNode");
    ros::NodeHandle nh;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}