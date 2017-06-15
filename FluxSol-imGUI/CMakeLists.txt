cmake_minimum_required(VERSION 2.8.4)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

set (NAME FluxSol-GUI)
project(${NAME})

#VTK
IF(NOT VTK_BINARY_DIR)
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF()
INCLUDE(${VTK_USE_FILE})
ENDIF()

set (GLFW_DIR "D:/Luciano/Libs/glfw-install")
set (IMGUI_DIR "D:/Luciano/Libs/imgui-v1.5")

include_directories("${GLFW_DIR}/include")
include_directories("${IMGUI_DIR}")

link_directories ("D:/Luciano/Libs/VTK-6.3.0-Install/lib")
link_directories("${GLFW_DIR}/lib")
link_directories ("${IMGUI_DIR}/bin")

#
# vtkFLTK source files
#
SET (Kit_SRCS
Main.cpp
imgui_impl_glfw.cpp
vtkImRenderWindowInteractor.cpp
#Timer.cpp
#ifdef WIN32
Timer_win32.cpp
#sigev_thread.cpp
#endif
)

#
# Create the vtkFLTK C++ library.
#
ADD_EXECUTABLE (${NAME} ${Kit_SRCS} )
TARGET_LINK_LIBRARIES (${NAME} glfw3 imgui ${VTK_LIBRARIES})
