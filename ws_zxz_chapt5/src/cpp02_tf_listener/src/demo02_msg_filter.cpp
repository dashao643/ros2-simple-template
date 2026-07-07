#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.hpp"
#include "tf2_ros/buffer.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

#include "tf2_ros/create_timer_ros.hpp"
#include "message_filters/subscriber.h"
#include "tf2_ros/message_filter.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

using namespace std::placeholders;
using namespace std::chrono_literals;

class TF_PointListener : public rclcpp::Node
{
public:
  TF_PointListener(std::string name)
    : Node(name)
  {
    RCLCPP_INFO(this->get_logger(), "坐标点变换节点创建");
    /********************* 构造buffer *********************/
    buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());

    // rclcpp::node_interfaces::NodeBaseInterface::SharedPtr node_base, 
    // rclcpp::node_interfaces::NodeTimersInterface::SharedPtr node_timers, 
    timer_ = std::make_shared<tf2_ros::CreateTimerROS>(
      this->get_node_base_interface(),
      this->get_node_timers_interface());

    // tf2_ros::CreateTimerInterface::SharedPtr create_timer_interface
    buffer_->setCreateTimerInterface(timer_);

    /********************* buffer_赋值给listener *********************/
    listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_, this);

    /********************* 订阅坐标点 *********************/
    pointSub_.subscribe(this, "topic_point");

    /********************* listener_ 和 pointSub_ 赋值给过滤器 *********************/ 
    // F &f, 
    // tf2_ros::Buffer &buffer, 
    // const std::string &target_frame, 
    // uint32_t queue_size, 
    // const rclcpp::Node::SharedPtr &node, 换成 node_logging node_clock
    // std::chrono::duration<TimeRepT, TimeT> buffer_timeout
    filter_ = std::make_shared<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>>(
      pointSub_, *buffer_, "base_link", 10,       
      this->get_node_logging_interface(),
      this->get_node_clock_interface(), 
      1s
    );
    // 解析数据
    filter_->registerCallback(&TF_PointListener::pointCallback, this);
  }

private:
  std::unique_ptr<tf2_ros::Buffer> buffer_;
  std::shared_ptr<tf2_ros::TransformListener> listener_;
  std::shared_ptr<tf2_ros::CreateTimerROS> timer_;
  message_filters::Subscriber<geometry_msgs::msg::PointStamped> pointSub_;
  std::shared_ptr<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>> filter_;

  void pointCallback(const geometry_msgs::msg::PointStamped &pointStamped)
  {
    // 被转换的数据 目标坐标系
    auto resPs = buffer_->transform(pointStamped, "base_link");
    // 坐标系若不存在，会抛异常
    RCLCPP_INFO(this->get_logger(), 
      "坐标点相对于base_link的坐标:(%.2f,%.2f,%.2f)",
      resPs.point.x,
      resPs.point.y,
      resPs.point.z
    );
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<TF_PointListener>("sub_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}
