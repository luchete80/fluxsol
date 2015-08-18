#ifndef MANAGEBCDIALOG_H
#define MANAGEBCDIALOG_H

#include <QDialog>

namespace Ui {
class ManageBCDialog;
}

class ManageBCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageBCDialog(QWidget *parent = 0);
    ~ManageBCDialog();

private:
    Ui::ManageBCDialog *ui;
};

#endif // NEWBC_H
