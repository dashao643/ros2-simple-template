#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"

/*
订阅
话题：/turtle1/pose
话题消息类型：turtlesim/msg/Pose
话题消息参数：
float32 x
float32 y
float32 theta
float32 linear_velocity
float32 angular_velocity

发布
话题：/t2/turtle1/cmd_vel
话题消息类型：geometry_msgs/msg/Twist
话题消息参数：
Vector3  linear
	float64 x
	float64 y
	float64 z
Vector3  angular
	float64 x
	float64 y
	float64 z
*/

// using namespace turtlesim::msg;
using turtlesim::msg::Pose;
using geometry_msgs::msg::Twist;
using namespace std::placeholders;

class SubPub: public rclcpp::Node 
{
public:
  SubPub(std::string name)
  : Node(name)
  {
    RCLCPP_INFO(this->get_logger(),"create nodeSubPub");
    // 订阅用回调
    subPose_ = this->create_subscription<Pose>("/turtle1/pose", 10, std::bind(&SubPub::subPoseCallBack, this , _1));
    // 发布用定时器,此工程在订阅回调中发布
    pubTwist_ = this->create_publisher<Twist>("/t2/turtle1/cmd_vel", 10);
  } 

private:
  // 订阅pose
  rclcpp::Subscription<Pose>::SharedPtr subPose_;
  // 发布cmd_vel
  rclcpp::Publisher<Twist>::SharedPtr pubTwist_;

  void subPoseCallBack(const Pose &pose)
  {
    RCLCPP_INFO(this->get_logger(),"sub t1 pose");
    Twist twist;
    twist.linear.x = pose.linear_velocity;
    twist.angular.z = pose.angular_velocity;

    pubTwist_->publish(twist);
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<SubPub>("nodeSubPub");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}