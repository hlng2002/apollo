/*********************************************************************
*
* Software License Agreement (TBD)
*
*  Copyright (c) 2017, XXX
*  All rights reserved.
*
*
*********************************************************************/

#include "modules/drivers/usb_cam_new/nodes/usb_cam_wrapper.h"

#include <boost/thread.hpp>

#include <ros/ros.h>
#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>

namespace usb_cam_new {

class UsbCamNodelet: public nodelet::Nodelet {
 public:
  UsbCamNodelet() {}
  ~UsbCamNodelet() {
    ROS_INFO("shutting down driver thread");
    if (device_thread_ != nullptr && device_thread_->joinable()) {
      device_thread_->join();
    }
    ROS_INFO("driver thread stopped");
  }

 private:
  virtual void onInit();
  boost::shared_ptr<UsbCamWrapper> usb_cam_ = nullptr;
  boost::shared_ptr<boost::thread> device_thread_ = nullptr;
};

void UsbCamNodelet::onInit() {
  ROS_INFO("Usb cam nodelet init");
  usb_cam_.reset(new UsbCamWrapper(getNodeHandle(), getPrivateNodeHandle()));
  // spawn device poll thread
  device_thread_ = boost::shared_ptr<boost::thread>
        (new boost::thread(boost::bind(&UsbCamWrapper::spin, usb_cam_)));
}

}  // namespace usb_cam_new

// Register this plugin with pluginlib.  Names must match nodelets.xml.
//
// parameters: package, class name, class type, base class type
PLUGINLIB_DECLARE_CLASS(usb_cam, UsbCamNodelet,
                        usb_cam_new::UsbCamNodelet, nodelet::Nodelet);
