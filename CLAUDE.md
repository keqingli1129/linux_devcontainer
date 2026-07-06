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
