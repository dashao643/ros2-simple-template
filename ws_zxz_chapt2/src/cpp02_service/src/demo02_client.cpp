#include "rclcpp/rclcpp.hpp"
#include "base_interfaces/srv/add_two_ints.hpp"

using base_interfaces::srv::AddTwoInts;
using namespace std::chrono_literals;

class MyClient: public rclcpp::Node 
{
public:
  MyClient(std::string name)
  : Node(name)
  , cnt(0)
  {
    client_ = this->create_client<AddTwoInts>("add_server");
    RCLCPP_INFO(this->get_logger(),"client create");
  }

  bool connectToServer()
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

  rclcpp::Client<AddTwoInts>::FutureAndRequestId send_request(int num1, int num2)
  {
    auto request = std::make_shared<AddTwoInts::Request>();
    request->num1 = num1;
    request->num2 = num2;
    return client_->async_send_request(request);
  }

private:
  int cnt;
  rclcpp::Client<AddTwoInts>::SharedPtr client_;
};

int main(int argc, char* argv[])
{
  if(argc != 3){
    RCLCPP_ERROR(rclcpp::get_logger("debug"),"input two argument");
    return -1;
  }

  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MyClient>("client_node");

  if(!node->connectToServer()){
    RCLCPP_INFO(rclcpp::get_logger("debug"),"connect server fail");
    return -2;
  }

  auto future = node->send_request(atoi(argv[1]),atoi(argv[2]));

  if(rclcpp::spin_until_future_complete(node,future) == rclcpp::FutureReturnCode::SUCCESS){
    RCLCPP_INFO(rclcpp::get_logger("debug"),"response success,sum=%d",future.get()->sum);
  }
  else{
    RCLCPP_INFO(rclcpp::get_logger("debug"),"response fail");
  }

  rclcpp::shutdown();
  
  return 0;
}