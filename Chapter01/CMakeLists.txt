add_library(Chapter01
    Lib/lib.cpp
    Lib/ScopeTest.cpp
    Lib/shared.cpp
    Lib/unique.cpp
)
target_include_directories(Chapter01 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Chapter01 PRIVATE project_warnings)

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
