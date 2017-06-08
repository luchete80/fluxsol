#ifndef _VTKFRAME_H_
#define _VTKFRAME_H_

#include "gVTKRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"

#include "windows.h"


class MyApp;
class MyFrame;

// Define a new application type, each program should derive a class from wxApp
class MyApp //: public wxApp
{
public:
   // this one is called on application startup and is a good place for the app
   // initialization (doing it here and not in the ctor allows to have an error
   // return: if OnInit() returns false, the application terminates)
   virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame //: public wxFrame
{
public:
   // ctor(s)
   //MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	MyFrame(const HWND &hWnd=NULL);
   ~MyFrame();

   // event handlers (these functions should _not_ be virtual)
   // void OnQuit(wxCommandEvent& event);
   // void OnAbout(wxCommandEvent& event);

protected:

   void ConstructVTK();
   void ConfigureVTK();
   void DestroyVTK();

private:
 HINSTANCE vtk_hinstance;
 gVTKRenderWindowInteractor *m_pVTKWindow;
 HWND vtkwin;
 HWND vtkwinsep; //ONLY FOR TESTING
 HWND *parent;

 // vtk classes
 vtkRenderer       *pRenderer;
 vtkRenderWindow   *pRenderWindow;
 vtkPolyDataMapper *pConeMapper;
 vtkActor          *pConeActor;
 vtkConeSource     *pConeSource;

private:
   // any class wishing to process wxWindows events must use this macro
   //DECLARE_EVENT_TABLE()
};



#endif
