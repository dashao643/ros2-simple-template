#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

#include <string.h>

// 服务通信客户端：通过参数指定位置生成乌龟

using turtlesim::srv::Spawn;
using namespace std::chrono_literals;

class ClientSpawn : public rclcpp::Node
{
public:
  ClientSpawn(std::string name)
    : Node(name)
  {
    RCLCPP_INFO(this->get_logger(), "create client node");

    // 声明参数
    this->declare_parameter("x",8.0);
    this->declare_parameter("y",8.0);
    this->declare_parameter("theta",0.0);
    this->declare_parameter("turtle_name","turtle2");

    client_ = this->create_client<Spawn>("/spawn");
    request_ = std::make_shared<turtlesim::srv::Spawn::Request>();

    // 获取参数
    request_->x = this->get_parameter("x").as_double();
    request_->y = this->get_parameter("y").as_double();
    request_->theta = this->get_parameter("theta").as_double();
    request_->name = this->get_parameter("turtle_name").as_string();
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

  // void sendRequest(double x, double y)
  // {
  //   auto req = std::make_shared<turtlesim::srv::Spawn::Request>();
  //   req->x = x;
  //   req->y = y;
  //   req->name = "turtle2";

  //   auto future = client_->async_send_request(req);

  //   if (rclcpp::spin_until_future_complete(this->shared_from_this(), future) !=
  //       rclcpp::FutureReturnCode::SUCCESS) {
  //     RCLCPP_ERROR(this->get_logger(), "service call failed");
  //     return;
  //   }
  //   std::string turtleName = future.get()->name;

  //   if(turtleName.empty())
  //     RCLCPP_INFO(this->get_logger(), "乌龟重名 生成失败\n");
  //   else
  //     RCLCPP_INFO(this->get_logger(), "spawn suc, name = %s", turtleName.c_str());
  // }
  void sendRequest()
  {
    auto future = client_->async_send_request(request_);

    if (rclcpp::spin_until_future_complete(this->shared_from_this(), future) !=
        rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_ERROR(this->get_logger(), "service call failed");
      return;
    }
    std::string turtleName = future.get()->name;

    if(turtleName.empty())
      RCLCPP_INFO(this->get_logger(), "乌龟重名 生成失败\n");
    else
      RCLCPP_INFO(this->get_logger(), "spawn suc, name = %s", turtleName.c_str());
  }
private:
  rclcpp::Client<Spawn>::SharedPtr client_;
  turtlesim::srv::Spawn::Request::SharedPtr request_;
};

int main(int argc, char* argv[])
{
  // if (argc != 3) {
  //   RCLCPP_ERROR(rclcpp::get_logger("debug"), "请输入新乌龟的 x y 坐标");
  //   return -1;
  // }

  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<ClientSpawn>("spawn_client_node");

  if (!node->connectToService()) {
    return -2;
  }

  // node->sendRequest(atof(argv[1]), atof(argv[2]));
  node->sendRequest();
  
  rclcpp::shutdown();
  
  return 0;
}
