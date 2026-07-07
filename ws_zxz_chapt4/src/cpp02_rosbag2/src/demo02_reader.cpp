#include "rclcpp/rclcpp.hpp"
#include "rosbag2_cpp/reader.hpp"
#include "geometry_msgs/msg/twist.hpp"

// 3.定义节点类；
class SimpleBagPlayer: public rclcpp::Node {
public:
  SimpleBagPlayer():Node("simple_bag_player"){
    // 3-1.创建读取对象指针；
    reader_ = std::make_unique<rosbag2_cpp::Reader>();
    // 3-2.设置读取的目标文件；
    reader_->open("my_bag");
    // 3-3.读消息；
    while (reader_->has_next())
    {
      geometry_msgs::msg::Twist twist = reader_->read_next<geometry_msgs::msg::Twist>();
      RCLCPP_INFO(this->get_logger(),"%.2f ---- %.2f",twist.linear.x, twist.angular.z);
    }
    // 3-4.关闭文件。
    reader_->close();
  }
private:
  std::unique_ptr<rosbag2_cpp::Reader> reader_;

};

int main(int argc, char const *argv[])
{
  rclcpp::init(argc,argv);

  rclcpp::spin(std::make_shared<SimpleBagPlayer>());

  rclcpp::shutdown();

  return 0;
}