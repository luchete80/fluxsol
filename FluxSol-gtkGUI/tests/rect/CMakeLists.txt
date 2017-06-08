cmake_minimum_required(VERSION 2.6)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

PROJECT (Rect)

ADD_EXECUTABLE(Rect WIN32 rect.cpp)