#ifndef _JOB_H_
#define _JOB_H_

//#include "Model.h"
#include <QtCore>
#include "Model.h"

#include "MsgWindow.h"  //To notify

//Job main architecture
//See from C++Gui Programming with Qt Page 381




class Job
:
    public QThread
{

    Q_OBJECT

    private:
    QString messageStr;
    volatile bool stopped; //because it is accessed from different
//                            //threads and we want to be sure that it is freshly read every time it is needed.
//                            //If we omitted the volatile keyword, the compiler might optimize access to the
//                            //variable, possibly leading to incorrect results.

    protected:

    int iter;

	CFDModel *model;	//INCLUDES INHERITED CFDMODELS, SIMPLE, THERMAL, AND SO ON
    void run();         //REIMPLEMENTED
    MsgWindow *msgwin;  //Write Notifications


	public:
		Job();
		Job(const CFDModel &cfdmodel);
		void setMessage(const QString &message);
        void stop();
		//Model * Model(){return model;}
		//SolveIter(){model->SolveIter();}

		MsgWinAddress(MsgWindow &msgwin_){msgwin=&msgwin_;}


};

#endif
