set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")
include ("${FLUXSOL_DIR}/Src/defs.cmake")
#Depending on vtk
include_directories ("${MY_PETSC_DIR}/include" "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/include")
	link_directories(${MPI_LIB_DIR})
	include_directories(${MPI_INC_DIR})
	set (CGNS_LIBRARY_DIRS "D:/Luciano/Numerico/MecSol/FluxSol/Libs/cgnslib_3.2.1/src/lib")
set (ALGLIB_LIBRARY_DIRS "D:/Luciano/Numerico/MecSol/FluxSol/Libs/alglib-3.8.2/bin")
	set (CGNS_INCLUDE_DIRS "D:/Luciano/Numerico/MecSol/FluxSol/Libs/cgnslib_3.2.1/src")
set (ALGLIB_INCLUDE_DIRS "D:/Luciano/Numerico/MecSol/FluxSol/Libs/alglib-3.8.2/src")

	include_directories(${CGNS_INCLUDE_DIRS})
#include_directories(${LASPACK_INCLUDE_DIRS})
include_directories(${ALGLIB_INCLUDE_DIRS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

#add_subdirectory(vtk)

set (NAME Graphics_)
set (sources GraphicModel.cpp)
add_library (${NAME} OBJECT ${sources})
add_library (Graphics SHARED ${sources})

target_link_libraries (Graphics
fluxsol-IO
fluxsol-Type
fluxsol-Solver
fluxsol-FvMesh
fluxsol-Model)