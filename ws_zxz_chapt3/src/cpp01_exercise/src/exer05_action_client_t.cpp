#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "turtlesim/action/rotate_absolute.hpp"

/*
接口名称：/turtle1/rotate_absolute
类型：turtlesim/action/RotateAbsolute

# 请求 The desired heading in radians 
float32 theta
---
# 结果 The angular displacement in radians to the starting position
float32 delta
---
# 中间过程 The remaining rotation in radians
float32 remaining
*/

using turtlesim::action::RotateAbsolute;
using namespace std::placeholders;
using namespace std::chrono_literals;

class ActionClientT: public rclcpp::Node 
{
public:
  ActionClientT(std::string name)
  : Node(name)
  {
    RCLCPP_INFO(this->get_logger(),"create action client node");
    client_ = rclcpp_action::create_client<RotateAbsolute>(this, "/turtle1/rotate_absolute");
  }
  void sendGoal(float theta)
  {
    RCLCPP_INFO(this->get_logger(),"connecting...");
    if(!client_->wait_for_action_server(5s)){
      RCLCPP_INFO(this->get_logger(),"connect fail");
      return;
    }
    RCLCPP_INFO(this->get_logger(),"connected");
    RotateAbsolute::Goal goal;
    goal.theta = theta;

    rclcpp_action::Client<RotateAbsolute>::SendGoalOptions options;
    options.goal_response_callback = std::bind(&ActionClientT::goalResponseCallback, this, _1);
    options.feedback_callback = std::bind(&ActionClientT::feedbackCallback, this, _1, _2);
    options.result_callback = std::bind(&ActionClientT::resultCallback, this, _1);

    auto future = client_->async_send_goal(goal, options);
  }

private:
  rclcpp_action::Client<RotateAbsolute>::SharedPtr client_;

  void goalResponseCallback(rclcpp_action::ClientGoalHandle<RotateAbsolute>::SharedPtr goal)
  {
    if(!goal)
      RCLCPP_INFO(this->get_logger(),"goal reject");
    else
      RCLCPP_INFO(this->get_logger(),"goal processing...");
  }

  void feedbackCallback(rclcpp_action::ClientGoalHandle<RotateAbsolute>::SharedPtr goal, 
                        const std::shared_ptr<const RotateAbsolute::Feedback> feedback)
  {
    RCL_UNUSED(goal);
    
    RCLCPP_INFO(this->get_logger(),"剩余旋转量：%.2f",  feedback->remaining);
  }

  void resultCallback(const rclcpp_action::ClientGoalHandle<RotateAbsolute>::WrappedResult &result)
  {
    if(result.code == rclcpp_action::ResultCode::SUCCEEDED){
      RCLCPP_INFO(this->get_logger(),"角位移总量: %.2f",  result.result->delta);
    }
    else if(result.code == rclcpp_action::ResultCode::ABORTED){
      RCLCPP_INFO(this->get_logger(),"处理中断\n");
    }
    else if(result.code == rclcpp_action::ResultCode::CANCELED){
      RCLCPP_INFO(this->get_logger(),"处理取消\n");
    }
    else if(result.code == rclcpp_action::ResultCode::UNKNOWN){
      RCLCPP_INFO(this->get_logger(),"未知错误\n");
    }
  }
};

int main(int argc, char* argv[])
{
  if(argc != 2) {
    RCLCPP_ERROR(rclcpp::get_logger("debug"), "请提交一个乌龟朝向角度(浮点数据)");
    return -1;
  }
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<ActionClientT>("ros2_node");
  node->sendGoal(atof(argv[1]));
  
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}