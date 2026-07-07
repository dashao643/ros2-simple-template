# 基础启动相关类
from launch import LaunchDescription
from launch_ros.actions import Node
# 参数声明与获取
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command,LaunchConfiguration
# 获取功能包下share目录路径
from ament_index_python.packages import get_package_share_directory
# import os

def generate_launch_description():
  model = DeclareLaunchArgument(name="model", default_value=get_package_share_directory("cpp01_urdf") + "/urdf/urdf/demo01.urdf")
  p_value = ParameterValue(Command(["xacro ", LaunchConfiguration("model")]))
  robot_state_pub = Node(package="robot_state_publisher", executable="robot_state_publisher", 
                         parameters=[{"robot_description": p_value}])
  joint_state_pub = Node(package="joint_state_publisher", executable="joint_state_publisher", )
  # 加载 urdf.rviz 配置文件
  rviz2 = Node(package="rviz2", executable="rviz2", arguments=["-d", get_package_share_directory("cpp01_urdf") + 
                                                               "/rviz/urdf.rviz"], )
  return LaunchDescription([rviz2, robot_state_pub, joint_state_pub, model])
