#ifndef _JOBWORKER_H
#define _JOBWORKER_H

#include <QtCore>
//#include "Job.h"
#include "Model.h"
#include "MsgWindow.h"

//From http://doc.qt.io/qt-4.8/qthread.html

class Worker : public QObject
{
    Q_OBJECT
    QThread workerThread;
    CFDModel *model;

    bool stopped;
    int iter;

public slots:
    void doWork(const QString &parameter) {
        // ...
        QString result="Hi...";
        emit resultReady(result);
    }

    void Solve();
    void AddText(const QString &line)
    {
        //emit statusChanged( line.str().c_str() );
    }

    public:

    Worker()
    {

    }
    Worker(const CFDModel &mod)
    {
        iter=0;
        model=&mod;
        stopped=false;
    }

    signals:
    void resultReady(const QString &result);
    void DrawChart();
    void AddMsg(const string &str);
    void statusChanged( const string &str);
};


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
        connect(worker, SIGNAL(AddMsg(string)), worker, SLOT(msgwin->AddString(string)));
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

        worker->moveToThread(&workerThread);
        connect(&workerThread, SIGNAL(started()), worker, SLOT(Solve()));
        connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(this, SIGNAL(operate(QString)), worker, SLOT(doWork(QString)));
    }
    AddMsgWin(const MsgWindow &mw){msgwin=&mw;}

    QThread* Thread(){return &workerThread;}

    ~JobThread() {
        workerThread.quit();
        workerThread.wait();
    }

public slots:
    void handleResults(const QString &){};


signals:
    void operate(const QString &);

    //void Solve(){worker->Solve()}
};

#endif
