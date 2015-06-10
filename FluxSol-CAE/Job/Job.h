#ifndef _JOB_H_
#define _JOB_H_

//#include "Model.h"
#include <QtCore>
#include "Model.h"
#include "ui_JobSubmitDialog.h"

// TO DRAW VTK CHART
#include <vtkVersion.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkAxesActor.h>

#include <vtkRenderWindowInteractor.h>
//NEW!! Extracted from Shadows test project
#if VTK_MAJOR_VERSION <= 5
#include <vtkRenderer.h>
#else
#include <vtkOpenGLRenderer.h>
#endif

//Job main architecture
//See from C++Gui Programming with Qt Page 381
class ResChart
{

    vtkSmartPointer<vtkOpenGLRenderer> ren;

};



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

    vtkSmartPointer<vtkContextView> view;       //Chart View


      // Create a table with some points in it
  vtkSmartPointer<vtkTable> table;
  vtkSmartPointer<vtkFloatArray> arrX;
  vtkSmartPointer<vtkFloatArray> arrC;
  vtkSmartPointer<vtkFloatArray> arrS;

  vtkPlot *line;

    // Add multiple line plots, setting the colors etc
  vtkSmartPointer<vtkChartXY> chart;

	CFDModel *model;	//INCLUDES INHERITED CFDMODELS, SIMPLE, THERMAL, AND SO ON
    void run();         //REIMPLEMENTED

    void InitResChart();

    void DrawResChart();

    QDialog *submitdialog;
    Ui_JobSubmitDialog uisubmitdialog;
    vtkSmartPointer<vtkOpenGLRenderer> ren; //TO MODIFY, create chart object
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

	public:
		Job();
		Job(const CFDModel &cfdmodel);
		void setMessage(const QString &message);
        void stop();
		//Model * Model(){return model;}
		//SolveIter(){model->SolveIter();}


};

#endif
