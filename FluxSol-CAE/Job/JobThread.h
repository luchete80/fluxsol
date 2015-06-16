#ifndef _JOBTHREAD_H_
#define _JOBTHREAD_H_

//#include "JobClasses.h"
#include "JobWorker.h"
#include "MsgWindow.h"

class JobThread : public QObject
{
    Q_OBJECT
    QThread workerThread;
    //Job *job;
    Worker *worker;
    MsgWindow *msgwin;

    //JobSubmitDialog *jobdialog;

public:
    JobThread() {

        //Worker *worker = new JobWorker;    //Originally, now is not static
        worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, SIGNAL(started()), worker, SLOT(Solve()));
        connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(this, SIGNAL(operate(QString)), worker, SLOT(doWork(QString)));
        //connect(worker, SIGNAL(AddMsg(string)), &workerThread, SLOT(msgwin->AddString(string)));
        //connect(worker, SIGNAL(resultReady(QString)), this, SLOT(handleResults(QString)));
        //connect(worker, SIGNAL(started()), Worker, SLOT(Solve()));

        //connect( worker, SIGNAL(start()), Worker, jobdialog->StartStopJob() );

        //make sure you use Qt::QueuedConnection connection
    }

    JobThread(const CFDModel &cfdmodel)
    //:JobThread()
    {
        //job=new Job(cfdmodel);
        //model=&cfdmodel;

        worker=new Worker(cfdmodel);


        connect(&workerThread, SIGNAL(started()), worker, SLOT(Solve()));
        connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(this, SIGNAL(operate(QString)), worker, SLOT(doWork(QString)));
        //connect(worker, SIGNAL(AddMsg(string)), &workerThread, SLOT(msgwin->AddString(string)));
        //connect(worker, SIGNAL(AddMsg(string)), worker, SLOT(AddString(string)));
        worker->moveToThread(&workerThread);
    }

    void AddMsgWin(MsgWindow &mw){msgwin=&mw;}

    QThread* Thread(){return &workerThread;}

    ~JobThread() {
        workerThread.quit();
        workerThread.wait();
    }

    Worker & WorkerT(){return *worker;}


    void Stop()
    {
        worker->Stop(); //Stops Worker Iter while loop
        //workerThread.exit();    //Or exit
    }

public slots:
    void handleResults(const QString &){};
    void AddString(const string &str){msgwin->AddString(str);}


signals:
    void operate(const QString &);

    //void Solve(){worker->Solve()}
};


#endif
