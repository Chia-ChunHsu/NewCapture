/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *labelScale;
    QSpinBox *spinBox;
    QPushButton *saveButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout;
    QLabel *labelall;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QSlider *UpperSlider;
    QSlider *LowerSlider;
    QPushButton *SaveRGBData;
    QSpinBox *xSpinBox;
    QSpinBox *ySpinBox;
    QLabel *label;
    QLabel *pn1;
    QLabel *pn2;
    QLabel *pn3;
    QLabel *pn4;
    QLabel *pn5;
    QLabel *pn6;
    QLabel *ndiv1;
    QLabel *ndiv2;
    QLabel *ndiv3;
    QLabel *ndiv4;
    QLabel *ndiv5;
    QLabel *ndiv6;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1051, 580);
        labelScale = new QLabel(Dialog);
        labelScale->setObjectName(QStringLiteral("labelScale"));
        labelScale->setGeometry(QRect(720, 10, 250, 250));
        labelScale->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        spinBox = new QSpinBox(Dialog);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(720, 270, 71, 71));
        QFont font;
        font.setPointSize(20);
        spinBox->setFont(font);
        spinBox->setMinimum(1);
        spinBox->setMaximum(17);
        saveButton = new QPushButton(Dialog);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(720, 350, 171, 71));
        saveButton->setFont(font);
        scrollArea = new QScrollArea(Dialog);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(10, 10, 701, 411));
        scrollArea->setContextMenuPolicy(Qt::DefaultContextMenu);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 682, 392));
        horizontalLayout = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        labelall = new QLabel(scrollAreaWidgetContents);
        labelall->setObjectName(QStringLiteral("labelall"));
        labelall->setContextMenuPolicy(Qt::NoContextMenu);
        labelall->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(labelall);

        scrollArea->setWidget(scrollAreaWidgetContents);
        label1 = new QLabel(Dialog);
        label1->setObjectName(QStringLiteral("label1"));
        label1->setGeometry(QRect(800, 280, 41, 21));
        label2 = new QLabel(Dialog);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setGeometry(QRect(850, 280, 41, 21));
        label3 = new QLabel(Dialog);
        label3->setObjectName(QStringLiteral("label3"));
        label3->setGeometry(QRect(800, 320, 41, 21));
        label4 = new QLabel(Dialog);
        label4->setObjectName(QStringLiteral("label4"));
        label4->setGeometry(QRect(850, 320, 41, 21));
        UpperSlider = new QSlider(Dialog);
        UpperSlider->setObjectName(QStringLiteral("UpperSlider"));
        UpperSlider->setGeometry(QRect(10, 520, 701, 22));
        UpperSlider->setMaximum(255);
        UpperSlider->setValue(200);
        UpperSlider->setOrientation(Qt::Horizontal);
        LowerSlider = new QSlider(Dialog);
        LowerSlider->setObjectName(QStringLiteral("LowerSlider"));
        LowerSlider->setGeometry(QRect(10, 550, 701, 22));
        LowerSlider->setMaximum(255);
        LowerSlider->setValue(50);
        LowerSlider->setOrientation(Qt::Horizontal);
        SaveRGBData = new QPushButton(Dialog);
        SaveRGBData->setObjectName(QStringLiteral("SaveRGBData"));
        SaveRGBData->setGeometry(QRect(720, 430, 171, 71));
        QFont font1;
        font1.setFamily(QStringLiteral("Times New Roman"));
        font1.setPointSize(20);
        SaveRGBData->setFont(font1);
        xSpinBox = new QSpinBox(Dialog);
        xSpinBox->setObjectName(QStringLiteral("xSpinBox"));
        xSpinBox->setGeometry(QRect(10, 430, 131, 61));
        xSpinBox->setFont(font);
        xSpinBox->setMinimum(-10);
        xSpinBox->setMaximum(10);
        ySpinBox = new QSpinBox(Dialog);
        ySpinBox->setObjectName(QStringLiteral("ySpinBox"));
        ySpinBox->setGeometry(QRect(160, 430, 131, 61));
        ySpinBox->setFont(font);
        ySpinBox->setMinimum(-10);
        ySpinBox->setMaximum(10);
        label = new QLabel(Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(440, 450, 47, 12));
        pn1 = new QLabel(Dialog);
        pn1->setObjectName(QStringLiteral("pn1"));
        pn1->setGeometry(QRect(910, 270, 47, 12));
        pn2 = new QLabel(Dialog);
        pn2->setObjectName(QStringLiteral("pn2"));
        pn2->setGeometry(QRect(970, 270, 47, 12));
        pn3 = new QLabel(Dialog);
        pn3->setObjectName(QStringLiteral("pn3"));
        pn3->setGeometry(QRect(910, 300, 47, 12));
        pn4 = new QLabel(Dialog);
        pn4->setObjectName(QStringLiteral("pn4"));
        pn4->setGeometry(QRect(970, 300, 47, 12));
        pn5 = new QLabel(Dialog);
        pn5->setObjectName(QStringLiteral("pn5"));
        pn5->setGeometry(QRect(910, 330, 47, 12));
        pn6 = new QLabel(Dialog);
        pn6->setObjectName(QStringLiteral("pn6"));
        pn6->setGeometry(QRect(970, 330, 47, 12));
        ndiv1 = new QLabel(Dialog);
        ndiv1->setObjectName(QStringLiteral("ndiv1"));
        ndiv1->setGeometry(QRect(920, 390, 47, 12));
        ndiv2 = new QLabel(Dialog);
        ndiv2->setObjectName(QStringLiteral("ndiv2"));
        ndiv2->setGeometry(QRect(980, 390, 47, 12));
        ndiv3 = new QLabel(Dialog);
        ndiv3->setObjectName(QStringLiteral("ndiv3"));
        ndiv3->setGeometry(QRect(920, 420, 47, 12));
        ndiv4 = new QLabel(Dialog);
        ndiv4->setObjectName(QStringLiteral("ndiv4"));
        ndiv4->setGeometry(QRect(980, 420, 47, 12));
        ndiv5 = new QLabel(Dialog);
        ndiv5->setObjectName(QStringLiteral("ndiv5"));
        ndiv5->setGeometry(QRect(920, 450, 47, 12));
        ndiv6 = new QLabel(Dialog);
        ndiv6->setObjectName(QStringLiteral("ndiv6"));
        ndiv6->setGeometry(QRect(980, 450, 47, 12));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        labelScale->setText(QApplication::translate("Dialog", "TextLabel", 0));
        saveButton->setText(QApplication::translate("Dialog", "Save", 0));
        labelall->setText(QApplication::translate("Dialog", "TextLabel", 0));
        label1->setText(QApplication::translate("Dialog", "TextLabel", 0));
        label2->setText(QApplication::translate("Dialog", "TextLabel", 0));
        label3->setText(QApplication::translate("Dialog", "TextLabel", 0));
        label4->setText(QApplication::translate("Dialog", "TextLabel", 0));
        SaveRGBData->setText(QApplication::translate("Dialog", "Save RGB Data", 0));
        label->setText(QApplication::translate("Dialog", "TextLabel", 0));
        pn1->setText(QApplication::translate("Dialog", "TextLabel", 0));
        pn2->setText(QApplication::translate("Dialog", "TextLabel", 0));
        pn3->setText(QApplication::translate("Dialog", "TextLabel", 0));
        pn4->setText(QApplication::translate("Dialog", "TextLabel", 0));
        pn5->setText(QApplication::translate("Dialog", "TextLabel", 0));
        pn6->setText(QApplication::translate("Dialog", "TextLabel", 0));
        ndiv1->setText(QApplication::translate("Dialog", "TextLabel", 0));
        ndiv2->setText(QApplication::translate("Dialog", "TextLabel", 0));
        ndiv3->setText(QApplication::translate("Dialog", "TextLabel", 0));
        ndiv4->setText(QApplication::translate("Dialog", "TextLabel", 0));
        ndiv5->setText(QApplication::translate("Dialog", "TextLabel", 0));
        ndiv6->setText(QApplication::translate("Dialog", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
