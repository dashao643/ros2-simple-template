
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces/msg/student.hpp"

using namespace std::chrono_literals;
using base_interfaces::msg::Student;

class PubStu: public rclcpp::Node 
{
public:
	PubStu(std::string name)
  : Node(name)
  , age(0)
  {
    pub_stu_ = this->create_publisher<Student>("stu_topic",10);
    timer_ = this->create_wall_timer(1s,std::bind(&PubStu::timer_callback,this));
	}

private:
  int age;
  rclcpp::Publisher<Student>::SharedPtr pub_stu_;
  rclcpp::TimerBase::SharedPtr timer_;

  void timer_callback()
  {
    Student stu;
    stu.age = age++;
    stu.height = 170.5;
    stu.name = "dashao";
    pub_stu_->publish(stu);
  }
};

int main(int argc, char* argv[])
{
	rclcpp::init(argc, argv);
	
	rclcpp::spin(std::make_shared<PubStu>("dashao_node"));
	
	rclcpp::shutdown();

	return 0;
}
