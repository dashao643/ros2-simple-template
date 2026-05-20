#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class MyParamClient: public rclcpp::Node 
{
public:
  MyParamClient(std::string name)
  : Node(name)
  , cnt_(0)
  {
    RCLCPP_INFO(this->get_logger(),"hello ros2");
    // 参数服务工具类，接口为节点名称，不是话题名称，使用c++创建指针
    client_ = std::make_shared<rclcpp::SyncParametersClient>(this,"node_server");
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
    auto vec = client_->get_parameters({"name","name2","age","height"});
    for(const auto &param : vec){
      RCLCPP_INFO(this->get_logger(),"key=%s,value=%s\n",param.get_name().c_str(),param.value_to_string().c_str());
    }
    if(client_->has_parameter("width")){
      double width = client_->get_parameter<double>("width");
      RCLCPP_INFO(this->get_logger(),"width=%.2f\n",width);
    }
  }
  // 修改参数
  void updataParam()
  {
    RCLCPP_INFO(this->get_logger(),"-----------参数客户端修改参数-----------");
    client_->set_parameters({
      rclcpp::Parameter("name","jiangchuan"),
      rclcpp::Parameter("age",24),
      // 服务端不存在的参数，只有服务端设置了rclcpp::NodeOptions().allow_undeclared_parameters(true)时，
      // 这个参数才会被成功设置
      rclcpp::Parameter("width",2.0)
    });
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
  auto node = std::make_shared<MyParamClient>("node_client");
  // rclcpp::spin(node);
  if(node->connectToServer()){
    node->readParam();
    node->updataParam();
    node->readParam();
  }
  
  rclcpp::shutdown();
  
  return 0;
}