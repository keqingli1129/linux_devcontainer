FROM mcr.microsoft.com/devcontainers/cpp:2-ubuntu24.04

# The FBISD network intercepts HTTPS and re-signs it with the district CA
# (FBISD_PA_Trust), which the image doesn't trust — build-time downloads
# (CUDA keyring, apt over HTTPS, LibTorch) fail without it. Harmless when
# building off-network. Re-export from the Windows cert store if it rotates.
COPY ./fbisd-pa-trust.crt /usr/local/share/ca-certificates/fbisd-pa-trust.crt
RUN update-ca-certificates

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
# libglu1-mesa-dev: GL/glu.h, required by Open3D's bundled GLEW headers.
# libc++1/libc++abi1: the official Open3D prebuilt binary links libc++, not libstdc++.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends libopencv-dev \
    x11-apps libgtk-3-dev unzip libglu1-mesa-dev \
    libc++1 libc++abi1 \
    && rm -rf /var/lib/apt/lists/*

# CUDA toolkit 12.8 – minimum required for native Blackwell (RTX 50-series) support.
# Needed so CMake's TorchConfig can locate CUDA headers/libs.
RUN curl -fsSL https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb \
        -o /tmp/cuda-keyring.deb \
    && dpkg -i /tmp/cuda-keyring.deb && rm /tmp/cuda-keyring.deb \
    && apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        cuda-toolkit-12-8 \
        cudnn-jit \
        cusparselt-cuda-12 \
    && rm -rf /var/lib/apt/lists/*

ENV PATH="/usr/local/cuda-12.8/bin:${PATH}"
ENV LD_LIBRARY_PATH="/usr/local/cuda-12.8/lib64:${LD_LIBRARY_PATH}"

# LibTorch (PyTorch C++ API), CUDA build.
ARG LIBTORCH_VERSION="2.7.1"
ARG LIBTORCH_CUDA="cu128"
RUN curl -fsSL "https://download.pytorch.org/libtorch/${LIBTORCH_CUDA}/libtorch-cxx11-abi-shared-with-deps-${LIBTORCH_VERSION}%2B${LIBTORCH_CUDA}.zip" -o /tmp/libtorch.zip \
    && unzip -q /tmp/libtorch.zip -d /opt \
    && rm /tmp/libtorch.zip

ENV CMAKE_PREFIX_PATH="/opt/libtorch"

# Open3D (prebuilt devel package, CUDA build, cxx11 ABI to match LibTorch above).
ARG OPEN3D_VERSION="0.19.0"
RUN curl -fsSL "https://github.com/isl-org/Open3D/releases/download/v${OPEN3D_VERSION}/open3d-devel-linux-x86_64-cxx11-abi-cuda-${OPEN3D_VERSION}.tar.xz" -o /tmp/open3d.tar.xz \
    && mkdir -p /opt/open3d \
    && tar -xJf /tmp/open3d.tar.xz -C /opt/open3d --strip-components=1 \
    && rm /tmp/open3d.tar.xz

ENV CMAKE_PREFIX_PATH="/opt/libtorch:/opt/open3d"

# ROS 2 Jazzy (LTS for Ubuntu 24.04)
RUN curl -fsSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key \
        -o /usr/share/keyrings/ros-archive-keyring.gpg \
    && echo "deb [arch=amd64 signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu noble main" \
        > /etc/apt/sources.list.d/ros2.list \
    && apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        ros-jazzy-desktop \
        ros-dev-tools \
        ros-jazzy-ros-gz \
    && rm -rf /var/lib/apt/lists/*

RUN echo "source /opt/ros/jazzy/setup.bash" >> /home/vscode/.bashrc

# VirtualGL: Gazebo's Ogre2 renderer needs a real GL context, but the
# linux-ssh devcontainer's DISPLAY is forwarded over the network (indirect
# GLX), which NVIDIA's driver won't accelerate and which hangs Gazebo's
# context setup. vglrun redirects the GL calls to the container's real GPU
# and only streams finished frames to the forwarded X display.
ARG VIRTUALGL_VERSION="3.1.4"
RUN curl -fsSL "https://github.com/VirtualGL/virtualgl/releases/download/${VIRTUALGL_VERSION}/virtualgl_${VIRTUALGL_VERSION}_amd64.deb" \
        -o /tmp/virtualgl.deb \
    && apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends /tmp/virtualgl.deb \
    && rm -f /tmp/virtualgl.deb && rm -rf /var/lib/apt/lists/* \
    && groupadd -r vglusers \
    && usermod -aG vglusers vscode

# nvidia-container-toolkit mounts the driver's .so files at container start
# (--gpus=all) but not a GLVND vendor manifest for them, so without this,
# EGL_PLATFORM_DEVICE enumeration only finds Mesa's software EGL and Gazebo's
# GL context silently falls back to CPU rendering (works, but far too slow —
# reads as a hung/unresponsive window instead of an error).
RUN mkdir -p /usr/share/glvnd/egl_vendor.d && \
    printf '%s\n' \
        '{' \
        '    "file_format_version" : "1.0.0",' \
        '    "ICD" : {' \
        '        "library_path" : "libEGL_nvidia.so.0"' \
        '    }' \
        '}' > /usr/share/glvnd/egl_vendor.d/10_nvidia.json

COPY ./gzrun.sh /usr/local/bin/gzrun
RUN chmod +x /usr/local/bin/gzrun
