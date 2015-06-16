#ifndef _QVTKRESWIDGET_H_
#define _QVTKRESWIDGET_H_

#include <QtCore>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include "QVTKWidget.h"
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QThread>

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


class QVTKResWidget:
public QVTKWidget
{
    protected:

      vtkSmartPointer<vtkTable> table;
      vtkSmartPointer<vtkFloatArray> arrX;
      vtkSmartPointer<vtkFloatArray> arrC;
      vtkSmartPointer<vtkFloatArray> arrS;

      vtkPlot *line;

      QThread *thread;

      //Worker *worker;
        // Add multiple line plots, setting the colors etc
      vtkSmartPointer<vtkChartXY> chart;

      //View Stuff
        vtkSmartPointer<vtkOpenGLRenderer> ren; //TO MODIFY, create chart object
        vtkSmartPointer<vtkRenderWindow> renderWindow;
        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

        vtkSmartPointer<vtkContextView> view;       //Chart View

    public:
    QVTKResWidget();
    QVTKResWidget(QWidget* parent=0);

    void InitVTK();
    void InitChart();
    void Draw();
    void DrawAlt();

    public slots:

};

#endif
