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

#include <vtkRenderWindow.h>
#include <vtkVectorText.h>

#include <vtkRenderWindowInteractor.h>

//GRID
#include <vtkGeometryFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGridGeometryFilter.h>
#include <vtkDataSetSurfaceFilter.h>

#include <vtkShrinkFilter.h>
#include <vtkProperty.h>
#include <vtkOrientationMarkerWidget.h>

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkLookupTable.h>

//NEW!! Extracted from Shadows test project
#if VTK_MAJOR_VERSION <= 5
#include <vtkRenderer.h>
#else
#include <vtkOpenGLRenderer.h>
#endif

#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>


#include "vtkSmartPointer.h"
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include <vector>
#include <sstream>

using namespace std;
// Constructor
SimpleView::SimpleView()
{
  this->ui = new Ui_SimpleView;
  this->ui->setupUi(this);

  // Qt Table View
  this->TableView = vtkSmartPointer<vtkQtTableView>::New();

  // Place the table view in the designer form
  this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());

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
    vector<double> results;
    fstream in_stream("results.txt");
    double minv,maxv;
    minv=1.0e6;maxv=0.;
    while (!in_stream.eof())
   {
       string line;
        getline(in_stream, line);
        stringstream ss;
        ss << line;
        double d;
        if (d<minv)minv=d;
        if (d>maxv)maxv=d;
        ss >> d;
        results.push_back(d);
   }

    cout << "Results size: " << results.size()<<endl;

    // Data taken from vtk examples ColoredElevationMap and Filled Contours
    //This is interesting too
    //http://www.vtk.org/Wiki/VTK/Examples/Cxx/Visualization/ElevationBandsWithGlyphs
    //CONTOUR INFORMATION
    //
//
//   //converter para polydata
    //There are two choices to convert mesh to polydata
    //But geometryfilter
   vtkUnstructuredGrid * unstructured= vtkUnstructuredGrid::New();
   unstructured = reader ->GetOutput();
   //unstructured ->Update();
//

  vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
    vtkSmartPointer<vtkDataSetSurfaceFilter>::New();

  vtkSmartPointer<vtkGeometryFilter> geometryFilter2 =
    vtkSmartPointer<vtkGeometryFilter>::New();
//
//      vtkSmartPointer<vtkUnstructuredGridGeometryFilter> geometryFilter2 =
//    vtkSmartPointer<vtkUnstructuredGridGeometryFilter>::New();
    #if VTK_MAJOR_VERSION <= 5
      geometryFilter2->SetInput(unstructured);
      surfaceFilter->SetInput(unstructured);
    #else
      geometryFilter2->SetInputData(unstructured);
      surfaceFilter->SetInputData(unstructured);
    #endif
      geometryFilter2->Update();
      surfaceFilter->Update();
//
    vtkPolyData * polydata= geometryFilter2 ->GetOutput ();
   // vtkPolyData* polydata = surfaceFilter->GetOutput();

    //vtkPolyData * polydata= geometryFilter2 ->GetOutput ();
//
//    //polydata -> SetPoints(teste->GetOutput()->GetPoints());
//
//**************************** RANGE AND COLORS ******************************
  // Generate the colors for each point based on the color map
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
//        cout << p[0] << " " << p[1] << p[2]<<endl;
        double dcolor[3];
        colorLookupTable->GetColor(results[i], dcolor);
 //       std::cout << "dcolor: "
//                  << dcolor[0] << " "
//                  << dcolor[1] << " "
//                  << dcolor[2] << std::endl;

        for(unsigned int j = 0; j < 3; j++)
          {
          color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
          }
//        std::cout << "color: "
//                  << (int)color[0] << " "
//                  << (int)color[1] << " "
//                  << (int)color[2] << std::endl;

        colors->InsertNextTupleValue(color);
    }

    cout << "Maxs " << minv << " " <<maxv <<endl;

    polydata->GetPointData()->SetScalars(colors);

//       //TO OBTAIN RANGE
     double scalarRange[2];
//     polydata->GetPointData()->GetScalars()->GetRange(scalarRange);

    //cout << "Scalar Range: "<<scalarRange.size()<<endl;
//    cout << scalarRange[0]<<" "<<scalarRange[1]<<endl;

//
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
    //************************************** RENDER PHASE

  // Actor in scene
  VTK_CREATE(vtkActor, actor);
  //Options are mapper
  //MESH
  //actor->SetMapper(mapper);

  actor->SetMapper(pdmapper);

  //actor->GetProperty()->SetEdgeColor(0, 0, 0);
  actor->GetProperty()->EdgeVisibilityOn();

  /////

  // VTK Renderer
  //VTK_CREATE(vtkRenderer, ren);

//#if VTK_MAJOR_VERSION <= 5
//  vtkSmartPointer<vtkRenderer> ren =
//    vtkSmartPointer<vtkRenderer>::New();
//#else
  vtkSmartPointer<vtkOpenGLRenderer> ren =
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


  vtkSmartPointer<vtkOrientationMarkerWidget> widget =
    vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
  widget->SetOrientationMarker( axes );
  widget->SetInteractor( renderWindowInteractor );
  widget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
  widget->SetEnabled( 1 );
  widget->InteractiveOn();



  ren->SetBackground(.2, .3, .4);
  // Add Actor to renderer
  ren->AddActor(actor);
  ren->AddActor(axes);

  // VTK/Qt wedded
  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren);

  // Just a bit of Qt interest: Culling off the
  // point data and handing it to a vtkQtTableView
  VTK_CREATE(vtkDataObjectToTable, toTable);
  toTable->SetInputConnection(elevation->GetOutputPort());

  toTable->SetInputConnection(geometryFilter->GetOutputPort());

  toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);

  // Here we take the end of the VTK pipeline and give it to a Qt View
  this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());

  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));


  //ren->ResetCamera();
  //renderWindow->Render();

    // Begin mouse interaction
  //renderWindowInteractor->Start();



};

SimpleView::~SimpleView()
{
  // The smart pointers should clean up for up

}

// Action to be taken upon file open
void SimpleView::slotOpenFile()
{

}

void SimpleView::slotExit() {
  qApp->exit();
}
