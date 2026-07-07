# 基础启动相关类
from launch import LaunchDescription
from launch_ros.actions import Node

# 事件相关 
from launch.event_handlers import OnProcessStart, OnProcessExit
from launch.actions import ExecuteProcess, RegisterEventHandler, LogInfo

# "x: 0.0
# y: 0.0
# theta: 0.0
# name: ''
# "

def generate_launch_description():
  turtle1 = Node(package="turtlesim", executable="turtlesim_node", name="t1")
  # turtle2 = Node(package="turtlesim", executable="turtlesim_node", name="t2")
  spawn = ExecuteProcess(
    cmd=["ros2 service call /spawn turtlesim/srv/Spawn \"{'x': 8.0, 'y': 8.0, 'theta': 3.14, 'name': 't2'}\""],
    output="both",
    shell=True
  )
  # 注册开始事件
  event_start = RegisterEventHandler(
    event_handler=OnProcessStart(
      target_action=turtle1,
      on_start=spawn
    )
  )
  # 注册退出事件
  event_exit = RegisterEventHandler(
    event_handler=OnProcessExit(
      target_action=turtle1,
      on_exit=LogInfo(msg="node exit")
    )
  )

  return LaunchDescription([turtle1, event_start, event_exit])