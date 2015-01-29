/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */


#include "ui_SimpleView.h"
#include "SimpleView.h"

#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkQtTreeView.h>
#include <vtkAxesActor.h>

#include <vtkTransform.h>

#include <vtkTextActor.h>
#include <vtkTextProperty.h>

//GRID
#include <vtkGeometryFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGridGeometryFilter.h>

#include <vtkShrinkFilter.h>
#include <vtkProperty.h>
#include <vtkOrientationMarkerWidget.h>

#include <QFileDialog>

//Visualization
#include <vtkCamera.h>

#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>


#include "vtkSmartPointer.h"
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//// TEST SLIDER; TO DELETE
  
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation2D.h>

#include <QvtkInteractor.h>


// COMMAND AND INTERACTOR
#include <vtkCommand.h>


class vtkImageInteractionCallback1  : public vtkCommand
{
public:
  static vtkImageInteractionCallback1 *New() 
    {
    return new vtkImageInteractionCallback1; 
    }
	void SetRenderer(vtkOpenGLRenderer* ren)
	{
		this->renderer=ren;
	}
  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {

		vtkSmartPointer<vtkTextActor> textActor2 = 
		vtkSmartPointer<vtkTextActor>::New();
		textActor2->GetTextProperty()->SetFontSize ( 24 );
		textActor2->SetPosition2 ( 50, 40 );
		this->renderer->AddActor2D ( textActor2 );
		textActor2->SetInput ( "Test" );
		textActor2->GetTextProperty()->SetColor ( 0.08,0.0,0.4 );

	}
	
	private:
	
	vtkOpenGLRenderer* renderer;
	
  };
 
