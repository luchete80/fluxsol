#include "JobSubmitDialog.h"
#include "JobWorker.h"


JobSubmitDialog::JobSubmitDialog(const CFDModel &model_,QWidget *parent)
:QDialog(parent)
{

  //this->ui = new Ui_JobSubmitDialog;
  this->setupUi(this);
  model=&model_;
  stopped=false;
  iter=0;

    //job=&job_;

//    ren =
//    vtkSmartPointer<vtkOpenGLRenderer>::New();
//
//
//    // WINDOW AND INTERACTOR
//  renderWindow =
//    vtkSmartPointer<vtkRenderWindow>::New();
//
//  //renderWindow->AddRenderer(ren);
//
//
//  ren->SetBackground(.2, .3, .4);
//
//  ren->GradientBackgroundOn();
//  //ren->SetBackground(0.6,0.7,1.);
//  ren->SetBackground(1.,1.,1.);
//  ren->SetBackground2(0,0,1);
//
//    renderWindow->Render();	//If i want to obtain coordinates must to activate renderwindows with Render()
//
//    // VTK/Qt wedded
//    this->qvtkResChart->GetRenderWindow()->AddRenderer(ren);	//Add
//
//    //This is like the example Four Pane Viewer
//    renderWindowInteractor = this->qvtkResChart->GetInteractor();

    connect(StartStopButton, SIGNAL(clicked()),this, SLOT(StartStopJob()));


//    InitResChart();
//    DrawResChartAlt();
//
//
    thread=new QThread();
    worker=new Worker(model_);


    // RESIDUAL THREADS AND WORKER
    resworker = new ResWidgetWorker(*qvtkResChart);  //MUST BW CONSTRUCTED WITH MODEL TO GET RESIDUALS FROM IT
    worker->AddResWorker(*resworker);

    resworker->moveToThread(qvtkResChart->Thread());
    QObject::connect (resworker, SIGNAL(DrawTest()), qvtkResChart, SLOT(DrawAlt()),Qt::QueuedConnection);
    QObject::connect (qvtkResChart->Thread(), SIGNAL(started()), qvtkResChart, SLOT(DrawAlt()));



//
//
//    //connect(worker, SIGNAL(AddMsg(string)), this, SLOT(AddString(string)),Qt::QueuedConnection);
    connect(worker, SIGNAL(&Worker::AddMsg(string)), this, SLOT(&JobSubmitDialog::AddString(string)));
    connect(worker, SIGNAL(DrawChart()), this, SLOT(DrawResChartAlt()));

    worker->moveToThread(thread);

    jobthread=new JobThread(model_);
    //QObject::connect (worker, SIGNAL(AddMsg(string)), this, SLOT(AddString(string)));

    QObject::connect (this->thread, SIGNAL(started()), this->worker, SLOT(Solve()));
    QObject::connect (this->worker, SIGNAL(ChgButton(string)), this, SLOT(ChangeStartStopButton(string)),Qt::QueuedConnection);
    QObject::connect (this->jobthread, SIGNAL(ChgButton(string)), this, SLOT(ChangeStartStopButton(string)));
    QObject::connect (this->jobthread, SIGNAL(AddMsg(string)), this, SLOT(&JobSubmitDialog::AddString(string)));

    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));

    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));


    qvtkResChart->InitVTK();
    qvtkResChart->InitChart();
    qvtkResChart->Draw();

    qvtkResChart->Thread()->start();
    emit resworker->DrawTest();


}

void JobSubmitDialog::StartStopJob()
{
    //Or if job->IsStopped()
    if (thread->isRunning())
    {
        //job->stop();
        //thread->Stop(); //This not only sopt thread but also finishes loop
        stopped=true;
        worker->Stop();
        thread->wait();
        StartStopButton->setText(tr("Start"));
    }
    else
    {
        //qvtkResChart->DrawAlt();
        //update();
        ResidualMsg->AddString("Job Submitted...\n");
        AddString("Job Submitted 2 ...\n");
        emit worker->AddMsg("Worker Msg...\n");
        emit jobthread->AddMsg("jobthread Msg...\n");

        emit jobthread->ChgButton("TEST\n");

        //connect(&thread->WorkerT(), SIGNAL(AddMsg(string)), this, SLOT(AddString(string)),Qt::QueuedConnection);

        //connect(&thread->WorkerT(), SIGNAL(AddMsg(string)), this, SLOT(AddString(string)));

        //emit thread->WorkerT().AddMsg("Test\n");
        qvtkResChart->Thread()->start();
        emit resworker->DrawTest();

        thread->start();
        //jobthread->start();
        //thread->WorkerT().Solve();
        cout << "COUT"<<endl;

        //QApplication::processEvents();


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

    //qvtkResChart->GetRenderWindow()->AddRenderer(view->GetRenderer());



    //This is EXPLAINED
    view->SetInteractor(renderWindowInteractor);
    qvtkResChart->SetRenderWindow(view->GetRenderWindow());



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

void JobSubmitDialog::DrawResChartAlt()
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

}
