cmake_minimum_required(VERSION 3.13)
project(imgui)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(imgui OBJECT
  src/imgui.cpp
  src/imgui_demo.cpp
  src/imgui_draw.cpp
  src/imgui_impl_opengl3.cpp
  src/imgui_impl_glfw.cpp
  src/imgui_tables.cpp
  src/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC include ${OpenGL_INCLUDE_DIRS})
target_link_directories(imgui PUBLIC ${OpenGL_LIBRARY_DIRS})
target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARIES} glfw3)
target_compile_definitions(imgui PRIVATE
  $<$<NOT:$<CONFIG:Debug>>:IMGUI_DISABLE_DEMO_WINDOWS IMGUI_DISABLE_DEBUG_TOOLS>
)
