# 基础启动相关类
from launch import LaunchDescription
from launch_ros.actions import Node

# 封装终端指令相关类
from launch.actions import ExecuteProcess
from launch.substitutions import FindExecutable

def generate_launch_description():
  # 启动一个 turtlesim
  turtle1 = Node(package="turtlesim", executable="turtlesim_node", name="turtle1")
  # 执行终端命令生成乌龟
  spawn = ExecuteProcess(
    cmd = [
      FindExecutable(name="ros2"),
      " service call", 
      " /spawn turtlesim/srv/Spawn",
      " \"{x: 1.0, y: 1.0,theta: 0.0, name: 'turtle2'}\" "
    ],
    output="both",  # 输出到屏幕 + 日志
    shell=True      # 用系统 shell 执行（把列表拼成字符串）
  )
  # 启动可执行程序
  exec = Node(package="cpp03_exercise", executable="demo01_turtle_follow")

  return LaunchDescription([turtle1, spawn, exec])