#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class InfoPublisher: public rclcpp::Node 
{
public:
	InfoPublisher(std::string name)
  : Node(name)
  , cnt_(0)
  {
    infoPub_ = this->create_publisher<std_msgs::msg::String>("topic",10);
    timer_ = this->create_wall_timer(1s,std::bind(&InfoPublisher::timer_callback,this));
		RCLCPP_INFO(this->get_logger(),"hello dashao");
	}

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr infoPub_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t cnt_;

  void timer_callback(void)
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello World" + std::to_string(cnt_++);
    infoPub_->publish(message);
    RCLCPP_INFO(this->get_logger(),"publish%s\n", message.data.c_str());
  }
};

int main(int argc, char* argv[])
{
	rclcpp::init(argc, argv);
	
	rclcpp::spin(std::make_shared<InfoPublisher>("dashao_node"));
	
	rclcpp::shutdown();

	return 0;
}
