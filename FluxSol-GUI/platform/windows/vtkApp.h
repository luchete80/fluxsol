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

class myVTKApp:
    public vtkRenderWindowInteractor
{
    protected:
              myVTKApp * New();
              myVTKApp();
public:
  myVTKApp(HWND parent);
  ~myVTKApp();
private:
  vtkRenderWindow *renWin;
  vtkRenderer *renderer;
  vtkConeSource *cone;
  vtkPolyDataMapper *coneMapper;
  vtkActor *coneActor;


  public:
     // vtkRenderWindowInteractor overrides
//   void Initialize();
//   void Enable();
//   void Disable();
//   void Start();
//   void SetRenderWindow(vtkRenderWindow *aren);
//   void UpdateSize(int x, int y);
//   int CreateTimer(int timertype);
//   int DestroyTimer();
   void OnTimer(void);
//   void TerminateApp();
};

#endif
