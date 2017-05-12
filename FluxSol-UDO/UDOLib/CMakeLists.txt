cmake_minimum_required(VERSION 2.6)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

set (NAME UDOLib)
project(${NAME})

set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")

set (FLUXSOL_LIB_DIR ${FLUXSOL_LIB_DIR})

set (LASPACK_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/laspack/src)
set (CGNS_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src)
set (ALGLIB_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/src)

set (CGNS_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src/lib)
set (ALGLIB_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/bin)
set (LASPACK_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/laspack/bin)


set (MY_PETSC_DIR "$ENV{PETSC_DIR}")
set (MPI_LIB_DIR "$ENV{MPI_LIB_DIR}")
set (MPI_INC_DIR "$ENV{MPI_INC_DIR}")
set (MY_PETSC_ARCH "$ENV{PETSC_ARCH}")

set (PARMETIS_DIR "$ENV{PARMETIS_DIR}")

#Temporalmente no lo incluyo (librerias)
include_directories(${CGNS_INCLUDE_DIRS})
include_directories(${LASPACK_INCLUDE_DIRS})
include_directories(${ALGLIB_INCLUDE_DIRS})

include_directories("${PARMETIS_DIR}/include")
include_directories("${PARMETIS_DIR}/metis/include")

link_directories(${CGNS_LIBRARY_DIRS})
link_directories(${ALGLIB_LIBRARY_DIRS})
link_directories(${LASPACK_LIBRARY_DIRS})
link_directories (${FLUXSOL_LIB_DIR})

set (FLUXSOL_SRC_DIR "$ENV{FLUXSOL_DIR}/Src/FluxSol-Solver")

set (FLUXSOL_LIB fluxsollib)

include_directories(.)
include_directories("${FLUXSOL_SRC_DIR}")
include_directories("${FLUXSOL_SRC_DIR}/Analysis")
include_directories("${FLUXSOL_SRC_DIR}/Boundary")
include_directories("${FLUXSOL_SRC_DIR}/Field")
include_directories("${FLUXSOL_SRC_DIR}/FiniteVolume")
include_directories("${FLUXSOL_SRC_DIR}/FiniteVolume/Div")
include_directories("${FLUXSOL_SRC_DIR}/FiniteVolume/Grad")
include_directories("${FLUXSOL_SRC_DIR}/FiniteVolume/Laplacian")
include_directories("${FLUXSOL_SRC_DIR}/FiniteVolume/Mesh")
include_directories("${FLUXSOL_SRC_DIR}/Input")
include_directories("${FLUXSOL_SRC_DIR}/Interpolation")
include_directories("${FLUXSOL_SRC_DIR}/Libs/Laspack")
include_directories("${FLUXSOL_SRC_DIR}/Materials")
include_directories("${FLUXSOL_SRC_DIR}/Mesh")
include_directories("${FLUXSOL_SRC_DIR}/Model")
include_directories("${FLUXSOL_SRC_DIR}/Output")
include_directories("${FLUXSOL_SRC_DIR}/SistEcuac")
include_directories("${FLUXSOL_SRC_DIR}/Solver")
include_directories("${FLUXSOL_SRC_DIR}/Tests")
include_directories("${FLUXSOL_SRC_DIR}/Time")
include_directories("${FLUXSOL_SRC_DIR}/Type")
include_directories("${FLUXSOL_SRC_DIR}/UDO")
include_directories("${FLUXSOL_SRC_DIR}/Utils")
include_directories("${FLUXSOL_DIR}/Src/FluxSol-UDO/CodeWriter")

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



set (NAME UDO)
set (SOURCES 
UDOLib.cpp
UDOCreator.cpp
#udf_boundary.cpp
${FLUXSOL_SRC_DIR}/Input/Input.cpp
${FLUXSOL_SRC_DIR}/Field/Field.cpp
${FLUXSOL_SRC_DIR}/Field/PatchField.cpp
${FLUXSOL_SRC_DIR}/Utils/Utils.cpp
${FLUXSOL_SRC_DIR}/SistEcuac/SistEcuac.cpp
${FLUXSOL_SRC_DIR}/Field/BoundaryField.cpp
${FLUXSOL_SRC_DIR}/Field/_CC_Fv_field.cpp
${FLUXSOL_SRC_DIR}/FiniteVolume/Laplacian/Laplacian.cpp
${FLUXSOL_SRC_DIR}/Type/Scalar.cpp
${FLUXSOL_SRC_DIR}/Type/Vec3D.cpp
${FLUXSOL_DIR}/Src/FluxSol-UDO/CodeWriter/FileSearch.cpp
)



FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )
install (FILES ${NAME}.h DESTINATION include)
#install (install (TARGETS ${NAME}lib DESTINATION UDO-Lib))

#set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")

add_library(${NAME} SHARED ${SOURCES})
#target_link_libraries (${NAME} fluxsollib)