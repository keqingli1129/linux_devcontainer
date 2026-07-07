cmake_minimum_required(VERSION 3.18.0)
project(linux_devcontainer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

# Warnings for project targets only; third-party/imported targets are unaffected.
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Wshadow)
# Exported under Chapter01Targets because Chapter01 privately links it; a static
# library's install(EXPORT ...) must be able to resolve every linked target.
install(TARGETS project_warnings EXPORT Chapter01Targets)

include(FetchContent)
FetchContent_Declare(
    Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL
)
set(EIGEN_BUILD_DOC OFF)
set(EIGEN_BUILD_TESTING OFF)
set(BUILD_TESTING OFF)
FetchContent_MakeAvailable(Eigen)

find_package(OpenCV REQUIRED)

set(USE_SYSTEM_NVTX ON CACHE BOOL "Use system NVTX for libtorch")
set(CAFFE2_USE_CUDNN ON CACHE BOOL "Enable cuDNN support")
set(CAFFE2_USE_CUSPARSELT ON CACHE BOOL "Enable cuSPARSELt support")
find_package(Torch REQUIRED PATHS /opt/libtorch NO_DEFAULT_PATH)

add_subdirectory(Chapter01)

# `cpack --config build/CPackConfig.cmake` bundles the install() rules above
# (Chapter01 lib, headers, find_package config) into a distributable archive.
set(CPACK_PACKAGE_NAME "Chapter01")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "keqing.li@fortbendisd.gov")
set(CPACK_GENERATOR "TGZ")
include(CPack)
