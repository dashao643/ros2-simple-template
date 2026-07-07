# 基础启动相关类
from launch import LaunchDescription
from launch_ros.actions import Node

# # 封装终端指令相关类
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable

# 参数声明与获取
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration

# launch文件启动:不要命令行传参,用节点参数
def generate_launch_description():
  # 启动 turtlesim
  turtle_master = Node(package="turtlesim", executable="turtlesim_node", name="turtle1")

  # 启动 spawn 程序 
  back_spawn = Node(package="cpp03_exercise", executable="demo02_trutle_spawn",
               parameters=[{"x": 2.0, "y": 5.0, "turtle_name": "turtle_back"}])
  left_spawn = Node(package="cpp03_exercise", executable="demo02_trutle_spawn",
               parameters=[{"x": 5.0, "y": 8.0, "turtle_name": "turtle_left"}])
  right_spawn = Node(package="cpp03_exercise", executable="demo02_trutle_spawn",
               parameters=[{"x": 5.0, "y": 2.0, "turtle_name": "turtle_right"}])

  # 发布 turtle_master turtle_back 相对于world坐标
  tf_bc_turtle1_world = Node(package="cpp03_exercise",executable="demo02_turtle_dynamic_tf",name="turtle1_world")
  tf_bc_turtle2_world = Node(package="cpp03_exercise",executable="demo02_turtle_dynamic_tf",name="turtle2_world",
                         parameters=[{"turtle_name": "turtle_back"}])
  tf_bc_turtle3_world = Node(package="cpp03_exercise",executable="demo02_turtle_dynamic_tf",name="turtle3_world",
                         parameters=[{"turtle_name": "turtle_left"}])
  tf_bc_turtle4_world = Node(package="cpp03_exercise",executable="demo02_turtle_dynamic_tf",name="turtle4_world",
                         parameters=[{"turtle_name": "turtle_right"}])
  # 发布相对于 turtle_master 后方的静态坐标变换
  # static_transform_publisher 的命令行参数是空格分隔的列表
  tf_bc_turtle_master_back = Node(package="tf2_ros",executable="static_transform_publisher",name="static_turtle_back",
                              arguments=[
                                "--frame-id", "turtle1",
                                "--child-frame-id", 
                                "tf_bc_turtle_master_back",
                                "--x", "-1.5"
                            ])
  tf_bc_turtle_master_left = Node(package="tf2_ros",executable="static_transform_publisher",name="static_turtle_left",
                              arguments=[
                                "--frame-id", "turtle1",
                                "--child-frame-id", 
                                "tf_bc_turtle_master_left",
                                "--y", "1.5"
                            ])
  tf_bc_turtle_master_right = Node(package="tf2_ros",executable="static_transform_publisher",name="static_turtle_back",
                              arguments=[
                                "--frame-id", "turtle1",
                                "--child-frame-id", 
                                "tf_bc_turtle_master_right",
                                "--y", "-1.5"
                            ])
  # 监听 turtle_master turtle_back 坐标,并变换为 turtle_master(子级) 相对于 turtle_back(父级) 坐标
  tf_listener1 = Node(package="cpp03_exercise",executable="demo02_turtle_listener",
                         parameters=[{"target_frame": "turtle_back", "source_frame": "tf_bc_turtle_master_back"}])
  tf_listener2 = Node(package="cpp03_exercise",executable="demo02_turtle_listener",
                         parameters=[{"target_frame": "turtle_left", "source_frame": "tf_bc_turtle_master_left"}])
  tf_listener3 = Node(package="cpp03_exercise",executable="demo02_turtle_listener",
                         parameters=[{"target_frame": "turtle_right", "source_frame": "tf_bc_turtle_master_right"}])

  return LaunchDescription([turtle_master, back_spawn, left_spawn,right_spawn,
                            tf_bc_turtle1_world, tf_bc_turtle2_world, tf_bc_turtle3_world, tf_bc_turtle4_world,
                            tf_bc_turtle_master_back, tf_bc_turtle_master_left, tf_bc_turtle_master_right,
                            tf_listener1, tf_listener2, tf_listener3])