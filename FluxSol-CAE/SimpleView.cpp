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


using namespace FluxSol;
using namespace std;

#include <map>

#include <vector>

using namespace std;

void addTreeChild(QTreeWidgetItem *parent,
                  QString name, QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
}


template<class TReader> vtkDataSet *ReadAnXMLFile(const char*fileName)
{
  vtkSmartPointer<TReader> reader =
    vtkSmartPointer<TReader>::New();
  reader->SetFileName(fileName);
  reader->Update();
  reader->GetOutput()->Register(reader);
  return vtkDataSet::SafeDownCast(reader->GetOutput());
}

void FindAllData(vtkPolyData* polydata);

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

		 //this->renderer->AddActor2D ( textActor2 );	//But this mantain the actor permanent
		 textActor2->SetInput ( "Test" );
		 textActor2->GetTextProperty()->SetColor ( 0.08,0.0,0.4 );

	}

	private:

	vtkOpenGLRenderer* renderer;
	QVTKWidget *widget;

  };

///////

// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnKeyPress()
    {
      // Get the keypress
      vtkRenderWindowInteractor *rwi = this->Interactor;
      std::string key = rwi->GetKeySym();

      // Output the key that was pressed
      std::cout << "Pressed " << key << std::endl;

      // Handle an arrow key
      if(key == "Up")
        {
        std::cout << "The up arrow was pressed." << std::endl;
        }

      // Handle a "normal" key
      if(key == "a")
        {
        std::cout << "The a key was pressed." << std::endl;
        }

      // Forward events
      vtkInteractorStyleTrackballCamera::OnKeyPress();
    }

};
vtkStandardNewMacro(KeyPressInteractorStyle);

// Constructor
SimpleView::SimpleView()
{
  this->ui = new Ui_SimpleView;
  this->ui->setupUi(this);

  // Qt Table View
  this->TableView = vtkSmartPointer<vtkQtTableView>::New();

//    this->ui->tabWidget->currentWidget()->ExpandAll();
//	QTreeWidget *modtree=qobject_cast<this->ui->tabWidget->currentWidget()>;
//    this->ui->ModelTree->ExpandAll();
	//Another way to access tree item is widget(int)

	// Place the table view in the designer form
  //this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());


    this->comboBox = new QComboBox(this);
    this->ui->toolBar_Results->addWidget(comboBox);

    this->ui->ModelTree->expandAll();


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

    double bounds[6];
    //reader->GetBounds(bounds);

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
    //vtkSmartPointer<vtkRenderer>::New();
//#endif


    // WINDOW AND INTERACTOR
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(ren);



  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();


   _vtkAxes =
    vtkSmartPointer<vtkAxesActor>::New();

  //Moving axes to screen corner

  //vtkSmartPointer<vtkTransform> transform =
  //  vtkSmartPointer<vtkTransform>::New();
  //transform->Translate(0.1, 0.0, 0.0);

  // The axes are positioned with a user transform
  //axes->SetUserTransform(transform);
  _vtkAxes->AxisLabelsOff();



  //////////////////////////// EX

  ren->SetBackground(.2, .3, .4);

  ren->GradientBackgroundOn();
  //ren->SetBackground(0.6,0.7,1.);
  ren->SetBackground(1.,1.,1.);
  ren->SetBackground2(0,0,1);


	vtkSmartPointer<vtkTextActor> textActor =
    vtkSmartPointer<vtkTextActor>::New();
  textActor->GetTextProperty()->SetFontSize ( 24 );
  ren->AddActor2D ( textActor );
  textActor->SetInput ( "FluxSol" );
  //textActor->SetPosition2 ( 1., 0.);
  //textActor->SetPosition ( 400., 400.);
  textActor->SetPosition ( 600, 600);
  double xpos,ypos;

  //GetRenderWindow()->GetSize()
  cout << "xsize: "<<this->ui->qvtkWidget->GetRenderWindow()->GetSize()[0]<<endl;
  cout << "ysize: "<<this->ui->qvtkWidget->GetRenderWindow()->GetSize()[1]<<endl;

  xpos=(double)(renderWindow->GetSize()[0])*0.9;
 //textActor->SetPosition ( 800, 50);
//  textActor->GetTextProperty()->SetColor ( 0.08,0.0,0.4 );
//  textActor->SetInput ( "v0.0.1" );





   // Add Actor to renderer
  ren->AddActor(actor);
  ren->AddActor(_vtkAxes);

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


  // VTK/Qt wedded
  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren);	//Add

  //This is like the example Four Pane Viewer
  renderWindowInteractor = this->ui->qvtkWidget->GetInteractor();

	// ----------------------- AXES
