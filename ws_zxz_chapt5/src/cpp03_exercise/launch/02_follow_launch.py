# 基础启动相关类
from launch import LaunchDescription
from launch_ros.actions import Node

# # 封装终端指令相关类
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable

# 参数声明与获取
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration

#   
# launch文件启动:不要命令行传参,用节点参数
def generate_launch_description():
  # 启动 turtlesim
  turtle1 = Node(package="turtlesim", executable="turtlesim_node", name="turtle1")
  # 启动 spawn 程序 
  # x y 必须传对应的double类型
  exec1 = Node(package="cpp03_exercise", executable="demo02_trutle_spawn",
               parameters=[{"x": 1.0, "y": 1.0, "turtle_name": "turtle2"}]) 
  exec2 = Node(package="cpp03_exercise", executable="demo02_trutle_spawn",
               parameters=[{"x": 8.0, "y": 8.0, "turtle_name": "turtle3"}])
  # 发布turtle1 turtle2 相对于world坐标
  tf_broadcaster1 = Node(package="cpp03_exercise",executable="demo02_turtle_dynamic_tf",name="bc1")
  tf_broadcaster2 = Node(package="cpp03_exercise",executable="demo02_turtle_dynamic_tf",name="bc2",
                         parameters=[{"turtle_name": "turtle2"}])
  tf_broadcaster3 = Node(package="cpp03_exercise",executable="demo02_turtle_dynamic_tf",name="bc3",
                         parameters=[{"turtle_name": "turtle3"}])
  # 启动坐标变换监听,计算,发布速度节点
  tf_listener1 = Node(package="cpp03_exercise",executable="demo02_turtle_listener",
                         parameters=[{"target_frame": "turtle2", "source_frame": "turtle1"}])
  tf_listener2 = Node(package="cpp03_exercise",executable="demo02_turtle_listener",
                         parameters=[{"target_frame": "turtle3", "source_frame": "turtle1"}])

  return LaunchDescription([turtle1, exec1, exec2, tf_broadcaster1, tf_broadcaster2, tf_broadcaster3,
                            tf_listener1, tf_listener2])