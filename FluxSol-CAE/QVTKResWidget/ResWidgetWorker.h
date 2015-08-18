#ifndef _RESWIDGETWORKER_H_
#define _RESWIDGETWORKER_H_

#include "QVTKResWidget.h"
#include <vector>

using namespace std;

class ResWidgetWorker:
    public QObject
{
	Q_OBJECT

    protected:

        QVTKResWidget *reswidget;

    public:
    ResWidgetWorker(const QVTKResWidget &rw){reswidget=&rw;}

    public slots:


    signals:
        void DrawResiduals();       //
        void DrawTest();
        void DrawTest2();
        void AddRes(const std::vector <double>&);
        void AddRes(const double &x,const double &y, const double &z);

};


#endif // _RESWIDGETWORKER_H_
