#include "JobThread.h"

void JobThread::run()
{
    emit AddMsg("Solving JobThread\n");
    while (!stopped)
    {
        model->SolveIter();

        //model_itlog+="Iter \n";
        //cout << "Test"<<endl;
        emit AddMsg(model->ItLog());
        //cout << "Test"<<endl;
        //QCoreApplication::processEvents();
        //emit statusChanged( model->ItLog());
        string test="test\n";
        emit ChgButton(test);
        iter++;
        if (iter>500)
            {   stopped=true;
                emit AddMsg("Process terminated.\n");
            }
    }

    emit this->AddMsg("Job Stopped...\n");

}
