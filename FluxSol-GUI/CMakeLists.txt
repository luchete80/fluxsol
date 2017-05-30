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
set (GODOTGUI_SRC_DIR "D:/Luciano/Libs/GodotGUI")
set (GODOTGUI_BIN_DIR "$ENV{GODOTGUI_BIN_DIR}")

link_directories ("D:/Luciano/Libs/VTK-6.3.0-Install/lib")
link_directories ("${GODOTGUI_BIN_DIR}")


#GODOTGUI
include_directories("${GODOTGUI_SRC_DIR}")
include_directories("${GODOTGUI_SRC_DIR}/core")
include_directories("${GODOTGUI_SRC_DIR}/core/math")
include_directories("${GODOTGUI_SRC_DIR}/core/io")
include_directories("${GODOTGUI_SRC_DIR}/core/os")
include_directories("${GODOTGUI_SRC_DIR}/drivers")
include_directories("${GODOTGUI_SRC_DIR}/drivers/freetype")
include_directories("${GODOTGUI_SRC_DIR}/drivers/freetype/freetype/include")
include_directories("${GODOTGUI_SRC_DIR}/drivers/png")
include_directories("${GODOTGUI_SRC_DIR}/drivers/windows")
include_directories("${GODOTGUI_SRC_DIR}/tools/editor")
include_directories("${GODOTGUI_SRC_DIR}/tools/editor/plugins")
include_directories("${GODOTGUI_SRC_DIR}/tools/editor/io_plugins")
include_directories("${GODOTGUI_SRC_DIR}/scene/2d")
include_directories("${GODOTGUI_SRC_DIR}/scene/gui")
include_directories("${GODOTGUI_SRC_DIR}/platform/windows")
include_directories("${GODOTGUI_SRC_DIR}/servers")
include_directories("${GODOTGUI_SRC_DIR}/servers/physics_2d")
include_directories("${GODOTGUI_SRC_DIR}/servers/visual")


#FLUXSOL
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
add_subdirectory("${PROJECT_SOURCE_DIR}/gVTK")

add_library(main SHARED			$<TARGET_OBJECTS:main_>)
add_library(editor SHARED 	$<TARGET_OBJECTS:editor_>
							$<TARGET_OBJECTS:editorplugins_>
							$<TARGET_OBJECTS:editorioplugins_>
							)

add_library(gVTK $<TARGET_OBJECTS:gVTK_>)							
target_link_libraries (editor scene servers core)

target_link_libraries (main editor servers driverstypes core)

target_link_libraries (gVTK editor scene core)


#Shlwapi is for PathFileExistsW
#target_link_libraries (windows core zlib Shlwapi)

add_executable (${NAME} 
$<TARGET_OBJECTS:app_>	
#$<TARGET_OBJECTS:gVTK_>
)

#BESIDES MAIN AND CORE HAVE IT,
#SERVER LIBRARY MUST BE LINKED ALSO IN ORDER TO LINK VISUAL SERVER GLES2
target_link_libraries (${NAME} 
servers driverstypes main core scene
windows unix 
gl_context gles2
winmm 
opengl32
)
