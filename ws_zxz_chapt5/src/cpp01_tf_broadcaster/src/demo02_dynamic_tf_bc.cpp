#include "rclcpp/rclcpp.hpp"
// tf2动态发布
#include "tf2_ros/transform_broadcaster.hpp"
// 订阅消息类型
#include "turtlesim/msg/pose.hpp"
// 发布消息类型
#include "geometry_msgs/msg/transform_stamped.hpp"
// 欧拉角转换四元数
#include "tf2/LinearMath/Quaternion.hpp"

using turtlesim::msg::Pose;
using namespace std::placeholders;

class TF_DynamicBc : public rclcpp::Node
{
public:
  TF_DynamicBc(std::string name)
    : Node(name)
    // , cnt_(0)
  {
    broadcaseter_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
    sub_ = this->create_subscription<Pose>("/turtle1/pose", 10, 
      std::bind(&TF_DynamicBc::subCallback, this, _1)
    );
  }
  
private:
  std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaseter_;
  rclcpp::Subscription<Pose>::SharedPtr sub_;

  void subCallback(const Pose &pose)
  {
    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = this->now();

    transform.header.frame_id = "world";
    transform.child_frame_id = "turtle1";
    transform.transform.translation.x = pose.x;
    transform.transform.translation.y = pose.y;
    transform.transform.translation.z = 0;
    // 欧拉角转化成四元数
    tf2::Quaternion qtn;
    qtn.setRPY(0, 0, pose.theta);

    transform.transform.rotation.x = qtn.getX();
    transform.transform.rotation.y = qtn.getY();
    transform.transform.rotation.z = qtn.getZ();
    transform.transform.rotation.w = qtn.getW();

    broadcaseter_->sendTransform(transform);
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<TF_DynamicBc>("node_pub_d_tf");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}
