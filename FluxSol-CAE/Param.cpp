/********************************************************************************
** Form generated from reading UI file 'Param.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PARAM_H
#define PARAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QDoubleSpinBox *SpinRot;
    QSlider *horizontalSlider_2;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QDoubleSpinBox *SpinAvance;
    QSlider *SliderAvance;
    QGroupBox *groupBox_2;
    QComboBox *comboBox;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QComboBox *comboBox_2;
    QWidget *layoutWidget3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QDoubleSpinBox *SpinAncho;
    QSlider *horizontalSlider_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer;
    QDoubleSpinBox *doubleSpinBox_4;
    QSlider *horizontalSlider_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_3;
    QDoubleSpinBox *doubleSpinBox_5;
    QSlider *horizontalSlider_5;
    QGroupBox *groupBox_4;
    QWidget *layoutWidget4;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QComboBox *comboBox_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QDoubleSpinBox *SpinAncho_2;
    QSlider *horizontalSlider_6;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_12;
    QDoubleSpinBox *SpinAncho_4;
    QSlider *horizontalSlider_9;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_13;
    QDoubleSpinBox *SpinAncho_5;
    QSlider *horizontalSlider_10;
    QGroupBox *groupBox_5;
    QWidget *layoutWidget_5;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QDoubleSpinBox *SpinRot_2;
    QSlider *horizontalSlider_7;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(566, 503);
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(200, 450, 201, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(Dialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 80, 251, 101));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        layoutWidget_2 = new QWidget(groupBox);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 60, 221, 31));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        SpinRot = new QDoubleSpinBox(layoutWidget_2);
        SpinRot->setObjectName(QStringLiteral("SpinRot"));

        horizontalLayout_4->addWidget(SpinRot);

        horizontalSlider_2 = new QSlider(layoutWidget_2);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(horizontalSlider_2);

        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 20, 221, 31));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        SpinAvance = new QDoubleSpinBox(layoutWidget1);
        SpinAvance->setObjectName(QStringLiteral("SpinAvance"));

        horizontalLayout_3->addWidget(SpinAvance);

        SliderAvance = new QSlider(layoutWidget1);
        SliderAvance->setObjectName(QStringLiteral("SliderAvance"));
        SliderAvance->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(SliderAvance);


        horizontalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(Dialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 20, 151, 51));
        comboBox = new QComboBox(groupBox_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(10, 20, 131, 22));
        pushButton_2 = new QPushButton(Dialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(420, 260, 71, 23));
        groupBox_3 = new QGroupBox(Dialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(290, 20, 221, 161));
        layoutWidget2 = new QWidget(groupBox_3);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 20, 110, 22));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        comboBox_2 = new QComboBox(layoutWidget2);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));

        horizontalLayout->addWidget(comboBox_2);

        layoutWidget3 = new QWidget(groupBox_3);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 59, 191, 80));
        verticalLayout = new QVBoxLayout(layoutWidget3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_3 = new QLabel(layoutWidget3);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        SpinAncho = new QDoubleSpinBox(layoutWidget3);
        SpinAncho->setObjectName(QStringLiteral("SpinAncho"));

        horizontalLayout_5->addWidget(SpinAncho);

        horizontalSlider_3 = new QSlider(layoutWidget3);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(horizontalSlider_3);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_5 = new QLabel(layoutWidget3);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        doubleSpinBox_4 = new QDoubleSpinBox(layoutWidget3);
        doubleSpinBox_4->setObjectName(QStringLiteral("doubleSpinBox_4"));

        horizontalLayout_6->addWidget(doubleSpinBox_4);

        horizontalSlider_4 = new QSlider(layoutWidget3);
        horizontalSlider_4->setObjectName(QStringLiteral("horizontalSlider_4"));
        horizontalSlider_4->setOrientation(Qt::Horizontal);

        horizontalLayout_6->addWidget(horizontalSlider_4);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_6 = new QLabel(layoutWidget3);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_7->addWidget(label_6);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        doubleSpinBox_5 = new QDoubleSpinBox(layoutWidget3);
        doubleSpinBox_5->setObjectName(QStringLiteral("doubleSpinBox_5"));

        horizontalLayout_7->addWidget(doubleSpinBox_5);

        horizontalSlider_5 = new QSlider(layoutWidget3);
        horizontalSlider_5->setObjectName(QStringLiteral("horizontalSlider_5"));
        horizontalSlider_5->setOrientation(Qt::Horizontal);

        horizontalLayout_7->addWidget(horizontalSlider_5);


        verticalLayout->addLayout(horizontalLayout_7);

        groupBox_4 = new QGroupBox(Dialog);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(20, 260, 281, 131));
        layoutWidget4 = new QWidget(groupBox_4);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(10, 20, 262, 108));
        verticalLayout_2 = new QVBoxLayout(layoutWidget4);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_8 = new QLabel(layoutWidget4);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_8->addWidget(label_8);

        comboBox_3 = new QComboBox(layoutWidget4);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));

        horizontalLayout_8->addWidget(comboBox_3);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_7 = new QLabel(layoutWidget4);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        SpinAncho_2 = new QDoubleSpinBox(layoutWidget4);
        SpinAncho_2->setObjectName(QStringLiteral("SpinAncho_2"));

        horizontalLayout_9->addWidget(SpinAncho_2);

        horizontalSlider_6 = new QSlider(layoutWidget4);
        horizontalSlider_6->setObjectName(QStringLiteral("horizontalSlider_6"));
        horizontalSlider_6->setOrientation(Qt::Horizontal);

        horizontalLayout_9->addWidget(horizontalSlider_6);


        verticalLayout_2->addLayout(horizontalLayout_9);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_12 = new QLabel(layoutWidget4);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_13->addWidget(label_12);

        SpinAncho_4 = new QDoubleSpinBox(layoutWidget4);
        SpinAncho_4->setObjectName(QStringLiteral("SpinAncho_4"));

        horizontalLayout_13->addWidget(SpinAncho_4);

        horizontalSlider_9 = new QSlider(layoutWidget4);
        horizontalSlider_9->setObjectName(QStringLiteral("horizontalSlider_9"));
        horizontalSlider_9->setOrientation(Qt::Horizontal);

        horizontalLayout_13->addWidget(horizontalSlider_9);


        verticalLayout_2->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_13 = new QLabel(layoutWidget4);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_14->addWidget(label_13);

        SpinAncho_5 = new QDoubleSpinBox(layoutWidget4);
        SpinAncho_5->setObjectName(QStringLiteral("SpinAncho_5"));

        horizontalLayout_14->addWidget(SpinAncho_5);

        horizontalSlider_10 = new QSlider(layoutWidget4);
        horizontalSlider_10->setObjectName(QStringLiteral("horizontalSlider_10"));
        horizontalSlider_10->setOrientation(Qt::Horizontal);

        horizontalLayout_14->addWidget(horizontalSlider_10);


        verticalLayout_2->addLayout(horizontalLayout_14);

        layoutWidget->raise();
        label_12->raise();
        label_12->raise();
        groupBox_5 = new QGroupBox(Dialog);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(20, 190, 251, 61));
        layoutWidget_5 = new QWidget(groupBox_5);
        layoutWidget_5->setObjectName(QStringLiteral("layoutWidget_5"));
        layoutWidget_5->setGeometry(QRect(10, 20, 221, 31));
        horizontalLayout_10 = new QHBoxLayout(layoutWidget_5);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(layoutWidget_5);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_10->addWidget(label_9);

        SpinRot_2 = new QDoubleSpinBox(layoutWidget_5);
        SpinRot_2->setObjectName(QStringLiteral("SpinRot_2"));

        horizontalLayout_10->addWidget(SpinRot_2);

        horizontalSlider_7 = new QSlider(layoutWidget_5);
        horizontalSlider_7->setObjectName(QStringLiteral("horizontalSlider_7"));
        horizontalSlider_7->setOrientation(Qt::Horizontal);

        horizontalLayout_10->addWidget(horizontalSlider_7);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("Dialog", "Par\303\241metros de proceso", 0));
        label_2->setText(QApplication::translate("Dialog", "Rotaci\303\263n [rpm]    ", 0));
        label->setText(QApplication::translate("Dialog", "Avance [mm/min]", 0));
        groupBox_2->setTitle(QApplication::translate("Dialog", "Modelo", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("Dialog", "T\303\251rmico", 0)
         << QApplication::translate("Dialog", "T\303\251rmico + Mec\303\241nico", 0)
        );
        pushButton_2->setText(QApplication::translate("Dialog", "Detalles", 0));
        groupBox_3->setTitle(QApplication::translate("Dialog", "Placas", 0));
        label_4->setText(QApplication::translate("Dialog", "Material", 0));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("Dialog", "AA7075", 0)
        );
        label_3->setText(QApplication::translate("Dialog", "Ancho   ", 0));
        label_5->setText(QApplication::translate("Dialog", "Largo    ", 0));
        label_6->setText(QApplication::translate("Dialog", "Espesor", 0));
        groupBox_4->setTitle(QApplication::translate("Dialog", "Detalles Modelo", 0));
        label_8->setText(QApplication::translate("Dialog", "Tipo", 0));
        comboBox_3->clear();
        comboBox_3->insertItems(0, QStringList()
         << QApplication::translate("Dialog", "Lineal", 0)
         << QApplication::translate("Dialog", "Cuadr\303\241tico", 0)
        );
        label_7->setText(QApplication::translate("Dialog", "Elementos", 0));
        label_12->setText(QApplication::translate("Dialog", "Enfriamiento [s]", 0));
        label_13->setText(QApplication::translate("Dialog", "Paso [s]", 0));
        groupBox_5->setTitle(QApplication::translate("Dialog", "Herramienta", 0));
        label_9->setText(QApplication::translate("Dialog", "Di\303\241metro [mm]  ", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PARAM_H
