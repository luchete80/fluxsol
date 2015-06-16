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

#include "./Job/JobSubmitDialog.h"


using namespace FluxSol;
using namespace std;

#include <map>

#include <vector>

#include <vtkCallbackCommand.h>


static void CameraModifiedCallback(vtkObject* caller,
                                   long unsigned int vtkNotUsed(eventId),
                                   void* vtkNotUsed(clientData),
                                   void* vtkNotUsed(callData) )
{
//  std::cout << caller->GetClassName() << " modified" << std::endl;

  vtkCamera* camera = static_cast<vtkCamera*>(caller);
  // print the interesting stuff
//  std::cout << "\tPosition: "
//            << camera->GetPosition()[0] << ", "
//            << camera->GetPosition()[1] << ", "
//            << camera->GetPosition()[2] << std::endl;
//  std::cout << "\tFocal point: "
//            << camera->GetFocalPoint()[0] << ", "
//            << camera->GetFocalPoint()[1] << ", "
//            << camera->GetFocalPoint()[2] << std::endl;
}

void WindowModifiedCallback( vtkObject*
                      caller, long unsigned int vtkNotUsed(eventId), void* vtkNotUsed(clientData), void* vtkNotUsed(callData))
{
  std::cout << "Window modified" << std::endl;
  std::cout << caller->GetClassName() << std::endl;

  vtkRenderWindow* window = static_cast<vtkRenderWindow*>(caller); //Observer is put in renderwindow


  vtkSmartPointer<vtkOpenGLRenderer>  renderer =
    vtkSmartPointer<vtkOpenGLRenderer>::New();

  int* windowSize = window->GetSize();
  std::cout << "Size: " << windowSize[0] << " " << windowSize[1] << std::endl;

  //if(windowSize[0] > 400)
    {
    //window->SetSize(400, windowSize[1]);
    //window->Render();
    //window->Modified();
    //window->Render();

    }

vtkSmartPointer<vtkTextActor> textActor =
    vtkSmartPointer<vtkTextActor>::New();
  textActor ->GetTextProperty()->SetFontSize ( 24 );
  //renderer->AddActor2D ( textActor  );
  textActor ->SetInput ( "FluxSol" );
  //textActor->SetPosition2 ( 1., 0.);
  //textActor->SetPosition ( 400., 400.);
   double xpos,ypos;
//
//  //GetRenderWindow()->GetSize()
////  cout << "xsize: "<<this->ui->qvtkWidget->GetRenderWindow()->GetSize()[0]<<endl;
//  //cout << "ysize: "<<this->ui->qvtkWidget->GetRenderWindow()->GetSize()[1]<<endl;
//
//    //double xsize=(double)(this->ui->qvtkWidget->GetRenderWindow()->GetSize()[0]);
//    double xsize=windowSize[0];
//    xpos=xsize*0.95;
//  cout << "xsize" << xsize << "xpos" <<xpos<<endl;
// textActor ->SetPosition ( xpos, 200);
//
    //window->Modified();
    //window->AddRenderer(renderer);
    //renderer->AddActor(textActor);
//
    //window->Render();

}

void showContextMenu(const QPoint &pos, QTreeWidget &tree)
{
  QMenu menu;

  QTreeWidgetItem* item = tree.itemAt(pos);
 // switch (item->type()) {
 // case ItemType1:
    menu.addAction("This is a type 1");
    //break;

//  case ItemType2:
 //   menu.addAction("This is a type 2");
  //  break;

  menu.exec(tree.mapToGlobal(pos));
}


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

		 this->renderer->AddActor2D ( textActor2 );	//But this mantain the actor permanent
		 textActor2->SetInput ( "Test" );
		 textActor2->GetTextProperty()->SetColor ( 0.08,0.0,0.4 );

	}

	private:

	vtkOpenGLRenderer* renderer;
	QVTKWidget *widget;

  };

//Like previous callback, must to obtain current render
class vtkWindowsModInteractionCallback  : public vtkCommand
{
public:
  static vtkWindowsModInteractionCallback *New()
    {
    return new vtkWindowsModInteractionCallback;
    }
	void SetRenderer(vtkOpenGLRenderer* ren)
	{
		this->renderer=ren;
	}

