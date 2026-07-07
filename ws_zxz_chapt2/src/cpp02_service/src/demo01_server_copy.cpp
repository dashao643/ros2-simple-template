#include "rclcpp/rclcpp.hpp"
#include "base_interfaces/srv/string.hpp"

using namespace std::placeholders;
using base_interfaces::srv::String;

class MyServer: public rclcpp::Node 
{
public:
  MyServer(std::string name)
  : Node(name)
  , cnt(0)
  {
    server = this->create_service<String>("name",std::bind(&MyServer::call_back,this,_1,_2));
    RCLCPP_INFO(this->get_logger(),"server listening");
  }
  
private:
  int cnt;
  rclcpp::Service<String>::SharedPtr server;

  void call_back(const String::Request::SharedPtr req, String::Response::SharedPtr res) const
  {
    // odd
    if(req->num%2){
      res->name = "dashao";
    } 
    // even
    else{
      res->name = "fenkai";
    }
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