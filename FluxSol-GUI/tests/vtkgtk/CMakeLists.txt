# Set the name and the supported language of the project
set (NAME FluxSol-GUI)
project(${NAME})

# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 2.6)

ADD_DEFINITIONS( "-std=c++11" )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
#target_compile_features(foobar PRIVATE cxx_range_for)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
#pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
#pkg_check_modules(GTKGLEXT REQUIRED gtkglext-1.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
#VTK
IF(NOT VTK_BINARY_DIR)
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF()
INCLUDE(${VTK_USE_FILE})
ENDIF()

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${GTKGLEXT_INCLUDE_DIRS})
include_directories ("D:/Luciano/Libs/gtkglext3-install/include")
include_directories ("D:/Luciano/Libs/gtkglext3-install/include/gdk")
include_directories ("D:/Luciano/Libs/gtkglext3-install/include/gtk")

link_directories(${GTK3_LIBRARY_DIRS})
link_directories(${GTKGLEXT_LIBRARY_DIRS})
link_directories ("D:/Luciano/Libs/VTK-6.3.0-Install/lib")
link_directories ("D:/Luciano/Libs/gtkglext3-install/lib")

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})
# Add an executable compiled from hello.c
add_executable(${NAME} creating-sphere-and-cube.cxx vtkGtkRenderWindowInteractor.cxx)
# Link the target to the GTK+ libraries
target_link_libraries(${NAME} gtkglext-3.0 gdkglext-3.0 ${VTK_LIBRARIES}  ${GTK3_LIBRARIES})