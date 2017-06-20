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

IF(NOT VTK_BINARY_DIR)
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF()
INCLUDE(${VTK_USE_FILE})
ENDIF()

link_directories ("$ENV{VTK_DIR}/lib")

#if (USE_PETSC)
	find_package (PETSc REQUIRED HINTS "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/conf")
	include_directories (..)
	include_directories ("${MY_PETSC_DIR}/include" "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/include" ${PETSC_INCLUDE_PATH})
	
	#THIS IS NOT USED IN WINDOWS, USED MSMPI
	#include_directories ("${MY_PETSC_DIR}/include/mpiuni")
	
	include_directories ("E:/Programas/cygwin64/petsc-3.4.4/include")
	find_library (PETSC_LIB petsc PATH "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/lib/") 
	if (${PETSC_LIB} STREQUAL "PETSC_LIB-NOTFOUND")
		message (FATAL_ERROR "PETSc library not found")
	else ()
		message ("Found PETSC")
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

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set (NAME Graphics_)
set (sources GraphicModel.cpp SimpleView.cpp)
add_library (${NAME} OBJECT ${sources})
add_library (Graphics SHARED ${sources})

target_link_libraries (Graphics ${VTK_LIBRARIES})


target_link_libraries (Graphics
${VTK_LIBRARIES}
fluxsol-IO
fluxsol-Type
fluxsol-Solver
fluxsol-FvMesh
fluxsol-Model)