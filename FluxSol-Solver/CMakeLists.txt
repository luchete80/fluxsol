cmake_minimum_required(VERSION 2.6)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

#######################################################################
######################## Options ######################################
#######################################################################
option (USE_PETSC "Enable PETSc support (required for parallel)" OFF)
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
	set (CMAKE_CXX_FLAGS "-fpermissive -g")
else()
	set (CMAKE_BUILD_TYPE Release)
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpermissive")
endif ()

set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpermissive")

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set (CMAKE_CXX_FLAGS_PROFILING "${CMAKE_CXX_FLAGS_PROFILING} -O2 -pg")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set (NAME fluxsol)

project(${NAME})

set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")
#set (FLUXSOL_DIR ${PROJECT_SOURCE_DIR}/../)

set (LASPACK_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/laspack/src)
set (CGNS_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src)
set (ALGLIB_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/src)

set (CGNS_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src/lib)
set (ALGLIB_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/bin)
set (LASPACK_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/laspack/bin_release)

#set (fluxsol_VERSION_MAJOR 2)
#set (fluxsol_VERSION_MINOR 0)

#list (APPEND CMAKE_MODULE_PATH "${fluxsol_SOURCE_DIR}/CMake")

# Temporalmente no lo incluyo
# Pass some CMake settings to source code through a header file
#configure_file (
#	"${PROJECT_SOURCE_DIR}/cmake_vars.h.in"
#	"${PROJECT_BINARY_DIR}/cmake_vars.h"
#)

set (CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../)

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
include_directories("${PROJECT_SOURCE_DIR}/Libs/Laspack")
include_directories("${PROJECT_SOURCE_DIR}/Materials")
include_directories("${PROJECT_SOURCE_DIR}/Mesh")
include_directories("${PROJECT_SOURCE_DIR}/Model")
include_directories("${PROJECT_SOURCE_DIR}/Output")
include_directories("${PROJECT_SOURCE_DIR}/SistEcuac")
include_directories("${PROJECT_SOURCE_DIR}/Solver")
include_directories("${PROJECT_SOURCE_DIR}/Tests")
include_directories("${PROJECT_SOURCE_DIR}/Type")
include_directories("${PROJECT_SOURCE_DIR}/UDO")
include_directories("${PROJECT_SOURCE_DIR}/Utils")

#Temporalmente no lo incluyo (librerias)
include_directories(${CGNS_INCLUDE_DIRS})
include_directories(${LASPACK_INCLUDE_DIRS})
include_directories(${ALGLIB_INCLUDE_DIRS})
#include_directories(${PARMETIS_INCLUDE_DIRS})
#include_directories(${PETSC_INCLUDE_DIRS})
link_directories(${CGNS_LIBRARY_DIRS})
link_directories(${ALGLIB_LIBRARY_DIRS})
link_directories(${LASPACK_LIBRARY_DIRS})
#link_directories(${PARMETIS_LIBRARY_DIRS})
#link_directories(${PETSC_LIBRARY_DIRS})

add_subdirectory(Analysis)
add_subdirectory(Boundary)
add_subdirectory(Field)
add_subdirectory(FiniteVolume)
add_subdirectory(FiniteVolume/Div)
add_subdirectory(FiniteVolume/Grad)
add_subdirectory(FiniteVolume/Laplacian)
add_subdirectory(FiniteVolume/Mesh)
add_subdirectory(Input)
add_subdirectory(Interpolation)
add_subdirectory(Materials)
add_subdirectory(Mesh)
add_subdirectory(Model)
add_subdirectory(Output)
add_subdirectory(SistEcuac)
add_subdirectory(Solver)
add_subdirectory(Tests)
add_subdirectory(Type)
add_subdirectory(UDO)
add_subdirectory(Utils)

set (FSOL_LIBS Analysis Boundary Mesh Field FvMesh Input Tests Type Utils Output)

set (EXTRA_LIBS cgns alglib laspack)

#add the executable
set (SOURCES
FluxSol.cpp
)

#Para cuando agregue las librerias
#target_link_libraries (fluxsol ${FCFD_LIBS} ${EXTRA_LIBS})

if (USE_SHARED_LIB)
#Before creating library
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")

add_library(${NAME}lib SHARED FluxSol_lib.cpp 	$<TARGET_OBJECTS:Analysis>
												$<TARGET_OBJECTS:Boundary>
												$<TARGET_OBJECTS:Field>
												$<TARGET_OBJECTS:Input>
												$<TARGET_OBJECTS:Laplacian>
												$<TARGET_OBJECTS:Materials>
												$<TARGET_OBJECTS:Mesh>
												$<TARGET_OBJECTS:Model>
												$<TARGET_OBJECTS:FvMesh>												
												$<TARGET_OBJECTS:Output>
												$<TARGET_OBJECTS:SistEcuac>
												$<TARGET_OBJECTS:Solver>	
												$<TARGET_OBJECTS:Tests>
												$<TARGET_OBJECTS:Type>
												$<TARGET_OBJECTS:UDO>
												$<TARGET_OBJECTS:Utils>
											)
#target_link_libraries (${NAME} ${EXTRA_LIBS} ${EXT_LIBS})
target_link_libraries (${NAME}lib ${EXTRA_LIBS} ${EXT_LIBS})
else ()
    set (USE_STATIC_LIB ON)
endif ()

if (USE_STATIC_LIB)
add_library(${NAME}lib_static STATIC FluxSol_lib.cpp 	$<TARGET_OBJECTS:Analysis>
														$<TARGET_OBJECTS:Boundary>
														$<TARGET_OBJECTS:Field>
														$<TARGET_OBJECTS:Input>
														$<TARGET_OBJECTS:Laplacian>
														$<TARGET_OBJECTS:Materials>														
														$<TARGET_OBJECTS:Mesh>
														$<TARGET_OBJECTS:Model>														
														$<TARGET_OBJECTS:FvMesh>
														$<TARGET_OBJECTS:Output>
														$<TARGET_OBJECTS:SistEcuac>
														$<TARGET_OBJECTS:Solver>	
														$<TARGET_OBJECTS:Tests>
														$<TARGET_OBJECTS:Type>
														$<TARGET_OBJECTS:UDO>
														$<TARGET_OBJECTS:Utils>
											)

#target_link_libraries (${NAME} ${EXTRA_LIBS} ${EXT_LIBS})
target_link_libraries (${NAME}lib_static ${EXTRA_LIBS} ${EXT_LIBS})
endif()


### INSTALL ####

install (TARGETS ${NAME}lib DESTINATION lib)
install (FILES ${NAME}.h DESTINATION include)

#FLUXSOL EXECUTABLE
add_executable(${NAME} ${SOURCES})
target_link_libraries (${NAME} ${NAME}lib)
 
