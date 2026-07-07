#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using example_interfaces::action::Fibonacci;
using namespace std::placeholders;
using namespace std::chrono_literals;

/*
rclcpp_action::Client<ActionT>::SharedPtr create_client<ActionT, NodeT>(NodeT node, const std::string &name, 
  rclcpp::CallbackGroup::SharedPtr group = nullptr, 
  const rcl_action_client_options_t &options = rcl_action_client_get_default_options())

using GoalHandle = ClientGoalHandle<ActionT>;

using GoalResponseCallback = std::function<void (typename GoalHandle::SharedPtr)>;

using FeedbackCallback = std::function<void (
  typename ClientGoalHandle<ActionT>::SharedPtr,
  const std::shared_ptr<const Feedback>
)>;

using WrappedResult = typename GoalHandle::WrappedResult;
using ResultCallback = std::function<void (const WrappedResult & result)>;
*/
class ActionClientF: public rclcpp::Node 
{
public:
  ActionClientF(std::string name)
  : Node(name)
  {
    RCLCPP_INFO(this->get_logger(),"create action client node");
    client_ = rclcpp_action::create_client<Fibonacci>(this, "actFibonacci");
  }
  void sendOrder(int order)
  {
    RCLCPP_INFO(this->get_logger(),"connecting...");
    if(!client_->wait_for_action_server(5s)){
      RCLCPP_INFO(this->get_logger(),"connect fail");
      return;
    }
    RCLCPP_INFO(this->get_logger(),"connected");
    Fibonacci::Goal goal;
    goal.order = order;

    rclcpp_action::Client<Fibonacci>::SendGoalOptions options;
    options.goal_response_callback = std::bind(&ActionClientF::goalResponseCallback, this, _1);
    options.feedback_callback = std::bind(&ActionClientF::feedbackCallback, this, _1, _2);
    options.result_callback = std::bind(&ActionClientF::resultCallback, this, _1);

    auto future = client_->async_send_goal(goal, options);
  }

private:
  rclcpp_action::Client<Fibonacci>::SharedPtr client_;

  void goalResponseCallback(rclcpp_action::ClientGoalHandle<Fibonacci>::SharedPtr goal)
  {
    if(!goal)
      RCLCPP_INFO(this->get_logger(),"goal reject");
    else
      RCLCPP_INFO(this->get_logger(),"goal processing...");
  }

  void feedbackCallback(rclcpp_action::ClientGoalHandle<Fibonacci>::SharedPtr goal, 
                        const std::shared_ptr<const Fibonacci::Feedback> feedback)
  {
    RCL_UNUSED(goal);
    
    std::stringstream ss;
    ss << "当前进度: ";

    std::vector<int32_t> v = feedback->sequence;
    for(int32_t val : v){
      ss << val << " ";
    }
    RCLCPP_INFO_STREAM(this->get_logger(), ss.str());
  }

  void resultCallback(const rclcpp_action::ClientGoalHandle<Fibonacci>::WrappedResult &result)
  {
    if(result.code == rclcpp_action::ResultCode::SUCCEEDED){
      std::stringstream ss;
      ss << "最终结果: ";

      std::vector<int32_t> v = result.result->sequence;
      for(int32_t val : v){
        ss << val << " ";
      }
      RCLCPP_INFO_STREAM(this->get_logger(), ss.str());
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
    RCLCPP_ERROR(rclcpp::get_logger("debug"), "请提交一个整型数据");
    return -1;
  }
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<ActionClientF>("ros2_node");
  node->sendOrder(atoi(argv[1]));
  
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}