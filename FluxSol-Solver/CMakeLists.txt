cmake_minimum_required(VERSION 2.6)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

#######################################################################
######################## Options ######################################
#######################################################################
option (USE_PETSC "Enable PETSc support (required for parallel)" ON)
option (USE_METIS "Enable METIS support	                       " OFF)
if (${CMAKE_VERSION} VERSION_GREATER "2.8.7") # Compability with older version of cmake
    option (USE_SHARED_LIB "Build shared library" ON)
    option (USE_STATIC_LIB "Build static library" OFF)
endif ()
option (DEBUG_MODE "Enable FLUXSOL DEBUG" OFF)


if(DEBUG_MODE)
# Default is debug build
#if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Debug)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
	set (CMAKE_BUILD_TYPE Release)
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpermissive")
	set (CMAKE_CXX_FLAGS "-fpermissive")
endif ()

set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpermissive")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set (CMAKE_CXX_FLAGS_PROFILING "${CMAKE_CXX_FLAGS_PROFILING} -O2 -pg")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set (NAME fluxsol)

project(${NAME})

set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")
#set (FLUXSOL_DIR ${PROJECT_SOURCE_DIR}/../)

#set (LASPACK_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/laspack/src)
set (CGNS_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src)
set (ALGLIB_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/src)

set (CGNS_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src/lib)
set (ALGLIB_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/bin)
#set (LASPACK_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/laspack/bin)



set (MY_PETSC_DIR "$ENV{PETSC_DIR}")
set (MPI_LIB_DIR "$ENV{MPI_LIB_DIR}")
set (MPI_INC_DIR "$ENV{MPI_INC_DIR}")
set (MY_PETSC_ARCH "$ENV{PETSC_ARCH}")

set (PARMETIS_DIR "$ENV{PARMETIS_DIR}")

#if(DEBUG_MODE)
#set (MY_PETSC_ARCH arch-mswin-c-debug-shared-mpi0-fc0)
#else ()
#set (MY_PETSC_ARCH arch-mswin-c-release-mpi0-fc0)
#endif ()



#set (fluxsol_VERSION_MAJOR 2)
#set (fluxsol_VERSION_MINOR 0)

#list (APPEND CMAKE_MODULE_PATH "${fluxsol_SOURCE_DIR}/CMake")

# Temporalmente no lo incluyo
# Pass some CMake settings to source code through a header file
#configure_file (
#	"${PROJECT_SOURCE_DIR}/cmake_vars.h.in"
#	"${PROJECT_BINARY_DIR}/cmake_vars.h"
#)

#This must be included before including another thing else. By now
if (USE_PETSC)
	find_package (PETSc REQUIRED HINTS "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/conf")
	include_directories (..)
	include_directories ("${MY_PETSC_DIR}/include" "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/include" ${PETSC_INCLUDE_PATH})
	
	#THIS IS NOT USED IN WINDOWS, USED MSMPI
	#include_directories ("${MY_PETSC_DIR}/include/mpiuni")
	
	include_directories ("E:/Programas/cygwin64/petsc-3.4.4/include")
	find_library (PETSC_LIB petsc PATH "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/lib/") 
	if (${PETSC_LIB} STREQUAL "PETSC_LIB-NOTFOUND")
		message (FATAL_ERROR "PETSc library not found")
	endif ()
	##set (EXT_LIBS ${EXT_LIBS} ${PETSC_LIB} ${PETSC_PACKAGE_LIBS})

	#Blas And Lapack, Manual, can be also automatic
	set (PETSC_LIBRARY_DIRS  ${MY_PETSC_DIR}/${MY_PETSC_ARCH}/lib)
	link_directories(${PETSC_LIBRARY_DIRS})
	set (EXT_LIBS ${EXT_LIBS} ${PETSC_LIB} flapack fblas)

	set (MODULE_LIST ${MODULE_LIST} "PETSc")
	set (USE_MPI ON)
	add_definitions (-D__PETSC_MODULE)


	link_directories(${MPI_LIB_DIR})
	include_directories(${MPI_INC_DIR})

	set (EXT_LIBS ${EXT_LIBS} msmpi gfortran)
	

endif ()

set (CMAKE_INSTALL_PREFIX ${FLUXSOL_DIR}/bin)

