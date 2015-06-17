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

#include "JobWorker.h"
#include "Model.h"
#include "JobThread.h"


//This is repeated on ui_JobSubmitDialog
class JobSubmitDialog:
public QDialog,
private Ui::JobSubmitDialog
{
	Q_OBJECT

    public:
        JobSubmitDialog(const CFDModel &model_,QWidget *parent = 0 );
        MsgWindow& ResMsgWindow() {return *ResidualMsg;}
        vtkPlot & LinePlot(){return *line;}
        //Ui_JobSubmitDialog *ui;

    private slots:
        void StartStopJob();

    public slots:
        void AddString(const string &str){
            //ResidualMsg->AddString(str);
            cout << str<<endl;
            this->update();}
        void ChangeStartStopButton (const string &str){StartStopButton->setText(QString::fromUtf8(str.c_str()));}

    private:

    protected:


        bool stopped;
        int iter;
        //Job *job;     //WHIS IS OLD
        // Create a table with some points in it
      vtkSmartPointer<vtkTable> table;
      vtkSmartPointer<vtkFloatArray> arrX;
      vtkSmartPointer<vtkFloatArray> arrC;
      vtkSmartPointer<vtkFloatArray> arrS;

      vtkPlot *line;

      CFDModel *model;

      QThread *thread;

      JobThread *jobthread;

      Worker *worker;                   //MAIN WORKER
      ResWidgetWorker *resworker;       //To draw residuals, this is created with NEW

        // Add multiple line plots, setting the colors etc
      vtkSmartPointer<vtkChartXY> chart;
    void InitResChart();

    void DrawResChart();
    void DrawResChartAlt();

    vtkSmartPointer<vtkOpenGLRenderer> ren; //TO MODIFY, create chart object
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

    vtkSmartPointer<vtkContextView> view;       //Chart View



};

#endif
