#include "walleditdialog.h"
#include "ui_walleditdialog.h"

WallEditDialog::WallEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WallEditDialog)
{
    ui->setupUi(this);
}

WallEditDialog::~WallEditDialog()
{
    delete ui;
}
