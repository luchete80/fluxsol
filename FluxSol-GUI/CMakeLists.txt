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
set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")

set (MY_PETSC_DIR "$ENV{PETSC_DIR}")
set (MPI_LIB_DIR "$ENV{MPI_LIB_DIR}")
set (MPI_INC_DIR "$ENV{MPI_INC_DIR}")
set (MY_PETSC_ARCH "$ENV{PETSC_ARCH}")

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

# include_directories("${FLUXSOL_DIR}/FluxSol-Solver")
# include_directories("${FLUXSOL_DIR}")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Analysis")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Boundary")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Field")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/FiniteVolume")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/FiniteVolume/Div")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/FiniteVolume/Grad")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/FiniteVolume/Laplacian")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/FiniteVolume/Mesh")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Input")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Interpolation")
# #include_directories("${PROJECT_SOURCE_DIR}/Libs/Laspack")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Materials")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Mesh")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Model")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Nastran")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Output")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/SistEcuac")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Solver")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Tests")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Time")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Type")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/UDO")
# include_directories("${FLUXSOL_DIR}/FluxSol-Solver/Utils")

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


add_library(main SHARED		$<TARGET_OBJECTS:main_>)
add_library(editor SHARED 	$<TARGET_OBJECTS:editor_>
							$<TARGET_OBJECTS:editorplugins_>
							$<TARGET_OBJECTS:editorioplugins_>
							)
							
target_link_libraries (editor scene servers core)

target_link_libraries (main editor servers driverstypes core)


#Shlwapi is for PathFileExistsW
#target_link_libraries (windows core zlib Shlwapi)

add_executable (${NAME} 
$<TARGET_OBJECTS:app_>	$<TARGET_OBJECTS:main_>
#$<TARGET_OBJECTS:gVTK_>
#GraphicModel.cpp
)

# set (USE_PETSC ON)
# if (USE_PETSC)
	# find_package (PETSc REQUIRED HINTS "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/conf")
	# include_directories (..)
	# include_directories ("${MY_PETSC_DIR}/include" "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/include" ${PETSC_INCLUDE_PATH})
	
	# #THIS IS NOT USED IN WINDOWS, USED MSMPI
	# #include_directories ("${MY_PETSC_DIR}/include/mpiuni")
	
	# include_directories ("E:/Programas/cygwin64/petsc-3.4.4/include")
	# find_library (PETSC_LIB petsc PATH "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/lib/") 
	# if (${PETSC_LIB} STREQUAL "PETSC_LIB-NOTFOUND")
		# message (FATAL_ERROR "PETSc library not found")
	# endif ()
	# ##set (EXT_LIBS ${EXT_LIBS} ${PETSC_LIB} ${PETSC_PACKAGE_LIBS})

	# #Blas And Lapack, Manual, can be also automatic
	# set (PETSC_LIBRARY_DIRS  ${MY_PETSC_DIR}/${MY_PETSC_ARCH}/lib)
	# link_directories(${PETSC_LIBRARY_DIRS})
	# set (EXT_LIBS ${EXT_LIBS} ${PETSC_LIB} flapack fblas)

	# set (MODULE_LIST ${MODULE_LIST} "PETSc")
	# set (USE_MPI ON)
	# add_definitions (-D__PETSC_MODULE)


	# link_directories(${MPI_LIB_DIR})
	# include_directories(${MPI_INC_DIR})

	# set (EXT_LIBS ${EXT_LIBS} msmpi gfortran)
	

# endif ()


#BESIDES MAIN AND CORE HAVE IT,
#SERVER LIBRARY MUST BE LINKED ALSO IN ORDER TO LINK VISUAL SERVER GLES2
target_link_libraries (${NAME} 
main
servers driverstypes core scene
windows unix 
gl_context gles2
winmm 
opengl32
stdc++
${VTK_LIBRARIES}
)
