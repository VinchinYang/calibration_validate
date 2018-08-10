#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/PointCloud2.h"
#include "beginner_tutorials/marker_6dof.h"

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
using namespace sensor_msgs;
using namespace message_filters;


/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void pointCloudCallback(const sensor_msgs::PointCloud2::ConstPtr& msg)
{
  ROS_INFO_STREAM("Velodyne scan received at " << msg->header.stamp.toSec());
}

void markerCallback(const beginner_tutorials::marker_6dof::ConstPtr& msg_rt)
{
  ROS_INFO_STREAM("marker_6dof received at " << msg_rt->header.stamp.toSec());
}

void callback(const sensor_msgs::PointCloud2::ConstPtr& msg_pc, const sensor_msgs::Image::ConstPtr& msg_image){
  ROS_INFO_STREAM("Velodyne scan received at " << msg_pc->header.stamp.toSec());
  ROS_INFO_STREAM("Image received at " << msg_image->header.stamp.toSec());
  pcl::PointCloud<pcl::PointXYZ> point_cloud;
  //pcl::PointCloud<myPointXYZRID> newPoint_cloud;
  pcl::fromROSMsg(*msg_pc, point_cloud);
  //pcl::transformPointCloud(pc, new_cloud, transf);

}


int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  //ros::Punlisher pub = projectedHandle.advertise<pcl::PointXYZ> ("projectedPoints", 1);
 
  //ros::Subscriber sub = n.subscribe("/pandar_points", 10, pointCloudCallback);
  //ros::Subscriber sub = n.subscribe("/lidar_camera_calibration_rt", 10, markerCallback);

  message_filters::Subscriber<sensor_msgs::PointCloud2> cloud_sub(n, "/pandar_points", 1);
  message_filters::Subscriber<beginner_tutorials::marker_6dof> rt_sub(n, "/lidar_camera_calibration_rt", 1);
  message_filters::Subscriber<sensor_msgs::Image> image_sub(n, "/camera/image", 1);
  //typedef sync_policies::ApproximateTime<sensor_msgs::PointCloud2, beginner_tutorials::marker_6dof> MySyncPolicy;
  typedef sync_policies::ApproximateTime<sensor_msgs::PointCloud2, sensor_msgs::Image> MySyncPolicy;
  Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), cloud_sub, image_sub);
  sync.registerCallback(boost::bind(&callback, _1, _2));
  ROS_INFO("sync, ok");
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}
