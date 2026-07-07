from launch import LaunchDescription
from launch_ros.actions import Node

from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

# 启动时不传参为默认值，传参：<name>:=<value>
def generate_launch_description():
  bg_r = DeclareLaunchArgument("bg_r", default_value="255")
  bg_g = DeclareLaunchArgument("bg_g", default_value="255")
  bg_b = DeclareLaunchArgument("bg_b", default_value="255")

  turtle = Node(
    package="turtlesim", 
    executable="turtlesim_node",
    parameters=[{"background_r": LaunchConfiguration("bg_r"),
                 "background_g": LaunchConfiguration("bg_g"),
                 "background_b": LaunchConfiguration("bg_b")}]
  )

  return LaunchDescription([bg_r, bg_g, bg_b, turtle])