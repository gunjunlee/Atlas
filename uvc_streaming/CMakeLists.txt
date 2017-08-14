cmake_minimum_required(VERSION 2.8.3)
project(uvc_streaming)

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs sensor_msgs image_transport)
generate_messages(DEPENDENCIES std_msgs)
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES uvc_streaming
  CATKIN_DEPENDS roscpp std_msgs sensor_msgs image_transport
  DEPENDS system_lib
)

include_directories(include ${catkin_INCLUDE_DIRS})
add_executable(uvc_cam_msg_subscriber src/uvc_cam_msg_subscriber.cpp)
add_dependencies(uvc_cam_msg_subscriber uvc_cam_msg_subscriber_cpp)
target_link_libraries(uvc_cam_msg_subscriber ${catkin_LIBRARIES})
add_executable(uvc_cam_msg_publisher src/uvc_cam_msg_publisher.cpp)
target_link_libraries(uvc_cam_msg_publisher ${catkin_LIBRARIES})
add_dependencies(uvc_cam_msg_publisher uvc_cam_msg_publisher_cpp)

