// From: VTK/Examples/GUI/Win32/SimpleCxx/Win32Cone.cxx

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    Win32Cone.cxx
  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
//
// This example is a windows application (instead of a console application)
// version of Examples/Tutorial/Step1/Cxx/Cone.cxx. It is organized in a more
// object oriented manner and shows a fairly minimal windows VTK application.
//

#include "vtkApp.h"
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyle.h>
#include <vtkVersion.h>
#include <vtkCommand.h>

// define the vtk part as a simple c++ class


// int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    // LPSTR /* lpszCmdParam */, int nCmdShow)
// {
  // static char szAppName[] = "Win32Cone";
  // HWND        hwnd ;
  // MSG         msg ;
  // WNDCLASS    wndclass ;

  // if (!hPrevInstance)
  // {
    // wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    // wndclass.lpfnWndProc   = WndProc ;
    // wndclass.cbClsExtra    = 0 ;
    // wndclass.cbWndExtra    = 0 ;
    // wndclass.hInstance     = hInstance;
    // wndclass.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    // wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
    // wndclass.lpszMenuName  = NULL;
    // wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    // wndclass.lpszClassName = szAppName;
    // RegisterClass (&wndclass);
  // }

  // hinst = hInstance;
  // hwnd = CreateWindow ( szAppName,
                        // "Draw Window",
                        // WS_OVERLAPPEDWINDOW,
                        // CW_USEDEFAULT,
                        // CW_USEDEFAULT,
                        // 400,
                        // 480,
                        // NULL,
                        // NULL,
                        // hInstance,
                        // NULL);
  // ShowWindow (hwnd, nCmdShow);
  // UpdateWindow (hwnd);
  // while (GetMessage (&msg, NULL, 0, 0))
  // {
    // TranslateMessage (&msg);
    // DispatchMessage (&msg);
  // }
  // return msg.wParam;
// }

 LRESULT CALLBACK vtk_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
 {
   static HWND ewin;
   static myVTKApp *theVTKApp;

   switch (message)
   {
     case WM_CREATE:
     {
//       ewin = CreateWindow("button","Exit",
//                           WS_CHILD | WS_VISIBLE | SS_CENTER,
//                           0,400,400,60,
//                           hwnd,(HMENU)2,
//                           (HINSTANCE)vtkGetWindowLong(hwnd,vtkGWL_HINSTANCE),
//                           NULL);
//       theVTKApp = new myVTKApp(hwnd);
       return 0;
     }

     case WM_COMMAND:
       switch (wParam)
       {
         case 2:
           PostQuitMessage (0);
           delete theVTKApp;
           theVTKApp = NULL;
           break;
       }
       return 0;

     case WM_DESTROY:
       PostQuitMessage (0);
       delete theVTKApp;
       theVTKApp = NULL;
       return 0;
   }
   return DefWindowProc (hwnd, message, wParam, lParam);
 }

myVTKApp * myVTKApp::New()
{
    // we don't make use of the objectfactory, because we're not registered
    return new myVTKApp;
}

myVTKApp::myVTKApp()
:vtkRenderWindowInteractor()
{

}
myVTKApp::myVTKApp(HWND hwnd)
:vtkRenderWindowInteractor()
{
  // Similar to Examples/Tutorial/Step1/Cxx/Cone.cxx
  // We create the basic parts of a pipeline and connect them
  this->renderer = vtkRenderer::New();
  this->renWin = vtkRenderWindow::New();
  this->renWin->AddRenderer(this->renderer);

  // setup the parent window
  this->renWin->SetParentId(hwnd);
//  this->iren = vtkRenderWindowInteractor::New();
//  this->iren->SetRenderWindow(this->renWin);
    //this->SetRenderWindow(this->renWin);
    vtkRenderWindowInteractor::SetRenderWindow(this->renWin);
  this->cone = vtkConeSource::New();
  this->cone->SetHeight( 3.0 );
  this->cone->SetRadius( 1.0 );
  this->cone->SetResolution( 10 );
  this->coneMapper = vtkPolyDataMapper::New();
  this->coneMapper->SetInputConnection(this->cone->GetOutputPort());
  this->coneActor = vtkActor::New();
  this->coneActor->SetMapper(this->coneMapper);

  this->renderer->AddActor(this->coneActor);
  this->renderer->SetBackground(0.2,0.4,0.3);
  this->renWin->SetSize(200,200);

  // Finally we start the interactor so that event will be handled
  //this->renWin->Render();
}

void myVTKApp::OnTimer(void)
{
    if (!Enabled)
      return;
    // this is all we need to do, InteractorStyle is stateful and will
    // continue with whatever it's busy

#if (VTK_MAJOR_VERSION >= 4)
    // new style
    this->InvokeEvent(vtkCommand::TimerEvent, NULL);
#else
    // old style
    InteractorStyle->OnTimer();
#endif
//this->draw();

this->Render();

}


myVTKApp::~myVTKApp()
{
    renWin->Delete();
    renderer->Delete();
    //iren->Delete();
    cone->Delete();
    coneMapper->Delete();
    coneActor->Delete();
}
