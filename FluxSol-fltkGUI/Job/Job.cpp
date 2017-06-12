#include "Job.h"

Job::Job()
{
    iter=0;
    stopped=false;

}

Job::Job(const CFDModel &cfdmodel)
:Job()
{
    model=&cfdmodel;    //TO MODIFY, CAN INCLUDE NEW MODEL?
}

void Job::run()
{
    while (!stopped)
    {
        model->SolveIter();
        //msgwin->AddString(model->ItLog());
        //msgwin->AddString("Hola\n");
        //cout << "iter: " <<iter<<endl;
        cout << model->ItLog()<<endl;
        //msgwin->append(QString::fromStdString(model->ItLog()));
        //line->SetColor(0, 0, 0, 255);
        msleep(10);
        iter++;
        if (iter>500)
            stopped=true;
    }
    //stopped=false;
}

void Job::stop()
{
    stopped=true;
}


