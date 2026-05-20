#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces/action/progress.hpp"

using namespace std::placeholders;
using namespace std::chrono_literals;
using base_interfaces::action::Progress;

/*
using GoalCallback = std::function<GoalResponse(
  const GoalUUID &, 
  std::shared_ptr<const typename ActionT::Goal>
)>;
using CancelCallback = std::function<CancelResponse(
  std::shared_ptr<ServerGoalHandle<ActionT>>
)>;
using AcceptedCallback = std::function<void (
  std::shared_ptr<ServerGoalHandle<ActionT>>
)>;
*/

class MyActionServer: public rclcpp::Node 
{
public:
  MyActionServer(std::string name)
  : Node(name)
  , cnt_(0)
  {
    RCLCPP_INFO(this->get_logger(),"action server creat");
    actionServer_ = rclcpp_action::create_server<Progress>(this, "get_sum", 
      std::bind(&MyActionServer::goalCallBack, this, _1, _2),
      std::bind(&MyActionServer::cancelCallBack, this, _1),
      std::bind(&MyActionServer::acceptedCallBack, this, _1)
    );
  }

private:
  int cnt_;
  rclcpp_action::Server<Progress>::SharedPtr actionServer_;

  rclcpp_action::GoalResponse goalCallBack(const rclcpp_action::GoalUUID &uuid, 
                                           const std::shared_ptr<const Progress::Goal> goal)
  {
    (void)uuid;
    if(goal->num <= 1) {
      RCLCPP_INFO(this->get_logger(),"error num");
      return rclcpp_action::GoalResponse::REJECT;
    }
    RCLCPP_INFO(this->get_logger(),"正确 num");
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse cancelCallBack(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> Handle)
  {
    (void)Handle;
    RCLCPP_INFO(this->get_logger(),"cancel");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goalHandle)
  {
    int num = goalHandle->get_goal()->num;
    int sum = 0;
    auto feedback = std::make_shared<Progress::Feedback>();
    auto result = std::make_shared<Progress::Result>();
    // 设置休眠
    rclcpp::Rate rate(1s);
    for(int i = 0; (i <= num)&&rclcpp::ok(); i++){
      sum += i;

      // Check cancel request
      if (goalHandle->is_canceling()) {
        result->sum = sum;
        goalHandle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "任务取消");
        return;
      }
      feedback->progress = i / (double)num;
      goalHandle->publish_feedback(feedback);
      rate.sleep();
    }
    if(rclcpp::ok()){
      result->sum = sum;
      goalHandle->succeed(result);
    }
  }

  void acceptedCallBack(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goalHandle)
  {
    // 耗时操作，单独创建线程
    std::thread(std::bind(&MyActionServer::execute,this,goalHandle)).detach();
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MyActionServer>("ros2_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}
