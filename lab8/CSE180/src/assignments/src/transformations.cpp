#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TransformStamped.h>
#include <stdlib.h>
#include <math.h>
#include <std_msgs/Float64.h>
#include <nav_msgs/Odometry.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <iostream>
#include <tf/transform_listener.h>
#include <tf2_msgs/TFMessage.h>

 tf2_msgs::TFMessage dune;
 tf2_msgs::TFMessage foot;
tf::Matrix3x3 p;
tf::Matrix3x3 f;
tf::Matrix3x3 transform;
geometry_msgs::Quaternion q;
geometry_msgs::Quaternion t;
tf::Quaternion fat;
tf::Quaternion chad;
//int wait = 0;

void staticcallback(const tf2_msgs::TFMessage& staz){

dune = staz;

//wait++;


}


void tfcallback(const tf2_msgs::TFMessage& rock){

foot = rock;

}

void convert(){
foot.transforms.resize(4);
dune.transforms.resize(4);
ROS_INFO_STREAM("transform matrix between pioneer and odom");
ros::spinOnce();
//ROS_INFO_STREAM("transform matrix between pioneer and odom");
q =dune.transforms[0].transform.rotation;
tf::quaternionMsgToTF(q,fat);
p.setRotation(fat);
//ROS_INFO_STREAM("transform matrix between pioneer and odom");
t =foot.transforms[0].transform.rotation;
tf::quaternionMsgToTF(t,chad);
f.setRotation(chad);
//ROS_INFO_STREAM("transform matrix between pioneer and odom");
for(int i = 0; i< 3; i++){

	for(int j = 0; j < 3; j++){
		for(int k = 0; k < 3; k++){

	transform[i][j] += p[i][k] * f[k][j];
		
}
}
}

ROS_INFO_STREAM(transform[0][0] << " " << transform[0][1] << " " << transform[0][2] << " "<< dune.transforms[0].transform.translation.x - foot.transforms[0].transform.translation.x);
ROS_INFO_STREAM(transform[1][0] << " " << transform[1][1] << " " << transform[1][2] << " " << dune.transforms[0].transform.translation.y - foot.transforms[0].transform.translation.y);
ROS_INFO_STREAM(transform[2][0] << " " << transform[2][1] << " " << transform[2][2] << " " << dune.transforms[0].transform.translation.z - foot.transforms[0].transform.translation.z );
ROS_INFO_STREAM("0" << " " << "0" << " " << "0" << " " << "1" );
}




int main (int argc, char** argv){

ros::init(argc, argv, "transformations");

ros:: NodeHandle nh;

ros:: Subscriber bub = nh.subscribe("/pioneer/tf_static",1000, staticcallback);

ros:: Subscriber sub = nh.subscribe("/pioneer/tf",1000, tfcallback);
//ros:: Publisher pub =nh.advertise("/pioneer/odom",1000);

tf::Quaternion quatty;
tf::Matrix3x3 m;
tf::Matrix3x3 p;
tf::Matrix3x3 f;
tf::TransformListener listener;


ros::Rate rate(1);

//while(wait < 5){

//ros::spinOnce();
//}

while (ros::ok()){


tf::StampedTransform transform;

try{

	listener.lookupTransform("/front_sonar", "/back_sonar",ros::Time(0),transform);

	}

catch(tf::TransformException &ex){

ROS_ERROR("%s",ex.what());
ros::Duration(1.0).sleep();
continue;
}

m.setRotation(transform.getRotation());
ROS_INFO_STREAM("transform matrix between front and back");
ROS_INFO_STREAM(m[0][0] << " " << m[0][1] << " " <<m[0][2] << " " << transform.getOrigin().x());
ROS_INFO_STREAM(m[1][0] << " " << m[1][1] << " " <<m[1][2] << " " << transform.getOrigin().y());
ROS_INFO_STREAM(m[2][0] << " " << m[2][1] << " " <<m[2][2] << " " << transform.getOrigin().z());
ROS_INFO_STREAM("0" << " " << "0" << " " << "0" << " " << "1");
for(int i = 0 ; i < 20; i++){
	ros::spinOnce();
}

//ROS_INFO_STREAM("transform matrix between pioneer and odom");
convert();


}



}
