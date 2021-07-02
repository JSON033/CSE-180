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


double position[] ={-5,-5,-5,5,5,5,5,-5};
int npositions = 4;
int exe = 0, why = 1;
int np = 0;
int error = 0;
geometry_msgs::PoseWithCovarianceStamped old;
geometry_msgs::PoseWithCovarianceStamped currents;
geometry_msgs::PoseStamped goal;

void current( const geometry_msgs::PoseWithCovarianceStamped& curr){
currents = curr;
}


float distance2D(geometry_msgs::PoseStamped pose1, geometry_msgs::PoseWithCovarianceStamped pose2){
	float distance = sqrt(((pose1.pose.position.x - pose2.pose.pose.position.x)*(pose1.pose.position.x - pose2.pose.pose.position.x)) +((pose1.pose.position.y - pose2.pose.pose.position.y)*(pose1.pose.position.y - pose2.pose.pose.position.y)));

return distance;
}

float check(geometry_msgs::PoseWithCovarianceStamped pose1, geometry_msgs::PoseWithCovarianceStamped pose2){




float distance = 

sqrt(((pose1.pose.pose.position.x - pose2.pose.pose.position.x)*(pose1.pose.pose.position.x - pose2.pose.pose.position.x)) +((pose1.pose.pose.position.y - pose2.pose.pose.position.y)*(pose1.pose.pose.position.y - pose2.pose.pose.position.y)));

return distance;

}



int main (int argc, char** argv){

ros::init(argc, argv, "navigate");

ros:: NodeHandle nh;


ros:: Publisher pub = nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1000);

ros:: Subscriber sub = nh.subscribe("/amcl_pose",1000, &current);

goal.pose.position.x = position[exe];
goal.pose.position.y = position[why];
goal.pose.orientation.w = 1;
goal.header.frame_id = "map"; 

old.pose.pose.position.x = position[exe];
old.pose.pose.position.y = position[why];



ros::Rate rate(1.0);

ros::Duration wait (4.0);

while(ros::ok()){


ros::spinOnce();

if (distance2D(goal,currents) <= 0.4){
	np++;

	if (np == npositions){
	
	np = 0;
	

	}
	exe =  2*np;
	why = 1 + 2*np;
 	goal.pose.position.x = position[exe];
	goal.pose.position.y = position[why];
	
	pub.publish(goal);
	wait.sleep();
}
else{

	if(check(old, currents) == 0){

	error++;

	}


	pub.publish(goal);
	wait.sleep();
}

if (error > 3){
	ros::shutdown();
ROS_INFO_STREAM("Shutting DOwn");

}


	old = currents;

	rate.sleep();

}

}




