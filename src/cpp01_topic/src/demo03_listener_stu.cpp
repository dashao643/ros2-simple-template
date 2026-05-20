#include "rclcpp/rclcpp.hpp"
#include "base_interfaces/msg/student.hpp"

using base_interfaces::msg::Student;
using namespace std::placeholders;

class MySub: public rclcpp::Node 
{
public:
	MySub(std::string name)
  : Node(name)
  , cnt(0)
  {
		sub_ = this->create_subscription<Student>("stu_topic",10,std::bind(&MySub::sub_callback,this,_1));
	}

private:
  int cnt;
	rclcpp::Subscription<Student>::SharedPtr sub_;

	void sub_callback(Student s)
	{
		RCLCPP_INFO(this->get_logger(),"%d,%s,%.2f\n",s.age,s.name.c_str(),s.height);
	}
};

int main(int argc, char* argv[])
{
	// ROS Client Library for C++
	rclcpp::init(argc, argv);
	
	rclcpp::spin(std::make_shared<MySub>("dashao_node"));
	
	rclcpp::shutdown();

	return 0;
}
