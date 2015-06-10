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
    while (!stopped && iter<20)
    {
        model->SolveIter();
        msgwin->AddString(model->ItLog());
        msleep(10);
        iter++;
    }
    stopped=false;
}

void Job::stop()
{
    stopped=true;
}


