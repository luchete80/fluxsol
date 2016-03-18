#ifndef _JOBWORKER_H
#define _JOBWORKER_H

#include <QtCore>
#include <string>
//#include "Job.h"
//#include "JobClasses.h"
#include "Model.h"
#include "MsgWindow.h"
#include "ui_JobSubmitDialog.h"

#include "ResWidgetWorker.h"        //To Draw residuals
#include <QString>
//#include "JobSubmitDialog.h"

using namespace std;

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
    std::string model_itlog;

    ResWidgetWorker *resworker;

public slots:
    void doWork(const QString &parameter) {
        // ...
//        QString result="Hi...";
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
    void AddResWorker(const ResWidgetWorker &rw){resworker=&rw;}

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
    void ChgButton(const string &str);
    void AddMsg(const QString &str);
    void statusChanged( const string &str);
};


#endif