/////// 
 
  
// Constructor
SimpleView::SimpleView()
{
  this->ui = new Ui_SimpleView;
  this->ui->setupUi(this);

  // Qt Table View
  this->TableView = vtkSmartPointer<vtkQtTableView>::New();
  
  this->ui->treeWidget=new QTreeWidget;
  this->ui->treeWidget->expandAll();

  // Place the table view in the designer form
  //this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());

  // Geometry
  VTK_CREATE(vtkVectorText, text);
  text->SetText("VTK and Qt!");
  VTK_CREATE(vtkElevationFilter, elevation);
  elevation->SetInputConnection(text->GetOutputPort());
  elevation->SetLowPoint(0,0,0);
  elevation->SetHighPoint(10,0,0);

  // Mapper
  VTK_CREATE(vtkPolyDataMapper, textmapper);

  vtkSmartPointer<vtkDataSetMapper> mapper =
    vtkSmartPointer<vtkDataSetMapper>::New();
    //vtkSmartPointer<vtkUnstructuredGridMapper> mapper =
    //vtkSmartPointer<vtkUnstructuredGridMapper>::New();

  textmapper->ImmediateModeRenderingOn();
  textmapper->SetInputConnection(elevation->GetOutputPort());


  /// MESH READING
    // Read the file
    //vtkSmartPointer<vtkUnstructuredGrid> ugrid;

    vtkUnstructuredGrid *ugrid;

  vtkSmartPointer<vtkUnstructuredGridReader> reader =
    vtkSmartPointer<vtkUnstructuredGridReader>::New();
  //reader->SetFileName(inputFilename.c_str());
  reader->SetFileName("mesh.vtk");
  reader->Update();

  ugrid = reader->GetOutput();

  vtkSmartPointer<vtkUnstructuredGridGeometryFilter> geometryFilter =
    vtkSmartPointer<vtkUnstructuredGridGeometryFilter>::New();
  geometryFilter->SetInputConnection(reader->GetOutputPort());
  geometryFilter->Update();



  //mapper->SetInputConnection(geometryFilter->GetOutputPort());
    mapper->SetInputConnection(reader->GetOutputPort());
    //mapper->SetInputConnection(shrinkFilter->GetOutputPort());

    //**********************************************************************************
    //Read values At First
//    vector<double> results;
//    fstream in_stream("results.txt");
//    while (!in_stream.eof())
//   {
//       string line;
//        getline(in_stream, line);
//        stringstream ss;
//        ss << line;
//        double d;
//        ssd >> d;
//        results.push_back(d);
//   }



    // Data taken from vtk examples ColoredElevationMap and Filled Contours
    //This is interesting too
    //http://www.vtk.org/Wiki/VTK/Examples/Cxx/Visualization/ElevationBandsWithGlyphs
    //CONTOUR INFORMATION
    //
//
//   //converter para polydata
//   vtkUnstructuredGrid * unstructured= vtkUnstructuredGrid::New();
//   unstructured = reader ->GetOutput();
//   //unstructured ->Update();
//
//   vtkGeometryFilter *geometryFilter2  = vtkGeometryFilter::New();
//    #if VTK_MAJOR_VERSION <= 5
//      geometryFilter2->SetInput(unstructured);
//    #else
//      geometryFilter->SetInputData(unstructured);
//    #endif
//      geometryFilter2->Update();
//
//    vtkPolyData * polydata= vtkPolyData::New();
//    polydata = geometryFilter2 ->GetOutput ();
//
//    //polydata -> SetPoints(teste->GetOutput()->GetPoints());
//
//       //TO OBTAIN RANGE
//     double scalarRange[2];
////     polydata->GetPointData()->GetScalars()->GetRange(scalarRange);
//
//    //************************************* MAPPER
//
//     vtkSmartPointer<vtkPolyDataMapper> pdmapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
//
//    //pdmapper->ScalarVisibilityOff();
//
//    #if VTK_MAJOR_VERSION <= 5
//      pdmapper->SetInputConnection(polydata->GetProducerPort());
//    #else
//      mapper->SetInputData(polydata);
//    #endif
    //************************************** RENDER PHASE

  // Actor in scene
  VTK_CREATE(vtkActor, actor);
  //Options are mapper
  actor->SetMapper(mapper);

  actor->GetProperty()->SetEdgeColor(0, 0, 0);
  actor->GetProperty()->EdgeVisibilityOn();

  /////

  // VTK Renderer
  //VTK_CREATE(vtkRenderer, ren);

//#if VTK_MAJOR_VERSION <= 5
//  vtkSmartPointer<vtkRenderer> ren =
//    vtkSmartPointer<vtkRenderer>::New();
//#else
  this-> ren =
    vtkSmartPointer<vtkOpenGLRenderer>::New();
//#endif


  vtkSmartPointer<vtkAxesActor> axes =
    vtkSmartPointer<vtkAxesActor>::New();

    // WINDOW AND INTERACTOR
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
	
  renderWindow->AddRenderer(ren);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
  //


  // vtkSmartPointer<vtkOrientationMarkerWidget> widget =
  // vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  // widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
  // widget->SetOrientationMarker( axes );
  // widget->SetInteractor( renderWindowInteractor );
  // widget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
  // widget->SetEnabled( 1 );
  // widget->InteractiveOn();


  //Moving axes to screen corner

  //vtkSmartPointer<vtkTransform> transform =
  //  vtkSmartPointer<vtkTransform>::New();
  //transform->Translate(0.1, 0.0, 0.0);
  
  // The axes are positioned with a user transform
  //axes->SetUserTransform(transform);

  
  
  //////////////////////////// EX
  
  ren->SetBackground(.2, .3, .4);

  ren->GradientBackgroundOn();
  ren->SetBackground(1,1,1);
  ren->SetBackground2(0,0,1);


	vtkSmartPointer<vtkTextActor> textActor = 
    vtkSmartPointer<vtkTextActor>::New();
  textActor->GetTextProperty()->SetFontSize ( 24 );
  textActor->SetPosition2 ( 10, 40 );
  ren->AddActor2D ( textActor );
  textActor->SetInput ( "FluxSol" );
  textActor->GetTextProperty()->SetColor ( 0.08,0.0,0.4 );

  
//**************************** RANGE AND COLORS ******************************
 /*  // Generate the colors for each point based on the color map
  vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  colors->SetName("Colors");

    //Lookuptable
  // Create the color map
  vtkSmartPointer<vtkLookupTable> colorLookupTable =
    vtkSmartPointer<vtkLookupTable>::New();
  colorLookupTable->SetTableRange(minv, maxv);
  colorLookupTable->Build();


    for(int i = 0; i < polydata->GetNumberOfPoints(); i++)
    {
        unsigned char color[3];
        double p[3];
        polydata->GetPoint(i,p);
        double dcolor[3];
        colorLookupTable->GetColor(results[i], dcolor);
        for(unsigned int j = 0; j < 3; j++)
          color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
        colors->InsertNextTupleValue(color);
    }  
	
	polydata->GetPointData()->SetScalars(colors);
 
//    //************************************* MAPPER
//
     vtkSmartPointer<vtkPolyDataMapper> pdmapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
//
//    //pdmapper->ScalarVisibilityOff();
//
    #if VTK_MAJOR_VERSION <= 5
      pdmapper->SetInputConnection(polydata->GetProducerPort());
    #else
     pdmapper->SetInputData(polydata);
    #endif
 */
	/////// END OF CONTOUR
  
  ///////////////////////// RENDERING ////
 

	
	
 
   // Add Actor to renderer
  ren->AddActor(actor);
  ren->AddActor(axes);
  
 renderWindow->Render();	//If i want to obtain coordinates must to activate renderwindows with Render() 
 
  ///// NEED TO DETERMINE AXES WORLD POSITION GIVEN COORDINATES /////
  double coordinates[3]; 
  coordinates[0]=renderWindow->GetSize()[0]*0.9; 
  coordinates[1]=renderWindow->GetSize()[1]*0.1; 
  coordinates[2]=0.01; 
  cout << "Screen Size" << renderWindow->GetSize()[0] << " " << renderWindow->GetSize()[1]<< " " << renderWindow->GetSize()[2]<<endl;
	cout << "Display Point: "<<coordinates[0] << " "<<coordinates[1]<<" "<<coordinates[2]<<endl;
  //vtkDebugMacro("GetWorldToDisplayCoordinates: RAS " << worldPoints[0] << ", " << worldPoints[1] << ", " << worldPoints[2]) 
  
  ren->SetDisplayPoint(coordinates); 
  //ren->DisplayToView(); 
  //ren->GetViewPoint(coordinates); 
  //cout << "View Point: "<<coordinates[0] << " "<<coordinates[1]<<endl;
  //ren->ViewToWorld(); 
  //ren->GetWorldPoint();
	ren->DisplayToWorld();  
  double worldPt[3];
  ren->GetWorldPoint(worldPt);
  cout << "World Point: "<<worldPt[0] << " "<<worldPt[1]<<" "<<worldPt[2]<<endl;
  
  //double * worldcoor=new double [3];
  //worldcoor=ren->GetWorldPoint();
  //delete worldcoor;
  
  //double displayPt[3];
  //ren->GetDisplayPoint(displayPt);
  
  //Moving axes
    vtkSmartPointer<vtkTransform> transform =
    vtkSmartPointer<vtkTransform>::New();
  //transform->Translate(worldPt);
  //axes->SetUserTransform(transform);
  
  
	//////////////////// END AXES POSITIONING /////////



  // VTK/Qt wedded
  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren);	//Add 
  
  
  	//this->ui->qvtkWidget->SetRenderWindow(renderWindow);
	//renderWindow->SetInteractor(this->ui->qvtkWidget->GetInteractor());
  
  
  
  //renderWindowInteractor->SetRenderWindow(this->ui->qvtkWidget->GetRenderWindow()); 
  //this->ui->qvtkWidget->GetRenderWindow()->Render();

  // Just a bit of Qt interest: Culling off the
  // point data and handing it to a vtkQtTableView
  //VTK_CREATE(vtkDataObjectToTable, toTable);
  //toTable->SetInputConnection(elevation->GetOutputPort());

  //toTable->SetInputConnection(geometryFilter->GetOutputPort());

  //toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);

  // Here we take the end of the VTK pipeline and give it to a Qt View
  //this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());

  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->actionImport_in, SIGNAL(triggered()), this, SLOT(slotImportIn()));
  connect(this->ui->actionView_Z, SIGNAL(triggered()), this, SLOT(slotViewZpos()));
  

  //ren->ResetCamera();
  //renderWindow->Render();

    // Begin mouse interaction
  //renderWindowInteractor->Start();

  //renderWindowInteractor->Initialize();
  //renderWindowInteractor->Start();
  
    //vtkSmartPointer<vtkImageInteractionCallback1> callback =
    //vtkSmartPointer<vtkImageInteractionCallback1>::New();
	
	//callback->SetRenderer(ren);
	
	//renderWindowInteractor->AddObserver( vtkCommand::MouseMoveEvent, callback );
	renderWindowInteractor->SetRenderWindow ( renderWindow );
	renderWindowInteractor->Initialize();	
	//renderWindowInteractor->Start();
	
	//QvtkInteractor *iter;
	//vtkSmartPointer<QvtkInteractor> iter=
	//vtkSmartPointer<QvtkInteractor>::New();
	
	
	//this->ui->qvtkWidget->GetInteractor()->Start(); // ????
  
};

SimpleView::~SimpleView()
{
  // The smart pointers should clean up for up

}

// Action to be taken upon file open
void SimpleView::slotOpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("Open FluxSol Database"), ".",
	tr("FluxSol Database files (*.fdb)"));
	//if (!fileName.isEmpty())
		//loadFile(fileName);
}

void SimpleView::slotViewZpos()
{
	
	vtkSmartPointer<vtkCamera> camera = 
		vtkSmartPointer<vtkCamera>::New();	//Camera must be a Member??

	camera->SetPosition(0, 0, 20);
	camera->SetFocalPoint(0, 0, 0);
	this->ren->SetActiveCamera(camera);

	this->ui->qvtkWidget->GetRenderWindow()->Render();	//This updates de view
		
}

void SimpleView::slotImportIn()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("Open FluxSol Input File"), ".",
	tr("FluxSol Input Files (*.in)"));
	//if (!fileName.isEmpty())
		//loadFile(fileName);
}

void SimpleView::slotExit() {
  qApp->exit();
}
