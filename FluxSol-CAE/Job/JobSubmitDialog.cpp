#include "JobSubmitDialog.h"


JobSubmitDialog::JobSubmitDialog(Job &job_, QWidget *parent)
:QDialog(parent)
{

  this->ui = new Ui_JobSubmitDialog;
  this->ui->setupUi(this);

    //job=&job_;

    ren =
    vtkSmartPointer<vtkOpenGLRenderer>::New();


    // WINDOW AND INTERACTOR
  renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();

  //renderWindow->AddRenderer(ren);


  ren->SetBackground(.2, .3, .4);

  ren->GradientBackgroundOn();
  //ren->SetBackground(0.6,0.7,1.);
  ren->SetBackground(1.,1.,1.);
  ren->SetBackground2(0,0,1);

    renderWindow->Render();	//If i want to obtain coordinates must to activate renderwindows with Render()

    // VTK/Qt wedded
    this->ui->qvtkResChart->GetRenderWindow()->AddRenderer(ren);	//Add

    //This is like the example Four Pane Viewer
    renderWindowInteractor = this->ui->qvtkResChart->GetInteractor();

    connect(ui->StartStopButton, SIGNAL(clicked()),this, SLOT(StartStopJob()));


    InitResChart();
    DrawResChart();
}

void JobSubmitDialog::StartStopJob()
{
    //Or if job->IsStopped()
    if (thread->Thread()->isRunning())
    {
        //job->stop();
        thread->Stop(); //This not only sopt thread but also finishes loop
        ui->StartStopButton->setText(tr("Start"));
    }
    else
    {
        ui->ResidualMsg->AddString("Job Submitted...\n");
        thread->Thread()->start();
        //job->start();
        ui->StartStopButton->setText(tr("Stop"));
        cout << "Stop Cout"<<endl;
    }

}


void JobSubmitDialog::InitResChart()
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

  // Set up the view
  view =
    vtkSmartPointer<vtkContextView>::New();

  // Add multiple line plots, setting the colors etc
 chart =
    vtkSmartPointer<vtkChartXY>::New();

    view->GetScene()->AddItem(chart);

    line = chart->AddPlot(vtkChart::LINE);


    //      TEST; THIS IS NOT ART OF THE EXAMPLE
    //view->SetRenderer(ren);

  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);


  //view->GetRenderWindow()->SetMultiSamples(0);



  //view->GetRenderWindow()->Render();
  //view->GetInteractor()->GetRenderWindow()->Render();

 //   uisubmitdialog.qvtkResChart->GetRenderWindow()->AddRenderer(view->GetRenderer());

  //This in tjeory works
    //THIS LINE IS OK FOR SHOW ONLY WITHOUR INTERACTION

    //ui->qvtkResChart->GetRenderWindow()->AddRenderer(view->GetRenderer());



    //This is EXPLAINED
    view->SetInteractor(renderWindowInteractor);
    ui->qvtkResChart->SetRenderWindow(view->GetRenderWindow());



    //THIS WAS THE ORIGINAL VTK EXAMPLE FILE
    //DOES NOT WORK IN QVTKWIDGET
    // Start interactor
    //  view->GetInteractor()->Initialize();
    //  view->GetInteractor()->Start();



  //Mode with actor
    //vtkSmartPointer <vtkAxesActor> actor_grid = vtkSmartPointer::New();
    //VTK_CREATE(vtkActor, actor_grid);
	//actor_grid->GetScene()->AddItem(chart);
	//ren->AddActor(actor_grid);
}

// EXTRACTED FROM VTK CHART 2D EXAMPLE
void JobSubmitDialog::DrawResChart()
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
