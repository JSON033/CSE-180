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




//double position[] ={2,2,4,0,9,0,9,2,4,2,4,4,9,4,9,5,4,5,4,6,9,6};
double position[] = { 0,5, 4,0,9,0,9,3,4,3,4,6,9,6};
int exe = 0, why = 1;

  typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
   
    int main(int argc, char** argv){
     ros::init(argc, argv, "simple_navigation_goals");
   
     //tell the action client that we want to spin a thread by default
     MoveBaseClient ac("move_base", true);
   
    //wait for the action server to come up
     while(!ac.waitForServer(ros::Duration(5.0))){
      ROS_INFO("Waiting for the move_base action server to come up");
     }
   
     move_base_msgs::MoveBaseGoal goal;
  
     //we'll send a goal to the robot to move 1 meter forward
     goal.target_pose.header.frame_id = "map";
     goal.target_pose.header.stamp = ros::Time::now();
  
	for(int i = 0; i < 7 ; i++){

     goal.target_pose.pose.position.x = position[exe + 2*i];
     goal.target_pose.pose.position.y = position[why + 2*i];
     goal.target_pose.pose.orientation.w = 1.0;
   
     ROS_INFO("Sending goal");
     ac.sendGoal(goal);
   
     ac.waitForResult();
   
     if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("Hooray, the base moved 1 meter forward");
    else
       ROS_INFO("The base failed to move forward 1 meter for some reason");
	}
  
     return 0;
   }


