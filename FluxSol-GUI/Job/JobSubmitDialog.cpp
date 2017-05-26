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
    connect(worker, SIGNAL(AddMsg(QString)), this, SLOT(AddString(QString)),Qt::QueuedConnection);
    //connect(worker, SIGNAL(DrawChart()), this, SLOT(DrawResChartAlt()));

    worker->moveToThread(thread);

    jobthread=new JobThread(model_);
    //QObject::connect (worker, SIGNAL(AddMsg(string)), this, SLOT(AddString(string)));


    //QObject::connect (this->thread, SIGNAL(started()), this->worker, SLOT(Solve()));
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

        //Thread can be paused
        if (this->worker->isPaused())
        {
            ResidualMsg->AddString("Job Resumed...\n");
            StartStopButton->setText(tr("Pause"));
            worker->Resume();
        }
        else
        {
            ResidualMsg->AddString("Job Paused...\n");
            StartStopButton->setText(tr("Start"));
            worker->Pause();
            stopped=true;
        }

        //job->stop();
        //thread->Stop(); //This not only sopt thread but also finishes loop
//        stopped=true;
        //worker->Stop();
    }
    else //Not running, starts or continues thread
    {
        stopped=false;
        ResidualMsg->AddString("Job Submitted...\n");
        StartStopButton->setText(tr("Stop"));
        //qvtkResChart->Thread()->start();

        thread->start();    //THIS IS THE IMPORTANT
                            //Is connected with Solve()
        this->worker->Solve();
//        thread->Worker().Solve();

        //StartStopButton->setText(tr("Stop"));
        //jobthread->start();
        //thread->WorkerT().Solve();
        //cout << "COUT"<<endl;

    }

}

// TO PAUSE / RESUME QTHREAD
// http://stackoverflow.com/questions/9075837/pause-and-resume-a-qthread

//Ok, so I suggest you make internal thread variable that will be checked in each step of your loop + QWaitCondition to resume it.
//
//Create pause method where you will switch on "pause field" (bool?), don't forget to synchronize it
//In your own loop use QWaitCondition (see Qt docs) to pause thread execution
//Create resume method where you will switch off "pause field" and wake QWaitCondition
//
//class MyWorker: public QThread
//{
//private:
//    QMutex sync;
//    QWaitCondition pauseCond;
//    bool pause;
//
//public:
//    MyWorker(...): pause(false) {}
//
//    void resume()
//    {
//        sync.lock();
//        pause = false;
//        sync.unlock();
//        pauseCond.wakeAll();
//    }
//
//    void pause()
//    {
//        sync.lock();
//        pause = true;
//        sync.unlock();
//    }
//
//protected:
//    void run()
//    {
//        while(someCondition) // gues it's your loop
//        {
//             sync.lock();
//             if(pause)
//                 pauseCond.wait(&sync); // in this place, your thread will stop to execute until someone calls resume
//             sync.unlock();
//
//             // do your operation
//        }
//    }
//};


