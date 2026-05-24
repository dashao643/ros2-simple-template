#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std_msgs::msg;
using namespace std::chrono_literals;

class MyNode: public rclcpp::Node 
{
public:
  MyNode(std::string name)
  : Node(name)
  , cnt_(0)
  {
    RCLCPP_INFO(this->get_logger(),"create node");
    pub_ = this->create_publisher<String>("time_topic",10);
    
    // test_rate();
    // test_time();
    // test_duration();
    test_opt();
  }

private:
  int cnt_;
  rclcpp::Publisher<String>::SharedPtr pub_;
  
  // 运行时间
  void test_rate()
  {
    rclcpp::Rate rate(1s);
    while(rclcpp::ok()){
      RCLCPP_INFO(this->get_logger(),"----------------");
      String str;
      str.data = "dashao";
      pub_->publish(str);
      // rate1_.sleep();
      rate.sleep();
    }
  }

  // 时间点
  void test_time()
  {
    rclcpp::Time time1(1,0);
    rclcpp::Time time2(2,1000000000);
    rclcpp::Time now = this->now();

    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",time1.seconds(),time1.nanoseconds());
    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",time2.seconds(),time2.nanoseconds());
    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",now.seconds(),now.nanoseconds());
  }

  // 持续时间
  void test_duration()
  {
    rclcpp::Duration d1(1,0);
    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",d1.seconds(),d1.nanoseconds());
  }

  // 时间运算
  void test_opt()
  {
    rclcpp::Time time1(10,0);
    rclcpp::Time time2(30,0);

    rclcpp::Duration d1(8,0);
    rclcpp::Duration d2(20,0);

    RCLCPP_INFO(this->get_logger(),"s=%.2f",(time2-time1).seconds());
    RCLCPP_INFO(this->get_logger(),"s=%.2f",(time2+d1).seconds());
    RCLCPP_INFO(this->get_logger(),"s=%.2f",(d1-d2).seconds());
    RCLCPP_INFO(this->get_logger(),"s=%.2f",(d2-d1).seconds());
    RCLCPP_INFO(this->get_logger(),"s=%.2f",(d1*10).seconds());
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MyNode>("ros2_node");
  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}
