#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces/action/progress.hpp"

using base_interfaces::action::Progress;
using namespace std::placeholders;
using namespace std::chrono_literals;

/*
->std::shared_future<rclcpp_action::ClientGoalHandle<base_interfaces::action::Progress>::SharedPtr> 
const base_interfaces::action::Progress::Goal &goal, 
const rclcpp_action::Client<base_interfaces::action::Progress>::SendGoalOptions &options

std::function<void (std::shared_ptr<rclcpp_action::ClientGoalHandle<base_interfaces::action::Progress>>, 
std::shared_ptr<const base_interfaces::action::Progress_Feedback>)> 
rclcpp_action::Client<base_interfaces::action::Progress>::SendGoalOptions::feedback_callback

std::function<void (std::shared_ptr<rclcpp_action::ClientGoalHandle<base_interfaces::action::Progress>>)> 
rclcpp_action::Client<base_interfaces::action::Progress>::SendGoalOptions::goal_response_callback

std::function<void (const rclcpp_action::ClientGoalHandle<base_interfaces::action::Progress>::WrappedResult &result)> 
rclcpp_action::Client<base_interfaces::action::Progress>::SendGoalOptions::result_callback
*/

class MyActionClient: public rclcpp::Node 
{
public:
  MyActionClient(std::string name)
  : Node(name)
  , cnt_(0)
  {
    client_ = rclcpp_action::create_client<Progress>(this,"get_sum");
    RCLCPP_INFO(this->get_logger(),"hello ros2");
  }

  void send_num(int num)
  {
    if(!client_->wait_for_action_server(1s)){
      RCLCPP_INFO(this->get_logger(),"连接失败");
      return;
    }
    RCLCPP_INFO(this->get_logger(),"成功");
    auto goal = Progress::Goal();
    auto options = rclcpp_action::Client<Progress>::SendGoalOptions();
    options.goal_response_callback = std::bind(&MyActionClient::goal_response_callback,this,_1);
    options.feedback_callback = std::bind(&MyActionClient::feedback_callback,this,_1,_2);
    options.result_callback = std::bind(&MyActionClient::result_callback,this,_1);
    goal.num = num;
    auto future = client_->async_send_goal(goal, options);
  }

private:
  int cnt_;
  rclcpp_action::Client<Progress>::SharedPtr client_;


  void goal_response_callback(std::shared_ptr<rclcpp_action::ClientGoalHandle<Progress>> goalPtr)
  {
    if(!goalPtr)
      RCLCPP_INFO(this->get_logger(),"目标被拒绝");
    else
      RCLCPP_INFO(this->get_logger(),"目标处理中");
  }
  void feedback_callback(std::shared_ptr<rclcpp_action::ClientGoalHandle<Progress>> goalPtr,
                    std::shared_ptr<const Progress::Feedback> feedbackPtr)
  {
    (void)goalPtr;
    
    RCLCPP_INFO(this->get_logger(),"当前进度:%.2f\n", feedbackPtr->progress);
  }
  // rclcpp_action::ResultCode rclcpp_action::ClientGoalHandle<base_interfaces::action::Progress>::WrappedResult::code
  void result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult &result)
  {
    if(result.code == rclcpp_action::ResultCode::SUCCEEDED){
      RCLCPP_INFO(this->get_logger(),"最终结果:%d\n", result.result->sum);
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
  
  auto node = std::make_shared<MyActionClient>("ros2_node");
  node->send_num(atoi(argv[1]));
  rclcpp::spin(node);

  rclcpp::shutdown();
  
  return 0;
}