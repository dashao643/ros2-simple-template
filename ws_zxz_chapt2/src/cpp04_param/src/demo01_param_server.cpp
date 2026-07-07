#include "rclcpp/rclcpp.hpp"

class MyParamServer: public rclcpp::Node 
{
public:
  MyParamServer(std::string name)
  : Node(name,rclcpp::NodeOptions().allow_undeclared_parameters(true))
  , cnt_(0)
  {
    RCLCPP_INFO(this->get_logger(),"hello ros2");
  }

  void creat_param()
  {
    RCLCPP_INFO(this->get_logger(),"creat");
    this->declare_parameter("name","dashao");
    this->declare_parameter("age",20);
    this->declare_parameter("height",1.70);

    // 此属性：allow_undeclared_parameters(true)，可以用set_parameter新增参数
    this->set_parameter(rclcpp::Parameter("name2","fenkai"));
  }

  void read_param()
  {
    RCLCPP_INFO(this->get_logger(),"read");
    auto nameValue = this->get_parameter("name");
    RCLCPP_INFO(this->get_logger(),"key=%s,value=%s\n",nameValue.get_name().c_str(),nameValue.as_string().c_str());
    auto params = this->get_parameters({"name","age","height"});
    for(const rclcpp::Parameter &param : params){
      RCLCPP_INFO(this->get_logger(),"key=%s,value=%s\n",param.get_name().c_str(),param.value_to_string().c_str());
    }
    // 查询key
    bool isHave1 = this->has_parameter("name");
    RCLCPP_INFO(this->get_logger(),"isHave1=%d\n",isHave1);
    isHave1 = this->has_parameter("dashao");
    RCLCPP_INFO(this->get_logger(),"isHave1=%d\n",isHave1);
  }

  void updata_param()
  {
    this->set_parameter(rclcpp::Parameter("name","fenkai"));
    RCLCPP_INFO(this->get_logger(),"key=name,value=%s\n",this->get_parameter("name").as_string().c_str());
  }

  void delete_param()
  {
    RCLCPP_INFO(this->get_logger(),"name2是否存在=%d\n",this->has_parameter("name2"));
    // 使用declare_parameter创建的参数，禁止删除
    // this->undeclare_parameter("name");
    this->undeclare_parameter("name2");
    RCLCPP_INFO(this->get_logger(),"name2是否存在=%d\n",this->has_parameter("name2"));
  }

private:
  int cnt_;
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<MyParamServer>("node_server");

  node->creat_param();
  node->read_param();
  node->updata_param();
  node->delete_param();

  rclcpp::spin(node);
  
  rclcpp::shutdown();
  
  return 0;
}