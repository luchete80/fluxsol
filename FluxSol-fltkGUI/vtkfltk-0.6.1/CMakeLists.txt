# -*- Mode: CMAKE; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
#
# $Id: CMakeLists.txt,v 1.18 2004/06/16 18:11:20 xpxqx Exp $
#
# Copyright (c) 2002 - 2004 Sean McInerney
# All rights reserved.
#
# See Copyright.txt or http://vtkfltk.sourceforge.net/Copyright.html
# for details.
#
#    This software is distributed WITHOUT ANY WARRANTY; without even 
#    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
#    PURPOSE.  See the above copyright notice for more information.
#
cmake_minimum_required(VERSION 2.8.4)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)


SET (KIT FLTK)
SET (UKIT FLTK)

set (NAME vtkfltk-0.6.1)
project(${NAME})

#VTK
IF(NOT VTK_BINARY_DIR)
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF()
INCLUDE(${VTK_USE_FILE})
ENDIF()

set (FLTK_DIR "D:/Luciano/Libs/FLTK-1.3.4-1-install")

include_directories("${FLTK_DIR}/include")

link_directories ("D:/Luciano/Libs/VTK-6.3.0-Install/lib")

#
# vtkFLTK source files
#
SET (Kit_SRCS
  Fl_VTK_Window.cxx
  vtkFLTKObjectFactory.cxx
  vtkFLTKOpenGLRenderWindow.cxx
  vtkFLTKRenderWindowInteractor.cxx
)

# SET_SOURCE_FILES_PROPERTIES (
  # Fl_VTK_Window
  # PROPERTIES WRAP_EXCLUDE TRUE
# )

#
# List the kits from VTK that are needed by vtkFLTK.
#
SET (Kit_LIBS
  ${VTK_LIBRARIES}
  ${FLTK_LIBRARIES}
  ${OPENGL_LIBRARIES}
)


#
# Create the vtkFLTK C++ library.
#
ADD_LIBRARY (${NAME} ${Kit_SRCS} ${KitInstantiator_SRCS})
LINK_DIRECTORIES (${VTK_FLTK_LIBRARY_DIRS_CONFIG})
TARGET_LINK_LIBRARIES (${NAME} ${Kit_LIBS})
#INSTALL_TARGETS (/lib/vtk vtk${KIT})
#INSTALL_FILES (/include/vtk .+[.][Hh])

# #
# # if we are wrapping into Tcl then add the library and extra
# # source files for vtkFLTKTCL
# #
# IF (VTK_WRAP_TCL AND VTK_FLTK_WRAP_TCL)
  # IF (COMMAND VTK_WRAP_TCL2)
    # VTK_WRAP_TCL2 (vtk${KIT}TCL KitTCL_SRCS ${Kit_SRCS})
  # ELSE (COMMAND VTK_WRAP_TCL2)
    # VTK_WRAP_TCL (vtk${KIT}TCL KitTCL_SRCS ${Kit_SRCS})
  # ENDIF (COMMAND VTK_WRAP_TCL2)
  # ADD_LIBRARY (vtk${KIT}TCL ${KitTCL_SRCS})
  # TARGET_LINK_LIBRARIES (vtk${KIT}TCL vtk${KIT} vtkRenderingTCL)
  # INSTALL_TARGETS (/lib/vtk vtk${KIT}TCL)
# ENDIF (VTK_WRAP_TCL AND VTK_FLTK_WRAP_TCL)

# #
# # if we are wrapping into Python then add the library and extra
# # source files for vtkFLTKPython
# #
# IF (VTK_WRAP_PYTHON AND VTK_FLTK_WRAP_PYTHON)
  # IF (COMMAND VTK_WRAP_PYTHON2)
    # VTK_WRAP_PYTHON2 (vtk${KIT}Python KitPython_SRCS ${Kit_SRCS})
  # ELSE (COMMAND VTK_WRAP_PYTHON2)
    # VTK_WRAP_PYTHON (vtk${KIT}Python KitPython_SRCS ${Kit_SRCS})
  # ENDIF (COMMAND VTK_WRAP_PYTHON2)
  # ADD_LIBRARY (vtk${KIT}Python MODULE ${KitPython_SRCS})
  # TARGET_LINK_LIBRARIES (vtk${KIT}Python vtk${KIT} vtkRenderingPython)
  # INSTALL_TARGETS (/lib/vtk vtk${KIT}Python)
# ENDIF (VTK_WRAP_PYTHON AND VTK_FLTK_WRAP_PYTHON)

