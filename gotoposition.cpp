#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <math.h>
#include <std_msgs/Float64.h>
#include <nav_msgs/Odometry.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <iostream>

float X;
float Y;
float THETA;
float x;
float y;
nav_msgs::Odometry coolio;



void basecallback (nav_msgs::Odometry base){
x = base.pose.pose.position.x;
y=  base.pose.pose.position.y;
coolio = base;

}


int main (int argc, char** argv){
std::cout << "input X" <<std::endl;
std:: cin >> X;
std::cout << "input Y" << std::endl;
std:: cin >> Y;
std::cout << "input Theta" << std::endl;
std:: cin >> THETA;


ros::init(argc, argv, "gotoposition");

ros:: NodeHandle nh;

ros:: Subscriber bub = nh.subscribe("/pioneer/base_pose_ground_truth",1000, basecallback);

ros:: Publisher pub = nh.advertise<geometry_msgs::Twist>("/pioneer/cmd_vel",1000);

geometry_msgs::Twist location;


double roll, pitch, yaw, distance, turn;
turn = atan2(Y, X) ;
bool spin = true;
bool straight = true;
ros::Rate rate(1);

while (ros::ok()){
ros::spinOnce();

tf2::Quaternion q(coolio.pose.pose.orientation.x,coolio.pose.pose.orientation.y,coolio.pose.pose.orientation.z,coolio.pose.pose.orientation.w);


tf2:: Matrix3x3 R(q);

R.getRPY(roll, pitch, yaw);

distance = sqrt(((coolio.pose.pose.orientation.x - X)*(coolio.pose.pose.orientation.x - X)) +((coolio.pose.pose.orientation.y - Y)*(coolio.pose.pose.orientation.y - Y)));


std::cout << yaw << std::endl;


if (spin){

	if (yaw != turn){

	location.angular.z = turn - yaw;
	if ((turn - yaw) > 0.5 || turn -yaw < -0.5){
		if ((turn - yaw) > 0.5){
		location.angular.z = 0.5;
		}
		if ((turn -yaw) < -0.5){
		location.angular.z = -0.5;
		}
	}	 

	}
	
	else {

	location.angular.z = 0;
	spin = false;
	}
}
if (spin == false && straight == true){
	
	if (distance != 0){
		location.linear.x = distance;
	}
	else {
	location.linear.x = 0;
	straight = false;
	}

}
if (spin == false && straight == false){
	if (yaw != THETA){
		if (yaw < THETA){

		location.angular.z = 0.1;
 		}
		if (yaw > THETA){
		location.angular.z = -0.1;
		}
	}
	

}

pub.publish(location);
rate.sleep();





}



}

