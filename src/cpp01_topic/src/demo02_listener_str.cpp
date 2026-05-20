#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::placeholders;

class MySub: public rclcpp::Node 
{
public:
	MySub(std::string name)
  : Node(name)
  {
    sub_ = create_subscription<std_msgs::msg::String>("topic",10, std::bind(&MySub::sub_callback,this,_1));
	}

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;

  void sub_callback(std_msgs::msg::String str) const
  {
    RCLCPP_INFO(this->get_logger(),"sub:%s\n",str.data.c_str());
  }
};

int main(int argc, char* argv[])
{
	rclcpp::init(argc, argv);
	
	rclcpp::spin(std::make_shared<MySub>("sub_node"));
	
	rclcpp::shutdown();

	return 0;
}
