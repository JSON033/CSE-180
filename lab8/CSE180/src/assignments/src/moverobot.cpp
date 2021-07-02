#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>

int main(int argc, char **argv){ 

ros::init(argc,argv,"moverobot");
ros::NodeHandle nh;

ros:: Publisher pub = nh.advertise<geometry_msgs::Twist>("/pioneer/cmd_vel",1000);
//ros:: Publisher turn = nh.advertise<geometry_msgs::Twist>("turn",1000);

ros:: Rate rate(1);

geometry_msgs:: Twist forward;
geometry_msgs:: Twist twist;

while (ros::ok()){


forward.linear.x = 1.0;
twist.angular.z=M_PI/2;


pub.publish(forward);
rate.sleep();
pub.publish(twist);
rate.sleep();


}

}
