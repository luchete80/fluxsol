#include "QVTKResWidget.h"

QVTKResWidget::QVTKResWidget()
:QVTKWidget()
{

    InitVTK();
    InitChart();
    thread = new QThread();
}
QVTKResWidget::QVTKResWidget(QWidget* parent)
:QVTKWidget(parent)
{

    InitVTK();
    InitChart();
    thread = new QThread();

}



void QVTKResWidget::InitVTK()
{



    //////////////////////////////////////////////////
    //INIT VIEW STUFF
    //THIS WAS FORMERLY IN JOBDIALOG
        ren =
    vtkSmartPointer<vtkOpenGLRenderer>::New();


    // WINDOW AND INTERACTOR
  renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();

  renderWindow->AddRenderer(ren);


  ren->SetBackground(.2, .3, .4);

  ren->GradientBackgroundOn();
  //ren->SetBackground(0.6,0.7,1.);
  ren->SetBackground(1.,1.,1.);
  ren->SetBackground2(0,0,1);

    //renderWindow->Render();	//If i want to obtain coordinates must to activate renderwindows with Render()

    // VTK/Qt wedded
    GetRenderWindow()->AddRenderer(ren);	//Add

    //GetRenderWindow()->Render();

    //This is like the example Four Pane Viewer
    renderWindowInteractor = GetInteractor();

    ///////////////////////////////////////////////





    /////////////////////////////////////////////////////////////////
    //// THIS WAS IN INIT()

    //THIS WAS THE ORIGINAL VTK EXAMPLE FILE
    //DOES NOT WORK IN QVTKWIDGET
    // Start interactor
    //  view->GetInteractor()->Initialize();
    //  view->GetInteractor()->Start();



  //Mode with actor
//    vtkSmartPointer <vtkAxesActor> actor_grid = vtkSmartPointer::New();
//    VTK_CREATE(vtkActor, actor_grid);
//	actor_grid->GetScene()->AddItem(chart);
//	ren->AddActor(actor_grid);




  //view->GetRenderWindow()->SetMultiSamples(0);



  //view->GetRenderWindow()->Render();
  //view->GetInteractor()->GetRenderWindow()->Render();

 //   uisubmitdialog.qvtkResChart->GetRenderWindow()->AddRenderer(view->GetRenderer());

  //This in tjeory works
    //THIS LINE IS OK FOR SHOW ONLY WITHOUR INTERACTION

    //qvtkResChart->GetRenderWindow()->AddRenderer(view->GetRenderer());

      // Set up the view
      view =
        vtkSmartPointer<vtkContextView>::New();

    //This is EXPLAINED
    view->SetInteractor(renderWindowInteractor);
    SetRenderWindow(view->GetRenderWindow());

    //SetRenderWindow(GetRenderWindow());
    //GetRenderWindow()->Render();

    //view->GetRenderWindow()->Render();

}

void QVTKResWidget::InitChart()
{



  // Create a table with some points in it
  table =
    vtkSmartPointer<vtkTable>::New();

  arrX =
    vtkSmartPointer<vtkFloatArray>::New();
  arrX->SetName("X Axis");
  table->AddColumn(arrX);

  arrC =
    vtkSmartPointer<vtkFloatArray>::New();
  arrC->SetName("Cosine");
  table->AddColumn(arrC);

  arrS =
    vtkSmartPointer<vtkFloatArray>::New();
  arrS->SetName("Sine");
  table->AddColumn(arrS);


  // Add multiple line plots, setting the colors etc
 chart =
    vtkSmartPointer<vtkChartXY>::New();

    view->GetScene()->AddItem(chart);

    line = chart->AddPlot(vtkChart::LINE);


    //      TEST; THIS IS NOT ART OF THE EXAMPLE
    //view->SetRenderer(ren);

    //view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
}


// EXTRACTED FROM VTK CHART 2D EXAMPLE
void QVTKResWidget::Draw()
{

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

}

void QVTKResWidget::DrawAlt()
{

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





#if VTK_MAJOR_VERSION <= 5
  line->SetInput(table, 0, 1);
#else
  line->SetInputData(table, 0, 1);
#endif
  line->SetColor(0, 0, 255, 255);
  line->SetWidth(1.0);
  line = chart->AddPlot(vtkChart::LINE);
#if VTK_MAJOR_VERSION <= 5
  line->SetInput(table, 0, 2);
#else
  line->SetInputData(table, 0, 2);
#endif
  line->SetColor(0, 255, 0, 255);
  line->SetWidth(7.0);

  // For dotted line, the line type can be from 2 to 5 for different dash/dot
  // patterns (see enum in vtkPen containing DASH_LINE, value 2):
#ifndef WIN32
  line->GetPen()->SetLineType(vtkPen::DASH_LINE);
#endif
  // (ifdef-ed out on Windows because DASH_LINE does not work on Windows
  //  machines with built-in Intel HD graphics card...)

  //view->GetRenderWindow()->Render();

}


