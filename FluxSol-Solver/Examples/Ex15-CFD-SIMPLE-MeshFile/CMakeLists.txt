cmake_minimum_required(VERSION 2.8)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

#FLUENT MESH READING EXAMPLE
set (NAME Ex15-Nastran)
project(${NAME})



# add to the include search path
include_directories(.)
#include_directories("${FLUXSOL_INCLUDE_DIR}/../..")
#set(FLUXSOL_DIR c:/Users/samsung/Dropbox/Numerico/MecSol/FluxSol)
set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")
include_directories(${FLUXSOL_DIR}/Src/FluxSol-Solver)

set (FLUXSOL_INCLUDE_DIR ${FLUXSOL_DIR}/Src/FluxSol-Solver)
set (FLUXSOL_LIB_DIR "$ENV{FLUXSOL_LIB_DIR}")

set (LASPACK_INCLUDE ${FLUXSOL_DIR}/Libs/laspack/src/)
set (LASPACK_LIBRARY_DIR ${FLUXSOL_DIR}/Libs/laspack/bin)

set (ALGLIB_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/src)
set (ALGLIB_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/bin)


set (MY_PETSC_DIR "$ENV{PETSC_DIR}")
set (MPI_LIB_DIR "$ENV{MPI_LIB_DIR}")
set (MPI_INC_DIR "$ENV{MPI_INC_DIR}")
set (MY_PETSC_ARCH "$ENV{PETSC_ARCH}")

#if (USE_PETSC)
	find_package (PETSc REQUIRED HINTS "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/conf")
	include_directories (..)
	include_directories ("${MY_PETSC_DIR}/include" "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/include" ${PETSC_INCLUDE_PATH})
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
	

#endif ()

include (${FLUXSOL_DIR}/Src/defs.cmake)


include_directories(${FLUXSOL_INCLUDE_DIR})
include_directories(${LASPACK_INCLUDE})

link_directories(${LASPACK_LIBRARY_DIR})
link_directories (${FLUXSOL_LIB_DIR})

include_directories(${ALGLIB_INCLUDE_DIRS})
link_directories(${ALGLIB_LIBRARY_DIRS})

set (FLUXSOL_LIB fluxsollib)

#add the executable
set (SOURCES
${NAME}.cpp
)
add_executable(${NAME} ${SOURCES})

target_link_libraries (${NAME} ${FLUXSOL_LIB} ${EXT_LIBS} laspack alglib Nastran_)
