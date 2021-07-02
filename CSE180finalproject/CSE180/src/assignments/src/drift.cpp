#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <math.h>
#include <std_msgs/Float64.h>
#include <nav_msgs/Odometry.h>


nav_msgs::Odometry ms;
nav_msgs::Odometry pos;

void odomcallback (nav_msgs::Odometry odo){

ms = odo;

}

void basecallback (nav_msgs::Odometry base){


pos = base;

}


int main (int argc, char** argv){

ros::init(argc, argv, "drift");

ros:: NodeHandle nh;



ros:: Subscriber sub = nh.subscribe("/pioneer/odom",1000, odomcallback);

ros:: Subscriber bub = nh.subscribe("/pioneer/base_pose_ground_truth",1000, basecallback);

ros:: Publisher pub = nh.advertise<std_msgs::Float64>("/posedrift",1000);

ros::Rate rate(1);

while (ros::ok()){

std_msgs::Float64 distance;
ros:: spinOnce();

distance.data = sqrt(((ms.pose.pose.position.x - pos.pose.pose.position.x)*(ms.pose.pose.position.x - pos.pose.pose.position.x)) + ((ms.pose.pose.position.y - pos.pose.pose.position.y)*(ms.pose.pose.position.y - pos.pose.pose.position.y)));
pub.publish(distance);



}

}









