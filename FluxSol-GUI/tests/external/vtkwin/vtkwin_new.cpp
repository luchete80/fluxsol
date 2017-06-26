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

#include "windows.h"

// first include the required header files for the vtk classes we are using
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include <vtkProperty.h>

#include <vtkExternalOpenGLRenderWindow.h>
#include <ExternalVTKWidget.h>

#include <vtkTransform.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <vtkNew.h>
#include <vtkLight.h>
#include <vtkLightActor.h>


LONG WINAPI
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    static PAINTSTRUCT ps;

    switch(uMsg) {
    case WM_PAINT:
	//display();
	//BeginPaint(hWnd, &ps);
	//EndPaint(hWnd, &ps);
	return 0;

    case WM_SIZE:
	//glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
	//PostMessage(hWnd, WM_PAINT, 0, 0);
	return 0;

    case WM_CHAR:
	switch (wParam) {
	case 27:			/* ESC key */
	    PostQuitMessage(0);
	    break;
	}
	return 0;

    case WM_CLOSE:
	PostQuitMessage(0);
	return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 


HWND
CreateOpenGLWindow(char* title, int x, int y, int width, int height, 
		   BYTE type, DWORD flags)
{
    int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance) {
	hInstance = GetModuleHandle(NULL);
	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass(&wc)) {
	    MessageBox(NULL, "RegisterClass() failed:  "
		       "Cannot register window class.", "Error", MB_OK);
	    return NULL;
	}
    }

    hWnd = CreateWindow("OpenGL", title, WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
		   "Error", MB_OK);
	return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 24;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
	MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
	return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	MessageBox(NULL, "SetPixelFormat() failed:  "
		   "Cannot set format specified.", "Error", MB_OK);
	return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hWnd,hDC);

    return hWnd;
}    


static HANDLE hinst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// define the vtk part as a simple c++ class
class myVTKApp
{
public:
  myVTKApp(HWND parent);
  ~myVTKApp();
  
  void Draw();
private:
  //vtkRenderWindow *renWin; //OLD
  vtkExternalOpenGLRenderWindow *renWin; //NEW
  ExternalVTKWidget *externalVTKWidget;
  
  vtkRenderer *renderer;
  vtkRenderWindowInteractor *iren;
  vtkConeSource *cone;
  vtkPolyDataMapper *coneMapper;
  vtkActor *coneActor;
};


int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR /* lpszCmdParam */, int nCmdShow)
{
  static char szAppName[] = "Win32Cone";
  HWND        hwnd ;
  MSG         msg ;
  WNDCLASS    wndclass ;

HDC hDC;				/* device context */
HGLRC hRC;				/* opengl context */
  
 
  
  
    hwnd = CreateOpenGLWindow("minimal", 0, 0, 500, 500, PFD_TYPE_RGBA, 0);
    if (hwnd == NULL)
	exit(1);

    hDC = GetDC(hwnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hwnd, nCmdShow);

      // HWND vtkwin = CreateWindow("button",NULL,
                 // WS_CHILD | WS_VISIBLE | SS_CENTER,
                 // 100,100,400,400,
                 // hwnd,NULL,
                 // //(HINSTANCE)vtkGetWindowLong(hWnd,vtkGWL_HINSTANCE),
                 // (HINSTANCE)vtkGetWindowLong(hwnd,vtkGWL_HINSTANCE),
                 // NULL);
				 
	  //LUCIANO
	  //AFTER CREATE CONTEXT!
  myVTKApp *theVTKApp=new myVTKApp(hwnd);
  

  
  
  //main loop

    while(GetMessage(&msg, hwnd, 0, 0)) {

	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_DEPTH_TEST);

  // // // Buffers being managed by external application i.e. GLUT in this case.
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(100.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

  glBegin(GL_TRIANGLES);
    glVertex3f(-0.5,-0.5,0.0);
    glVertex3f(1.5,0.0,0.0);
    glVertex3f(0.0,1.5,1.0);
  glEnd();
  
	theVTKApp->Draw();
	
	glFlush();
	
	
		TranslateMessage(&msg);
	DispatchMessage(&msg);
	
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hwnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hwnd);

    return msg.wParam;
  
}


