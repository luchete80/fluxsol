#include "newbc.h"
#include "ui_newbc.h"

NewBC::NewBC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewBC)
{
    ui->setupUi(this);
}

NewBC::~NewBC()
{
    delete ui;
}
