cmake_minimum_required(VERSION 3.14)
project(fmt)

set(CMAKE_CXX_STANDARD 17)

add_library(fmt OBJECT src/format.cc src/os.cc)
set_property(TARGET fmt PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(fmt PUBLIC include)
