#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/teleport_relative.hpp"

using turtlesim::srv::TeleportRelative;
using namespace std::chrono_literals;

/*
相对位置传送乌龟
/turtle1/teleport_relative: turtlesim/srv/TeleportRelative
float32 linear
float32 angular
---
*/

class ClientAdd : public rclcpp::Node
{
public:
  ClientAdd(std::string name)
    : Node(name)
  {
    RCLCPP_INFO(this->get_logger(), "create client node");
    client_ = this->create_client<TeleportRelative>("/turtle1/teleport_relative");
  }

  bool connectToService(void)
  {
    while(!client_->wait_for_service(1s)){
      if(!rclcpp::ok()){
        RCLCPP_INFO(rclcpp::get_logger("debug"), "node irq");
        return false;
      }
      RCLCPP_INFO(this->get_logger(), "connecting...");
    }
    RCLCPP_INFO(this->get_logger(), "connected");
    return true;
  }

  void sendRequest(float linear, float angular)
  {
    auto req = std::make_shared<TeleportRelative::Request>();
    req->linear = linear;
    req->angular = angular;
    auto future = client_->async_send_request(req);

    if (rclcpp::spin_until_future_complete(this->shared_from_this(), future) !=
        rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_ERROR(this->get_logger(), "service call failed");
      return;
    }
    RCLCPP_INFO(this->get_logger(), "service call suc");
  }

private:
  rclcpp::Client<TeleportRelative>::SharedPtr client_;
};

int main(int argc, char* argv[])
{
  if (argc != 3) {
    RCLCPP_ERROR(rclcpp::get_logger("debug"), "请输入线速度和角速度");
    return -1;
  }

  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<ClientAdd>("tp_client_node");

  if (!node->connectToService()) {
    return -2;
  }

  node->sendRequest(atof(argv[1]), atof(argv[2]));
  
  rclcpp::shutdown();
  
  return 0;
}