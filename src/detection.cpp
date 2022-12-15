/**
 * @file detection.cpp
 * @author Aman Sharma (amankrsharma3@gmail.com)
 * @brief Detection class
 * @version 0.1
 * @date 2022-12-11
 *
 * @copyright MIT License (c) 2022 Shailesh Pranav Rajendran
 *
 */

#include "../include/wall-e/detection.hpp"

DetectObject::DetectObject(ros::NodeHandle* node_handle)
    : image_transport_(*node_handle) {
  // initiating the subscriber for the image
  image_sub_ = image_transport_.subscribe(
      "/explorer/camera/rgb/image_raw", 1, &DetectObject::image_cb, this,
      image_transport::TransportHints("compressed"));
  is_object_detected = false;
  cv::namedWindow("1", 0);
  cv::namedWindow("2", 0);
  ROS_INFO_STREAM("[DetectObject] DetectObject object initialized");
}
/**
 * @brief Function to be used for detecting the object to be collected. This
 * will give out true if the object is detected while printing the x and y
 * location for the object. otherwise it will return false
 *
 * @return true
 * @return false
 */
bool DetectObject::detect_object() {
  cv::cvtColor(img_bgr_, frame_hsv_, cv::COLOR_BGR2HSV);

  // changing the Color space of the image to make it more robotust
  cv::inRange(frame_hsv_, cv::Scalar(69, 50, 0), cv::Scalar(120, 255, 255),
              frame_thresh_);
  // Show the frames
  cv::findContours(frame_thresh_, contours_, CV_RETR_EXTERNAL,
                   CV_CHAIN_APPROX_SIMPLE);
  if (contours_.size() > 0) {
    //  bbox = cv::Rect(0, 0, 0, 0);
    std::vector<std::vector<cv::Point> > max_contour;
    max_contour.push_back(contours_.at(contours_.size() - 1));
    cv::Rect bbox = cv::boundingRect(max_contour.at(0));
    int x = bbox.x + bbox.width / 2;
    int y = bbox.y + bbox.height / 2;
    cv::circle(img_bgr_, cv::Point(x, y), 2, cv::Scalar(0, 255, 0), 2);
    if (is_object_detected == false) {
      is_object_detected = true;
      ROS_INFO_STREAM(
          "[ObjectDetector] Object Detected!! Its location in image is at X: "
          << x << ",\ty: " << y);
    }
  }
  cv::imshow("1", img_bgr_);
  cv::imshow("2", frame_thresh_);
  cv::waitKey(1);
  return true;
}

void DetectObject::image_cb(const sensor_msgs::ImageConstPtr& msg) {
  cv_bridge::CvImagePtr cvPtr;
  try {
    cvPtr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  } catch (cv_bridge::Exception& e) {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
  cvPtr->image.copyTo(img_bgr_);
  this->detect_object();
}
