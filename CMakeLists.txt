cmake_minimum_required(VERSION 3.13)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

project(debug)
set(CMAKE_CXX_STANDARD 20)

include(FindPkgConfig)
pkg_search_module(MPV REQUIRED mpv>=0.37.0)

add_subdirectory(third_party/fmt)
add_subdirectory(third_party/imgui)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
add_library(debug SHARED
    src/debug.cpp
    src/main.cpp
)

target_include_directories(debug PUBLIC include PRIVATE ${MPV_INCLUDE_DIRS})
target_link_libraries(debug PUBLIC fmt imgui)
target_compile_definitions(debug PRIVATE
    $<$<BOOL:${WIN32}>:MPV_CPLUGIN_DYNAMIC_SYM>
)