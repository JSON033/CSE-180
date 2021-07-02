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

float X;
float Y;
int countdown = 0;
nav_msgs::Odometry coolio;
sensor_msgs::LaserScan vision;
std_msgs::Bool goals;
//ranges middle is 540 or 360


bool orient = true;
bool pivot = false;
bool end = false;
bool keep = false;
bool check = false;


void hitormiss( const std_msgs::Bool& goalss){

goals.data = goalss.data;


}


void scammer(const geometry_msgs::Vector3& face){

X = face.x;
Y = face.y;

}

void scanner(const sensor_msgs::LaserScan& base){

vision = base;
countdown++;

}

void filtered( const nav_msgs::Odometry& letsparty){

coolio = letsparty;

}







int main (int argc, char** argv){

ros::init(argc, argv, "gotopoint");

ros:: NodeHandle nh;

ros:: Subscriber cub = nh.subscribe("/missorhit",1000, &hitormiss);
ros:: Subscriber sub = nh.subscribe("/scan",1000, &scanner);
ros:: Subscriber bub = nh.subscribe("/odometry/filtered",1000, &filtered);
ros:: Publisher pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1000);
ros:: Publisher lub = nh.advertise<std_msgs::Bool>("/hitormiss",1000);
ros:: Subscriber dub = nh.subscribe("/positions",1000, &scammer);





double yaw, turn , distance,blue, test, rick, morty; 

blue = 3.0;
 
geometry_msgs::Twist location;

turn = atan2(Y, X) ;
ros::Rate rate(10);
ros::Rate longrate(10);

while(countdown < 10){
ros::spinOnce();
}


ros::spinOnce();
while (ros::ok()){


ros::spinOnce();


turn = atan2(Y-coolio.pose.pose.position.y ,X- coolio.pose.pose.position.x ) ;
//rick =Y-coolio.pose.pose.position.y;
//morty = X- coolio.pose.pose.position.x;

tf::Quaternion q(coolio.pose.pose.orientation.x,coolio.pose.pose.orientation.y,coolio.pose.pose.orientation.z,coolio.pose.pose.orientation.w);
yaw = tf::getYaw(q);
distance = sqrt(((coolio.pose.pose.position.x - X)*(coolio.pose.pose.position.x - X)) +((coolio.pose.pose.position.y - Y)*(coolio.pose.pose.position.y - Y)));

if (orient == true && end == false){
if((yaw <=  turn+0.1) && (yaw >= turn -0.1 )){
	location.linear.x = 0;
	location.angular.z = 0;
	orient = false;
	pivot = true;
	keep = true;
std::cout <<"end turn" << std::endl;
	
	}	 
	
	else {
		location.linear.x = 0;
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
test = location.angular.z;

//std::cout << "ranges " << std::endl;
std::cout << "ranges 360 " << vision.ranges[360] << std::endl;
//std::cout << "ranges 270 " << vision.ranges[270] << std::endl;
//std::cout << "ranges 450 " << vision.ranges[450] << std::endl;
std::cout << "ranges 240 " << vision.ranges[240] << std::endl;
std::cout << "ranges 480 " << vision.ranges[480] << std::endl;


//std::cout << "rick " << rick << std::endl;
//std::cout << "morty" << morty << std::endl;

//std::cout <<"yaw " << yaw << std::endl;
//std::cout << " turn " << turn <<std::endl;
std::cout << "distance " <<distance << std::endl;
std::cout <<"test " << test << std::endl;

std::cout <<"X " << X << std::endl;
std::cout <<"Y " << Y << std::endl;
std::cout <<"bool " << std::endl;
if(goals.data == false){
	std::cout <<"false " << std::endl;
}
else{
	std::cout <<"true " << std::endl;
}




if  (pivot == true){
if(vision.ranges[360] > distance && vision.ranges[270] > distance&&vision.ranges[450] > distance && keep == true &&vision.ranges[480] > distance &&vision.ranges[240] > distance){

end = true;
//location.linear.x = 0.1;
//location.angular.z = 0;

	if (distance < 0.1){
	goals.data = true;
	location.linear.x = 0;
	location.angular.z = 0;
	end = false;
	pivot = false;
	orient = true;
	check = false;
	lub.publish(goals);
	rate.sleep();
	continue;


}
	if (distance >= 0.1){
	
	if( distance < 1 && check == false){
		goals.data = false;
std::cout << "checking here" <<std::endl;
		check = true;
		pivot = false;
		orient = true;
		end = false;
		continue;

			}
		location.linear.x = distance;
		if(distance > 0.1){
		location.linear.x = 0.1;
		location.angular.z = 0;
		}
	}
	else {
	goals.data = true;
	location.linear.x = 0;
	location.angular.z = 0;
	end = false;
	pivot = false;
	orient = true;
	check = false;
	
	}



}

else{

	if (vision.ranges[360] > blue &&vision.ranges[270] > blue && vision.ranges[450] > blue && vision.ranges[240] > blue  && vision.ranges[480] > blue){
		location.angular.z = 0;
	std::cout <<"moving forward" << std::endl;
		location.linear.x = 1;

	pub.publish(location);
	rate.sleep();
	for(int i = 0; i < 10; i++){
		std::cout << i << std::endl;
		pub.publish(location);
	rate.sleep();
		}
		orient = true;
		pivot = false;
		location.linear.x = 0;
	location.angular.z = 0;
		
			}

	else{

	location.angular.z = 0.1;
	location.linear.x = 0;
	keep = false;
		}



}

}






pub.publish(location);
lub.publish(goals);

rate.sleep();


}






}

