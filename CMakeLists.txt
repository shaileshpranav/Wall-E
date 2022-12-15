cmake_minimum_required(VERSION 3.0.2)
project(wall-e)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
    gazebo_msgs
    geometry_msgs
    roscpp
    sensor_msgs
    std_msgs
    moveit_core
    moveit_ros_planning
    moveit_ros_planning_interface
    tf2_eigen
    tf2_geometry_msgs
    tf2_ros
    moveit_msgs
    image_transport
    cv_bridge
    control_msgs
    actionlib
    tf2
)

find_package(OpenCV COMPONENTS opencv_highgui opencv_features2d opencv_xfeatures2d opencv_calib3d)
if(NOT OPENCV_FOUND)
	find_package(OpenCV REQUIRED COMPONENTS opencv_highgui opencv_features2d opencv_calib3d)
	add_definitions(-DNO_CV_XFEATURES2D)
endif()


find_package(Boost REQUIRED system filesystem date_time thread)
find_package(Eigen3 REQUIRED)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES wall-e
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")


add_library(navigation_library src/navigation.cpp)
add_library(navigation_node_library src/navigation_node.cpp)
add_library(object_spawner_library src/object_spawner.cpp)
add_library(detection_library src/detection.cpp)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)


add_executable(Navigation_node src/navigation_node.cpp)
target_link_libraries(Navigation_node
  navigation_library
  detection_library
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)

add_executable(object_spawner_node src/object_spawner.cpp)
target_link_libraries(object_spawner_node
object_spawner_library
  ${catkin_LIBRARIES}
)


#############
## Testing ##
#############


if(CATKIN_ENABLE_TESTING)
  find_package(rostest REQUIRED)
  find_package(OpenCV REQUIRED)  
#   find_package(cv_bridge REQUIRED)

  add_rostest_gtest(main_rostest
    test/main_test.test
    test/main.cpp
    test/navigation_test.cpp
    test/detection_test.cpp
    test/object_spawner_test.cpp

#     # src/navigation.cpp
#     # src/object_spawner.cpp
#     # src/detection.cpp
#     # src/navigation_node.cpp
  )
  target_link_libraries(main_rostest
    ${catkin_LIBRARIES}
    ${OpenCV_LIBRARIES}

    navigation_library
    detection_library
    object_spawner_library
    navigation_node_library
  )
endif()
