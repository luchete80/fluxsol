cmake_minimum_required(VERSION 2.8.9)

set (CMAKE_BUILD_TYPE Release)

#set(CMAKE_PREFIX_PATH $ENV{QTDIR}) # Unnecessary on Windows apparently
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -static-libgcc -static-libstdc++")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpermissive -static-libgcc -static-libstdc++")

set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s")
#SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mno-cygwin")
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -static-libgcc")

#link_directories("/cygdrive/c/cygwin64/lib/gcc/x86_64-w64-mingw32/4.8.2")
set (NAME Tetrahedron)
project(${NAME})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(OpenGL)
#find_package(Qt5OpenGLExtensions)
#find_package(hola)
#find_package(Qt5 REQUIRED)
set( QT_USE_QTOPENGL TRUE )

#add_executable(qt5-hello main.cpp)
add_executable(${NAME} Tetrahedron.cpp main.cpp)

set (EXTRA_LIBS opengl32 glu32 gdi32)
target_link_libraries (${NAME} ${EXTRA_LIBS} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
#target_link_libraries (qt5-hello gcc_s)

#  qt5_use_modules(hello_world Widgets Declarative)
# This is similar to how qmake operates:
# TARGET = hello_world
# QT += widgets declarative


qt5_use_modules(${NAME} Widgets OpenGL)