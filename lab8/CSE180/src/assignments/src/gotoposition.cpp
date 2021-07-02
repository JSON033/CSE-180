#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <math.h>
#include <std_msgs/Float64.h>
#include <nav_msgs/Odometry.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <iostream>
#include <tf/tf.h>

float X;
float Y;
float THETA;
float x;
float y;
nav_msgs::Odometry coolio;



void basecallback (const nav_msgs::Odometry& base){
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

ros:: Subscriber bub = nh.subscribe("/base_pose_ground_truth",1000, &basecallback);

ros:: Publisher pub = nh.advertise<geometry_msgs::Twist>("/pioneer/cmd_vel",1000);

geometry_msgs::Twist location;


double roll, pitch, yaw, distance, turn;
turn = atan2(Y, X) ;
bool spin = true;
bool straight = true;
ros::Rate rate(1);
ros::spinOnce();
while (ros::ok()){
ros::spinOnce();

tf::Quaternion q(coolio.pose.pose.orientation.x,coolio.pose.pose.orientation.y,coolio.pose.pose.orientation.z,coolio.pose.pose.orientation.w);


//tf2:: Matrix3x3 R(q);

//R.getRPY(roll, pitch, yaw);
 yaw = tf::getYaw(q);

distance = sqrt(((coolio.pose.pose.position.x - X)*(coolio.pose.pose.position.x - X)) +((coolio.pose.pose.position.y - Y)*(coolio.pose.pose.position.y - Y)));


std::cout << yaw << std::endl;
std::cout << " turn " << turn <<std::endl;
std::cout << "distance " <<distance << std::endl;


if (spin){

	if ((yaw <=  turn+0.1) && (yaw >= turn -0.1 )){

	location.angular.z = 0;
	spin = false;

	
	}	 

	
	
	else {

		location.angular.z = turn - yaw;
	if ((turn - yaw) > 0.1 || turn -yaw < -0.1){
		if ((turn - yaw) > 0.1){
		location.angular.z = 0.1;
		}
		if ((turn -yaw) < -0.1){
		location.angular.z = -0.1;
		}


	}
}
}
if (spin == false && straight == true){
	
	if (distance >= 0.1){
		location.linear.x = distance;
		if(distance > 0.1){
		location.linear.x = 0.1;

		}
	}
	else {
	location.linear.x = 0;
	straight = false;
	}

}
if (spin == false && straight == false){
	if ((yaw <=  THETA+0.1) && (yaw >= THETA -0.1 )){

	location.angular.z = 0;
	

	
	}	 

	
	
	else {

		location.angular.z = THETA - yaw;
	if ((THETA - yaw) > 0.1 || THETA -yaw < -0.1){
		if ((THETA - yaw) > 0.1){
		location.angular.z = 0.1;
		}
		if ((THETA -yaw) < -0.1){
		location.angular.z = -0.1;
		}


	}
}
	

}

pub.publish(location);
rate.sleep();





}



}

