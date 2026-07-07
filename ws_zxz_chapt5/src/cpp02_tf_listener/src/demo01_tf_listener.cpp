#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.hpp"
#include "tf2_ros/buffer.hpp"
#include "tf2/LinearMath/Quaternion.h"

// 坐标系变换：
// 发布camera相对于base_link的坐标
// 发布laser相对于base_link的坐标
// 计算出 source_frame 相对于 target_frame 的坐标关系

using namespace std::placeholders;
using namespace std::chrono_literals;

class TF_Listener : public rclcpp::Node
{
public:
  TF_Listener(std::string name)
    : Node(name)
  {
    RCLCPP_INFO(this->get_logger(), "坐标变换节点创建");
    buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    // buffer_赋值给listener_,listener_监听 /tf 和 /tf_static 话题
    listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_, this);

    timer_ = this->create_wall_timer(1000ms, std::bind(&TF_Listener::timerCallback, this));
  }

private:
  std::unique_ptr<tf2_ros::Buffer> buffer_;
  std::shared_ptr<tf2_ros::TransformListener> listener_;
  rclcpp::TimerBase::SharedPtr timer_;

  void timerCallback()
  {
    // 转换坐标系
    try{
      // res: 新的坐标帧 args: 父级坐标系(target) 子级坐标系(source) {source 在 target 里的位置} 时间点 
      // geometry_msgs::msg::TransformStamped lookupTransform(
      //   const std::string &target_frame, const std::string &source_frame, const tf2::TimePoint &time)
      auto transformStamped = buffer_->lookupTransform("camera", "laser", tf2::TimePointZero);
      RCLCPP_INFO(this->get_logger(),"----------------------转换结果----------------------");
      RCLCPP_INFO(this->get_logger(),"frame_id:%s",transformStamped.header.frame_id.c_str());
      RCLCPP_INFO(this->get_logger(),"child_frame_id:%s",transformStamped.child_frame_id.c_str());
      RCLCPP_INFO(this->get_logger(),"坐标:(%.2f,%.2f,%.2f)",
                transformStamped.transform.translation.x,
                transformStamped.transform.translation.y,
                transformStamped.transform.translation.z
      );
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
