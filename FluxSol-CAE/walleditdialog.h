#ifndef WALLEDITDIALOG_H
#define WALLEDITDIALOG_H

#include <QDialog>

namespace Ui {
class WallEditDialog;
}

class WallEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WallEditDialog(QWidget *parent = 0);
    ~WallEditDialog();

private:
    Ui::WallEditDialog *ui;
};

#endif // WALLEDITDIALOG_H
