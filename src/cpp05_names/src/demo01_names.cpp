#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std_msgs::msg;

class MyNode: public rclcpp::Node 
{
public:
  MyNode(std::string name)
  : Node(name,"dashao_name")
  , cnt_(0)
  {
    RCLCPP_INFO(this->get_logger(),"create node");
    // 全局话题名称
    // public_ = this->create_publisher<String>("/t1/node_name",10);
    // 相对话题名称
    // public_ = this->create_publisher<String>("node_name",10);
    // 私有话题名称
    public_ = this->create_publisher<String>("~/node_name",10);
  }

private:
  int cnt_;
  rclcpp::Publisher<String>::SharedPtr public_;
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MyNode>("ros2_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();

  return 0;
}
