cmake_minimum_required(VERSION 2.6)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

PROJECT (Win32Cone)

IF(NOT VTK_BINARY_DIR)
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF()
INCLUDE(${VTK_USE_FILE})
ENDIF()

# El original es así
# LINK_LIBRARIES(
  # vtkRendering
  # vtkGraphics
  # vtkImaging
  # vtkIO
  # vtkFiltering
  # vtkCommon
# )

#ESTO ES NECESARIO
link_directories ("D:/Luciano/Libs/VTK-6.3.0-Install/lib")

LINK_LIBRARIES(
  # vtkRenderingCore-6.3
  # vtkRenderingOpenGL-6.3
  # vtkRenderingContextOpenGL-6.3
  # vtkInteractionStyle-6.3
  # vtkRenderingFreeType-6.3
  # vtkFiltersCore-6.3
  # vtkFiltersSources-6.3 #Cone Source
  # vtkCommonCore-6.3
  stdc++
  ${VTK_LIBRARIES}
)

ADD_EXECUTABLE(Win32Cone WIN32 Win32Cone.cxx)