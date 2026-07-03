cmake_minimum_required(VERSION 3.10.0)
project(linux_devcontainer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    GIT_SHALLOW TRUE
)
set(EIGEN_BUILD_DOC OFF)
set(EIGEN_BUILD_TESTING OFF)
set(BUILD_TESTING OFF)
FetchContent_MakeAvailable(Eigen)
find_package(OpenCV REQUIRED)
add_subdirectory(Chapter01)

add_executable(linux_devcontainer main.cpp)
target_link_libraries(linux_devcontainer PRIVATE Chapter01 Eigen3::Eigen ${OpenCV_LIBS})
