cmake_minimum_required(VERSION 2.8)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

#CONVECTION - DIFFUSION EXAMPLE
set (NAME Ex3-Conv-Diff-Single)
project(${NAME})

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fpermissive")
set (CMAKE_CXX_FLAGS "-fpermissive -g")


# add to the include search path
include_directories(.)
#include_directories("${FLUXSOL_INCLUDE_DIR}/../..")
#set(FLUXSOL_DIR c:/Users/samsung/Dropbox/Numerico/MecSol/FluxSol)
set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")
include_directories(${FLUXSOL_DIR}/Src)

set (FLUXSOL_INCLUDE_DIR ${FLUXSOL_DIR}/Src)
set (FLUXSOL_LIB_DIR ${FLUXSOL_DIR}/Bin)

set (LASPACK_INCLUDE ${FLUXSOL_DIR}/Libs/laspack/src/)
set (LASPACK_LIBRARY_DIR ${FLUXSOL_DIR}/Libs/laspack/bin)

set (ALGLIB_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/src)
set (ALGLIB_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/bin)


include_directories("${FLUXSOL_INCLUDE_DIR}")
include_directories("${FLUXSOL_INCLUDE_DIR}/Analysis")
include_directories("${FLUXSOL_INCLUDE_DIR}/Boundary")
include_directories("${FLUXSOL_INCLUDE_DIR}/Field")
include_directories("${FLUXSOL_INCLUDE_DIR}/FiniteVolume")
include_directories("${FLUXSOL_INCLUDE_DIR}/FiniteVolume/Div")
include_directories("${FLUXSOL_INCLUDE_DIR}/FiniteVolume/Grad")
include_directories("${FLUXSOL_INCLUDE_DIR}/FiniteVolume/Laplacian")
include_directories("${FLUXSOL_INCLUDE_DIR}/FiniteVolume/Mesh")
include_directories("${FLUXSOL_INCLUDE_DIR}/Input")
include_directories("${FLUXSOL_INCLUDE_DIR}/Libs/Laspack")
include_directories("${FLUXSOL_INCLUDE_DIR}/Materials")
include_directories("${FLUXSOL_INCLUDE_DIR}/Mesh")
include_directories("${FLUXSOL_INCLUDE_DIR}/Model")
include_directories("${FLUXSOL_INCLUDE_DIR}/Output")
include_directories("${FLUXSOL_INCLUDE_DIR}/SistEcuac")
include_directories("${FLUXSOL_INCLUDE_DIR}/Solver")
include_directories("${FLUXSOL_INCLUDE_DIR}/Tests")
include_directories("${FLUXSOL_INCLUDE_DIR}/Type")
include_directories("${FLUXSOL_INCLUDE_DIR}/Utils")


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

target_link_libraries (${NAME} ${FLUXSOL_LIB} laspack)
