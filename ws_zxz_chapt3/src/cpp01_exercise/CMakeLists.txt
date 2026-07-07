cmake_minimum_required(VERSION 3.8)
project(cpp01_exercise)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(turtlesim REQUIRED)
find_package(base_interfaces REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(example_interfaces REQUIRED)

add_executable(exer01_pub_sub src/exer01_pub_sub.cpp)
add_executable(exer02_service_add src/exer02_service_add.cpp)
add_executable(exer02_client_add src/exer02_client_add.cpp)
add_executable(exer03_service_turtle src/exer03_service_turtle.cpp)
add_executable(exer04_action_server_f src/exer04_action_server_f.cpp)
add_executable(exer04_action_client_f src/exer04_action_client_f.cpp)
add_executable(exer05_action_client_t src/exer05_action_client_t.cpp)
add_executable(exer06_param_t src/exer06_param_t.cpp)
add_executable(exer07_service_turtle src/exer07_service_turtle.cpp)

target_include_directories(exer01_pub_sub PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(exer01_pub_sub PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  exer01_pub_sub
  "rclcpp"
  "turtlesim"
  "geometry_msgs"
)

ament_target_dependencies(
  exer02_service_add
  "rclcpp"
  example_interfaces
)

ament_target_dependencies(
  exer02_client_add
  "rclcpp"
  example_interfaces
)

ament_target_dependencies(
  exer03_service_turtle
  "rclcpp"
  turtlesim
)

ament_target_dependencies(
  exer04_action_server_f
  "rclcpp"
  rclcpp_action
  example_interfaces
)

ament_target_dependencies(
  exer04_action_client_f
  "rclcpp"
  rclcpp_action
  example_interfaces
)

ament_target_dependencies(
  exer05_action_client_t
  "rclcpp"
  rclcpp_action
  turtlesim
)

ament_target_dependencies(
  exer06_param_t
  "rclcpp"
  turtlesim
)

ament_target_dependencies(
  exer07_service_turtle
  "rclcpp"
  turtlesim
)

install(TARGETS 
  exer01_pub_sub
  exer02_service_add
  exer02_client_add
  exer03_service_turtle
  exer04_action_server_f
  exer04_action_client_f
  exer05_action_client_t
  exer06_param_t
  exer07_service_turtle
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY 
  launch 
  DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
