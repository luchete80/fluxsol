#include "JobSubmitDialog.h"
#include "JobWorker.h"
#include <vector>

// WORKERS BELONGS TO THIS CLASS

using namespace std;

#include<QMetaType>
typedef std::vector <double> MyArray;

// ...


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

    // You should call this function before any (queued)
    // signal/slot connection involving the type

    //qRegisterMetaType<std::vector>();

    resworker->moveToThread(qvtkResChart->Thread());
    //resworker->moveToThread(thread);
    QObject::connect (resworker, SIGNAL(DrawTest()), qvtkResChart, SLOT(DrawAlt()));
    QObject::connect (resworker, SIGNAL(DrawTest2()), qvtkResChart, SLOT(Draw()));
    //QObject::connect (resworker, SIGNAL(AddRes(const std::vector<double> &)), qvtkResChart, SLOT(AddRes(const std::vector<double> &)));
    qRegisterMetaType<MyArray>("MyArray");
    QObject::connect (resworker, SIGNAL(AddRes(const MyArray &)), qvtkResChart, SLOT(AddRes(const MyArray &)));
    QObject::connect (resworker, SIGNAL(AddRes(const double &x, const double &y, const double &z)), qvtkResChart, SLOT(AddRes(const double &x, const double &y, const double &z)));
    //QObject::connect (qvtkResChart->Thread(), SIGNAL(started()), qvtkResChart, SLOT(DrawAlt()));



//
//
//    //connect(worker, SIGNAL(AddMsg(string)), this, SLOT(AddString(string)),Qt::QueuedConnection);
    connect(worker, SIGNAL(&Worker::AddMsg(QString)), this, SLOT(&JobSubmitDialog::AddString(QString)));
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
    //qvtkResChart->Draw();

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

        //qvtkResChart->Thread()->start();

        thread->start();    //THIS IS THE IMPORTANT
                            //Is connected with Solve()

        StartStopButton->setText(tr("Stop"));
        //jobthread->start();
        //thread->WorkerT().Solve();
        //cout << "COUT"<<endl;

    }

}


