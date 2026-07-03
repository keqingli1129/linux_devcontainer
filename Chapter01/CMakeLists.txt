add_library(Chapter01
    Lib/lib.cpp
    Lib/shared.cpp
)

target_include_directories(Chapter01 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
