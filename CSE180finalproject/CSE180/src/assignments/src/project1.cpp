#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseStamped.h> 
#include <geometry_msgs/PoseWithCovarianceStamped.h> 
#include <math.h>
#include <tf/tf.h>
#include <stdlib.h>
#include <time.h>

//double Count = 0;
int x =0;
int y =0;

void myrandom(){
	x = (rand()% 19 - 9);
	y = (rand()% 19 - 9);

}
   
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
  
 int main(int argc, char** argv){
   ros::init(argc, argv, "project1");
	

	//x = rand()% 19 - 9;
	//y = rand()% 19 - 9;
	myrandom();
    //tell the action client that we want to spin a thread by default
     MoveBaseClient ac("move_base", true);
     //wait for the action server to come up
     while(!ac.waitForServer(ros::Duration(5.0))){
       ROS_INFO_STREAM("Waiting for the move_base action server to come up"<<std::endl);
     }
	

   
     move_base_msgs::MoveBaseGoal goal;
	while(ros::ok()){
  //for (int k =0; k < numberCount; k++){
	
     //we'll send a goal to the robot to move 1 meter forward
     goal.target_pose.header.frame_id = "map";
     goal.target_pose.header.stamp = ros::Time::now();
   

	
     goal.target_pose.pose.position.x = x;
     goal.target_pose.pose.position.y = y;
goal.target_pose.pose.orientation.w = 1.0;
    // goal.target_pose.pose.orientation.w = 1.0;
	ROS_INFO_STREAM("point for X: "<<goal.target_pose.pose.position.x<<std::endl);
	ROS_INFO_STREAM("point for Y: "<<goal.target_pose.pose.position.y<<std::endl);
   

     //ROS_INFO("Sending goal");
     ac.sendGoal(goal);
   //ac.waitForResult(ros::Duration(20));
     ac.waitForResult();
   

     		if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
			ROS_INFO("The base succeed to move to location");
			myrandom();
		}
     		else if (ac.getState() == actionlib::SimpleClientGoalState::ABORTED){
       			ROS_INFO("The base failed to move to location");
			myrandom();
   		}
		else if(ac.getState() == actionlib::SimpleClientGoalState::PENDING){
			ac.waitForResult(ros::Duration(20));
		}
		//else{
			//myrandom();

		//}
	}
		
     return 0;
   }
