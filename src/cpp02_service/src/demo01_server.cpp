#include "rclcpp/rclcpp.hpp"
#include "base_interfaces/srv/add_two_ints.hpp"

using base_interfaces::srv::AddTwoInts;
using std::placeholders::_1;
using std::placeholders::_2;

class MyServer: public rclcpp::Node 
{
public:
  MyServer(std::string name)
  : Node(name)
  , cnt(0)
  {
    AddServer = this->create_service<AddTwoInts>(
      "add_server",
      std::bind(&MyServer::add_callback,this,_1,_2)
    );
  }

private:
  int cnt;
  rclcpp::Service<AddTwoInts>::SharedPtr AddServer;

  void add_callback(const AddTwoInts::Request::SharedPtr req, 
                    AddTwoInts::Response::SharedPtr res) const
  {
    res->sum = req->num1 + req->num2;
    RCLCPP_INFO(this->get_logger(),"sum=%d\n",res->sum);
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MyServer>("server_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}