//	char *filename="bunny.vtp";
//          // Read the polydata for the icon
//      vtkSmartPointer<vtkXMLPolyDataReader> bunnyreader =
//        vtkSmartPointer<vtkXMLPolyDataReader>::New();
//      reader->SetFileName(filename);
//
//      vtkSmartPointer<vtkDataSetMapper> iconMapper =
//        vtkSmartPointer<vtkDataSetMapper>::New();
//      iconMapper->SetInputConnection(bunnyreader->GetOutputPort());
//
//      vtkSmartPointer<vtkActor> iconActor =
//        vtkSmartPointer<vtkActor>::New();
//      iconActor->SetMapper(iconMapper);

    _vtkAxesWidget =
    vtkSmartPointer<vtkOrientationMarkerWidget>::New();

    _vtkAxesWidget->SetDefaultRenderer(ren);
    _vtkAxesWidget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    _vtkAxesWidget->SetOrientationMarker( _vtkAxes );
    //widget->SetOrientationMarker( iconActor );
    _vtkAxesWidget->SetInteractor(renderWindowInteractor );
    _vtkAxesWidget->SetViewport( 0., 0., 0.34, 0.34 );
    _vtkAxesWidget->SetEnabled( 1 );
    _vtkAxesWidget->InteractiveOn();





  //widget->SetInteractor( renderWindowInteractor);


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
  connect(this->ui->actionImportMesh, SIGNAL(triggered()), this, SLOT(slotImportMesh()));
  connect(this->ui->actionOpenResults, SIGNAL(triggered()), this, SLOT(slotOpenResults()));

  //ren->ResetCamera();
  //renderWindow->Render();

    vtkSmartPointer<vtkImageInteractionCallback1> callback =
    vtkSmartPointer<vtkImageInteractionCallback1>::New();

	callback->SetRenderer(ren);
	renderWindowInteractor->AddObserver( vtkCommand::MouseMoveEvent, callback );

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


// Action to be taken upon Impor Mesh
void SimpleView::slotImportMesh()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("Import Mesh"), ".",
	tr("Fluent Mesh files (*.msh)\n"
	   "CGNS Mesh files (*.cgns)\n"
	   "VTK Generic Mesh files (*.vtk)"));


	//Another option is to separate with commas
	if (!fileName.isEmpty())
	{
		Fv_CC_Grid mesh(fileName.toStdString());
		mesh.Log("Log.txt");


	    vtkUnstructuredGrid *ugrid;

        // ------- SIMPLE READER

          //read all the data from the file
      vtkSmartPointer<vtkXMLUnstructuredGridReader> ugreader =
        vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
      ugreader->SetFileName(fileName.toStdString().c_str());
      ugreader->Update();


          vtkSmartPointer<vtkUnstructuredGridGeometryFilter> geometryFilter =
            vtkSmartPointer<vtkUnstructuredGridGeometryFilter>::New();
      geometryFilter->SetInputConnection(ugreader->GetOutputPort());
      geometryFilter->Update();

      // Visualize
     // vtkSmartPointer<vtkPolyDataMapper> pdmapper =
     //   vtkSmartPointer<vtkPolyDataMapper>::New();
     // pdmapper->SetInputConnection(geometryFilter->GetOutputPort());



          vtkSmartPointer<vtkDataSetMapper> datasetmapper =
        vtkSmartPointer<vtkDataSetMapper>::New();
      datasetmapper ->SetInputConnection(ugreader->GetOutputPort());

      vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();
      actor->SetMapper(datasetmapper);


          actor->GetProperty()->SetEdgeColor(0, 0, 0);
          actor->GetProperty()->EdgeVisibilityOn();

      this->ren->AddActor( actor);

	}

}

