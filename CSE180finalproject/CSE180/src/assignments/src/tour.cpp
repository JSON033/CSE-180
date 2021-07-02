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

geometry_msgs::Vector3 trie;
double position[] ={1,2,5,6,0,7};
int npositions = 3;
std_msgs::Bool goals;
std_msgs::Bool check;

int exe = 0, why = 1;
int np = 0;
int npcheck = 0;


void hitormiss( const std_msgs::Bool& goalss){

goals = goalss;

if (goals.data == true && check.data == true){

goals.data = false;

}
check = goalss;
if(goals.data == false && check.data == true){
check.data = false;

}

}




int main (int argc, char** argv){

ros::init(argc, argv, "tour");

ros:: NodeHandle nh;


goals.data = false;
check.data = false;


ros:: Publisher grub = nh.advertise<std_msgs::Bool>("/missorhit",1000);
ros:: Subscriber cub = nh.subscribe("/hitormiss",1000, &hitormiss);
//ros:: Subscriber bub = nh.subscribe("/odometry/filtered",1000, &filtered);
ros:: Publisher pub = nh.advertise<geometry_msgs::Vector3>("/positions",1000);
//ros:: Publisher pub = nh.advertise<std_msgs::Bool>("/hitormiss",1000);

ros::Rate rate(10);

while (ros::ok()){

ros::spinOnce();

if(goals.data == false){
	trie.x = position[(exe +(2*np))];
	trie.y = position[(why +(2*np))];
}
else {
	np++;
	if (np == npcheck +2){
	np--;	
	}
	if (np == npositions){
	np = 0;
	}
	trie.x = position[(exe +(2*np))];
	trie.y = position[(why +(2*np))];
	npcheck = np;
	goals.data = false;
}

pub.publish(trie);
grub.publish(goals);










}

}
