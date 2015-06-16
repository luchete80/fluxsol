#ifndef _MSGWINDOWS_H_
#define _MSGWINDOWS_H_

#include <QTextEdit>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QString>
#include "QDebugStream.h"
#include "QDebugStream_Alt.h"

class MsgWindow: public QTextEdit
{
    Q_OBJECT
public:
    MsgWindow(){};
    MsgWindow(QWidget *parent = 0);
    AddString(const std::string &str)
    {
        QString temp(QString::fromStdString(str));
        //QString temp("hola");
        allmsg+=temp;
        this->setText(allmsg);
    }

protected:
    QString allmsg;
    Q_DebugStream *qout;
    QDebugStream_Alt *qout_alt;

//private slots:
//    void showContextMenu(const QPoint &pos);    //right click
//    void DoubleClickItem(QTreeWidgetItem *item, int column);
private:


};
#endif // _MODELTREE_H_
