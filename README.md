## **VREP_PLUGIN_IMU**
a VREP plugin that allows to publish **sensor_msgs/Imu messages**

## INSTALLATION
clone the repository in your ros_package directory (the one you used to install Vrep Ros plugins)
cd <catkin_ws>/src/ros_packages
git clone https://github.com/bartville/vrep_plugin_imu.git
compile the plugin
cd ~/<catkin_ws>
catkin_make --pkg vrep_plugin_imu
it will generate a shared object in your devel/lib directory.
Copy the generated library libv_repExtImu.so in your Vrep root
cp ~/<catkin_ws>/devel/lib/libv_repExtImu.so <YOUR-VREP-ROOT-DIRECTORY>
Launch roscore, then launch vrep, and check if "Imu" has been successfully loaded.

Use the Imu model in the attached scene, Imu.ttt.
Start your simulation.
Praise the Sun.
check if imu topic is publishing
rostopic echo /vrep/imu