	void SetTextActor(vtkTextActor *ta)
	{
		this->textActor=ta;
	}

  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {

        //int* windowSize = window->GetSize();

  vtkRenderWindow* window = static_cast<vtkRenderWindow*>(caller); //Observer is put in renderwindow

  int* windowSize = window->GetSize();

         double maxsize=windowSize[0]/40.;
         textActor->GetTextProperty()->SetFontSize ( (int) maxsize);
		 textActor->SetInput ( "FluxSol\nv0.0.1" );

		 //textActor->SetPosition ( windowSize[0]*0.85, windowSize[1]*0.85);
		 //textActor->GetTextProperty()->SetColor ( 1.,1.,1. );

		 textActor->SetPosition ( windowSize[0]*0.9, windowSize[1]*0.05);
		 textActor->GetTextProperty()->SetColor ( 0.,0.,0.3 );
//         std::cout << "Size: " << windowSize[0] << " " << windowSize[1] << std::endl;
//		 cout << "Mod 2"<<endl;

	}

	private:

	vtkOpenGLRenderer* renderer;
	QVTKWidget *widget;
	vtkTextActor *textActor;

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
    this->ResultsVarDim = new QComboBox(this);
    //this->ui->toolBar_Results->addWidget(comboBox);

    QWidget* empty = new QWidget();
    //empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    empty->resize(10, empty->height());
    this->ui->ResultsToolBar->addWidget(comboBox);
    this->ui->ResultsToolBar->addWidget(empty);
    this->ui->ResultsToolBar->addWidget(ResultsVarDim);


    this->ui->ModelTree->expandAll();

    //this->ui->ModelTree->setContextMenuPolicy(Qt::ActionsContextMenu);
    //this->ui->ModelTree->addAction(this->ui->actionOpenResults);    //DEFAULT IS RIGHT CLICK


    //ADDING MANUALLY QTREE ITEMS
   // ResultsTab = new ModelTreeWidget();
    //this->ui->tabWidget->addTab(this->ResultsTab, QString());
    //this->ui->tabWidget->addTab(this->ResultsTab, QString("Results"));

    this->ui->tabWidget->setCurrentIndex(0);
    this->ui->ToolbarTab->setCurrentIndex(0);


//
//  QMenu *menu = new QMenu(ui->ModelTree);
//  QAction*  myAction = menu->addAction("Remove");
//  //myAction->setIcon(QIcon(QString::fromUtf8("Resources/Remove.png")));
//  myAction->setShortcut(tr("Ctrl+D"));
//  myAction->setStatusTip(tr("Remove the respective material from the User DB"));
//  //myAction = menu->exec(ui->ModelTree->viewport()->mapToGlobal(pos));
//  myAction = menu->exec(its[0]);
//  if(myAction == NULL) return;
//  on_deletefile_clicked();

//    its[0]->addAction(this->ui->actionOpenResults);


  // Geometry
  VTK_CREATE(vtkVectorText, text);
  text->SetText("VTK and Qt!");
  VTK_CREATE(vtkElevationFilter, elevation);
  elevation->SetInputConnection(text->GetOutputPort());
  elevation->SetLowPoint(0,0,0);
  elevation->SetHighPoint(10,0,0);



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



  this->renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();


   _vtkAxes =
    vtkSmartPointer<vtkAxesActor>::New();

    _vtkOriginAxes =
    vtkSmartPointer<vtkAxesActor>::New();

  //Moving axes to screen corner

  //vtkSmartPointer<vtkTransform> transform =
  //  vtkSmartPointer<vtkTransform>::New();
  //transform->Translate(0.1, 0.0, 0.0);

  // The axes are positioned with a user transform
  //axes->SetUserTransform(transform);
  _vtkOriginAxes->AxisLabelsOff();
  _vtkOriginAxes->SetTipTypeToSphere();
  //_vtkOriginAxes->SetSphereResolution(0);
  _vtkOriginAxes->SetNormalizedTipLength(0.,0.,0.);

