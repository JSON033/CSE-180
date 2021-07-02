#include <ros/ros.h>
#include <vector>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <cmath>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf2_ros/transform_listener.h>


#include <laser_geometry/laser_geometry.h>
#include <sensor_msgs/PointCloud.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Point.h>

using namespace std;

//contains the laser data and their original index
struct Sample{

  int   original_index;
  geometry_msgs::Point32 converted_range;
	float laser_range;
 
};
//ros variables
sensor_msgs::LaserScan laser;    
geometry_msgs::PoseWithCovarianceStamped robot_pose;    
laser_geometry::LaserProjection projector;   
float x;
float y;
float check=0;
tf::TransformListener* listener = NULL;
//normal variables
vector<Sample> data; //contains lasers that are closely together and also stores their original indeces
  Sample sample;


double averageX;
double averageY;
geometry_msgs::TransformStamped transformStamped;

void VectorCheck(vector<Sample> data){
	
	for(int i =0; i < data.size(); i ++){
		averageX += data[i].converted_range.x;
		averageY += data[i].converted_range.y;

	}




//ROS_INFO_STREAM("data " <<data.size()<<" points");
	averageX = averageX/data.size();
	averageY = averageY/data.size();

 //ROS_INFO_STREAM("averageX:: "<< averageX);
 //ROS_INFO_STREAM("averageY :: "<< averageY);


geometry_msgs::PointStamped pointIn;
geometry_msgs::PointStamped pointOut;  
tf2_ros::Buffer buffer;

pointIn.header.frame_id = "base_laser";
//pointIn.header.stamp = ros::Time::now();
pointIn.point.x = averageX;
pointIn.point.y = averageY;
pointIn.point.z = 0;

 //ROS_INFO_STREAM("pointOut :: "<< pointOut);


//(buffer);
//tf2::TransformListener:tf_;
 
try{
	//tf.transformPoint("map", ,pointOut)
	listener->transformPoint("map",pointIn, pointOut);

x = pointOut.point.x;
y = pointOut.point.y;
if(check != x){
 ROS_INFO_STREAM("point x :: "<< x);
 ROS_INFO_STREAM("point y :: "<< y);
}
check = x;


}
catch(tf::TransformException &ex){
	ROS_ERROR("FAILURE %s", ex.what());
	ros::Duration(1.0).sleep();
	//continue;
}

// ROS_INFO_STREAM("point x pre:: "<< pointOut.point.x);
 //ROS_INFO_STREAM("point y pre:: "<< pointOut.point.y);


//ROS_INFO_STREAM("Obtained Transformation "<< transformStamped);


}


bool Mailbox(vector<Sample> data){

    int cluster = data.size(); //length of cluster vector
	//int counter;
    if(cluster == 0){
	//ROS_INFO_STREAM("hello");
        return false;
    }

    float angle_increment = 0.0065540750511;
    float mailbox = .60;

 
    //calculation to get size of the array in meters
    float angle = (cluster-1)*angle_increment;
    float length = data[ cluster - 1 ].laser_range*asinf(angle/2);
    float width = 2*length;
 
    if( abs(width - mailbox) <= 0.15 &&abs(width - mailbox) >=0.1){
        return true;
    }
    else{
        return false;
    }

}

bool Table(vector<Sample> data){
	int cluster = data.size();

	if(cluster ==0){
		return false;
	}
	float angle_increment = 0.0065540750511;
	float tableleg = 0.15;

    float angle = (cluster-1)*angle_increment;
    float length = data[ cluster - 1 ].laser_range*asinf(angle/2);
    float width = 2*length;
 
    if( abs(width - tableleg) <= 0.05){
        return true;
    }
    else{
        return false;
    }

}



void amclCB(const geometry_msgs:: PoseWithCovarianceStamped::ConstPtr &msg){

robot_pose = *msg;
return;

}


void laserscanCB(const sensor_msgs::LaserScan&msg) {
   // laser = *msg;
    sensor_msgs::PointCloud converted;
	projector.projectLaser(msg,converted);

	//ROS_INFO_STREAM("Received " <<msg.ranges.size()<<" points");
	//ROS_INFO_STREAM("Converted "<<converted.points.size()<<" points");
	//ROS_INFO_STREAM("Points expressed in frame " <<converted.header.frame_id);

//if(msg.ranges)
    
    for(int i = 0; i < msg.ranges.size(); i++){
      
      //if values are close together insert into array
      if((abs(msg.ranges[i] - msg.ranges[i+1]) <= 0.1 && msg.ranges[i] <= 3 && msg.ranges[i] <= 3) ){

        sample.original_index = i;
        sample.laser_range = msg.ranges[i];
	sample.converted_range = converted.points[i];
        data.push_back(sample);

      }
	else {
	
	if(Table(data)){
   		//ROS_INFO_STREAM("data size " <<data.size());
		ROS_INFO_STREAM("table at : ");
		VectorCheck(data);
		
		}

	else if(Mailbox(data)){
		  //ROS_INFO_STREAM("data size " <<data.size());
		ROS_INFO_STREAM("Mailbox at : ");
		VectorCheck(data);
}
}
	}


data.clear();

	
}












int main(int argc,char ** argv) {

	ros::init(argc,argv,"lasertest");
	ros::NodeHandle nh;
	listener = new (tf::TransformListener);
 
  ros::Subscriber odom_sub = nh.subscribe("/amcl_pose", 1000, &amclCB);

  ros::Subscriber laserscanning = nh.subscribe("/scan", 1000, &laserscanCB);
  
  
  //vector< pair<float, float> > Object_Position;//a vector that contains the positions of the objects
 	
  ros::Rate r(1.0);

  while (ros::ok()){
    
    ros::spinOnce();
	//Vectorcheck();

r.sleep();
    }
  
    

  }

 
