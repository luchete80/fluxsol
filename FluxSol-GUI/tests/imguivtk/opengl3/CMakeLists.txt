cmake_minimum_required(VERSION 2.8.4)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

set (NAME imguivtkdemo)
project(${NAME})

set (GLFW_DIR "$ENV{GLFW_DIR}")
set (IMGUI_DIR "$ENV{IMGUI_DIR}")

include_directories ("${IMGUI_DIR}")
include_directories ("${GLFW_DIR}/include")

link_directories ("${IMGUI_DIR}/bin")
link_directories ("${GLFW_DIR}/bin")

add_executable(${NAME} imgui_impl_glfw_gl3.cpp)

target_link_libraries (${NAME} imgui glfw)