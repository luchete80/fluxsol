#include "JobWorker.h"

void Worker::Solve()
{

    //cout << "Before while..."<<endl;
    emit AddMsg("Hi...\n");
    while (!stopped)
    {
        //cout << "inside while"<<endl;
        model->SolveIter();

        if (iter==100)  emit DrawChart();
        //msgwin->AddString(model->ItLog());
        //msgwin->AddString("Hola\n");
        //cout << "iter: " <<iter<<endl;
        //cout << model->ItLog()<<endl;
        //model_itlog+=model->ItLog();
        //msgwin->append(QString::fromStdString(model->ItLog()));
        //msgwin->append(QString::fromStdString( model_itlog ) );
        //line->SetColor(0, 0, 0, 255);
        //workerThread.msleep(10);
        //emit DrawChart();
        //emit AddMsg(model->ItLog());
        string str="test\n";
        emit ChgButton(str);
        QCoreApplication::processEvents();
        model_itlog+="Iter \n";
        //cout << "Test"<<endl;
        //cout << model->ItLog() <<endl;
        //emit AdTdMsg(model->ItLog());
        emit AddMsg(model_itlog);
        //QCoreApplication::processEvents();
        //emit statusChanged( model->ItLog());
        iter++;
        if (iter>500)
            {   stopped=true;
                emit AddMsg("Process terminated.\n");
            }
    }

    emit this->AddMsg("Job Stopped...\n");

}

void Worker::Stop()
{
    if (!stopped)
        stopped=true;
}
