set(Module_SRCS
  myExternalVTKWidget.cxx
  myvtkExternalLight.cxx
  myvtkExternalOpenGLCamera.cxx
  myvtkExternalOpenGLRenderer.cxx
  myvtkExternalOpenGLRenderWindow.cxx
  )

set(Module_HDRS
  myExternalVTKWidget.h
  myvtkExternalLight.h
  myvtkExternalOpenGLCamera.h
  myvtkExternalOpenGLRenderer.h
  myvtkExternalOpenGLRenderWindow.h
  )

add_definitions(-DVTK_OPENGL2)
#vtk_module_library(vtkRenderingExternal ${Module_SRCS})
#include(vtkOpenGL)
#vtk_opengl_link(${vtk-module})

#if ("${VTK_RENDERING_BACKEND}" STREQUAL "OpenGL2")
#  set_target_properties(
#    vtkRenderingExternal
#    PROPERTIES COMPILE_FLAGS -DVTK_OPENGL2)
#endif ()
add_library(MyvtkRenderingExternal ${Module_SRCS})
target_link_libraries(MyvtkRenderingExternal ${VTK_LIBRARIES})