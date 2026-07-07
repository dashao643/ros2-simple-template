#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include <cmath>
#include <algorithm>

/*
指定位置创建一只新乌龟turtle2
订阅turtle1的位置话题/turtle1/pose
获取turtle1的 x y 坐标
发布/turtle2/cmd_vel(geometry_msgs/msg/Twist)控制turtle2朝 x y 坐标移动
*/

using turtlesim::msg::Pose;
using geometry_msgs::msg::Twist;
using namespace std::placeholders;
using namespace std::chrono_literals;

class TurtleFollow : public rclcpp::Node
{
public:
  TurtleFollow(std::string name)
    : Node(name)
  {
    RCLCPP_INFO(this->get_logger(), "乌龟跟随节点已创建,单发布订阅");

    subT1Pose_ = this->create_subscription<Pose>("/turtle1/pose", 10,
      std::bind(&TurtleFollow::subT1Callback, this, _1));

    subT2Pose_ = this->create_subscription<Pose>("/turtle2/pose", 10,
      std::bind(&TurtleFollow::subT2Callback, this, _1));

    pubT2Twist_ = this->create_publisher<Twist>("/turtle2/cmd_vel", 10);
    timer_ = this->create_wall_timer(200ms, std::bind(&TurtleFollow::timerCallback, this));
  }

private:
  rclcpp::Subscription<Pose>::SharedPtr subT1Pose_;
  rclcpp::Subscription<Pose>::SharedPtr subT2Pose_;
  rclcpp::Publisher<Twist>::SharedPtr pubT2Twist_;
  rclcpp::TimerBase::SharedPtr timer_;
  Pose t1Pose_;
  Pose t2Pose_;
  bool t1_received_ = false;
  bool t2_received_ = false;
  
  void subT1Callback(const Pose &pose)
  {
    t1Pose_ = pose;
    t1_received_ = true;
  }

  void subT2Callback(const Pose &pose)
  {
    t2Pose_ = pose;
    t2_received_ = true;
  }

  void timerCallback()
  {
    if (!t1_received_ || !t2_received_) return;

    double dx = t1Pose_.x - t2Pose_.x;
    double dy = t1Pose_.y - t2Pose_.y;
    double distance = std::sqrt(dx*dx + dy*dy);
    
    double targetTheta = getYawRad(t2Pose_.x, t2Pose_.y, t1Pose_.x, t1Pose_.y);
    double diff = targetTheta - t2Pose_.theta;
    while (diff > M_PI) 
      diff -= 2 * M_PI;
    while (diff < -M_PI) 
      diff += 2 * M_PI;

    Twist twist;
    twist.linear.x = std::min(distance * 1.5, 2.0);
    twist.angular.z = diff * 3.0;
    pubT2Twist_->publish(twist);
  }

  // 返回 p2 相对于 p1 的弧度，范围 [-π, π]
  double getYawRad(double x1, double y1, double x2, double y2) 
  {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::atan2(dy, dx);
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<TurtleFollow>("follow_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}
