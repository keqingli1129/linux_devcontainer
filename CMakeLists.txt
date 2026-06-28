cmake_minimum_required(VERSION 3.10.0)
project(linux_devcontainer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Chapter01)

add_executable(linux_devcontainer main.cpp)
target_link_libraries(linux_devcontainer PRIVATE Chapter01)
