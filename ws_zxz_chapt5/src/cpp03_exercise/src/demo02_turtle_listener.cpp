#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.hpp"
#include "tf2_ros/buffer.hpp"
#include "tf2/LinearMath/Quaternion.h"

#include "geometry_msgs/msg/twist.hpp"

#include <math.h>

using namespace std::placeholders;
using namespace std::chrono_literals;
using geometry_msgs::msg::Twist;

// 控制谁，就把谁放 target

class TF_Listener : public rclcpp::Node
{
public:
  TF_Listener(std::string name)
    : Node(name)
  {
    RCLCPP_INFO(this->get_logger(), "坐标变换节点创建");

    // 声明并解析参数；
    this->declare_parameter("target_frame","turtle2");
    this->declare_parameter("source_frame","turtle1");
    target_frame_ = this->get_parameter("target_frame").as_string();  // turtle2 父级
    source_frame_ = this->get_parameter("source_frame").as_string();  // turtle1 子级

    // 控制 target_frame (新乌龟节点) 的速度 
    twist_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(target_frame_ + "/cmd_vel", 10);

    buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    // buffer_赋值给listener_,listener_监听 /tf 和 /tf_static 话题
    listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_, this);

    timer_ = this->create_wall_timer(1000ms, std::bind(&TF_Listener::timerCallback, this));
  }

private:
  std::unique_ptr<tf2_ros::Buffer> buffer_;
  std::shared_ptr<tf2_ros::TransformListener> listener_;
  rclcpp::TimerBase::SharedPtr timer_;

  rclcpp::Publisher<Twist>::SharedPtr twist_pub_;
  std::string target_frame_;  // 父级
  std::string source_frame_;  // 子级

  void timerCallback()
  {
    // 转换坐标系
    try{
      // res: 新的坐标帧 args: 父级坐标系(target) 子级坐标系(source) {source 在 target 里的位置} 时间点 
      // target_frame_: turtle2 父级
      // source_frame_: turtle1 子级
      // 获取 turtle1 相对于 turtle2 的坐标
      auto transformStamped = buffer_->lookupTransform(target_frame_, source_frame_, tf2::TimePointZero);

      // turtle2 -> turtle1
      double dx = transformStamped.transform.translation.x;
      double dy = transformStamped.transform.translation.y;

      double distance = std::sqrt(dx*dx + dy*dy);
      double theta = std::atan2(dy, dx);

      // 向 turtle2 发布速度指令
      geometry_msgs::msg::Twist twist;
      twist.linear.x = distance * 0.5;
      // theta 即为角度差
      twist.angular.z = theta;

      twist_pub_->publish(twist);
    }
    catch(const tf2::LookupException &e){
      RCLCPP_INFO(this->get_logger(), "坐标变换异常：%s", e.what());
    }
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<TF_Listener>("listener_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}
