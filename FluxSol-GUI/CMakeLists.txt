cmake_minimum_required(VERSION 2.8.4)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

set (NAME FluxSol-GUI)
project(${NAME})


include_directories(".")
include_directories("${PROJECT_SOURCE_DIR}")

include_directories("${PROJECT_SOURCE_DIR}/main")
include_directories("${PROJECT_SOURCE_DIR}/platform/windows")


add_subdirectory("${PROJECT_SOURCE_DIR}/main")
add_subdirectory("${PROJECT_SOURCE_DIR}/platform/windows")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools/editor")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools/editor/io_plugins")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools/editor/plugins")

set (SOURCES ./main/main.cpp)


add_library(main $<TARGET_OBJECTS:main_>)

add_library(editor SHARED $<TARGET_OBJECTS:editor_>
							$<TARGET_OBJECTS:editorplugins_>
							$<TARGET_OBJECTS:editorioplugins_>
							)

target_link_libraries (editor scene servers core)

#Shlwapi is for PathFileExistsW
target_link_libraries (windows core zlib Shlwapi)

target_link_libraries (main editor servers driverstypes)


add_executable (${NAME} $<TARGET_OBJECTS:app_> $<TARGET_OBJECTS:visualserver_>)
