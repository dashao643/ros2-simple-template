#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using example_interfaces::srv::AddTwoInts;
using namespace std::placeholders;

class ServiceAdd: public rclcpp::Node 
{
public:
  ServiceAdd(std::string name)
  : Node(name)
  , cnt_(0)
  {
    RCLCPP_INFO(this->get_logger(),"create service node");
    addService_ = this->create_service<AddTwoInts>("add",std::bind(&ServiceAdd::addCallback, this, _1, _2));
  }

private:
  int cnt_;
  rclcpp::Service<AddTwoInts>::SharedPtr addService_;

  void addCallback(const AddTwoInts::Request::SharedPtr req, AddTwoInts::Response::SharedPtr res) const
  {
    res->sum = req->a + req->b;
    RCLCPP_INFO(this->get_logger(),"sum=%ld", res->sum);
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<ServiceAdd>("add_service_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}