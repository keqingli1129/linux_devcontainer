# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

A C++ learning project organized into chapters (`Chapter01/`, ...), developed inside a Dev Container. There is currently no build system (no CMakeLists.txt or Makefile) — source files are compiled and run individually.

## Environment

Development happens in a VS Code Dev Container defined in `.devcontainer/`:
- Base image: `mcr.microsoft.com/devcontainers/cpp:2-ubuntu24.04` (GCC toolchain + vcpkg).
- `reinstall-cmake.sh` builds CMake 3.22.2 from source at image build time (controlled by the `REINSTALL_CMAKE_VERSION_FROM_SOURCE` build arg).
- The host is Windows, but compilation runs inside the Linux container — use the container's `g++`/`clang++`, not host tooling.

## Building and running

No project-wide build. Compile and run a single source file directly, e.g.:

```bash
g++ -std=c++17 Chapter01/Main.cc -o Main && ./Main
```

When introducing a build system or multi-file targets, prefer CMake (already available in the container via vcpkg) over ad-hoc compile commands.