# add to the include search path
include_directories(.)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/Analysis")
include_directories("${PROJECT_SOURCE_DIR}/Boundary")
include_directories("${PROJECT_SOURCE_DIR}/Field")
include_directories("${PROJECT_SOURCE_DIR}/FiniteVolume")
include_directories("${PROJECT_SOURCE_DIR}/FiniteVolume/Div")
include_directories("${PROJECT_SOURCE_DIR}/FiniteVolume/Grad")
include_directories("${PROJECT_SOURCE_DIR}/FiniteVolume/Laplacian")
include_directories("${PROJECT_SOURCE_DIR}/FiniteVolume/Mesh")
include_directories("${PROJECT_SOURCE_DIR}/Input")
include_directories("${PROJECT_SOURCE_DIR}/Interpolation")
#include_directories("${PROJECT_SOURCE_DIR}/Libs/Laspack")
include_directories("${PROJECT_SOURCE_DIR}/Materials")
include_directories("${PROJECT_SOURCE_DIR}/Mesh")
include_directories("${PROJECT_SOURCE_DIR}/Model")
include_directories("${PROJECT_SOURCE_DIR}/Nastran")
include_directories("${PROJECT_SOURCE_DIR}/Output")
include_directories("${PROJECT_SOURCE_DIR}/SistEcuac")
include_directories("${PROJECT_SOURCE_DIR}/Solver")
include_directories("${PROJECT_SOURCE_DIR}/Tests")
include_directories("${PROJECT_SOURCE_DIR}/Time")
include_directories("${PROJECT_SOURCE_DIR}/Type")
include_directories("${PROJECT_SOURCE_DIR}/UDO")
include_directories("${PROJECT_SOURCE_DIR}/Utils")

#Temporalmente no lo incluyo (librerias)
include_directories(${CGNS_INCLUDE_DIRS})
#include_directories(${LASPACK_INCLUDE_DIRS})
include_directories(${ALGLIB_INCLUDE_DIRS})

include_directories("${PARMETIS_DIR}/include")
include_directories("${PARMETIS_DIR}/metis/include")

#include_directories(${PARMETIS_INCLUDE_DIRS})
#include_directories(${PETSC_INCLUDE_DIRS})
link_directories(${CGNS_LIBRARY_DIRS})
link_directories(${ALGLIB_LIBRARY_DIRS})
link_directories(${LASPACK_LIBRARY_DIRS})

link_directories(${FLUXSOL_DIR}/Libs)
#link_directories(${PARMETIS_LIBRARY_DIRS})
#link_directories(${PETSC_LIBRARY_DIRS})

add_subdirectory(Analysis)
add_subdirectory(Boundary)
add_subdirectory(Field)
#add_subdirectory(FiniteVolume)
#add_subdirectory(FiniteVolume/Div)
add_subdirectory(FiniteVolume/Grad)
add_subdirectory(FiniteVolume/Laplacian)
add_subdirectory(FiniteVolume/Mesh)
add_subdirectory(FiniteVolume/Time)
add_subdirectory(Input)
#add_subdirectory(Interpolation)
add_subdirectory(Materials)
add_subdirectory(Mesh)
add_subdirectory(Model)
add_subdirectory(model/StatThermoFluidSol)
add_subdirectory(Nastran)
add_subdirectory(Output)
add_subdirectory(SistEcuac)
add_subdirectory(Solver)
add_subdirectory(Tests)
add_subdirectory(Time)
add_subdirectory(Type)
#add_subdirectory(UDO)
add_subdirectory(Utils)

set (FSOL_LIBS Analysis Boundary Mesh Field FvMesh Input Tests Type Utils Output)

#set (EXTRA_LIBS cgns alglib laspack)

set (EXTRA_LIBS cgns alglib)

#add the executable
set (SOURCES
FluxSol.cpp
)

add_library(fluxsol-Boundary  SHARED $<TARGET_OBJECTS:Boundary>)
add_library(fluxsol-Field     SHARED $<TARGET_OBJECTS:Field>)
add_library(fluxsol-FvMesh    SHARED $<TARGET_OBJECTS:FvMesh> $<TARGET_OBJECTS:Mesh>)
add_library(fluxsol-IO        SHARED $<TARGET_OBJECTS:Input> $<TARGET_OBJECTS:Output>)
add_library(fluxsol-Laplacian SHARED $<TARGET_OBJECTS:Laplacian>)
add_library(fluxsol-FvGrad    SHARED $<TARGET_OBJECTS:FvGrad>)
add_library(fluxsol-Model     SHARED $<TARGET_OBJECTS:Model>)
add_library(fluxsol-Solver    SHARED $<TARGET_OBJECTS:Solver>)
add_library(fluxsol-Type      SHARED $<TARGET_OBJECTS:Type>)
add_library(fluxsol-Utils     SHARED $<TARGET_OBJECTS:Utils>)
add_library(fluxsol-EqnSys    SHARED $<TARGET_OBJECTS:SistEcuac>)

#Para cuando agregue las librerias
#target_link_libraries (fluxsol ${FCFD_LIBS} ${EXTRA_LIBS})



