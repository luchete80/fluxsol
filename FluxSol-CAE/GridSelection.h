#ifndef _GRIDSELECTION_H_
#define _GRIDSELECTION_H_

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdTypeArray.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPlaneSource.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkObjectFactory.h>



// Catch mouse events
class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  public:
  static MouseInteractorStyle* New()
  {
      return new MouseInteractorStyle;
  }

  MouseInteractorStyle()
  {
    selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    selectedActor = vtkSmartPointer<vtkActor>::New();
  }

    virtual void OnLeftButtonDown();


    vtkSmartPointer<vtkPolyData> Data;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
    vtkSmartPointer<vtkActor> selectedActor;

};

#endif
