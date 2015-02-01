cmake_minimum_required(VERSION 2.8.9)

set (CMAKE_BUILD_TYPE Release)

#set(CMAKE_PREFIX_PATH $ENV{QTDIR}) # Unnecessary on Windows apparently
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -static-libgcc -static-libstdc++")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpermissive -static-libgcc -static-libstdc++")

set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s")
#SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -mno-cygwin")
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -static-libgcc")

#link_directories("/cygdrive/c/cygwin64/lib/gcc/x86_64-w64-mingw32/4.8.2")

project(qt5-hello)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
#add_executable(qt5-hello main.cpp)
add_executable(qt5-hello hello.cpp)

#target_link_libraries (qt5-hello gcc_s)

qt5_use_modules(qt5-hello Widgets)