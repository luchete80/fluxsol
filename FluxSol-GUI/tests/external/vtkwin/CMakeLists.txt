cmake_minimum_required(VERSION 2.6)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

set (NAME vtkwin)
project(${NAME})

set (FREEGLUT_DIR "$ENV{FREEGLUT_DIR}")

include_directories("${FREEGLUT_DIR}/include")
link_directories("${FREEGLUT_DIR}/lib")

#VTK
IF(NOT VTK_BINARY_DIR)
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF()
INCLUDE(${VTK_USE_FILE})
ENDIF()

 # set_target_properties(${NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
 
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mconsole")
set (CMAKE_LINKER_FLAGS "-mconsole")

add_executable(${NAME} vtkwin.cpp)
target_link_libraries (${NAME} 
opengl32 glu32
${VTK_LIBRARIES}
)