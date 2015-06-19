#include "JobWorker.h"

void Worker::Solve()
{
    vector <double> ures;
    //cout << "Before while..."<<endl;
    emit AddMsg("Hi...\n");
    //emit resworker->DrawTest();

    while (!stopped)
    {
        //cout << "inside while"<<endl;
        model->SolveIter();

        ures=model->U_Eqn().GlobalRes();

        if (iter>2) emit resworker->DrawTest2();
        //if (iter>2) emit resworker->AddRes(ures);

        //msgwin->append(QString::fromStdString(model->ItLog()));
        //msgwin->append(QString::fromStdString( model_itlog ) );

        string str="test\n";
        //emit ChgButton(str);
        //QCoreApplication::processEvents();
        model_itlog+="Iter \n";

        //IF RESWORKER THREAD IS RUNNING
        //if (iter==100) emit resworker->DrawTest();
        //if (iter==200) emit resworker->DrawTest2();

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
