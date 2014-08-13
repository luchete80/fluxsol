#set (CMAKE_CXX_FLAGS "-O3 -fpermissive")
#set (CMAKE_CXX_FLAGS "-fpermissive")
set (CMAKE_CXX_COMPILER g++)

#Temporalmente no lo incluyo (freecfd)
#set (CGNS_INCLUDE_DIRS /Users/esozer/fcfd/deps/cgnslib_2.5/include)
set (CGNS_INCLUDE_DIRS .\Libs\cgns3.1.4)
set (ALGLIB_INCLUDE_DIRS .\Libs\alglib3.x)
#set (CGNS_LIBRARY_DIRS /Users/esozer/fcfd/deps/cgnslib_2.5/lib)
#set (PARMETIS_INCLUDE_DIRS /Users/esozer/fcfd/deps/ParMetis-3.1.1)
#set (PARMETIS_LIBRARY_DIRS /Users/esozer/fcfd/deps/ParMetis-3.1.1)
#set (PETSC_INCLUDE_DIRS /Users/esozer/fcfd/deps/petsc-3.1-p2/include /Users/esozer/fcfd/deps/petsc-3.1-p2/darwin9.8.0-c-debug/include)
#set (PETSC_LIBRARY_DIRS /Users/esozer/fcfd/deps/petsc-3.1-p2/darwin9.8.0-c-debug/lib)

cmake_minimum_required(VERSION 2.6)


project(fluxsol)
#set (fluxsol_VERSION_MAJOR 2)
#set (fluxsol_VERSION_MINOR 0)

list (APPEND CMAKE_MODULE_PATH "${fluxsol_SOURCE_DIR}/CMake")

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
include_directories("${PROJECT_SOURCE_DIR}/Finite Volume")
include_directories("${PROJECT_SOURCE_DIR}/Finite Volume/Div")
include_directories("${PROJECT_SOURCE_DIR}/Finite Volume/Grad")
include_directories("${PROJECT_SOURCE_DIR}/Finite Volume/Laplacian")
include_directories("${PROJECT_SOURCE_DIR}/Finite Volume/Mesh")
include_directories("${PROJECT_SOURCE_DIR}/Input")
include_directories("${PROJECT_SOURCE_DIR}/Libs/Laspack")
include_directories("${PROJECT_SOURCE_DIR}/Mesh")
include_directories("${PROJECT_SOURCE_DIR}/Model")
include_directories("${PROJECT_SOURCE_DIR}/Output")
include_directories("${PROJECT_SOURCE_DIR}/SistEcuac")
include_directories("${PROJECT_SOURCE_DIR}/Solver")
include_directories("${PROJECT_SOURCE_DIR}/Tests")
include_directories("${PROJECT_SOURCE_DIR}/Type")
include_directories("${PROJECT_SOURCE_DIR}/Utils")

#Temporalmente no lo incluyo (librerias)
include_directories(${CGNS_INCLUDE_DIRS})
#include_directories(${PARMETIS_INCLUDE_DIRS})
#include_directories(${PETSC_INCLUDE_DIRS})
link_directories(${CGNS_LIBRARY_DIRS})
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
add_subdirectory(Libs/Laspack)
add_subdirectory(Mesh)
add_subdirectory(Model)
add_subdirectory(Output)
add_subdirectory(SistEcuac)
add_subdirectory(Solver)
add_subdirectory(Tests)
add_subdirectory(Type)
add_subdirectory(Utils)

set (FSOL_LIBS Analysis Boundary Mesh FvMesh Input Tests Type Utils Laspack Output)
#set (EXTRA_LIBS cgns metis parmetis petsc f2cblas f2clapack)
set (EXTRA_LIBS cgns alglib)

#add the executable
set (SOURCES
#Analysis.cpp Boundary.cpp FvExpGrad.cpp CC_Cell.cpp FvFace.cpp FvGrid.cpp MeshInfo.cpp Mesh.cpp Model.cpp
#Cadenas.cpp Caract.cpp Elementos.cpp Material.cpp Modelo.cpp Nastran.cpp Nodo.cpp Orientaciones.cpp Prop.cpp Rod.cpp SistCoord.cpp Varios.cpp
#SistEcuac.cpp Simple.cpp 
#Scalar.cpp Vec3d.cpp Operations.cpp
FluxSol.cpp
)
add_executable(fluxsol ${SOURCES})

#Para cuando agregue las librerias
#target_link_libraries (fluxsol ${FCFD_LIBS} ${EXTRA_LIBS})

target_link_libraries (fluxsol ${FSOL_LIBS} ${EXTRA_LIBS})

install (TARGETS fluxsol RUNTIME DESTINATION bin)
 
