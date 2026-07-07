#include "rclcpp/rclcpp.hpp"
// 发布消息类型
#include "geometry_msgs/msg/point_stamped.hpp"

using geometry_msgs::msg::PointStamped;
using namespace std::placeholders;
using namespace std::chrono_literals;

class PointBc : public rclcpp::Node
{
public:
  PointBc(std::string name)
    : Node(name)
  {
    RCLCPP_INFO(this->get_logger(), "发布节点创建");
    pub_ = this->create_publisher<PointStamped>("topic_point", 10);
    timer_ = this->create_wall_timer(1000ms, std::bind(&PointBc::timerCallback, this));
  }

private:
  void timerCallback()
  {
    PointStamped ps;
    // 发布相对于 frame_id 坐标系的坐标点数据
    ps.header.frame_id = "laser";
    // ps.header.frame_id = "base_link";
    ps.header.stamp = this->now();
    z_ += 0.1;
    if(z_ >= 1)
      z_ = 0;
    ps.point.x = 0.0;
    ps.point.y = 0.0;
    ps.point.z = z_;
    pub_->publish(ps);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<PointStamped>::SharedPtr pub_;
  double z_ = 0.0;
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<PointBc>("node_pub");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}