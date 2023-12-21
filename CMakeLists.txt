cmake_minimum_required(VERSION 3.14)

project(debug)
set(CMAKE_CXX_STANDARD 20)

include(FindPkgConfig)
pkg_search_module(MPV REQUIRED mpv>=0.24.0)

add_subdirectory(third_party/fmt)
add_subdirectory(third_party/imgui)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
add_library(debug SHARED
    src/debug.cpp
    src/main.cpp
)
set_property(TARGET debug PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(debug PRIVATE ${MPV_INCLUDE_DIRS})
target_link_libraries(debug PRIVATE fmt imgui)
target_compile_definitions(debug PRIVATE
    $<$<BOOL:${WIN32}>:MPV_CPLUGIN_DYNAMIC_SYM>
)
