#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using example_interfaces::srv::AddTwoInts;
using namespace std::chrono_literals;

class ClientAdd: public rclcpp::Node 
{
public:
  ClientAdd(std::string name)
  : Node(name)
  {
    RCLCPP_INFO(this->get_logger(),"create client node");
    addClient_ = this->create_client<AddTwoInts>("add");
  }

  bool connectToService(void)
  {
    while(!addClient_->wait_for_service(1s)){
      if(!rclcpp::ok()){
        RCLCPP_INFO(rclcpp::get_logger("debug"),"node irq");
        return false;
      }
      RCLCPP_INFO(this->get_logger(),"connecting...");
    }
    RCLCPP_INFO(this->get_logger(),"connected");
    return true;
  }
/*
rclcpp::Client<example_interfaces::srv::AddTwoInts>::FutureAndRequestId 
async_send_request(std::shared_ptr<example_interfaces::srv::AddTwoInts_Request> request)
*/
  void sendRequest(int num1, int num2)
  {
    auto req = std::make_shared<AddTwoInts::Request>();
    req->a = num1;
    req->b = num2;
    auto future = addClient_->async_send_request(req);

    // 把this普通指针转化成智能指针
    rclcpp::FutureReturnCode code = rclcpp::spin_until_future_complete(this->shared_from_this(), future);
    if(code != rclcpp::FutureReturnCode::SUCCESS){
      RCLCPP_ERROR(rclcpp::get_logger("debug"), "connected error");
    }
    else{
      RCLCPP_INFO(rclcpp::get_logger("debug"), "sum=%ld", future.get()->sum);
    }
  }

private:
  rclcpp::Client<AddTwoInts>::SharedPtr addClient_;
};

// 额外传入两个参数
int main(int argc, char* argv[])
{
  if(argc != 3){
    RCLCPP_ERROR(rclcpp::get_logger("debug"), "请输入两个整数");
    return -1;
  }
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<ClientAdd>("add_client_node");

  if(!node->connectToService()){
    return -2;
  }

  node->sendRequest(atoi(argv[1]), atoi(argv[2]));

  rclcpp::shutdown();
  
  return 0;
}