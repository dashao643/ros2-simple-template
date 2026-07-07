from launch import LaunchDescription
from launch_ros.actions import Node

# 获取功能包下share目录路径
from ament_index_python.packages import get_package_share_directory
import os

# param: executable the name of the executable to find if a package
#    is provided or otherwise a path to the executable to run.
# param: package the package in which the node executable can be found
# param: name the name of the node
# param: namespace the ROS namespace for this Node
# param: exec_name the label used to represent the process.
#    Defaults to the basename of node executable.
# param: parameters list of names of yaml files with parameter rules, 参数文件或参数字典	
#    or dictionaries of parameters.
# param: remappings ordered list of 'to' and 'from' string pairs to be 话题重映射
#    passed to the node as ROS remapping rules
# param: ros_arguments list of ROS arguments for the node ROS 专用参数
# param: arguments list of extra arguments for the node 普通命令行参数

def generate_launch_description():
  turtle1 = Node(
    package="turtlesim", 
    executable="turtlesim_node", 
    name="t1",
    ros_arguments=["--remap", "__ns:=/t1"],
  )
  turtle2 = Node(
    package="turtlesim", 
    executable="turtlesim_node", 
    name="t2",
    respawn=True,
    # 法一：直接设置参数
    # parameters=[{"background_r": 200, "background_g": 150, "background_b": 100}]
    # 法二：动态路径读取yaml文件
    parameters=[os.path.join(get_package_share_directory("cpp01_launch"), "config", "t2.yaml")]
  )
  
  return LaunchDescription([turtle1, turtle2])
