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

    connect(StartStopButton, SIGNAL(clicked()),this, SLOT(StartStopJob()));

    thread=new QThread();
    worker=new Worker(model_);


    // RESIDUAL THREADS AND WORKER
    resworker = new ResWidgetWorker(*qvtkResChart);  //MUST BW CONSTRUCTED WITH MODEL TO GET RESIDUALS FROM IT
    worker->AddResWorker(*resworker);

    resworker->moveToThread(qvtkResChart->Thread());
    //resworker->moveToThread(thread);
    QObject::connect (resworker, SIGNAL(DrawTest()), qvtkResChart, SLOT(DrawAlt()));
    QObject::connect (resworker, SIGNAL(DrawTest2()), qvtkResChart, SLOT(Draw()));
    QObject::connect (resworker, SIGNAL(AddRes(vector <double>&)), qvtkResChart, SLOT(AddRes(vector <double>&)));

    //QObject::connect (qvtkResChart->Thread(), SIGNAL(started()), qvtkResChart, SLOT(DrawAlt()));



//
//
//    //connect(worker, SIGNAL(AddMsg(string)), this, SLOT(AddString(string)),Qt::QueuedConnection);
    connect(worker, SIGNAL(&Worker::AddMsg(string)), this, SLOT(&JobSubmitDialog::AddString(string)));
    //connect(worker, SIGNAL(DrawChart()), this, SLOT(DrawResChartAlt()));

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

    //qvtkResChart->exec();

    //emit resworker->DrawTest();
    //QCoreApplication::processEvents();


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
        ResidualMsg->AddString("Job Submitted...\n");

        qvtkResChart->Thread()->start();

        thread->start();

        //jobthread->start();
        //thread->WorkerT().Solve();
        cout << "COUT"<<endl;

    }

}


