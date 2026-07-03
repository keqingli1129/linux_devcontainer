add_library(Chapter01
    Lib/lib.cpp
    Lib/ScopeTest.cpp
    Lib/shared.cpp
    Lib/unique.cpp
)

target_include_directories(Chapter01 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
