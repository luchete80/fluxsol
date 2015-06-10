#include "Job.h"

Job::Job()
{
    stopped=false;

    submitdialog=new QDialog();
    this->uisubmitdialog.setupUi(submitdialog);

    ren =
    vtkSmartPointer<vtkOpenGLRenderer>::New();
    //vtkSmartPointer<vtkRenderer>::New();
//#endif


    // WINDOW AND INTERACTOR
  renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();

  //renderWindow->AddRenderer(ren);


  ren->SetBackground(.2, .3, .4);

  ren->GradientBackgroundOn();
  //ren->SetBackground(0.6,0.7,1.);
  ren->SetBackground(1.,1.,1.);
  ren->SetBackground2(0,0,1);

DrawResChart();

    renderWindow->Render();	//If i want to obtain coordinates must to activate renderwindows with Render()

    // VTK/Qt wedded
    //this->uisubmitdialog.qvtkResChart->GetRenderWindow()->AddRenderer(ren);	//Add

    //This is like the example Four Pane Viewer
    renderWindowInteractor = uisubmitdialog.qvtkResChart->GetInteractor();

    DrawResChart();

    submitdialog->show();
}


void Job::run()
{
    while (!stopped)
    {

    }
    stopped=false;
}

void Job::stop()
{
    stopped=true;
}


// EXTRACTED FROM VTK CHART 2D EXAMPLE
void Job::DrawResChart()
{
  // Create a table with some points in it
  vtkSmartPointer<vtkTable> table =
    vtkSmartPointer<vtkTable>::New();

  vtkSmartPointer<vtkFloatArray> arrX =
    vtkSmartPointer<vtkFloatArray>::New();
  arrX->SetName("X Axis");
  table->AddColumn(arrX);

  vtkSmartPointer<vtkFloatArray> arrC =
    vtkSmartPointer<vtkFloatArray>::New();
  arrC->SetName("Cosine");
  table->AddColumn(arrC);

  vtkSmartPointer<vtkFloatArray> arrS =
    vtkSmartPointer<vtkFloatArray>::New();
  arrS->SetName("Sine");
  table->AddColumn(arrS);

  // Fill in the table with some example values
  int numPoints = 69;
  float inc = 7.5 / (numPoints-1);
  table->SetNumberOfRows(numPoints);
  for (int i = 0; i < numPoints; ++i)
  {
    table->SetValue(i, 0, i * inc);
    table->SetValue(i, 1, cos(i * inc));
    table->SetValue(i, 2, sin(i * inc));
  }

  // Set up the view
  vtkSmartPointer<vtkContextView> view =
    vtkSmartPointer<vtkContextView>::New();

//      TEST; THIS IS NOT ART OF THE EXAMPLE
    //view->SetRenderer(ren);

  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);

  // Add multiple line plots, setting the colors etc
  vtkSmartPointer<vtkChartXY> chart =
    vtkSmartPointer<vtkChartXY>::New();
  view->GetScene()->AddItem(chart);
  vtkPlot *line = chart->AddPlot(vtkChart::LINE);
#if VTK_MAJOR_VERSION <= 5
  line->SetInput(table, 0, 1);
#else
  line->SetInputData(table, 0, 1);
#endif
  line->SetColor(0, 255, 0, 255);
  line->SetWidth(1.0);
  line = chart->AddPlot(vtkChart::LINE);
#if VTK_MAJOR_VERSION <= 5
  line->SetInput(table, 0, 2);
#else
  line->SetInputData(table, 0, 2);
#endif
  line->SetColor(255, 0, 0, 255);
  line->SetWidth(5.0);

  // For dotted line, the line type can be from 2 to 5 for different dash/dot
  // patterns (see enum in vtkPen containing DASH_LINE, value 2):
#ifndef WIN32
  line->GetPen()->SetLineType(vtkPen::DASH_LINE);
#endif
  // (ifdef-ed out on Windows because DASH_LINE does not work on Windows
  //  machines with built-in Intel HD graphics card...)

  //view->GetRenderWindow()->SetMultiSamples(0);



  //view->GetRenderWindow()->Render();
  //view->GetInteractor()->GetRenderWindow()->Render();

 //   uisubmitdialog.qvtkResChart->GetRenderWindow()->AddRenderer(view->GetRenderer());

  //This in tjeory works
    //THIS LINE IS OK FOR SHOW ONLY WITHOUR INTERACTION
    uisubmitdialog.qvtkResChart->GetRenderWindow()->AddRenderer(view->GetRenderer());

    //This is EXPLAINED
    //view->SetInteractor(renderWindowInteractor);
    //uisubmitdialog.qvtkResChart->SetRenderWindow(view->GetRenderWindow());



    //THIS WAS THE ORIGINAL VTK EXAMPLE FILE
  // Start interactor
//  view->GetInteractor()->Initialize();
//  view->GetInteractor()->Start();

  //return EXIT_SUCCESS;
}
