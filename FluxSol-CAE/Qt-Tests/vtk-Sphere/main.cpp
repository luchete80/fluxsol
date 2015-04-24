
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL); 
VTK_MODULE_INIT(vtkInteractionStyle); 

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <QVTKWidget.h>
#include <vtkProperty.h>

int main(int argc, char *argv[]) 
{ 
    QApplication a(argc, argv); 

    QVTKWidget widget; 
      widget.resize(256,256); 

      // Setup sphere 
      vtkSmartPointer<vtkSphereSource> sphereSource = 
          vtkSmartPointer<vtkSphereSource>::New(); 
      sphereSource->Update(); 
      vtkSmartPointer<vtkPolyDataMapper> sphereMapper = 
          vtkSmartPointer<vtkPolyDataMapper>::New(); 
      sphereMapper->SetInputConnection(sphereSource->GetOutputPort()); 
      vtkSmartPointer<vtkActor> sphereActor = 
          vtkSmartPointer<vtkActor>::New(); 
      sphereActor->GetProperty()->SetFrontfaceCulling(true); 
      sphereActor->SetMapper(sphereMapper); 

      // Setup window 
      vtkSmartPointer<vtkRenderWindow> renderWindow = 
          vtkSmartPointer<vtkRenderWindow>::New(); 

      // Setup renderer 
      vtkSmartPointer<vtkRenderer> renderer = 
          vtkSmartPointer<vtkRenderer>::New(); 
      renderWindow->AddRenderer(renderer); 

      renderer->AddActor(sphereActor); 
      renderer->ResetCamera(); 

      widget.SetRenderWindow(renderWindow); 
      widget.show(); 

    return a.exec(); 
} 