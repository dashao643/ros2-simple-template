#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using example_interfaces::action::Fibonacci;
using namespace std::placeholders;
using namespace std::chrono_literals;

/*
example_interfaces/action/Fibonacci

# Goal
int32 order
---
# Result
int32[] sequence
---
# Feedback
int32[] sequence
*/

class ActionServerF: public rclcpp::Node 
{
public:
  ActionServerF(std::string name)
  : Node(name)
  {
    RCLCPP_INFO(this->get_logger(),"create action server node");
    ActServer_ = rclcpp_action::create_server<Fibonacci>(this, "actFibonacci", 
      std::bind(&ActionServerF::GoalCallback, this, _1, _2),
      std::bind(&ActionServerF::CancelCallback, this, _1),
      std::bind(&ActionServerF::AcceptedCallback,this, _1)
    );
  }

private:
  rclcpp_action::Server<Fibonacci>::SharedPtr ActServer_;

  rclcpp_action::GoalResponse GoalCallback(const rclcpp_action::GoalUUID & uuid, 
                                           const std::shared_ptr<const Fibonacci::Goal> goal) 
  {
    RCL_UNUSED(uuid);
    if(goal->order <= 0){
      RCLCPP_INFO(this->get_logger(),"order error");
      return rclcpp_action::GoalResponse::REJECT;
    }
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse CancelCallback(std::shared_ptr<rclcpp_action::ServerGoalHandle<Fibonacci>> handle)
  {
    RCL_UNUSED(handle);
    RCLCPP_INFO(this->get_logger(),"cancel");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void AcceptedCallback(std::shared_ptr<rclcpp_action::ServerGoalHandle<Fibonacci>> handle)
  {
    RCL_UNUSED(handle);
    std::thread(std::bind(&ActionServerF::execute, this, handle)).detach();
  }

  void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Fibonacci>> handle)
  {
    int32_t order = handle->get_goal()->order;
    std::vector<int32_t> v = {0, 1};
    auto feedbackMsg = std::make_shared<Fibonacci::Feedback>();
    auto resultMsg = std::make_shared<Fibonacci::Result>();
    rclcpp::Rate rate(1s);

    if(order == 1) v = {0};
    for(int i = 2; i < order; i++){
      if(handle->is_canceling()) {
        resultMsg->sequence = v;
        handle->canceled(resultMsg);
        RCLCPP_INFO(this->get_logger(), "cancel");
        return;
      }
      int32_t val = v.at(i-2) + v.at(i-1);
      v.push_back(val);
      feedbackMsg->sequence = v;
      handle->publish_feedback(feedbackMsg);
      rate.sleep();
    }
    resultMsg->sequence = v;
    handle->succeed(resultMsg);
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<ActionServerF>("ros2_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}