#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/Joy.h>
#include <iostream>
#include <std_msgs/Float64.h>
#include <cmath>
using namespace std;
class wheeltec_joy
{
public:
    wheeltec_joy();
private:
    void callback(const sensor_msgs::Joy::ConstPtr& Joy); 
    //实例化节点
    ros::NodeHandle n; 
    ros::Subscriber sub ;
    ros::Publisher pub ;
};

wheeltec_joy::wheeltec_joy() 
{
    ros::NodeHandle private_nh("~"); //创建节点句柄
    pub = n.advertise<geometry_msgs::PoseStamped>("imagine_pose",1);//将速度发给机器人底盘节点
    sub = n.subscribe<sensor_msgs::Joy>("joy",10,&wheeltec_joy::callback,this); //订阅手柄发来的数据
} 

void wheeltec_joy::callback(const sensor_msgs::Joy::ConstPtr& Joy) //键值回调函数
{
    double up_down_key,left_right_key;
    double degrees = 45.0;
    double radians = degrees * M_PI / 180.0;
    double sin_theta = sin(radians);
    double cos_theta = cos(radians);
    int down_left_key, down_right_key, up_left_key, up_right_key;
    geometry_msgs::PoseStamped pose;

    up_down_key =Joy->axes[7]; 
    left_right_key =Joy->axes[6];
    down_left_key = Joy->buttons[0];
    down_right_key = Joy->buttons[1];
    up_left_key = Joy->buttons[2];
    up_right_key = Joy->buttons[3];

    pose.header.stamp = ros::Time().now();
    pose.header.frame_id = "body";
    pose.pose.position.z = 0.0;
    pose.pose.orientation.x = 0.0;
    pose.pose.orientation.y = 0.0;
    pose.pose.orientation.z = 0.0;
    pose.pose.orientation.w = 1.0;
    
    if (up_down_key != 0){
      if (up_down_key == 1){
        pose.pose.position.x = 1.0;
        pose.pose.position.y = 0.0;
      }
      else{
        pose.pose.position.x = -1.0;
        pose.pose.position.y = 0.0;
      }
    }
    if (left_right_key != 0){
      if (left_right_key == 1){
        pose.pose.position.x = 0.0;
        pose.pose.position.y = 1.0;
      }
      else{
        pose.pose.position.x = 0.0;
        pose.pose.position.y = -1.0;
      }
    }
    if (up_left_key == 1){
      pose.pose.position.x = cos_theta;
      pose.pose.position.y = sin_theta;
    }
    if (up_right_key == 1){
      pose.pose.position.x = cos_theta;
      pose.pose.position.y = -sin_theta;
    }
    if (down_left_key == 1){
      pose.pose.position.x = -cos_theta;
      pose.pose.position.y = sin_theta;
    }
    if (down_right_key == 1){
      pose.pose.position.x = -cos_theta;
      pose.pose.position.y = -sin_theta;
    }
    pub.publish(pose);
  
}
int main(int argc,char** argv)
{
    ros::init(argc, argv, "joy_control");
    wheeltec_joy teleop_turtle;
    ros::spin();
    return 0;

} 
