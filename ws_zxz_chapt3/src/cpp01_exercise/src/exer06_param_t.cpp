#include "rclcpp/rclcpp.hpp"

/*
background_b
background_g
background_r
use_sim_time
*/
using namespace std::chrono_literals;
// #define RGB_ADD     1
// #define RGB_DES     0

class MyParamClient: public rclcpp::Node 
{
public:
  MyParamClient(std::string name)
  : Node(name)
  , cnt_(0)
  {
    RCLCPP_INFO(this->get_logger(),"create param client node");
    // 参数服务工具类，接口为节点名称，不是话题名称，使用c++创建指针
    client_ = std::make_shared<rclcpp::SyncParametersClient>(this, "/turtlesim");
  }

  bool connectToServer()
  {
    while(!client_->wait_for_service(1s)){
      if(!rclcpp::ok()){
        RCLCPP_INFO(rclcpp::get_logger("debug"),"客户端断开");
        return false;
      }
    }
    RCLCPP_INFO(rclcpp::get_logger("debug"),"客户端已连接");
    return true;
  }

  // 查询参数
  void readParam()
  {
    RCLCPP_INFO(this->get_logger(),"-----------参数客户端查询参数-----------");
    // 先查看参数是否存在，再去获取，防止崩溃
    // if(client_->has_parameter("name")){
    //   std::string name = client_->get_parameter<std::string>("name");
    //   RCLCPP_INFO(this->get_logger(),"name=%s\n",name.c_str());
    // }
    // if(client_->has_parameter("name2")){
    //   std::string name2 = client_->get_parameter<std::string>("name2");
    //   RCLCPP_INFO(this->get_logger(),"name2=%s\n",name2.c_str());
    // }
    // 或者批量获取，存入容器，只在容器查看获取到的

    std::vector<rclcpp::Parameter> vec = client_->get_parameters({
      "background_b",
      "background_g",
      "background_r",
      "use_sim_time"
    });
    for(const rclcpp::Parameter &param : vec){
      RCLCPP_INFO(this->get_logger(),"key=%s,value=%s\n", param.get_name().c_str(), param.value_to_string().c_str());
    }
  }
  // 修改参数
  void updataParam()
  {
    static int channel = 0;  // 0=R, 1=G, 2=B
    static int value = 0;
    static bool isAdd = true;

    // 修改当前通道
    if (channel == 0) {
      client_->set_parameters({rclcpp::Parameter("background_r", value)});
    } else if (channel == 1) {
      client_->set_parameters({rclcpp::Parameter("background_g", value)});
    } else {
      client_->set_parameters({rclcpp::Parameter("background_b", value)});
    }

    if(isAdd)
      value++;
    else
      value--;

    if (value > 255) {
      isAdd = !isAdd;
      channel = (channel + 1) % 3;
    }
    if(value <= 0){
      isAdd = !isAdd;
      channel = (channel + 1) % 3;
    }
  }
private:
  int cnt_;
  // 创建参数服务客户端
  rclcpp::SyncParametersClient::SharedPtr client_;
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  // 自己的节点名，不要和服务端的重复
  auto node = std::make_shared<MyParamClient>("node_param_client");
  // rclcpp::spin(node);
  if(node->connectToServer()){
    node->readParam();
    while(rclcpp::ok()){
      node->updataParam();
    }
    RCLCPP_INFO(rclcpp::get_logger("debug"),"客户端断开");
  }
  
  rclcpp::shutdown();
  
  return 0;
}