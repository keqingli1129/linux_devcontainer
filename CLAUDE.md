# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

A C++ learning project organized into chapters (`Chapter01/`, ...), developed inside a Dev Container. Built with CMake (presets in `CMakePresets.json`). Dependencies: Eigen (via FetchContent), OpenCV (system package), LibTorch with CUDA (unpacked at `/opt/libtorch` in the container image).

## Building and running

```bash
cmake --preset debug          # configure into build/ (exports compile_commands.json)
cmake --build --preset debug  # build all targets
./build/Chapter01/ch01_basics # run a demo
```

Each chapter is a subdirectory with its own `CMakeLists.txt` contributing one small executable per demo (`ch01_basics`, `ch01_eigen`, `ch01_torch`, `ch01_camera`) plus an optional chapter library (`Chapter01` from `Chapter01/Lib/`). When adding a new lesson, add a new `chNN_<topic>.cpp` and `add_executable` entry rather than editing an existing demo.

All project targets link `project_warnings` (an INTERFACE library defined in the top-level `CMakeLists.txt`) to get `-Wall -Wextra -Wpedantic -Wshadow`.

## Environment

Development happens in a VS Code Dev Container defined in `.devcontainer/`:
- Base image: `mcr.microsoft.com/devcontainers/cpp:2-ubuntu24.04` (GCC toolchain + vcpkg), plus CUDA toolkit 12.6 and LibTorch installed by the Dockerfile.
- The host is Windows (WSL2). Compilation runs inside the Linux container — use the container's toolchain, not host tooling.
- `devcontainer.json` passes through the GPU (`--gpus=all`) and the WSL2-attached webcam (`/dev/video0`, `/dev/video1`); container creation fails if no webcam is attached via usbipd.
- GUI output (`cv::imshow`) uses X11 via the `DISPLAY` env var set in `devcontainer.json`.

`ch01_camera` needs the webcam and an X display, so it can't be exercised headlessly; verify camera changes by building only.

## ROS 2 Workspace (`ros2_ws/`)

A colcon workspace with ROS 2 Jazzy packages for a four-wheel-steering (FWS) robot simulation.

### Packages

- `fws_robot_description` – URDF/xacro, meshes, ros2_control config, rviz config
- `fws_robot_sim` – Gazebo Harmonic simulation launch (world, spawn, controllers)
- `velocity_pub` / `my_cpp_pkg` – custom nodes

### Building and launching the simulation

```bash
cd /workspaces/linux_devcontainer/ros2_ws
colcon build
source install/setup.bash
ros2 launch fws_robot_sim fws_robot_spawn.launch.py
```

### Known issues & fixes (Jazzy / gz_ros2_control 1.2.19 / controller_manager 4.45.2)

1. **`gz_ros2_control-system` plugin not found** – Gazebo cannot locate the plugin at `/opt/ros/jazzy/lib/libgz_ros2_control-system.so` unless `GZ_SIM_SYSTEM_PLUGIN_PATH` is set. The launch file sets this via `SetEnvironmentVariable`.

2. **Controller loading fails with `Couldn't parse params file: '--params-file -p'`** – In controller_manager 4.45.2, the old `ros2 control load_controller` approach results in empty `--params-file` arguments passed to controller nodes. Fix: use `controller_manager` `spawner` nodes with `--param-file <yaml>` instead of `ExecuteProcess(['ros2', 'control', 'load_controller', ...])`. Additionally, controller_manager 4.45.2 has a bug where it always adds `--params-file` with an empty value to controller node args even when `<controller>.params_file` is not set. The launch file works around this by generating a patched YAML at launch time that injects `params_file` entries for each controller, then swapping the path in the URDF so `gz_ros2_control` uses the patched file.

3. **"No clock received" warnings** – The Gazebo simulation must be unpaused (press Play in the GUI, or ensure the `-r` flag is correctly forwarded through the `gzrun` VirtualGL wrapper) for the `/clock` topic to publish and `use_sim_time` to work. The launch file bridges `/clock` from Gazebo transport (`gz.msgs.Clock`) to ROS 2 (`rosgraph_msgs/msg/Clock`) via `ros_gz_bridge`.

4. **No GUI windows visible (linux-ssh devcontainer)** – The `DISPLAY` env var in `.devcontainer/linux-ssh/devcontainer.json` may not match the actual X11 socket. Check `echo $REMOTE_CONTAINERS_DISPLAY_SOCK` and update the `DISPLAY` value in `remoteEnv` to match (e.g. `:1` instead of `:0`). For the current session, `export DISPLAY=:1` in each terminal.
