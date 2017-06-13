# Set the name and the supported language of the project
set (NAME gears)
project(${NAME})

# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 2.6)

#ADD_DEFINITIONS( "-std=c++11" )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
#target_compile_features(foobar PRIVATE cxx_range_for)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
#pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
#pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
#pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
<<<<<<< HEAD
pkg_check_modules(GTKGLEXT REQUIRED gtkglext-1.0)
=======
#pkg_check_modules(GTKGLEXT REQUIRED gtkglext-1.0)
>>>>>>> 083b3352b2b933f80ef323fbf2b4a357b5062f96
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
include_directories(${GTK2_INCLUDE_DIRS})
<<<<<<< HEAD
#	include_directories("${GTK2_INCLUDE_DIRS}/gtk-2.0")
#include_directories(${GTKMM_INCLUDE_DIRS})
#include_directories(${GLIBMM_INCLUDE_DIRS})
include_directories(${GTKGLEXT_INCLUDE_DIRS})
include_directories ("D:/Luciano/Libs/gtkglext3-install/include")
include_directories ("D:/Luciano/Libs/gtkglext3-install/include/gdk")
include_directories ("D:/Luciano/Libs/gtkglext3-install/include/gtk")

link_directories(${GTK2_LIBRARY_DIRS})
#link_directories(${GTKMM_LIBRARY_DIRS})
#link_directories(${GLIBMM_INCLUDE_DIRS})
link_directories(${GTKGLEXT_LIBRARY_DIRS})
#link_directories ("D:/Luciano/Libs/VTK-6.3.0-Install/lib")
link_directories ("D:/Luciano/Libs/gtkglext3-install/lib")
link_directories ("D:/Luciano/Libs/gtk-2.24-bundle/lib")
=======
include_directories("D:/Luciano/Libs/gtkglext-1.2.0")
include_directories("D:/Luciano/Libs/gtkglext-1.2.0/gtk")
include_directories("D:/Luciano/Libs/gtkglext-1.2.0/gdk")

link_directories(${GTK2_LIBRARY_DIRS})
>>>>>>> 083b3352b2b933f80ef323fbf2b4a357b5062f96




# Add other flags to the compiler
<<<<<<< HEAD
add_definitions(${GTK3_CFLAGS_OTHER})
=======
#add_definitions(${GTK3_CFLAGS_OTHER})
>>>>>>> 083b3352b2b933f80ef323fbf2b4a357b5062f96

# If necessary, use the RELATIVE flag, otherwise each source file may be listed 
# with full pathname. RELATIVE may makes it easier to extract an executable name
# automatically.
# file( GLOB APP_SOURCES RELATIVE app/*.cxx )
# file( GLOB APP_SOURCES *.c )
# foreach( testsourcefile ${APP_SOURCES} )
    # # I used a simple string replace, to cut off .cpp.
    # string( REPLACE ".c" "" testname ${testsourcefile} )
    # add_executable( ${testname} ${testsourcefile} )
    # # Make sure YourLib is linked to each app
    # target_link_libraries( ${testname} gtkglext-3.0 gdkglext-3.0 ${GTK3_LIBRARIES})
# endforeach( testsourcefile ${APP_SOURCES} )

add_executable( ${NAME} gears.c)
<<<<<<< HEAD
target_link_libraries( ${NAME} gtkglext-3.0 gdkglext-3.0 ${GTK2_LIBRARIES} opengl32 glu32)
=======
target_link_libraries( ${NAME}  ${GTK2_LIBRARIES} gtkglext-win32-1.0 gdkglext-win32-1.0 opengl32 glu32)
>>>>>>> 083b3352b2b933f80ef323fbf2b4a357b5062f96
