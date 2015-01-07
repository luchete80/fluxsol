cmake_minimum_required(VERSION 2.8.9)

set (CMAKE_BUILD_TYPE Debug)

#set(CMAKE_PREFIX_PATH $ENV{QTDIR}) # Unnecessary on Windows apparently
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -static-libgcc -static-libstdc++ -g")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpermissive -static-libgcc -static-libstdc++ -g")

set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s -g")
SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -static-libgcc")

#link_directories("/cygdrive/c/cygwin64/lib/gcc/x86_64-w64-mingw32/4.8.2")
set (NAME FluxSolCAE)

project(${NAME})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(OpenGL)

include_directories(.)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/Qt")

include_directories("D:/Luciano/Numerico/Libs/QtSDK-5.2.1-x86_64-seh/include")



set (CMAKE_BUILD_TYPE Debug) #LUCIANO

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

# Set your files and resources here
#set( Srcs main.cpp SimpleView.cpp )
set( Srcs main.cpp MainWindow.cpp ParamDialog.cpp sortdialog.cpp SimpleView.cpp Tetrahedron.cpp)

set( Hdrs SimpleView.h Param.h ParamDialog.h sortdialog.h Tetrahedron.h)

set( MOC_Hdrs SimpleView.h )

set( UIs SimpleView.ui Param.ui) #THIS IS THE SAME TO uic -o Param.h Param.ui

set( QRCs Icons/icons.qrc )


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

  add_executable(${NAME} MACOSX_BUNDLE
    ${Srcs} 
	${Hdrs} 
	${UI_Srcs} 	#GENERA el ui_SimpleView.h
	${MOC_Hdrs} 
	${QRC_Srcs}
	)
  qt5_use_modules(${NAME} Core Gui Widgets)
  target_link_libraries(${NAME} ${VTK_LIBRARIES})
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
	${Hdrs} 
	${UI_Srcs} 
	${MOC_Hdrs} 
	${QRC_Srcs})
  target_link_libraries(${NAME}
    ${QT_LIBRARIES}
    ${VTK_LIBRARIES}
  )
endif()



set (EXTRA_LIBS opengl32 glu32 gdi32)
#target_link_libraries (${NAME} ${EXTRA_LIBS} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})


qt5_use_modules(${NAME} Widgets OpenGL)