#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"
#include "turtlesim/msg/pose.hpp"

/*
创建服务通信客户端，向乌龟节点请求服务，在指定位置生成一只乌龟
订阅乌龟的pose话题，计算与生成乌龟的距离
服务类型：
  turtlesim/srv/Spawn
服务参数：
  float32 x
  float32 y
  float32 theta
  string name
*/

using turtlesim::srv::Spawn;
using turtlesim::msg::Pose;
using namespace std::chrono_literals;
using namespace std::placeholders;

class MyNode: public rclcpp::Node 
{
public:
  MyNode(std::string name)
  : Node(name)
  {
    RCLCPP_INFO(this->get_logger(),"create exer03 node");
    client_ = this->create_client<Spawn>("/spawn");
    sub_ = this->create_subscription<Pose>("/turtle1/pose", 10, std::bind(&MyNode::subPoseCallBack, this , _1));
  }

  bool connectToService(void)
  {
    while(!client_->wait_for_service(1s)){
      if(!rclcpp::ok()){
        RCLCPP_INFO(rclcpp::get_logger("debug"),"node irq");
        return false;
      }
      RCLCPP_INFO(this->get_logger(),"connecting...");
    }

    RCLCPP_INFO(this->get_logger(),"connected");
    return true;
  }
// rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId 
// async_send_request(std::shared_ptr<turtlesim::srv::Spawn_Request> request)

  void sendRequest(float x, float y)
  {
    cnt_++;
    auto req = std::make_shared<Spawn::Request>();
    req->name = "t" + std::to_string(cnt_);;
    req->x = x;
    req->y = y;
    x_ = x;
    y_ = y;
    auto futureId = client_->async_send_request(req);

    if (rclcpp::spin_until_future_complete(this->shared_from_this(), futureId) == rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_INFO(this->get_logger(), "乌龟生成成功");
    }
    else{
      RCLCPP_ERROR(this->get_logger(), "乌龟生成失败");
    }
  }

private:
  rclcpp::Client<Spawn>::SharedPtr client_;
  rclcpp::Subscription<Pose>::SharedPtr sub_;
  float x_;
  float y_;
  int cnt_ = 1;

  void subPoseCallBack(const Pose &pose)
  {
    float distance = sqrt((pose.x - x_) * (pose.x - x_) + (pose.y - y_) * (pose.y - y_));
 
    RCLCPP_INFO(this->get_logger(),"t1 t2 distance:%.2f", distance);
  }
};

int main(int argc, char* argv[])
{
  if(argc != 3){
    RCLCPP_ERROR(rclcpp::get_logger("debug"), "请输入乌龟的x, y坐标");
    return -1;
  }
  float x = atof(argv[1]);
  float y = atof(argv[2]);

  if(x > 10 || y > 10){
    RCLCPP_ERROR(rclcpp::get_logger("debug"), "x, y坐标太大");
    return -3;
  }

  rclcpp::init(argc, argv);

  auto node = std::make_shared<MyNode>("exer03_node");
  if(!node->connectToService()){
    return -2;
  }
  node->sendRequest(x, y);

  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}