#ifndef _JOBSUBMITDIALOG_H_
#define _JOBSUBMITDIALOG_H_

#include <QDialog>


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

#include "ui_JobSubmitDialog.h"
#include "Job.h"

#include "JobWorker.h"


//This is repeated on ui_JobSubmitDialog
class JobSubmitDialog:
public QDialog
{
	Q_OBJECT

    public:
        JobSubmitDialog(Job &job_, QWidget *parent = 0);
        MsgWindow& ResMsgWindow() {return *ui->ResidualMsg;}
        vtkPlot & LinePlot(){return *line;}
        AddThread(JobThread &tr){thread=&tr;}
        Ui_JobSubmitDialog *ui;

    private slots:
        void StartStopJob();


    private:

    protected:



        //Job *job;     //WHIS IS OLD
        JobThread *thread;
        // Create a table with some points in it
      vtkSmartPointer<vtkTable> table;
      vtkSmartPointer<vtkFloatArray> arrX;
      vtkSmartPointer<vtkFloatArray> arrC;
      vtkSmartPointer<vtkFloatArray> arrS;

      vtkPlot *line;

        // Add multiple line plots, setting the colors etc
      vtkSmartPointer<vtkChartXY> chart;
    void InitResChart();

    void DrawResChart();

    vtkSmartPointer<vtkOpenGLRenderer> ren; //TO MODIFY, create chart object
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

    vtkSmartPointer<vtkContextView> view;       //Chart View



};

#endif
