cmake_minimum_required(VERSION 2.8)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

#CONVECTION - DIFFUSION EXAMPLE
set (NAME Test-petsc-MPI)
project(${NAME})


set (MY_PETSC_DIR "D:/Luciano/Programas/cygwin64/petsc-3.4.4")

set (MPI_LIB_DIR "D:/Luciano/Programas/Microsoft-HPCPack-2008R2/Lib/amd64")
set (MPI_INC_DIR "D:/Luciano/Programas/Microsoft-HPCPack-2008R2/Inc")

#if(DEBUG_MODE)
#set (MY_PETSC_ARCH arch-mswin-c-debug-shared-mpi0-fc0)
#else ()
#set (MY_PETSC_ARCH arch-mswin-c-release-mpi0-fc0)
#endif ()

set (MY_PETSC_ARCH arch-mswin-c-mpi-hpc-release-64)

#######################################################################
######################## External libaries ############################
#######################################################################

## PETSC
##if (USE_PETSC)
find_package (PETSc REQUIRED HINTS "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/conf")
include_directories (..)
include_directories ("${MY_PETSC_DIR}/include" "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/include" ${PETSC_INCLUDE_PATH})
include_directories ("${MY_PETSC_DIR}/include/mpiuni")
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
##endif ()


link_directories(${MPI_LIB_DIR})
include_directories(${MPI_INC_DIR})

set (EXT_LIBS ${EXT_LIBS} msmpi)

#add the executable
set (SOURCES
ex19.cpp
)
add_executable(${NAME} ${SOURCES})

target_link_libraries (${NAME} ${EXT_LIBS} gfortran)
