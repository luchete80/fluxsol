#include "GridSelection.h"


void MouseInteractorStyle::OnLeftButtonDown()
{
  // Get the location of the click (in window coordinates)
  int* pos = this->GetInteractor()->GetEventPosition();

  vtkSmartPointer<vtkCellPicker> picker =
    vtkSmartPointer<vtkCellPicker>::New();
  picker->SetTolerance(0.0005);

  // Pick from this location.
  picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

  double* worldPosition = picker->GetPickPosition();
  std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

  if(picker->GetCellId() != -1)
    {

    std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1]
              << " " << worldPosition[2] << endl;

    vtkSmartPointer<vtkIdTypeArray> ids =
      vtkSmartPointer<vtkIdTypeArray>::New();
    ids->SetNumberOfComponents(1);
    ids->InsertNextValue(picker->GetCellId());

    vtkSmartPointer<vtkSelectionNode> selectionNode =
      vtkSmartPointer<vtkSelectionNode>::New();
    selectionNode->SetFieldType(vtkSelectionNode::CELL);
    selectionNode->SetContentType(vtkSelectionNode::INDICES);
    selectionNode->SetSelectionList(ids);

    vtkSmartPointer<vtkSelection> selection =
      vtkSmartPointer<vtkSelection>::New();
    selection->AddNode(selectionNode);

    vtkSmartPointer<vtkExtractSelection> extractSelection =
      vtkSmartPointer<vtkExtractSelection>::New();
#if VTK_MAJOR_VERSION <= 5
    extractSelection->SetInput(0, this->Data);
    extractSelection->SetInput(1, selection);
#else
    extractSelection->SetInputData(0, this->Data);
    extractSelection->SetInputData(1, selection);
#endif
    extractSelection->Update();

    // In selection
    vtkSmartPointer<vtkUnstructuredGrid> selected =
      vtkSmartPointer<vtkUnstructuredGrid>::New();
    selected->ShallowCopy(extractSelection->GetOutput());

    std::cout << "There are " << selected->GetNumberOfPoints()
              << " points in the selection." << std::endl;
    std::cout << "There are " << selected->GetNumberOfCells()
              << " cells in the selection." << std::endl;

    for (int c=0;c<selected->GetNumberOfCells();c++)
    {
        std:: cout << "Cell " << c << "points: " <<selected->GetCell (c)-> GetNumberOfPoints()<<std::endl;
        // virtual vtkIdType* vtkCell::GetFaces	(		)

    }
//        std::cout << "There are " << selected->GetNumberOfFaces()
//                  << " faces in the selection." << std::endl;

#if VTK_MAJOR_VERSION <= 5
    selectedMapper->SetInputConnection(
      selected->GetProducerPort());
#else
    selectedMapper->SetInputData(selected);
#endif

    selectedActor->SetMapper(selectedMapper);
    selectedActor->GetProperty()->EdgeVisibilityOn();
    selectedActor->GetProperty()->SetEdgeColor(1,0,0);
    selectedActor->GetProperty()->SetLineWidth(3);

    this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);

    }
  // Forward events
  vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void PickCallbackFunction(vtkObject* caller,
                          long unsigned int vtkNotUsed(eventId),
                          void* vtkNotUsed(clientData),
                          void* vtkNotUsed(callData))
{
  std::cout << "Pick." << std::endl;
  vtkAreaPicker* areaPicker = static_cast<vtkAreaPicker*>(caller);
  vtkProp3DCollection* props = areaPicker->GetProp3Ds();

  for(vtkIdType i = 0; i < props->GetNumberOfItems(); i++)
    {
    vtkProp3D* prop = props->GetNextProp3D();
    std::cout << "Picked prop: " << prop << std::endl;
    }
}


