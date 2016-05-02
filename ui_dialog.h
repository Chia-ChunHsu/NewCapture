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

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(983, 516);
        labelScale = new QLabel(Dialog);
        labelScale->setObjectName(QStringLiteral("labelScale"));
        labelScale->setGeometry(QRect(720, 10, 250, 250));
        labelScale->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        spinBox = new QSpinBox(Dialog);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(720, 270, 151, 71));
        QFont font;
        font.setPointSize(20);
        spinBox->setFont(font);
        spinBox->setMinimum(1);
        spinBox->setMaximum(4);
        saveButton = new QPushButton(Dialog);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(720, 350, 251, 71));
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
        label1->setGeometry(QRect(880, 270, 41, 21));
        label2 = new QLabel(Dialog);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setGeometry(QRect(930, 270, 41, 21));
        label3 = new QLabel(Dialog);
        label3->setObjectName(QStringLiteral("label3"));
        label3->setGeometry(QRect(880, 310, 41, 21));
        label4 = new QLabel(Dialog);
        label4->setObjectName(QStringLiteral("label4"));
        label4->setGeometry(QRect(930, 310, 41, 21));
        UpperSlider = new QSlider(Dialog);
        UpperSlider->setObjectName(QStringLiteral("UpperSlider"));
        UpperSlider->setGeometry(QRect(10, 430, 701, 22));
        UpperSlider->setMaximum(255);
        UpperSlider->setValue(200);
        UpperSlider->setOrientation(Qt::Horizontal);
        LowerSlider = new QSlider(Dialog);
        LowerSlider->setObjectName(QStringLiteral("LowerSlider"));
        LowerSlider->setGeometry(QRect(10, 470, 701, 22));
        LowerSlider->setMaximum(255);
        LowerSlider->setValue(50);
        LowerSlider->setOrientation(Qt::Horizontal);

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
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
