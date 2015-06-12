#include "JobWorker.h"

void Worker::Solve()
{

    //cout << "Before while..."<<endl;
    while (!stopped)
    {
        //cout << "inside while"<<endl;
        model->SolveIter();
        //msgwin->AddString(model->ItLog());
        //msgwin->AddString("Hola\n");
        //cout << "iter: " <<iter<<endl;
        //cout << model->ItLog()<<endl;
        //msgwin->append(QString::fromStdString(model->ItLog()));
        //line->SetColor(0, 0, 0, 255);
        //workerThread.msleep(10);
        //emit DrawChart();
        emit AddMsg(model->ItLog());
        //emit statusChanged( model->ItLog());
        iter++;
        if (iter>500)
            stopped=true;
    }
}
