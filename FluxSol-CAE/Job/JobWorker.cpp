#include "JobWorker.h"

void Worker::Resume()
{
    stopped=false;
    sync.lock();
    pause = false;
    sync.unlock();
    pauseCond.wakeAll();

    this->Solve();
}


//THIS IS THE FUNCTION RELATED TO QThread
void Worker::Solve()
{
    vector <double> ures;
    //cout << "Before while..."<<endl;
//    emit AddMsg("Hi...\n");
    //emit resworker->DrawTest();

    while (!stopped)
    {

         sync.lock();
         if(pause)
        {
             pauseCond.wait(&sync); // in this place, your thread will stop to execute until someone calls resume
             cout << "job Paused"<<endl;
        }
         sync.unlock();

        //cout << "inside while"<<endl;
        model->SolveIter();

        ures=model->U_Eqn().GlobalRes();

        if (iter>2) emit resworker->AddRes(ures);
        if (iter>2) emit resworker->DrawTest2();
        //if (iter>2) emit resworker->AddRes(ures[0],ures[1],ures[2]);

        //msgwin->append(QString::fromStdString(model->ItLog()));
        //msgwin->append(QString::fromStdString( model_itlog ) );


        //QString str=ures.outstr();
        //QString str=model_itlog;
        //emit AddMsg(str);
        //emit ChgButton(str);
        //QCoreApplication::processEvents();
        model_itlog+="Iter \n";

        //IF RESWORKER THREAD IS RUNNING
        //TEST
        //if (iter==100) emit resworker->DrawTest();
        //if (iter==200) emit resworker->DrawTest2();

        QCoreApplication::processEvents();      //Not necessary
        iter++;
        if (iter>500)
            {   stopped=true;
                //emit AddMsg("Process terminated.\n");
            }
    }

    //emit this->AddMsg("Job Stopped...\n");

}

void Worker::Stop()
{
    //emit AddMsg("Stopping Process...\n");
    if (!stopped)
        stopped=true;
}
