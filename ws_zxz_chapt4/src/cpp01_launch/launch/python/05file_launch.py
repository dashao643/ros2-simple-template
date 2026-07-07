from launch import LaunchDescription
from launch_ros.actions import Node

from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
  include = IncludeLaunchDescription(
    launch_description_source=PythonLaunchDescriptionSource(
      launch_file_path=os.path.join(
        get_package_share_directory("cpp01_launch"),
        "launch/python",
        "04args_launch.py"
      )
    ),
    launch_arguments=[("bg_r", "80"), ("bg_g", "160"), ("bg_b", "240")]
  )

  return LaunchDescription([include])