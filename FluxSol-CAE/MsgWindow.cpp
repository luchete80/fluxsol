#include "MsgWindow.h"


MsgWindow::MsgWindow(QWidget *parent)
  : QTextEdit(parent)
{
    //this->setFont (QFont ("Courier", 9));
    //QFont font("Helvetica", iconSize.height(), QFont::Bold);

//    int id = this->fontDatabase.addApplicationFont(":/fonts/avenir_light.ttf");
//    // returns "Avenir LT Com"
//    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
//
//    // returns "Avenir Lt Com" -- same as other font)
//    QString family1 = QFontDatabase::applicationFontFamilies(id_b).at(0);

    QFont font("Avenir LT Com 95 Black");  // I have to put the family name + style together!
    font.setPixelSize(22);
    font.setWeight(QFont::Bold); // ...And still set the weight!
    font.setStyleStrategy(QFont::PreferAntialias);
    //ui->MyLabel->setFont(font);

    this->setReadOnly(true);
    this->setFont (QFont ("Helvetica", 11));
    //this->setFont(font);
    QString welcome="=====================================\n  Welcome to FluxSol v0.0.1 Beta Version !!!\n=====================================\n";

    allmsg+=welcome;

    //this->setText(welcome);
    this->setText(welcome);
}
