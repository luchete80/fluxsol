#ifndef _MODELTREE_H_
#define _MODELTREE_H_

#include <QTreeWidget>

static const int ItemType1 = QTreeWidgetItem::UserType + 1;
static const int ItemType2 = QTreeWidgetItem::UserType + 2;

class ModelTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ModelTreeWidget(QWidget *parent = 0);

private slots:
    void showContextMenu(const QPoint &pos);
};
#endif // _MODELTREE_H_