if (USE_SHARED_LIB)
#Before creating library
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")
add_library(Type_ SHARED $<TARGET_OBJECTS:Type>)
add_library(Nastran_ SHARED $<TARGET_OBJECTS:Nastran>)

#add_library(${NAME}lib SHARED FluxSol_lib.cpp 	$<TARGET_OBJECTS:Analysis>
#												$<TARGET_OBJECTS:Materials>
#												$<TARGET_OBJECTS:FvTime>
#												$<TARGET_OBJECTS:Tests>
#												$<TARGET_OBJECTS:Time>
#											)

else ()
    set (USE_STATIC_LIB ON)
endif ()

if (USE_STATIC_LIB)
# add_library(${NAME}lib_static STATIC FluxSol_lib.cpp 	$<TARGET_OBJECTS:Analysis>
														# $<TARGET_OBJECTS:Boundary>
														# $<TARGET_OBJECTS:Field>
														# $<TARGET_OBJECTS:IO>
														# $<TARGET_OBJECTS:Laplacian>
														# $<TARGET_OBJECTS:Materials>														
														# $<TARGET_OBJECTS:Mesh>														
														# $<TARGET_OBJECTS:FvGrad>
														# $<TARGET_OBJECTS:FvTime>
														# $<TARGET_OBJECTS:SistEcuac>
														# $<TARGET_OBJECTS:Solver>	
														# $<TARGET_OBJECTS:Tests>
														# $<TARGET_OBJECTS:Time>
														# $<TARGET_OBJECTS:Type>
														# $<TARGET_OBJECTS:Utils>
											# )

# ## PETSC

#target_link_libraries (${NAME} ${EXTRA_LIBS} ${EXT_LIBS})
#target_link_libraries (${NAME}lib_static ${EXTRA_LIBS} ${EXT_LIBS})
endif()



### INSTALL ####

#install (TARGETS ${NAME}lib DESTINATION bin)
set (LIBEXTEN dll)

# install (FILES libfluxsol-Boundary.${LIBEXTEN}  DESTINATION lib)
# install (FILES libfluxsol-Field.${LIBEXTEN}     DESTINATION lib)
# install (FILES libfluxsol-FvMesh.${LIBEXTEN}    DESTINATION lib)
# install (FILES libfluxsol-IO.${LIBEXTEN}        DESTINATION lib)
# install (FILES libfluxsol-Laplacian.${LIBEXTEN} DESTINATION lib)
# install (FILES libfluxsol-FvGrad.${LIBEXTEN}    DESTINATION lib)
# install (FILES libfluxsol-Model.${LIBEXTEN}     DESTINATION lib)
# install (FILES libfluxsol-Solver.${LIBEXTEN}    DESTINATION lib)
# install (FILES libfluxsol-Type.${LIBEXTEN}      DESTINATION lib)
# install (FILES libfluxsol-Utils.${LIBEXTEN}     DESTINATION lib)
# install (FILES libfluxsol-EqnSys.${LIBEXTEN}    DESTINATION lib)

install (FILES ${NAME}.h DESTINATION include)

#FLUXSOL EXECUTABLE

add_executable(${NAME} ${SOURCES} UDO)

target_link_libraries (fluxsol-EqnSys fluxsol-Type)
target_link_libraries (fluxsol-IO libUDO fluxsol-EqnSys fluxsol-Laplacian fluxsol-Field fluxsol-Type fluxsol-Utils)
target_link_libraries (fluxsol-Laplacian fluxsol-EqnSys fluxsol-Field)
target_link_libraries (fluxsol-FvGrad fluxsol-EqnSys fluxsol-Field)
target_link_libraries (fluxsol-Model fluxsol-FvGrad fluxsol-Solver fluxsol-Field fluxsol-FvMesh fluxsol-IO fluxsol-Field fluxsol-Type)
target_link_libraries (fluxsol-FvMesh fluxsol-Type fluxsol-Boundary Nastran_ ${EXTRA_LIBS})
target_link_libraries (fluxsol-Field fluxsol-Type)
target_link_libraries (fluxsol-Solver ${EXT_LIBS} fluxsol-Type)

#target_link_libraries (${NAME}lib libUDO fluxsol-IO fluxsol-FvMesh fluxsol-Model fluxsol-Solver fluxsol-Laplacian fluxsol-FvGrad fluxsol-Field fluxsol-EqnSys fluxsol-Type)
target_link_libraries (${NAME}    libUDO fluxsol-IO fluxsol-FvMesh fluxsol-Model fluxsol-Solver fluxsol-Laplacian fluxsol-FvGrad fluxsol-Field fluxsol-EqnSys fluxsol-Type)
#target_link_libraries (${NAME}lib libUDO ${EXTRA_LIBS} ${EXT_LIBS} Nastran_ Type_)