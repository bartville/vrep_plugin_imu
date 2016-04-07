cmake_minimum_required(VERSION 2.8.3)
project(vrep_plugin_imu)

find_package(catkin REQUIRED COMPONENTS
  roscpp vrep_skeleton_msg_and_srv
)

catkin_package(
               CATKIN_DEPENDS roscpp rosconsole std_msgs roslib
               DEPENDS roscpp rosconsole std_msgs roslib)

include_directories(include ${catkin_INCLUDE_DIRS})

link_directories("/opt/ros/groovy/lib")
link_directories("/opt/ros/indigo/lib")
link_directories("/opt/ros/hydro/lib")

add_library(v_repExtImu src/vrep_plugin_imu.cpp src/v_repLib.cpp src/ros_server_imu.cpp)

add_dependencies(v_repExtImu ${catkin_EXPORTED_TARGETS})
target_link_libraries(v_repExtImu
    pthread
    dl
    roslib
    rosconsole
    rostime
    rospack
    roscpp_serialization
    roscpp
    boost_system
   ${catkin_LIBRARIES}
)
