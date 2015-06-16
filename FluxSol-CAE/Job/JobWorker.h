#ifndef _JOBWORKER_H
#define _JOBWORKER_H

#include <QtCore>
//#include "Job.h"
//#include "JobClasses.h"
#include "Model.h"
#include "MsgWindow.h"
#include "ui_JobSubmitDialog.h"

//#include "JobSubmitDialog.h"


//From http://doc.qt.io/qt-4.8/qthread.html

class Worker : public QObject
{
    Q_OBJECT
    QThread workerThread;
    CFDModel *model;

    Ui_JobSubmitDialog *ui;

    bool stopped;
    int iter;
    MsgWindow *msgwin;
    string model_itlog;

public slots:
    void doWork(const QString &parameter) {
        // ...
        QString result="Hi...";
        emit resultReady(result);
    }


    void AddString(const string &str){msgwin->AddString(str);}

    void Solve();
    void Stop();
    void AddText(const QString &line)
    {
        //emit statusChanged( line.str().c_str() );
    }

    public:

    void AddMsgWin(MsgWindow &mw){msgwin=&mw;}
    void AddUi(Ui_JobSubmitDialog &ui_){ui=&ui_;}

    Worker()
    {

    }
    Worker(const CFDModel &mod)
    {
        iter=0;
        model=&mod;
        stopped=false;
        model_itlog="";
    }

    signals:
    void resultReady(const QString &result);
    void DrawChart();
    void AddMsg(const string &str);
    void statusChanged( const string &str);
};


#endif
