cmake_minimum_required(VERSION 2.8.9)

include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

option (DEBUG_MODE "Enable FLUXSOL DEBUG" OFF)

#set(CMAKE_PREFIX_PATH $ENV{QTDIR}) # Unnecessary on Windows apparently
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -static-libgcc -static-libstdc++")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpermissive -static-libgcc -static-libstdc++")

set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s -g")
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -static-libgcc")

set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")

#link_directories("/cygdrive/c/cygwin64/lib/gcc/x86_64-w64-mingw32/4.8.2")
set (FLUXSOL_SOLVER_DIR "${FLUXSOL_DIR}/Src/FluxSol-Solver")

set (NAME FluxSolCAE)

if(DEBUG_MODE)
# Default is debug build
#if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Debug)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
	set (CMAKE_BUILD_TYPE Release)
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpermissive")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
endif ()


project(${NAME})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(OpenGL)

include_directories(.)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/Job")
include_directories("${PROJECT_SOURCE_DIR}/Mesh")
include_directories("${PROJECT_SOURCE_DIR}/QVTKResWidget")
include_directories("${PROJECT_SOURCE_DIR}/Qt")

#including FluxSol Library
include_directories(${FLUXSOL_SOLVER_DIR})

include_directories("${FLUXSOL_SOLVER_DIR}/Analysis")
include_directories("${FLUXSOL_SOLVER_DIR}/Boundary")
include_directories("${FLUXSOL_SOLVER_DIR}/Field")
include_directories("${FLUXSOL_SOLVER_DIR}/FiniteVolume")
include_directories("${FLUXSOL_SOLVER_DIR}/FiniteVolume/Div")
include_directories("${FLUXSOL_SOLVER_DIR}/FiniteVolume/Grad")
include_directories("${FLUXSOL_SOLVER_DIR}/FiniteVolume/Laplacian")
include_directories("${FLUXSOL_SOLVER_DIR}/FiniteVolume/Mesh")
include_directories("${FLUXSOL_SOLVER_DIR}/Input")
include_directories("${FLUXSOL_SOLVER_DIR}/Interpolation")
include_directories("${FLUXSOL_SOLVER_DIR}/Job")
include_directories("${FLUXSOL_SOLVER_DIR}/Libs/Laspack")
include_directories("${FLUXSOL_SOLVER_DIR}/Materials")
include_directories("${FLUXSOL_SOLVER_DIR}/Mesh")
include_directories("${FLUXSOL_SOLVER_DIR}/Model")
include_directories("${FLUXSOL_SOLVER_DIR}/Output")
include_directories("${FLUXSOL_SOLVER_DIR}/SistEcuac")
include_directories("${FLUXSOL_SOLVER_DIR}/Solver")
include_directories("${FLUXSOL_SOLVER_DIR}/UDO")
include_directories("${FLUXSOL_SOLVER_DIR}/Tests")
include_directories("${FLUXSOL_SOLVER_DIR}/Type")
include_directories("${FLUXSOL_SOLVER_DIR}/Utils")


set (MY_PETSC_DIR "$ENV{PETSC_DIR}")
set (MY_PETSC_ARCH "$ENV{PETSC_ARCH}")
set (MPI_LIB_DIR "$ENV{MPI_LIB_DIR}")
set (MPI_INC_DIR "$ENV{MPI_INC_DIR}")

set (PARMETIS_DIR "$ENV{PARMETIS_DIR}")


set (LASPACK_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/laspack/src)
set (CGNS_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/cgnslib_3.2.1/src)
set (ALGLIB_INCLUDE_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/src)

set (LASPACK_LIBRARY_DIR ${FLUXSOL_DIR}/Libs/laspack/bin)
set (ALGLIB_LIBRARY_DIRS ${FLUXSOL_DIR}/Libs/alglib-3.8.2/bin)


set (FLUXSOL_LIB_DIR "$ENV{FLUXSOL_LIB_DIR}")


include_directories(${LASPACK_INCLUDE_DIRS})
include_directories(${CGNS_INCLUDE_DIRS})
include_directories(${ALGLIB_INCLUDE_DIRS})


include_directories("D:/Luciano/Numerico/Libs/QtSDK-5.2.1-x86_64-seh/include")


cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()


find_package(VTK REQUIRED)

#UNNECESARY
# find_package(VTK COMPONENTS
  # vtkCommonCore
  # vtkFiltersCore
  # vtkInfovisCore
  # vtkInteractionStyle
  # vtkRenderingFreeTypeOpenGL
  # vtkViewsQt
# )

include(${VTK_USE_FILE})

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

# Use the include path and library for Qt that is used by VTK.
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

#add_subdirectory(Mesh)

# Set your files and resources here
#set( Srcs main.cpp SimpleView.cpp )
set( Srcs main.cpp 
		  MainWindow.cpp 
		  ManageBCDialog.cpp
		  ModelTree.cpp
		  MsgWindow.cpp
		  ParamDialog.cpp 
		  sortdialog.cpp 
		  SimpleView.cpp 
		  Tetrahedron.cpp 
		  GraphicModel.cpp
		  GraphicResults.cpp
		  GridSelection.cpp)

