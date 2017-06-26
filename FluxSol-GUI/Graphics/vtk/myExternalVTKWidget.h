/*=========================================================================

  Program:   Visualization Toolkit
  Module:    ExternalVTKWidget.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME ExternalVTKWidget - Use VTK rendering in an external window/application
// .SECTION Description
// ExternalVTKWidget provides an easy way to render VTK objects in an external
// environment using the VTK rendering framework without drawing a new window.

#ifndef __ExternalVTKWidget_h
#define __ExternalVTKWidget_h

#include "myvtkExternalOpenGLRenderer.h"
#include "myvtkExternalOpenGLRenderWindow.h"
#include "vtkObject.h"
#include "vtkRenderingExternalModule.h" // For export macro

// Class that maintains an external render window.
class VTKRENDERINGEXTERNAL_EXPORT ExternalVTKWidget : public vtkObject
{
public:
  static ExternalVTKWidget *New();
  vtkTypeMacro(ExternalVTKWidget, vtkObject);
  void PrintSelf(ostream &os, vtkIndent indent);

  // Set/Get an external render window for the ExternalVTKWidget.
  // Since this is a special environment, the methods are limited to use
  // myvtkExternalOpenGLRenderWindow only.
  // \sa myvtkExternalOpenGLRenderWindow
  myvtkExternalOpenGLRenderWindow* GetRenderWindow(void);
  void SetRenderWindow(myvtkExternalOpenGLRenderWindow* renWin);

  // Creates a new renderer and adds it to the render window.
  // Returns a handle to the created renderer.
  // NOTE: To get a list of renderers, one must go through the RenderWindow API
  // i.e. ExternalVTKWidget->GetRenderWindow()->GetRenderers()
  // \sa vtkRenderWindow::GetRenderers()
  vtkExternalOpenGLRenderer* AddRenderer();

protected:
  ExternalVTKWidget();
  ~ExternalVTKWidget();

  myvtkExternalOpenGLRenderWindow* RenderWindow;

private:
  ExternalVTKWidget(const ExternalVTKWidget&) VTK_DELETE_FUNCTION;
  void operator=(const ExternalVTKWidget&) VTK_DELETE_FUNCTION;
};

#endif //__ExternalVTKWidget_h
