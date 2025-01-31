/*********************************************************************
 * Software License Agreement (CC BY-NC-SA 4.0 License)
 *
 *  Copyright (c) 2014, PAL Robotics, S.L.
 *  All rights reserved.
 *
 *  This work is licensed under the Creative Commons
 *  Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 *  To view a copy of this license, visit
 *  http://creativecommons.org/licenses/by-nc-sa/4.0/
 *  or send a letter to
 *  Creative Commons, 444 Castro Street, Suite 900,
 *  Mountain View, California, 94041, USA.
 *********************************************************************/

/*
 * @author Enrique Fernandez
 */

#include <ros/ros.h>
#include <ackermann_msgs/AckermannDrive.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include <string>

class AckermannMarker
{
public:

  AckermannMarker(double scale = 1.0, double z = 0.0, const std::string& frame_id = "base_footprint")
    : frame_id_(frame_id)
    , scale_(scale)
    , z_(z)
  {
    // ID and type:
    marker_.id = 0;
    marker_.type = visualization_msgs::Marker::ARROW;

    // Frame ID:
    marker_.header.frame_id = frame_id_;

    // Pre-allocate points for setting the arrow with the ackermann:
    marker_.points.resize(2);

    // Vertical position:
    marker_.pose.position.z = z_;

    // Scale:
    marker_.scale.x = 0.05 * scale_;
    marker_.scale.y = 2 * marker_.scale.x;

    // Color:
    marker_.color.a = 1.0;
    marker_.color.r = 0.0;
    marker_.color.g = 1.0;
    marker_.color.b = 0.0;
  }

  void update(const ackermann_msgs::AckermannDrive& ackermann)
  {
    marker_.points[1].x = ackermann.speed;
    marker_.points[1].y = ackermann.steering_angle;
  }

  const visualization_msgs::Marker& getMarker()
  {
    return marker_;
  }

private:
  visualization_msgs::Marker marker_;

  std::string frame_id_;
  double scale_;
  double z_;
};

class AckermannMarkerPublisher
{
public:

  AckermannMarkerPublisher(double scale = 1.0, double z = 0.0)
    : marker_(scale, z)
  {
    ros::NodeHandle nh;

    pub_ = nh.advertise<visualization_msgs::Marker>("marker", 1, true);
    sub_ = nh.subscribe("ackermann", 1, &AckermannMarkerPublisher::callback, this);
  }

  void callback(const ackermann_msgs::AckermannDriveConstPtr& ackermann)
  {
    marker_.update(*ackermann);

    pub_.publish(marker_.getMarker());
  }

private:
  ros::Subscriber sub_;
  ros::Publisher  pub_;

  AckermannMarker marker_;
};

int
main(int argc, char *argv[])
{
  ros::init(argc, argv, "ackermann_marker");

  AckermannMarkerPublisher t(1.0, 2.0);

  while (ros::ok())
  {
    ros::spin();
  }

  return EXIT_SUCCESS;
}