  //////////////////////////// EX

  ren->SetBackground(.2, .3, .4);

  ren->GradientBackgroundOn();
  //ren->SetBackground(0.6,0.7,1.);
  ren->SetBackground(1.,1.,1.);
  ren->SetBackground2(0,0,1);


	textFluxSolTitleActor =
    vtkSmartPointer<vtkTextActor>::New();
  textFluxSolTitleActor->GetTextProperty()->SetFontSize ( 24 );
  ren->AddActor2D ( textFluxSolTitleActor );
  textFluxSolTitleActor->SetInput ( "FluxSol" );
  //textActor->SetPosition2 ( 1., 0.);
  //textActor->SetPosition ( 400., 400.);
   double xpos,ypos;

  //GetRenderWindow()->GetSize()
  cout << "xsize: "<<this->ui->qvtkWidget->GetRenderWindow()->GetSize()[0]<<endl;
  cout << "ysize: "<<this->ui->qvtkWidget->GetRenderWindow()->GetSize()[1]<<endl;

    double xsize=(double)(this->ui->qvtkWidget->GetRenderWindow()->GetSize()[0]);
    xpos=xsize*0.95;
  cout << "xsize" << xsize << "xpos" <<xpos<<endl;
 textFluxSolTitleActor->SetPosition ( xpos, 200);
//  textActor->GetTextProperty()->SetColor ( 0.08,0.0,0.4 );
//  textActor->SetInput ( "v0.0.1" );





   // Add Actor to renderer

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

    vtkSmartPointer<vtkCallbackCommand> m_pModifiedCallback =
    vtkSmartPointer<vtkCallbackCommand>::New();
  m_pModifiedCallback->SetCallback (WindowModifiedCallback);
  //m_pModifiedCallback->SetClientData(this);

  vtkSmartPointer<vtkCallbackCommand> modifiedCallback =
    vtkSmartPointer<vtkCallbackCommand>::New();
  modifiedCallback->SetCallback (CameraModifiedCallback);

  //renderWindow->AddObserver(vtkCommand::ModifiedEvent,m_pModifiedCallback);
  ren->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent,modifiedCallback);

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

    vtkSmartPointer<vtkWindowsModInteractionCallback> callbackwinmod =
    vtkSmartPointer<vtkWindowsModInteractionCallback>::New();

	callback->SetRenderer(this->ren);

	//renderWindowInteractor->AddObserver( vtkCommand::MouseMoveEvent, callback );

	this->ui->qvtkWidget->GetRenderWindow()->AddObserver( vtkCommand::ModifiedEvent, callbackwinmod );

	//this->ui->qvtkWidget->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent,m_pModifiedCallback);

    callbackwinmod->SetRenderer(this->ren);
    callbackwinmod->SetTextActor(this->textFluxSolTitleActor);


    //Grid Selection

      this->gridselectionstyle =
    vtkSmartPointer<MouseInteractorStyle>::New();
  this->gridselectionstyle->SetDefaultRenderer(ren);
  //style->Data = triangleFilter->GetOutput();


  //renderWindowInteractor->SetInteractorStyle(style);

	//renderWindowInteractor->Start();

	//TO MODIFY
	modelcount=0;

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

    ImportMesh(fileName.toStdString());

}


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


    pdmapper->GetLookupTable()->SetRange(0,100.);