// Action to be taken upon Impor Mesh
void SimpleView::slotOpenResults()
{
	QString fileName = QFileDialog::getOpenFileName(this,
	tr("Open Results File"), ".",
	tr(	"VTK Unstructured Grid Files (*.vtu)\n"
		"VTK Polydata Files (*.vtp) \n"
		"VTK Structured Grid Files(*.vts)"));

	//Another option is to separate with commas
	if (!fileName.isEmpty())
	{

        vtkDataSet *dataSet;
        std::string extension =
          vtksys::SystemTools::GetFilenameLastExtension(fileName.toStdString().c_str());

        // Dispatch based on the file extension
        if (extension == ".vtu")
          {
          dataSet = ReadAnXMLFile<vtkXMLUnstructuredGridReader> (fileName.toStdString().c_str());
          }
        else if (extension == ".vtp")
          {
          dataSet = ReadAnXMLFile<vtkXMLPolyDataReader> (fileName.toStdString().c_str());
          }
        else if (extension == ".vts")
          {
          dataSet = ReadAnXMLFile<vtkXMLStructuredGridReader> (fileName.toStdString().c_str());
          }
        else if (extension == ".vtr")
          {
          dataSet = ReadAnXMLFile<vtkXMLRectilinearGridReader> (fileName.toStdString().c_str());
          }
        else if (extension == ".vti")
          {
          dataSet = ReadAnXMLFile<vtkXMLImageDataReader> (fileName.toStdString().c_str());
          }
        else if (extension == ".vto")
          {
          dataSet = ReadAnXMLFile<vtkXMLHyperOctreeReader> (fileName.toStdString().c_str());
          }
        else if (extension == ".vtk")
          {
          dataSet = ReadAnXMLFile<vtkDataSetReader> (fileName.toStdString().c_str());
          }
        else
          {
          std::cerr << fileName.toStdString().c_str() << " Unknown extenstion: " << extension << std::endl;
          return EXIT_FAILURE;
          }

        int numberOfCells = dataSet->GetNumberOfCells();
        int numberOfPoints = dataSet->GetNumberOfPoints();

        // Generate a report
        std::cout << "------------------------" << std::endl;
        std::cout << fileName.toStdString().c_str() << std::endl
             << " contains a " << std::endl
             << dataSet->GetClassName()
             <<  " that has " << numberOfCells << " cells"
             << " and " << numberOfPoints << " points." << std::endl;
        typedef std::map<int,int> CellContainer;
        CellContainer cellMap;
        for (int i = 0; i < numberOfCells; i++)
          {
          cellMap[dataSet->GetCellType(i)]++;
          }

        CellContainer::const_iterator it = cellMap.begin();
        while (it != cellMap.end())
          {
          std::cout << "\tCell type "
               << vtkCellTypes::GetClassNameFromTypeId(it->first)
               << " occurs " << it->second << " times." << std::endl;
          ++it;
          }

        // Now check for point data
        vtkPointData *pd = dataSet->GetPointData();
        if (pd)
          {
          std::cout << " contains point data with "
               << pd->GetNumberOfArrays()
               << " arrays." << std::endl;
          for (int i = 0; i < pd->GetNumberOfArrays(); i++)
            {
            std::cout << "\tArray " << i
                 << " is named "
                 << (pd->GetArrayName(i) ? pd->GetArrayName(i) : "NULL")
                 << std::endl;
            }
          }
        // Now check for cell data
        vtkCellData *cd = dataSet->GetCellData();
        if (cd)
          {
          std::cout << " contains cell data with "
               << cd->GetNumberOfArrays()
               << " arrays." << std::endl;
          for (int i = 0; i < cd->GetNumberOfArrays(); i++)
            {
            std::cout << "\tArray " << i
                 << " is named "
                 << (cd->GetArrayName(i) ? cd->GetArrayName(i) : "NULL")
                 << std::endl;
            }
          }
        // Now check for field data
        if (dataSet->GetFieldData())
          {
          std::cout << " contains field data with "
               << dataSet->GetFieldData()->GetNumberOfArrays()
               << " arrays." << std::endl;
          for (int i = 0; i < dataSet->GetFieldData()->GetNumberOfArrays(); i++)
            {
            std::cout << "\tArray " << i
                 << " is named " << dataSet->GetFieldData()->GetArray(i)->GetName()
                 << std::endl;
            }
          }
        //dataSet->Delete();


    // IF  /////////////////////////////

        //TO TEMPLATIZE
        //ugrid = meshreader ->GetOutput();

        double bounds[6];
        dataSet->GetBounds(bounds);

   std::cout  << "xmin: " << bounds[0] << " "
             << "xmax: " << bounds[1] << std::endl
             << "ymin: " << bounds[2] << " "
             << "ymax: " << bounds[3] << std::endl
             << "zmin: " << bounds[4] << " "
             << "zmax: " << bounds[5] << std::endl;


      vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
        vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

      reader->SetFileName(fileName.toStdString().c_str());
      reader->Update();

        vtkSmartPointer<vtkUnstructuredGrid> ugrid=
        vtkUnstructuredGrid::New();;
        ugrid = reader->GetOutput();

      double scalarRange[2];
      pd->GetArray(0);

      //int components =
        //this->PointData->GetScalars()->GetNumberOfComponents();
        double tuple[3];
        //double* tuple = pd->GetArray(0)->GetTuple( 1 );
        pd->GetArray(0)->GetTuple( 1 ,tuple);

        pd->GetArray(0)->GetRange(scalarRange);

        cout << "Tuple" << tuple[0]<<endl;

      std::cout << pd->GetArrayName(0)<<std::endl;

//    //GeometryFilter


      vtkSmartPointer<vtkGeometryFilter> geometryFilter =
        vtkSmartPointer<vtkGeometryFilter>::New();

        vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
        vtkSmartPointer<vtkDataSetSurfaceFilter>::New();

      surfaceFilter->SetInputData(ugrid);

    #if VTK_MAJOR_VERSION <= 5
      geometryFilter->SetInput(ugrid);
      surfaceFilter->SetInput(ugrid);
    #else
      geometryFilter->SetInputData(ugrid);
      surfaceFilter->SetInputData(ugrid);
    #endif
      geometryFilter->Update();
      surfaceFilter->Update();

    //vtkSmartPointer<vtkPolyData>
    vtkPolyData *polydata= geometryFilter ->GetOutput ();



//**************************** RANGE AND COLORS ******************************
//   Generate the colors for each point based on the color map
  vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  colors->SetName("Colors");

    //Lookuptable
  // Create the color map
  vtkSmartPointer<vtkLookupTable> colorLookupTable =
    vtkSmartPointer<vtkLookupTable>::New();

    double minv,maxv;
    minv=scalarRange[0];
    maxv=scalarRange[1];

    vector<double> results;
    results.assign(dataSet->GetNumberOfPoints(),0.);
//////////Assigning results
    for(int i = 0; i < polydata->GetNumberOfPoints(); i++)//Or dataset
    {
        results[i]=double(i)/double(polydata->GetNumberOfPoints());
    }

  colorLookupTable->SetTableRange(minv, maxv);
  colorLookupTable->Build();


    for(int i = 0; i < polydata->GetNumberOfPoints(); i++)
    {
        unsigned char color[3];
        double p[3];
        //polydata->GetPoint(i,p);
        double dcolor[3];
        pd->GetArray(0)->GetTuple( i ,tuple);
        colorLookupTable->GetColor(tuple[0], dcolor);
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

	/////// END OF CONTOUR

  ///////////////////////// RENDERING ////

        VTK_CREATE(vtkActor, actor);
        actor->SetMapper(pdmapper);
        actor->GetProperty()->EdgeVisibilityOn();
        this->ren->AddActor(actor);


////////////////////////////////////////// COLOR BAR

  vtkSmartPointer<vtkScalarBarActor> scalarBar =
    vtkSmartPointer<vtkScalarBarActor>::New();
  scalarBar->SetLookupTable(pdmapper->GetLookupTable());
  scalarBar->SetTitle(pd->GetArrayName(0));
  scalarBar->SetNumberOfLabels(4);
  //scalarBar->GetLabelTextProperty()->SetFontSize(4);
  scalarBar->SetHeight(0.40);
  scalarBar->SetWidth(0.10);
  scalarBar->GetTitleTextProperty()->SetFontFamilyToArial();
  scalarBar->GetTitleTextProperty()->ShadowOff();
  scalarBar->GetTitleTextProperty()->ItalicOff();
  scalarBar->GetTitleTextProperty()->BoldOff();
  scalarBar->GetTitleTextProperty()->SetFontSize(10);
    scalarBar->GetTitleTextProperty()->SetColor ( 0.0,0.0,0.0 );

  scalarBar->GetLabelTextProperty()->SetFontFamilyToArial();
  scalarBar->GetLabelTextProperty()->ShadowOff();
  scalarBar->GetLabelTextProperty()->ItalicOff();
  scalarBar->GetLabelTextProperty()->BoldOff();
  scalarBar->GetLabelTextProperty()->SetColor ( 0.0,0.0,0.0 );
  //scalarBar->SetPosition(0.04, 0.02);
//  this->colorBar->SetOrientationToHorizontal();

//vtkScalarBarActor::SizeTitle()

//  colorLookupTable->SetTableRange (0, 1);
//  colorLookupTable->SetHueRange (0, 1);
//  colorLookupTable->SetSaturationRange (1, 1);
//  colorLookupTable->SetValueRange (1, 1);
//  colorLookupTable->Build();

  pdmapper->SetLookupTable( colorLookupTable );
  scalarBar->SetLookupTable( colorLookupTable );

        //Results Combo Box
        this->comboBox->addItem(pd->GetArrayName(0));

        this->ren->AddActor2D(scalarBar);

	}//If file name is not empty




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
	if (!fileName.isEmpty())
    {

        QString name="test";
        QString Description="desc";

        //TO Modify, at first is only a CFD Model
        GraphicCFDModel model(fileName.toStdString());

        //QTreeWidgetItem *itm = new QTreeWidgetItem(this->ui->ModelTree); //WITH THIS ARGS DOES NOT WORK
        QTreeWidgetItem *itm = new QTreeWidgetItem();
        itm->setText(0,name);
        itm->setText(1,Description);
        //this->ui->ModelTree->topLevelItem( 0 )->addChild(itm);

        //item->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        //item->setCheckState(0, Qt::Unchecked);

        QList<QTreeWidgetItem*> items = this->ui->ModelTree->findItems("Models",Qt::MatchExactly);
        int num = items.count();
        cout <<"Items found"<<num;

        addTreeChild(items[0], fileName, "Desc");

        this->ui->ModelTree->update();
        //items[0]->addChild(itm);

        items = this->ui->ModelTree->findItems("Model-1",Qt::MatchExactly);
        cout <<"Items found"<<num<<endl;
        if (items.count()>0)
        addTreeChild(items[0],
                  "Hola2", "Desc");

        //items[0]->insertChild(0,itm);
        //items[0]->setText(0,"Changed");

        QTreeView modtreeview(this->ui->ModelTree);


        modtreeview.expandAll();
        modtreeview.show();

        //this->ui->ModelTree->expandAll();


        this->ui->ModelTree->update();
        this->ui->ModelTree->show();


        /////////////////////////////
        ///// MODEL VISUALIZATION ///
        /////////////////////////////

      vtkSmartPointer<vtkXMLUnstructuredGridReader> reader =
        vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();

      reader->SetFileName(fileName.toStdString().c_str());
      reader->Update();

        vtkSmartPointer<vtkUnstructuredGrid> ugrid=
        vtkUnstructuredGrid::New();;
        ugrid = reader->GetOutput();

      double scalarRange[2];
      //spd->GetArray(0);

      //int components =
        //this->PointData->GetScalars()->GetNumberOfComponents();
        double tuple[3];
        //double* tuple = pd->GetArray(0)->GetTuple( 1 );
        //pd->GetArray(0)->GetTuple( 1 ,tuple);
        //pd->GetArray(0)->GetRange(scalarRange);

        cout << "Tuple" << tuple[0]<<endl;

      //std::cout << pd->GetArrayName(0)<<std::endl;

//    //GeometryFilter


      vtkSmartPointer<vtkGeometryFilter> geometryFilter =
        vtkSmartPointer<vtkGeometryFilter>::New();

        vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
        vtkSmartPointer<vtkDataSetSurfaceFilter>::New();

      surfaceFilter->SetInputData(ugrid);

    #if VTK_MAJOR_VERSION <= 5
      geometryFilter->SetInput(ugrid);
      surfaceFilter->SetInput(ugrid);
    #else
      geometryFilter->SetInputData(ugrid);
      surfaceFilter->SetInputData(ugrid);
    #endif
      geometryFilter->Update();
      surfaceFilter->Update();

    //vtkSmartPointer<vtkPolyData>
    vtkPolyData *polydata= geometryFilter ->GetOutput ();



//**************************** RANGE AND COLORS ******************************
//   Generate the colors for each point based on the color map
  vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetNumberOfComponents(3);
  colors->SetName("Colors");

    for(int i = 0; i < polydata->GetNumberOfPoints(); i++)
    {
        unsigned char color[3];
        for(unsigned int j = 0; j < 3; j++)
          //color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
          color [j]=255.;
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

    // If i want to sinply display mesh
    // Visualize
    //  vtkSmartPointer<vtkDataSetMapper> mapper =
    //    vtkSmartPointer<vtkDataSetMapper>::New();
    //#if VTK_MAJOR_VERSION <= 5
    //  mapper->SetInputConnection(uGrid->GetProducerPort());
    //#else
    //  mapper->SetInputData(uGrid);
    //#endif



	/////// END OF CONTOUR

  ///////////////////////// RENDERING ////

        VTK_CREATE(vtkActor, actor);
        actor->SetMapper(pdmapper);
        actor->GetProperty()->EdgeVisibilityOn();
        this->ren->AddActor(actor);


    }
		//loadFile(fileName);
}

void SimpleView::slotExit() {
  qApp->exit();
}



void FindAllData(vtkPolyData* polydata)
{
  std::cout << "Normals: " << polydata->GetPointData()->GetNormals() << std::endl;

  vtkIdType numberOfPointArrays = polydata->GetPointData()->GetNumberOfArrays();
  std::cout << "Number of PointData arrays: " << numberOfPointArrays << std::endl;

  vtkIdType numberOfCellArrays = polydata->GetCellData()->GetNumberOfArrays();
  std::cout << "Number of CellData arrays: " << numberOfCellArrays << std::endl;

  std::cout << "Type table/key: " << std::endl;;
  //more values can be found in <VTK_DIR>/Common/vtkSetGet.h
  std::cout << VTK_UNSIGNED_CHAR << " unsigned char" << std::endl;
  std::cout << VTK_UNSIGNED_INT << " unsigned int" << std::endl;
  std::cout << VTK_FLOAT << " float" << std::endl;
  std::cout << VTK_DOUBLE << " double" << std::endl;

  for(vtkIdType i = 0; i < numberOfPointArrays; i++)
    {
    // The following two lines are equivalent
    //arrayNames.push_back(polydata->GetPointData()->GetArray(i)->GetName());
    //arrayNames.push_back(polydata->GetPointData()->GetArrayName(i));
    int dataTypeID = polydata->GetPointData()->GetArray(i)->GetDataType();
    std::cout << "Array " << i << ": " << polydata->GetPointData()->GetArrayName(i)
              << " (type: " << dataTypeID << ")" << std::endl;
    }

  for(vtkIdType i = 0; i < numberOfCellArrays; i++)
    {
    // The following two lines are equivalent
    //polydata->GetPointData()->GetArray(i)->GetName();
    //polydata->GetPointData()->GetArrayName(i);
    int dataTypeID = polydata->GetCellData()->GetArray(i)->GetDataType();
    std::cout << "Array " << i << ": " << polydata->GetCellData()->GetArrayName(i)
              << " (type: " << dataTypeID << ")" << std::endl;
    }
}
