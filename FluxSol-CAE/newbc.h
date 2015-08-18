#ifndef NEWBC_H
#define NEWBC_H

#include <QDialog>

namespace Ui {
class NewBC;
}

class NewBC : public QDialog
{
    Q_OBJECT

public:
    explicit NewBC(QWidget *parent = 0);
    ~NewBC();

private:
    Ui::NewBC *ui;
};

#endif // NEWBC_H
