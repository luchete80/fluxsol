#include "JobWorker.h"

void Worker::Solve()
{

    //cout << "Before while..."<<endl;
    emit AddMsg("Hi...\n");
    while (!stopped)
    {
        //cout << "inside while"<<endl;
        model->SolveIter();

        if (iter==2)  emit DrawChart();

        //msgwin->append(QString::fromStdString(model->ItLog()));
        //msgwin->append(QString::fromStdString( model_itlog ) );

        string str="test\n";
        emit ChgButton(str);
        //QCoreApplication::processEvents();
        model_itlog+="Iter \n";

        //IF RESWORKER THREAD IS RUNNING
        if (iter==100) emit resworker->DrawTest();
        if (iter==200) emit resworker->DrawTest2();

        QCoreApplication::processEvents();      //Not necessary
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
