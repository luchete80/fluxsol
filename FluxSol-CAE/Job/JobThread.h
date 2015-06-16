#ifndef _JOBTHREAD_H_
#define _JOBTHREAD_H_

#include "Model.h"
#include <QThread>
#include <QtCore>

class JobThread : public QThread
{
    Q_OBJECT

	CFDModel *model;
	public:
    JobThread() {

    }

    JobThread(const CFDModel &cfdmodel)
    {
		model=&cfdmodel;
		stopped=false;
		iter=0;
        //worker=new Worker(cfdmodel);

    }



    ~JobThread() {

    }



    void Stop()
    {

    }

	protected:
	    bool stopped;
		int iter;

		void run() Q_DECL_OVERRIDE;

public slots:
    void handleResults(const QString &){};
//    void AddString(const string &str){msgwin->AddString(str);}


signals:
    void operate(const QString &);
    void AddMsg(const string &);
    void ChgButton(const string &str);

    //void Solve(){worker->Solve()}
};


#endif
