import os
import tempfile
import xacro
import yaml
from pathlib import Path
from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.actions import RegisterEventHandler, SetEnvironmentVariable
from launch.event_handlers import OnProcessExit
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # Launch Arguments
    use_sim_time = LaunchConfiguration('use_sim_time', default=True)

    fws_robot_description_path = os.path.join(
        get_package_share_directory('fws_robot_description'))

    fws_robot_sim_path = os.path.join(
        get_package_share_directory('fws_robot_sim'))

    # Set gazebo sim resource path
    gazebo_resource_path = SetEnvironmentVariable(
        name='GZ_SIM_RESOURCE_PATH',
        value=[
            os.path.join(fws_robot_sim_path, 'worlds'), ':' +
            str(Path(fws_robot_description_path).parent.resolve())
            ]
        )

    gazebo_plugin_path = SetEnvironmentVariable(
        name='GZ_SIM_SYSTEM_PLUGIN_PATH',
        value=['/opt/ros/jazzy/lib']
        )

    arguments = LaunchDescription([
                DeclareLaunchArgument('world', default_value='fws_robot_world',
                          description='Gz sim World'),
           ]
    )

    # ros_gz_sim's gz_sim.launch.py runs `gz sim` directly, which hangs under
    # this devcontainer's forwarded (indirect-GLX) DISPLAY. Route it through
    # the `gzrun` wrapper (vglrun) instead, same as running `gzrun` by hand.
    gazebo = ExecuteProcess(
                cmd=['gzrun', [LaunchConfiguration('world'), '.sdf'],
                     '-v', '4', '-r'],
                output='screen',
             )

    xacro_file = os.path.join(fws_robot_description_path,
                              'robots',
                              'fws_robot.urdf.xacro')

    doc = xacro.process_file(xacro_file, mappings={'use_sim' : 'true'})

    robot_desc = doc.toprettyxml(indent='  ')

    # --- Workaround for controller_manager 4.45.2 bug ---
    # The CM adds --params-file with an empty value to controller node args
    # when <controller>.params_file is not set. Generate a patched YAML that
    # includes params_file entries for each controller, then swap the path
    # in the URDF so gz_ros2_control uses the patched file.
    controller_params_file = os.path.join(
        fws_robot_description_path, 'config', 'fws_robot_sim.yaml')

    with open(controller_params_file) as f:
        ctrl_params = yaml.safe_load(f)

    cm_ros_params = ctrl_params['controller_manager']['ros__parameters']
    for name in list(cm_ros_params.keys()):
        if isinstance(cm_ros_params[name], dict) and 'type' in cm_ros_params[name]:
            cm_ros_params[name]['params_file'] = controller_params_file

    patched_params_fd, patched_params_file = tempfile.mkstemp(
        suffix='.yaml', prefix='fws_ctrl_')
    with os.fdopen(patched_params_fd, 'w') as f:
        yaml.dump(ctrl_params, f, default_flow_style=False)

    # Replace the original params path in the URDF so the gz_ros2_control
    # plugin loads the patched file (which includes params_file entries).
    robot_desc = robot_desc.replace(controller_params_file, patched_params_file)

    params = {'robot_description': robot_desc}

    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[params]
    )

    gz_spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=['-string', robot_desc,
                   '-x', '0.0',
                   '-y', '0.0',
                   '-z', '0.07',
                   '-R', '0.0',
                   '-P', '0.0',
                   '-Y', '0.0',
                   '-name', 'fws_robot',
                   '-allow_renaming', 'false'],
    )

    load_joint_state_controller = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster',
                   '--param-file', controller_params_file],
        output='screen'
    )

    load_forward_velocity_controller = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['forward_velocity_controller',
                   '--param-file', controller_params_file],
        output='screen'
    )

    load_forward_position_controller = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['forward_position_controller',
                   '--param-file', controller_params_file],
        output='screen'
    )

    # Bridge
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=['/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock',
                   '/scan@sensor_msgs/msg/LaserScan@gz.msgs.LaserScan'],
        output='screen'
    )

    rviz_config_file = os.path.join(fws_robot_description_path, 'config', 'fws_robot_config.rviz')

    rviz = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="log",
        arguments=["-d", rviz_config_file],
    )

    return LaunchDescription([
        RegisterEventHandler(
            event_handler=OnProcessExit(
                target_action=gz_spawn_entity,
                on_exit=[load_joint_state_controller],
            )
        ),
        RegisterEventHandler(
            event_handler=OnProcessExit(
               target_action=load_joint_state_controller,
               on_exit=[load_forward_velocity_controller,
                        load_forward_position_controller],
            )
        ),
        gazebo_resource_path,
        gazebo_plugin_path,
        arguments,
        gazebo,
        node_robot_state_publisher,
        gz_spawn_entity,
        bridge,
        rviz,
    ])
