add_library(Chapter01
    src/basics/lib.cpp
    src/memory/ScopeTest.cpp
    src/memory/shared.cpp
    src/memory/unique.cpp
    src/demos.cpp
)
target_include_directories(Chapter01
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(Chapter01 PRIVATE project_warnings)

install(TARGETS Chapter01 EXPORT Chapter01Targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Lets consuming projects do find_package(Chapter01) + target_link_libraries(... Chapter01::Chapter01).
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/Chapter01ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Chapter01Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/Chapter01Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Chapter01
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/Chapter01Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Chapter01ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Chapter01
)
install(EXPORT Chapter01Targets
    FILE Chapter01Targets.cmake
    NAMESPACE Chapter01::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Chapter01
)

# One small executable per demo, so every lesson stays runnable.
add_executable(ch01_basics ch01_basics.cpp)
target_link_libraries(ch01_basics PRIVATE Chapter01 project_warnings)

add_executable(ch01_eigen ch01_eigen.cpp)
target_link_libraries(ch01_eigen PRIVATE Eigen3::Eigen project_warnings)

add_executable(ch01_torch ch01_torch.cpp)
target_link_libraries(ch01_torch PRIVATE ${TORCH_LIBRARIES} project_warnings)
target_compile_options(ch01_torch PRIVATE ${TORCH_CXX_FLAGS})

add_executable(ch01_camera ch01_camera.cpp)
target_link_libraries(ch01_camera PRIVATE ${OpenCV_LIBS} project_warnings)