//This is from http://www.vtk.org/Wiki/VTK/Examples/Cxx/Picking/HighlightSelectedPoints
    void HighlightInteractorStylePoints::OnLeftButtonUp()
    {
      // Forward events
      vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

      vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

      vtkSmartPointer<vtkExtractGeometry> extractGeometry =
        vtkSmartPointer<vtkExtractGeometry>::New();
      extractGeometry->SetImplicitFunction(frustum);
#if VTK_MAJOR_VERSION <= 5
      extractGeometry->SetInput(this->Points);
#else
      extractGeometry->SetInputData(this->Points);
#endif
      extractGeometry->Update();

      vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =
        vtkSmartPointer<vtkVertexGlyphFilter>::New();
      glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
      glyphFilter->Update();

      vtkPolyData* selected = glyphFilter->GetOutput();
      std::cout << "Selected " << selected->GetNumberOfPoints() << " points." << std::endl;
      std::cout << "Selected " << selected->GetNumberOfCells() << " cells." << std::endl;
#if VTK_MAJOR_VERSION <= 5
      this->SelectedMapper->SetInput(selected);
#else
      this->SelectedMapper->SetInputData(selected);
#endif
      this->SelectedMapper->ScalarVisibilityOff();


//        vtkIdTypeArray* ids;

     // vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetPointData()->GetArray("OriginalIds"));
    //  for(vtkIdType i = 0; i < ids->GetNumberOfTuples(); i++)
    //  {
    //   std::cout << "Id " << i << " : " << ids->GetValue(i) << std::endl;
    //}

    //cout << "Number of Point arrays: "<< this->idFilter->GetOutput()->GetPointData()->GetNumberOfArrays() <<endl;
    cout << "Number of Point arrays: "<< selected->GetPointData()->GetNumberOfArrays() <<endl;


    vtkIdTypeArray* pointIds = vtkIdTypeArray::SafeDownCast(this->idFilter->GetOutput()->GetPointData()->GetArray("ids"));
    std::cout << "There are " << pointIds->GetNumberOfTuples() << " point ids" << std::endl;
    for(vtkIdType i = 0; i < pointIds->GetNumberOfTuples(); i++)
    {
        std::cout << "Id " << i << " : " << pointIds->GetValue(i) << std::endl;
    }


      this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
      this->SelectedActor->GetProperty()->SetPointSize(5);

      this->CurrentRenderer->AddActor(SelectedActor);
      this->GetInteractor()->GetRenderWindow()->Render();
      this->HighlightProp(NULL);
    }


void HighlightInteractorStyleCells::OnLeftButtonUp()
{
  // Forward events
  vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

  if(this->CurrentMode == VTKISRBP_SELECT)
    {
    vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

    vtkSmartPointer<vtkExtractPolyDataGeometry> extractPolyDataGeometry =
      vtkSmartPointer<vtkExtractPolyDataGeometry>::New();
#if VTK_MAJOR_VERSION <= 5
    extractPolyDataGeometry->SetInput(this->PolyData);
#else
    extractPolyDataGeometry->SetInputData(this->PolyData);
#endif
    extractPolyDataGeometry->SetImplicitFunction(frustum);
    extractPolyDataGeometry->Update();

    std::cout << "Extracted " << extractPolyDataGeometry->GetOutput()->GetNumberOfCells() << " cells." << std::endl;

#if VTK_MAJOR_VERSION <= 5
    this->SelectedMapper->SetInputConnection(
      extractPolyDataGeometry->GetOutputPort());
#else
    this->SelectedMapper->SetInputData(extractPolyDataGeometry->GetOutput());
#endif
    this->SelectedMapper->ScalarVisibilityOff();


    for (int c=0;c<extractPolyDataGeometry->GetOutput()->GetNumberOfCells();c++)
    {
        std:: cout << "Cell " << c << "points: " <<extractPolyDataGeometry->GetOutput()->GetCell (c)-> GetNumberOfPoints()<<std::endl;
        // virtual vtkIdType* vtkCell::GetFaces	(		)

    }


    //vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetPointData()->GetArray("OriginalIds"));

    this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
    this->SelectedActor->GetProperty()->SetPointSize(5);

    this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(SelectedActor);
    this->GetInteractor()->GetRenderWindow()->Render();
    this->HighlightProp(NULL);
    }
}
