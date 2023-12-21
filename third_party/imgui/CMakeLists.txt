cmake_minimum_required(VERSION 3.14)
project(imgui)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.9
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

add_library(imgui OBJECT
  src/imgui.cpp
  src/imgui_demo.cpp
  src/imgui_draw.cpp
  src/imgui_impl_opengl3.cpp
  src/imgui_impl_glfw.cpp
  src/imgui_tables.cpp
  src/imgui_widgets.cpp
)
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(imgui PUBLIC include)
target_link_libraries(imgui PUBLIC glfw)
target_compile_definitions(imgui PRIVATE
  $<$<NOT:$<CONFIG:Debug>>:IMGUI_DISABLE_DEMO_WINDOWS IMGUI_DISABLE_DEBUG_TOOLS>
)
