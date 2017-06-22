#ifndef _VTKRENDERING_H_
#define _VTKRENDERING_H_ 

//LUCIANO, ONLY FOR TESTING

#include <vtkProperty.h>
//#include <vtkCallbackCommand.h>
//
//// VTK includes
#include <ExternalVTKWidget.h>
//#include <vtkActor.h>
//#include <vtkCallbackCommand.h>
//#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkExternalOpenGLRenderWindow.h>
#include <vtkLight.h>
// //#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkOpenGLRenderer.h>
#include <vtkConeSource.h>
#include <vtkRenderWindowInteractor.h>


///////////// IS THIS BELONGS TO OS_WinDOWS THE PROGRAM CRASHES
///////////// WHEN RETURNING SINGLETON
// Original was
extern  vtkRenderWindowInteractor *vtkriw;
extern vtkRenderWindow *renWindow;
extern vtkOpenGLRenderer *ren;

extern ExternalVTKWidget *externalVTKWidget;


//
//static void CameraModifiedCallback(vtkObject* caller,
//                                   long unsigned int vtkNotUsed(eventId),
//                                   void* vtkNotUsed(clientData),
//                                   void* vtkNotUsed(callData) )
//{
////  std::cout << caller->GetClassName() << " modified" << std::endl;
//
//  vtkCamera* camera = static_cast<vtkCamera*>(caller);
//  // print the interesting stuff
////  std::cout << "\tPosition: "
////            << camera->GetPosition()[0] << ", "
////            << camera->GetPosition()[1] << ", "
////            << camera->GetPosition()[2] << std::endl;
////  std::cout << "\tFocal point: "
////            << camera->GetFocalPoint()[0] << ", "
////            << camera->GetFocalPoint()[1] << ", "
////            << camera->GetFocalPoint()[2] << std::endl;
//}
//
//
//void WindowModifiedCallback( vtkObject*
//                      caller, long unsigned int vtkNotUsed(eventId), void* vtkNotUsed(clientData), void* vtkNotUsed(callData))
//{
//  std::cout << "Window modified" << std::endl;
//  std::cout << caller->GetClassName() << std::endl;
//
//  vtkRenderWindow* window = static_cast<vtkRenderWindow*>(caller); //Observer is put in renderwindow
//
//
//  vtkSmartPointer<vtkOpenGLRenderer>  renderer =
//    vtkSmartPointer<vtkOpenGLRenderer>::New();
//
//  int* windowSize = window->GetSize();
//  std::cout << "Size: " << windowSize[0] << " " << windowSize[1] << std::endl;
//
//  //if(windowSize[0] > 400)
//    {
//    //window->SetSize(400, windowSize[1]);
//    //window->Render();
//    //window->Modified();
//    //window->Render();
//
//    }
//
//
//   double xpos,ypos;
////
////  //GetRenderWindow()->GetSize()
//////  cout << "xsize: "<<this->ui->qvtkWidget->GetRenderWindow()->GetSize()[0]<<endl;
////  //cout << "ysize: "<<this->ui->qvtkWidget->GetRenderWindow()->GetSize()[1]<<endl;
////
////    //double xsize=(double)(this->ui->qvtkWidget->GetRenderWindow()->GetSize()[0]);
////    double xsize=windowSize[0];
////    xpos=xsize*0.95;
////  cout << "xsize" << xsize << "xpos" <<xpos<<endl;
//// textActor ->SetPosition ( xpos, 200);
////
//    //window->Modified();
//    //window->AddRenderer(renderer);
//    //renderer->AddActor(textActor);
////
//    //window->Render();
//
//}
///////////////////////////////////

#endif