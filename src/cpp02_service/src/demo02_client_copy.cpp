#include "rclcpp/rclcpp.hpp"
#include "base_interfaces/srv/string.hpp"

using base_interfaces::srv::String;
using namespace std::chrono_literals;

class MyClient: public rclcpp::Node 
{
public:
  MyClient(std::string name)
  : Node(name)
  , cnt_(0)
  {
    client_ = create_client<String>("name");
  }

  bool connect()
  {
    while(!client_->wait_for_service(1s)){
      if(!rclcpp::ok()){
        RCLCPP_INFO(rclcpp::get_logger("debug"),"program abort");
        return false;
      }
      RCLCPP_INFO(rclcpp::get_logger("debug"),"connecting...");
    }
    RCLCPP_INFO(rclcpp::get_logger("debug"),"connected");
    return true;
  }

  rclcpp::Client<String>::FutureAndRequestId sendToServer(int num)
  {
    auto req = std::make_shared<String::Request>();
    req->num = num;
    return client_->async_send_request(req);
  }
private:
  int cnt_;
  rclcpp::Client<String>::SharedPtr client_;
};

int main(int argc, char* argv[])
{
  if(argc != 2) return -1;

  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MyClient>("ros2_node");
  // rclcpp::spin(node);
  if(!node->connect()) return -2;
  
  auto future = node->sendToServer(atoi(argv[1]));

  if(rclcpp::spin_until_future_complete(node,future) == rclcpp::FutureReturnCode::SUCCESS){
    std::string name = future.get()->name;
    RCLCPP_INFO(rclcpp::get_logger("debug"), "%s\n", name.c_str());
  }
  
  rclcpp::shutdown();
  
  return 0;
}