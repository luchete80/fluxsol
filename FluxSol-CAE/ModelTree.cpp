#include "ModelTree.h"

#include <QMenu>
#include <QTreeWidgetItem>


#include <iostream>

#include "ParamDialog.h"

using namespace std;


ModelTreeWidget::ModelTreeWidget(QWidget *parent)
  : QTreeWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            SLOT(showContextMenu(const QPoint&)));

    //Another way to connect
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
            SLOT(DoubleClickItem(QTreeWidgetItem*, int)));

        //connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
    //        SLOT(DoubleClickItem(const QPoint&)));

//    addTopLevelItem(new QTreeWidgetItem(QStringList("A (type 1)"),
//                                          ItemType1));
//    addTopLevelItem(new QTreeWidgetItem(QStringList("B (type 1)"),
//                                          ItemType1));

//    addTopLevelItem(new QTreeWidgetItem(QStringList("C (type 2)"),
//                                      ItemType2));
//    addTopLevelItem(new QTreeWidgetItem(QStringList("D (type 2)"),
//                                          ItemType2));
    //show();

    pdialog=new ParamDialog();
    managebcdlg=new ManageBCDialog(this);


                //QMenu menu;
            bcmenu=new QMenu();
            bccreate=new QAction(tr("&Create ..."),bcmenu);
            QAction *bcmanage=new QAction(tr("&Manage..."),bcmenu);
            //menu.addAction("Create ...");
            bcmenu->addAction(bccreate);
            bcmenu->addAction(bcmanage);

            connect(bccreate, SIGNAL(triggered()), this,SLOT(ShowNewBC()));
            connect(bcmanage, SIGNAL(triggered()), this,SLOT(ShowManageBC()));

}



void ModelTreeWidget::showContextMenu(const QPoint &pos)
{

      QTreeWidgetItem* item = itemAt(pos);
      if (item->text(0)=="Jobs") //text from column 0, TO MODIFY; MUST BE ADDED: ITEM LEVEL MUST BE ZERO
                                 // && Item Level
        {
            QAction *create=new QAction(tr("&Create ..."),this);
            //menu.addAction("Create ...");
            cout << "Clicked"<<endl;
            system("dir");

        }  //  break;

        else if (item->text(0)=="BCs") //text from column 0, TO MODIFY; MUST BE ADDED: ITEM LEVEL MUST BE ZERO
                                     // && Item Level
        {
//            QMenu menu;
//            QAction *create=new QAction(tr("&Create ..."),&menu);
//            //menu.addAction("Create ...");
//            menu.addAction(create);

//            connect(create, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this,
//            SLOT(DoubleClickItem(QTreeWidgetItem*, int)));
//            connect(create, SIGNAL(triggered()), this,
//            SLOT(dialog.show()));
//            connect(create, SIGNAL(itemClicked(QMenuItem*, int)), this,
//            SLOT(dialog.show()));
            //menu.exec(mapToGlobal(pos));
            bcmenu->exec(mapToGlobal(pos));

//            connect(create, SIGNAL(triggered()), this,
//            SLOT(this->pdialog.exec()));

            //cout << "Clicked BCs..."<<endl;


        }  //  break;
//
//  case ItemType2:
//    menu.addAction("This is a type 2");
//    break;
//  }

}

void ModelTreeWidget::DoubleClickItem(QTreeWidgetItem *item, int column)
{
      //QTreeWidgetItem* item = itemAt(pos);
      if (item->text(0)=="Jobs") //text from column 0, TO MODIFY; MUST BE ADDED: ITEM LEVEL MUST BE ZERO
                                 // && Item Level
        {
            //menu.addAction("Create ...");
            cout << "Double Clicked Jobs"<<endl;

        }  //  break;

        else if (item->text(0)=="BCs") //text from column 0, TO MODIFY; MUST BE ADDED: ITEM LEVEL MUST BE ZERO
                                     // && Item Level
        {
            //menu.addAction("Create ...");
            cout << "Double Clicked BCs..."<<endl;
            this->pdialog->show();

        }  //  break;
//
}
