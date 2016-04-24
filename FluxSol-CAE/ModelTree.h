#ifndef _MODELTREE_H_
#define _MODELTREE_H_

#include <QTreeWidget>

#include <iostream>

#include "ParamDialog.h"
#include "ManageBCDialog.h"
#include "JobSubmitDialog.h"

#include <QFileDialog>


using namespace std;

static const int ItemType1 = QTreeWidgetItem::UserType + 1;
static const int ItemType2 = QTreeWidgetItem::UserType + 2;


class ModelTreeWidget : public QTreeWidget
{

    Q_OBJECT
    ParamDialog *pdialog;
    QMenu *bcmenu;
    QAction *bccreate;
    QAction *jobshowresults;
    QMenu *jobmenu;
    vector <JobSubmitDialog *> vjobdialog;

    ManageBCDialog *managebcdlg;

public:
    ModelTreeWidget(QWidget *parent = 0);
    void AddJobDialog(JobSubmitDialog *dialog)
    {
        this->vjobdialog.push_back(dialog);
    }

private slots:
    void showContextMenu(const QPoint &pos);    //right click
    void DoubleClickItem(QTreeWidgetItem *item, int column);

    void test()
    {
        cout << "Click"<<endl;
    }

    void slotOpenFile()
    {
        QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open FluxSol Database"), ".",
        tr("FluxSol Database files (*.fdb)"));
        //if (!fileName.isEmpty())
            //loadFile(fileName);
    }
    void ShowNewBC(){pdialog->show();}
    void ShowManageBC(){this->managebcdlg->show();}

    signals:
    void ShowResults();

};
#endif // _MODELTREE_H_
