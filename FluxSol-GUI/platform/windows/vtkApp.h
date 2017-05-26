#ifndef _VTKAPP_H_
#define _VTKAPP_H_

#include "windows.h"
// first include the required header files for the vtk classes we are using
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

LRESULT CALLBACK vtk_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class myVTKApp
{
public:
  myVTKApp(HWND parent);
  ~myVTKApp();
private:
  vtkRenderWindow *renWin;
  vtkRenderer *renderer;
  vtkRenderWindowInteractor *iren;
  vtkConeSource *cone;
  vtkPolyDataMapper *coneMapper;
  vtkActor *coneActor;
};

#endif
