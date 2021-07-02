#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <math.h>
#include <std_msgs/Float64.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <iostream>
#include <tf/tf.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseArray.h>

geometry_msgs::PoseWithCovarianceStamped pose;
geometry_msgs::PoseArray avgpose;
float px,py,pz,ox,oy,oz,ow;
float dx,dy,dz,doz,doy,dox,dow;
float size;

void averagecallback(const geometry_msgs::PoseArray& avg){

avgpose = avg;
size = avg.poses.size();

}

void possey( const geometry_msgs::PoseWithCovarianceStamped pse){


pose = pse;

}



int main (int argc, char** argv){

ros::init(argc, argv, "averagepose");

ros:: NodeHandle nh;


ros:: Subscriber sub = nh.subscribe("/particlecloud",1000, &averagecallback);
ros:: Subscriber bub = nh.subscribe("/amcl_pose",1000, &possey);




ros::Rate rate(10);

while(ros::ok()){


ros::spinOnce();

for(int i = 0; i< size; i++){

px += avgpose.poses[i].position.x;
py += avgpose.poses[i].position.y;
pz += avgpose.poses[i].position.z;

ox += avgpose.poses[i].orientation.x;
oy += avgpose.poses[i].orientation.y;
oz += avgpose.poses[i].orientation.z;
ow += avgpose.poses[i].orientation.w;


}

std::cout <<"computed average for x is : " << ( px/ size) << std::endl;
std::cout <<"computed average for y is : " <<  (py/ size) << std::endl;
std::cout <<"computed average for z is : "<< ( pz/ size) << std::endl;

std::cout <<"computed average for orientation x is :  " << (ox/ size) << std::endl;
std::cout <<"computed average for orientation y is :  " << (oy/ size) << std::endl;
std::cout <<"computed average for orientation z is :  " << ( oz/ size) << std::endl;
std::cout <<"computed average for orientation w is :  " << (ow/ size) << std::endl;

std::cout <<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<< std::endl;

dx = fabs((px /size) - pose.pose.pose.position.x);
dy = fabs((py /size) - pose.pose.pose.position.y);
dz = fabs((pz / size) - pose.pose.pose.position.z);

dox = fabs((ox /size) - pose.pose.pose.orientation.x);
doy = fabs((oy / size) - pose.pose.pose.orientation.y);
doz = fabs((oz /size) - pose.pose.pose.orientation.z);
dow = fabs((ow /size) - pose.pose.pose.orientation.w);

//std::cout << pose.pose.pose.position.x << std::endl;


std::cout <<"Difference between computed average and amcl_pose for x is : " <<  dx<< std::endl;
std::cout <<"Difference between computed average and amcl_pose for y is : " << dy<< std::endl;
std::cout <<"Difference between computed average and amcl_pose for z is : " << dz<< std::endl;

std::cout <<"Difference between computed average and amcl_pose for orientation x is :  " << dox<< std::endl;
std::cout <<"Difference between computed average and amcl_pose for orientation y is :  " << doy<< std::endl;
std::cout <<"Difference between computed average and amcl_pose for orientation z is :  " << doz<< std::endl;
std::cout <<"Difference between computed average and amcl_pose for orientation w is :  " << dow<< std::endl;

std::cout <<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<< std::endl;

px = 0;
py = 0;
pz = 0;
ox = 0;
oy = 0;
oz = 0;
ow = 0;



rate.sleep();




}











}
