from launch import LaunchDescription
from launch_ros.actions import Node

from launch.actions import ExecuteProcess
from launch.substitutions import FindExecutable

# "x: 0.0, y: 0.0, theta: 0.0, name: '' "

def generate_launch_description():
  turtle = Node(package="turtlesim", executable="turtlesim_node")
  spawn = ExecuteProcess(
    # cmd=["ros2 service call /spawn turtlesim/srv/Spawn \"{x: 8.0, y: 9.0,theta: 0.0, name: 'turtle2'}\""],
    cmd = [
      FindExecutable(name="ros2"),
      " service call", 
      " /spawn turtlesim/srv/Spawn",
      " \"{x: 8.0, y: 9.0,theta: 1.0, name: 'turtle2'}\" "
    ],
    output="both",  # 输出到屏幕 + 日志
    shell=True      # 用系统 shell 执行（把列表拼成字符串）
  )
  
  return LaunchDescription([turtle, spawn])