#include "rclcpp/rclcpp.hpp"

// key -> value -- description
class MyParam: public rclcpp::Node 
{
public:
  MyParam(std::string name)
  : Node(name)
  , cnt_(0)
  {
    rclcpp::Parameter p1("person_name", "dashao");
    rclcpp::Parameter p2("person_height", 1.70);
    rclcpp::Parameter p3("person_age", 20);

    RCLCPP_INFO(this->get_logger(),"%s:%s",p1.get_name().c_str(),p1.as_string().c_str());
    RCLCPP_INFO(this->get_logger(),"%s:%.2f",p2.get_name().c_str(),p2.as_double());
    RCLCPP_INFO(this->get_logger(),"%s:%ld",p3.get_name().c_str(),p3.as_int());

    RCLCPP_INFO(this->get_logger(),"name=%s",p1.get_name().c_str());
    RCLCPP_INFO(this->get_logger(),"type=%s",p1.get_type_name().c_str());
    RCLCPP_INFO(this->get_logger(),"type2str=%s",p3.value_to_string().c_str());

    // rclcpp::ParameterValue pv(100);
    // pv.to_value_msg()
  }

private:
  int cnt_;
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MyParam>("ros2_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}