////////////////////////////////////////// COLOR BAR

  vtkSmartPointer<vtkScalarBarActor> scalarBar =
    vtkSmartPointer<vtkScalarBarActor>::New();

  scalarBar->SetTitle(pd->GetArrayName(0));
  scalarBar->SetNumberOfLabels(4);
  //scalarBar->GetLabelTextProperty()->SetFontSize(4);
  scalarBar->SetHeight(0.60);
  scalarBar->SetWidth(0.10);
  scalarBar->GetTitleTextProperty()->SetFontFamilyToArial();
  scalarBar->GetTitleTextProperty()->ShadowOff();
  scalarBar->GetTitleTextProperty()->ItalicOff();
  scalarBar->GetTitleTextProperty()->BoldOff();
  scalarBar->GetTitleTextProperty()->SetFontSize(1);
  scalarBar->GetTitleTextProperty()->SetColor ( 0.0,0.0,0.0 );

  scalarBar->GetLabelTextProperty()->SetFontFamilyToArial();
  scalarBar->GetLabelTextProperty()->ShadowOff();
  scalarBar->GetLabelTextProperty()->ItalicOff();
  scalarBar->GetLabelTextProperty()->BoldOff();
  scalarBar->GetLabelTextProperty()->SetFontSize(4);
  scalarBar->GetLabelTextProperty()->SetColor ( 0.0,0.0,0.0 );

    //scalarBar->GetPositionCoordinate()->SetValue(val1,val2);
    //scalarBar->GetPosition2Coordinate()->SetValue(20.,20.);
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
    scalarBar->SetNumberOfLabels(5);
  pdmapper->SetScalarRange(scalarRange);

        //Results Combo Box
        this->comboBox->addItem(pd->GetArrayName(0));

        this->ren->AddActor2D(scalarBar);



        //Mouse Selection //Test
        this->gridselectionstyle->Data = polydata;
        this->renderWindowInteractor->SetInteractorStyle(this->gridselectionstyle);



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


        cout << "Importing Input File: " << fileName.toStdString() <<" ..."<<endl;

        //TO Modify, at first is only a CFD Model
        GraphicCFDModel model(fileName.toStdString());


        cout << "Input file has been successfully imported."<<endl;

        //QTreeWidgetItem *itm = new QTreeWidgetItem(this->ui->ModelTree); //WITH THIS ARGS DOES NOT WORK
        QTreeWidgetItem *itm = new QTreeWidgetItem();
        itm->setText(0,name);
        itm->setText(1,Description);
        //this->ui->ModelTree->topLevelItem( 0 )->addChild(itm);

        //item->setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        //item->setCheckState(0, Qt::Unchecked);

        QList<QTreeWidgetItem*> items = this->ui->ModelTree->findItems("Models",Qt::MatchExactly);
        int num = items.count();
        //cout <<"Items found"<<num;

        addTreeChild(items[0], fileName, "Desc");

        this->ui->ModelTree->update();
        //items[0]->addChild(itm);

        items = this->ui->ModelTree->findItems("Model-1",Qt::MatchExactly);

        if (items.count()>0)
        addTreeChild(items[0],
                  "Hola2", "Desc");
        //items[0]->addChild(itm);

        //sitems[0]->insertChild(0,itm);
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

                    vtkSmartPointer<vtkGeometryFilter> geometryFilter =
            vtkSmartPointer<vtkGeometryFilter>::New();
//
            #if VTK_MAJOR_VERSION <= 5
              geometryFilter->SetInput(model.UGrid());
              //surfaceFilter->SetInput(ugrid);
            #else
              geometryFilter->SetInputData(model.UGrid());
//              //surfaceFilter->SetInputData(ugrid);
            #endif
              geometryFilter->Update();
//              //surfaceFilter->Update();
//
            vtkSmartPointer<vtkPolyData>
            polydata= geometryFilter ->GetOutput ();



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
      vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();

            actor->SetMapper(pdmapper);

          actor->GetProperty()->SetEdgeColor(0, 0, 0);
          actor->GetProperty()->EdgeVisibilityOn();

      this->ren->AddActor( actor);

      //Add Mesh to Tree
      AddMeshToTree(model.Mesh());

      model.SolveIter();
      ui->MsgWin->AddString(model.ItLog());


      //this->vmodel.push_back(new GraphicCFDModel(model));
      this->vmodel.push_back(new GraphicCFDModel(fileName.toStdString()));
      this->vmodel[0]->SolveIter();
      //ui->MsgWin->AddString(this->vmodel[0]->ItLog());

      //vjob.push_back(new Job(*vmodel[0]) );
      vjobsubmitdialog.push_back(new JobSubmitDialog (*this->vmodel[0] , this)); //Model and parent
      vjobsubmitdialog[0]->exec();
      //vjobsubmitdialog[0]->show();


