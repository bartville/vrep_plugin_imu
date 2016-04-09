// Copyright 2006-2016 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// -------------------------------------------------------------------
// THIS FILE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// You are free to use/modify/distribute this file for whatever purpose!
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.3.0 on February 19th 2016

#include "../include/vrep_plugin_imu/ros_server_imu.h"
#include "../include/v_repLib.h"

ros::NodeHandle* ROS_server::node = NULL;

// Services:
ros::ServiceServer ROS_server::displayText_server;

// Publishers:
ros::Publisher ROS_server::imu_publisher;

// Subscribers:
ros::Subscriber ROS_server::addStatusBarMessage_subscriber;

bool ROS_server::initialize()
{
	int argc = 0;
	char** argv = NULL;
    ros::init(argc,argv,"vrep");

	if(!ros::master::check())
		return(false);
	
	node=new ros::NodeHandle("~");

	// Enable the services:
	displayText_server = node->advertiseService("displayText",ROS_server::displayText_service);

	// Enable the publishers:
	imu_publisher= node->advertise<sensor_msgs::Imu>("imu",10);

	// Enable the subscribers:
	addStatusBarMessage_subscriber=node->subscribe("addStatusbarMessage",1,&ROS_server::addStatusbarMessage_callback);

	return(true);
}

void ROS_server::shutDown()
{
	// Disable the subscribers:
	addStatusBarMessage_subscriber.shutdown();

	// Disable the publishers:
	imu_publisher.shutdown();

	// Disable the services:
	displayText_server.shutdown();

	// Shut down:
	ros::shutdown();
}

void ROS_server::instancePass()
{ // When simulation is not running, we "spinOnce" here:
	int simState=simGetSimulationState();
	if ((simState&sim_simulation_advancing)==0)
		spinOnce();
}

void ROS_server::mainScriptAboutToBeCalled()
{ // When simulation is running, we "spinOnce" here:
	spinOnce();
}

void ROS_server::simulationAboutToStart()
{
}

void ROS_server::simulationEnded()
{
}

void ROS_server::spinOnce()
{
	// Disable error reporting (it is enabled in the service processing part, but we don't want error reporting for publishers/subscribers)
	int errorModeSaved;
	simGetIntegerParameter(sim_intparam_error_report_mode,&errorModeSaved);
	simSetIntegerParameter(sim_intparam_error_report_mode,sim_api_errormessage_ignore);

	//Handle all streaming (publishers)
	streamAllData();

	//Process all requested services and topic subscriptions
	ros::spinOnce();

	// Restore previous error report mode:
	simSetIntegerParameter(sim_intparam_error_report_mode,errorModeSaved); 
}

// Services:
bool ROS_server::displayText_service(vrep_skeleton_msg_and_srv::displayText::Request &req,vrep_skeleton_msg_and_srv::displayText::Response &res)
{
	res.dialogHandle=simDisplayDialog("Message from a ROS node",req.textToDisplay.c_str(),sim_dlgstyle_message,NULL,NULL,NULL,NULL);
	return true;
}

// Publishers:
void ROS_server::streamAllData()
{
	// Take care of publishers here (i.e. have them publish their data):

	float val[6];
//	std::string string_signal_name = imu_publisher.auxStr;
//  std::string string_signal_name = imu_publisher.auxStr;
	int signalLength;
    const char* signalValue=simGetStringSignal("imu_signal",&signalLength);

	if (signalValue!=0)
	{
		if (signalLength>=6*sizeof(float))
			{
				for (int i=0;i<6;i++)
					val[i]=((float*)signalValue)[i];
			}
	}
	sensor_msgs::Imu fl;
	//fl.header.seq=_simulationFrameID;
	fl.header.stamp=ros::Time::now();
	fl.orientation.x=0.0; //TODO orientation
	fl.orientation.y=0.0;
	fl.orientation.z=0.0;
	fl.orientation.w=1.0;
	fl.angular_velocity.x=(double)val[0];
	fl.angular_velocity.y=(double)val[1];
	fl.angular_velocity.z=(double)val[2];
	fl.linear_acceleration.x=(double)val[3];
	fl.linear_acceleration.y=(double)val[4];
	fl.linear_acceleration.z=(double)val[5];

	imu_publisher.publish(fl);


}

// Subscribers:
void ROS_server::addStatusbarMessage_callback(const std_msgs::String::ConstPtr& msg)
{
	simAddStatusbarMessage(msg->data.c_str());
}