//LUCIANO
void myVTKApp::Draw()
{
	//this->iren->Render();
	//this->renWin->Render();
	externalVTKWidget->GetRenderWindow()->Render();
}

myVTKApp::myVTKApp(HWND hwnd)
{
  // Similar to Examples/Tutorial/Step1/Cxx/Cone.cxx
  // We create the basic parts of a pipeline and connect them
  
	
  renderer = vtkRenderer::New(); 
  this->renWin=vtkExternalOpenGLRenderWindow::New(); //NEW
  
  //this->renWin=vtkRenderWindow::New(); //OLD
  externalVTKWidget=ExternalVTKWidget::New();
  
  //New One
  this->renderer=this->externalVTKWidget->AddRenderer();
  this->externalVTKWidget->SetRenderWindow(this->renWin);
  
  ////OLD ONE
  // this->renWin = vtkRenderWindow::New();
  // this->renWin->AddRenderer(this->renderer);
  // renWin->InitializeFromCurrentContext();

  renWin->SetSize(200,200);
  // setup the parent window
  renWin->SetParentId(hwnd);
  this->iren = vtkRenderWindowInteractor::New();
  this->iren->SetRenderWindow(renWin);
  this->iren->Initialize();

  this->cone = vtkConeSource::New();
  this->cone->SetHeight( 3.0 );
  this->cone->SetRadius( 1.0 );
  this->cone->SetResolution( 8 );
  
  renWin->SetOffScreenRendering(1);
  
   vtkSmartPointer<vtkTransform> transform = 
    vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply(); //this is the key line
  transform->RotateZ(90.0);
  transform->RotateX(45.0);
  transform->Translate(0,1.0,0.0);
  transform->Scale(0.1,0.1,0.1);
  
  
  

  this->coneMapper = vtkPolyDataMapper::New();
  this->coneMapper->SetInputConnection(this->cone->GetOutputPort());
  this->coneActor = vtkActor::New();
  this->coneActor->SetMapper(this->coneMapper);
  this->coneActor->GetProperty()->SetColor(0.0, 1.0, 1.0);
  
   this->coneActor->SetUserTransform(transform);
   
   coneActor->GetProperty()->BackfaceCullingOn();
   coneActor->GetProperty()->LightingOn();
   coneActor->GetProperty()->ShadingOn();
   coneActor->GetProperty()->SetInterpolationToFlat();
   
   
     double lightPosition[3] = {0, 0, 1};
  // Create a light
  double lightFocalPoint[3] = {0,0,0};
  
  vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
  light->SetLightTypeToSceneLight();
  light->SetPosition(lightPosition[0], lightPosition[1], lightPosition[2]);
  light->SetPositional(true); // required for vtkLightActor below
  light->SetConeAngle(10);
  light->SetFocalPoint(lightFocalPoint[0], lightFocalPoint[1], lightFocalPoint[2]);
  light->SetDiffuseColor(1,0,0);
  light->SetAmbientColor(0,1,0);
  light->SetSpecularColor(0,0,1);
   //vtkLightCollection* originalLights = renderer->GetLights();
  //PRINT_LINE("Originally there are " << originalLights->GetNumberOfItems() << " lights." << std::endl;
  vtkSmartPointer<vtkLightActor> lightActor = vtkSmartPointer<vtkLightActor>::New();
  lightActor->SetLight(light);
  renderer->AddViewProp(lightActor);
  

  this->renderer->AddActor(this->coneActor);
  this->renderer->SetBackground(0.9,0.9,0.3);

  // Finally we start the interactor so that event will be handled
  renWin->Render();
}

myVTKApp::~myVTKApp()
{
    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    cone->Delete();
    coneMapper->Delete();
    coneActor->Delete();
}