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
#include <vtkPlanes.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkObjectFactory.h>

#include <vtkAreaPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkProp3DCollection.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackball.h>
#include <vtkInteractorStyleRubberBandPick.h>

#include <vtkExtractPolyDataGeometry.h>
#include <vtkExtractGeometry.h>

#include <vtkVertexGlyphFilter.h>
#include <vtkIdFilter.h>

#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1



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

void PickCallbackFunction(vtkObject* caller,
                          long unsigned int vtkNotUsed(eventId),
                          void* vtkNotUsed(clientData),
                          void* vtkNotUsed(callData));




// Define interaction style
class HighlightInteractorStyleCells : public vtkInteractorStyleRubberBandPick
{
  public:
    static HighlightInteractorStyleCells* New(){return new HighlightInteractorStyleCells;};
    vtkTypeMacro(HighlightInteractorStyleCells,vtkInteractorStyleRubberBandPick);

    HighlightInteractorStyleCells() : vtkInteractorStyleRubberBandPick()
    {
      this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
      this->SelectedActor = vtkSmartPointer<vtkActor>::New();
      this->SelectedActor->SetMapper(SelectedMapper);
    }

    virtual void OnLeftButtonUp();

    void SetPolyData(vtkSmartPointer<vtkPolyData> polyData) {this->PolyData = polyData;}
  private:
    vtkSmartPointer<vtkPolyData> PolyData;
    vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkDataSetMapper> SelectedMapper;

};

// Define interaction style
class HighlightInteractorStylePoints : public vtkInteractorStyleRubberBandPick
{
  public:
    static HighlightInteractorStylePoints* New(){return new HighlightInteractorStylePoints;};
    vtkTypeMacro(HighlightInteractorStylePoints,vtkInteractorStyleRubberBandPick);

    HighlightInteractorStylePoints()
    {
      this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
      this->SelectedActor = vtkSmartPointer<vtkActor>::New();
      this->SelectedActor->SetMapper(SelectedMapper);
    }

    virtual void OnLeftButtonUp();

    void SetPoints(vtkSmartPointer<vtkPolyData> points) {this->Points = points;}
  private:
    vtkSmartPointer<vtkPolyData> Points;
    vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkDataSetMapper> SelectedMapper;

};
//vtkStandardNewMacro(HighlightInteractorStylePoints);
//vtkStandardNewMacro(HighlightInteractorStyleCells);

#endif
