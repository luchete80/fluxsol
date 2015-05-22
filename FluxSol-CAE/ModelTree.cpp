#include "ModelTree.h"

#include <QMenu>
#include <QTreeWidgetItem>


#include <iostream>

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

}

void ModelTreeWidget::showContextMenu(const QPoint &pos)
{
      QMenu menu;

      QTreeWidgetItem* item = itemAt(pos);
      if (item->text(0)=="Jobs") //text from column 0, TO MODIFY; MUST BE ADDED: ITEM LEVEL MUST BE ZERO
                                 // && Item Level
        {
            menu.addAction("Create ...");
            cout << "Clicked"<<endl;
            system("dir");

        }  //  break;

        else if (item->text(0)=="BCs") //text from column 0, TO MODIFY; MUST BE ADDED: ITEM LEVEL MUST BE ZERO
                                     // && Item Level
        {
            menu.addAction("Create ...");
            cout << "Clicked BCs..."<<endl;

        }  //  break;
//
//  case ItemType2:
//    menu.addAction("This is a type 2");
//    break;
//  }

  menu.exec(mapToGlobal(pos));
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

        }  //  break;
//
}