//        jobthread.push_back(new JobThread(*vmodel[0]) );
//        //jobthread[0]->AddMsgWin(*vjobsubmitdialog[0]->ui->ResidualMsg);
//
//        jobthread[0]->WorkerT().AddMsgWin(*vjobsubmitdialog[0]->ui->ResidualMsg);
//      vjobsubmitdialog[0]->AddThread(*jobthread[0]);
//
//
//      vjobsubmitdialog[0]->show();
//
//    connect(jobthread[0]->Thread(), SIGNAL(started()), vjobsubmitdialog[0], SLOT(AddString("Hi  ")));
//    connect(&jobthread[0]->WorkerT(), SIGNAL(AddMsg(string)), vjobsubmitdialog[0], SLOT(AddString(string)),Qt::QueuedConnection);
//    emit jobthread[0]->WorkerT().AddMsg("TEST...");
//    QCoreApplication::processEvents();
        //vjob[0]->MsgWinAddress(vjobsubmitdialog[0]->ResMsgWindow());
        //vjob[0]->LinePlotAddress(vjobsubmitdialog[0]->LinePlot());
      //vjob[0]->start();



//
//      reader->SetFileName(fileName.toStdString().c_str());
//      reader->Update();
//
//        vtkSmartPointer<vtkUnstructuredGrid> ugrid=
//        vtkUnstructuredGrid::New();;
//        ugrid = reader->GetOutput();
//
//      double scalarRange[2];

//
//      int components =
//        this->PointData->GetScalars()->GetNumberOfComponents();
//        double tuple[3];
//        double* tuple = pd->GetArray(0)->GetTuple( 1 );
//        pd->GetArray(0)->GetTuple( 1 ,tuple);
//        pd->GetArray(0)->GetRange(scalarRange);



//      std::cout << pd->GetArrayName(0)<<std::endl;

    //GeometryFilter


//      vtkSmartPointer<vtkGeometryFilter> geometryFilter =
//        vtkSmartPointer<vtkGeometryFilter>::New();
//
//        vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
//        vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
//
//      surfaceFilter->SetInputData(model.UGrid());
//
//    #if VTK_MAJOR_VERSION <= 5
//      geometryFilter->SetInput(model.UGrid());
//      surfaceFilter->SetInput(model.UGrid());
//    #else
//      geometryFilter->SetInputData(model.UGrid());
//      surfaceFilter->SetInputData(model.UGrid());
//    #endif
//      geometryFilter->Update();
//      surfaceFilter->Update();
//
//    vtkPolyData *polydata= geometryFilter ->GetOutput ();



//**************************** RANGE AND COLORS ******************************
//   Generate the colors for each point based on the color map
//  vtkSmartPointer<vtkUnsignedCharArray> colors =
//    vtkSmartPointer<vtkUnsignedCharArray>::New();
//  colors->SetNumberOfComponents(3);
//  colors->SetName("Colors");
//
//    for(int i = 0; i < polydata->GetNumberOfPoints(); i++)
//    {
//        unsigned char color[3];
//        for(unsigned int j = 0; j < 3; j++)
//          //color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
//          color [j]=255.;
//        colors->InsertNextTupleValue(color);
//    }
//
//	polydata->GetPointData()->SetScalars(colors);
//
////    //************************************* MAPPER
//
//     vtkSmartPointer<vtkPolyDataMapper> pdmapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
////
////    //pdmapper->ScalarVisibilityOff();
////
//    #if VTK_MAJOR_VERSION <= 5
//      pdmapper->SetInputConnection(polydata->GetProducerPort());
//    #else
//     pdmapper->SetInputData(polydata);
//    #endif
//
//
//
//
//
//	/////// END OF CONTOUR
//
//  ///////////////////////// RENDERING ////
//
//        VTK_CREATE(vtkActor, actor);
//        actor->SetMapper(pdmapper);
//        actor->GetProperty()->EdgeVisibilityOn();
//        this->ren->AddActor(actor);
//
//        this->ren->AddActor(this->_vtkOriginAxes);

        //this->models.push_back(GraphicCFDModel(model));

    }//If not filename Empty
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
