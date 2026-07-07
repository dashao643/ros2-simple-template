#include "rclcpp/rclcpp.hpp"
// tf2发布
#include "tf2_ros/static_transform_broadcaster.hpp"
// 发布消息类型
#include "geometry_msgs/msg/transform_stamped.hpp"
// 欧拉角转换四元数
#include "tf2/LinearMath/Quaternion.hpp"

// using namespace std::placeholders;

class TF_Broadcast : public rclcpp::Node
{
public:
  TF_Broadcast(std::string name, char* argv[])
    : Node(name)
  {
    RCLCPP_INFO(this->get_logger(), "TF发布成功");
    broadcaseter_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
    tfStaticPub(argv);
  }

private:
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaseter_;

  void tfStaticPub(char* argv[])
  {
    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = this->now();
    transform.header.frame_id = argv[7];
    transform.child_frame_id = argv[8];
    transform.transform.translation.x = atof(argv[1]);
    transform.transform.translation.y = atof(argv[2]);
    transform.transform.translation.z = atof(argv[3]);
    // 欧拉角转化成四元数
    tf2::Quaternion qtn;
    qtn.setRPY(atof(argv[4]), atof(argv[5]), atof(argv[6]));

    transform.transform.rotation.x = qtn.getX();
    transform.transform.rotation.y = qtn.getY();
    transform.transform.rotation.z = qtn.getZ();
    transform.transform.rotation.w = qtn.getW();

    broadcaseter_->sendTransform(transform);
  }
};

int main(int argc, char* argv[])
{
  if(argc != 9){
    RCLCPP_ERROR(rclcpp::get_logger("debug"), "请传入8个参数:x y z r p y baseId childId");
    return -1;
  }

  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<TF_Broadcast>("node_pub_s_tf", argv);
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}
