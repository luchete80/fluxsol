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


#DIRECTORIES
link_directories ("D:/Luciano/Libs/VTK-6.3.0-Install/lib")

include_directories(".")
include_directories("${PROJECT_SOURCE_DIR}")

include_directories("${PROJECT_SOURCE_DIR}/main")
include_directories("${PROJECT_SOURCE_DIR}/platform/windows")
include_directories("${PROJECT_SOURCE_DIR}/tools/editor")


add_subdirectory("${PROJECT_SOURCE_DIR}/main")
add_subdirectory("${PROJECT_SOURCE_DIR}/platform/windows")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools/editor")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools/editor/io_plugins")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools/editor/plugins")


add_library(main 			$<TARGET_OBJECTS:main_>)
add_library(editor SHARED 	$<TARGET_OBJECTS:editor_>
							$<TARGET_OBJECTS:editorplugins_>
							$<TARGET_OBJECTS:editorioplugins_>
							)

target_link_libraries (editor scene servers core)

#Shlwapi is for PathFileExistsW
target_link_libraries (windows core zlib Shlwapi)

add_executable (${NAME} 
servers
stdc++
${VTK_LIBRARIES}
)

target_link_libraries (${NAME} main editor servers driverstypes)
