# Set the name and the supported language of the project
set (NAME gtkglarea-simple)
project(${NAME})

# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 2.6)

#ADD_DEFINITIONS( "-std=c++11" )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
#target_compile_features(foobar PRIVATE cxx_range_for)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
#pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

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

include_directories(${GTK3_INCLUDE_DIRS})
# include_directories("D:/Luciano/Programas/msys64/mingw64/include/atk-1.0")
# include_directories("D:/Luciano/Programas/msys64/mingw64/include/pango-1.0")
# include_directories("D:/Luciano/Programas/msys64/mingw64/include/gtk-3.0")
# include_directories("D:/Luciano/Programas/msys64/mingw64/include/glib-2.0")
# include_directories("D:/Luciano/Programas/msys64/mingw64/include/cairo")
# include_directories("D:/Luciano/Programas/msys64/mingw64/lib/glib-2.0/include")
# include_directories("D:/Luciano/Programas/msys64/mingw64/include/gdk-pixbuf-2.0")

link_directories(${GTK3_LIBRARY_DIRS})


# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable( ${NAME} simple.c)
target_link_libraries( ${NAME} ${GTK3_LIBRARIES} opengl32)