set (Meshlib_Srcs "${PROJECT_SOURCE_DIR}/Mesh/ImportMesh.cpp") 

set (Joblib_Srcs 	"${PROJECT_SOURCE_DIR}/Job/JobSubmitDialog.cpp"
					"${PROJECT_SOURCE_DIR}/Job/JobWorker.cpp"
					"${PROJECT_SOURCE_DIR}/Job/JobThread.cpp") 
					
set (ResWidget_Srcs "${PROJECT_SOURCE_DIR}/QVTKResWidget/QVTKResWidget.cpp"
					"${PROJECT_SOURCE_DIR}/QVTKResWidget/ResWidgetWorker.cpp"
					) 
		  
set( Hdrs 	${PROJECT_SOURCE_DIR}/Job/JobSubmitDialog.h
			${PROJECT_SOURCE_DIR}/Job/JobWorker.h
			${PROJECT_SOURCE_DIR}/Job/JobThread.h
			ManageBCDialog.h
			ModelTree.h 
			MsgWindow.h 
			SimpleView.h 
			Param.h 
			ParamDialog.h 
			sortdialog.h 
			Tetrahedron.h 
			GraphicModel.h 
			GraphicResults.h 
			GridSelection.h 
			QDebugStream.h 
			QDebugStream_Alt.h)

set( MOC_Hdrs SimpleView.h )

set( UIs 	SimpleView.ui 
			Param.ui
			NewJobDialog.ui			
			JobSubmitDialog.ui
			ManageBCDialog.ui) #THIS IS THE SAME TO uic -o Param.h Param.ui

set( QRCs Icons/icons.qrc )


link_directories (${FLUXSOL_LIB_DIR})
link_directories (${LASPACK_LIBRARY_DIR})
link_directories (${ALGLIB_LIBRARY_DIRS})


set (FLUXSOL_LIB fluxsollib)
set (EXTRA_LIBS opengl32 glu32 gdi32)

#if (USE_PETSC)
	find_package (PETSc REQUIRED HINTS "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/conf")
	include_directories (..)
	include_directories ("${MY_PETSC_DIR}/include" "${MY_PETSC_DIR}/${MY_PETSC_ARCH}/include" ${PETSC_INCLUDE_PATH})
	
	set (MODULE_LIST ${MODULE_LIST} "PETSc")
	set (USE_MPI ON)
	add_definitions (-D__PETSC_MODULE)


	link_directories(${MPI_LIB_DIR})
	include_directories(${MPI_INC_DIR})
#endif()


# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# The rest should just work (sure...)
if(VTK_QT_VERSION VERSION_GREATER "4")
  # We have ui files, this will bring in the macro: qt5_wrap_ui
  find_package(Qt5Widgets REQUIRED QUIET)
  qt5_wrap_ui(UI_Srcs ${UIs})
  qt5_add_resources(QRC_Srcs ${QRCs} )

  source_group("Resources" FILES
    ${UIs}
    ${QRCs}
    ${EXE_ICON} # Not present
  )

  source_group("Generated" FILES
    ${UI_Srcs}
    ${MOC_Srcs}
    ${QRC_Srcs}
    ${QRC_Srcs}
  )

  #add_library(CAE_Mesh ${Meshlib_Srcs})
  
  #WIN32 to exclude DOS Console Window 
  #add_executable(${NAME} WIN32 #MACOSX_BUNDLE
add_executable(${NAME} #MACOSX_BUNDLE
    ${Srcs}
	#$<TARGET_OBJECTS:Mesh>
	${Meshlib_Srcs}
	${Joblib_Srcs}
	${ResWidget_Srcs}
	${Hdrs} 
	${UI_Srcs} 	#GENERA el ui_SimpleView.h
	${MOC_Hdrs} 
	${QRC_Srcs}
	)

  #qt5_use_modules($<TARGET_OBJECTS:Mesh> Core Gui Widgets)
  qt5_use_modules(${NAME} Core Gui Widgets)
  target_link_libraries(${NAME} ${VTK_LIBRARIES} ${FLUXSOL_LIB} laspack alglib ${EXTRA_LIBS} )
								
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  # Use what VTK built with
  set(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
  set(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
  set(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
  qt4_wrap_ui(UI_Srcs ${UIs})
  qt4_wrap_cpp(MOC_Srcs ${MOC_Hdrs} )
  qt4_add_resources(QRC_Srcs ${QRCs})

  source_group("Resources" FILES
    ${UIs}
    ${QRCs}
    ${EXE_ICON} # Not present
  )

  source_group("Generated" FILES
    ${UI_Srcs}
    ${QRC_Srcs}
    ${QRC_Srcs}
  )

  add_definitions(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)

  add_executable(${NAME} MACOSX_BUNDLE
    ${Srcs} 
	$<TARGET_OBJECTS:Mesh>
	${Hdrs} 
	${UI_Srcs} 
	${MOC_Hdrs} 
	${QRC_Srcs})
  target_link_libraries(${NAME}
    ${QT_LIBRARIES}
    ${VTK_LIBRARIES}
  )
endif()




qt5_use_modules(${NAME} Widgets OpenGL)

