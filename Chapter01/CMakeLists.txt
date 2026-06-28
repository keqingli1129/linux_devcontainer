add_library(Chapter01
    Lib/lib.cc
)

target_include_directories(Chapter01 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
