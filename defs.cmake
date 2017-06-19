

set (MY_PETSC_DIR "$ENV{PETSC_DIR}")
set (MPI_LIB_DIR "$ENV{MPI_LIB_DIR}")
set (MPI_INC_DIR "$ENV{MPI_INC_DIR}")
set (MY_PETSC_ARCH "$ENV{PETSC_ARCH}")
set (PARMETIS_DIR "$ENV{PARMETIS_DIR}")
set (GODOTGUI_SRC_DIR "D:/Luciano/Libs/GodotGUI")
set (GODOTGUI_BIN_DIR "$ENV{GODOTGUI_BIN_DIR}")
set (FLUXSOL_SRC_DIR "$ENV{FLUXSOL_DIR}/Src")

set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")

#set (LASPACK_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/laspack/src)
set (CGNS_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src)
set (ALGLIB_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/src)

set (CGNS_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src/lib)
set (ALGLIB_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/bin)
#set (LASPACK_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/laspack/bin)

set (FLUXSOL_LIB_DIR ${FLUXSOL_DIR}/Bin_Branches/master)


if (USE_PETSC)
	add_definitions(-DUSE_PETSC_DEFINED)
	#find_package (PETSc REQUIRED HINTS "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/conf")
	include_directories ("${MY_PETSC_DIR}/include" "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/include" ${PETSC_INCLUDE_PATH})
	
	#THIS IS NOT USED IN WINDOWS, USED MSMPI
	#include_directories ("${MY_PETSC_DIR}/include/mpiuni")
	
	include_directories ("${MY_PETSC_DIR}/include")
	#find_library (PETSC_LIB petsc PATH "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/lib/") 
	# if (${PETSC_LIB} STREQUAL "PETSC_LIB-NOTFOUND")
		# message (FATAL_ERROR "PETSc library not found")
	# endif ()
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

#FLUXSOL DEPENDENCIES

# add to the include search path
include_directories(.)
include_directories("${FLUXSOL_SRC_DIR}")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Analysis")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Boundary")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Field")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/FiniteVolume")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/FiniteVolume/Div")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/FiniteVolume/Grad")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/FiniteVolume/Laplacian")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/FiniteVolume/Mesh")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Input")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Interpolation")
#include_directories("${FLUXSOL_SRC_DIR}/Libs/Laspack")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Materials")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Mesh")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Model")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Nastran")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Output")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/SistEcuac")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Solver")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Tests")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Time")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Type")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/UDO")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Utils")
include_directories("${FLUXSOL_SRC_DIR}/FluxSol-Solver/Variables")

link_directories(${CGNS_LIBRARY_DIRS})
link_directories(${ALGLIB_LIBRARY_DIRS})
link_directories(${LASPACK_LIBRARY_DIRS})
link_directories (${FLUXSOL_LIB_DIR}/FluxSol)
link_directories (${FLUXSOL_LIB_DIR}/UDO/UDOLib)