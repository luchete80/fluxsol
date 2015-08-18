#include "ManageBCDialog.h"
#include "ui_ManageBCDialog.h"

ManageBCDialog::ManageBCDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageBCDialog)
{
    ui->setupUi(this);
}

ManageBCDialog::~ManageBCDialog()
{
    delete ui;
}
