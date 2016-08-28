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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
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
    QGroupBox *groupBox;
    QRadioButton *radioButtonH;
    QRadioButton *radioButtonS;
    QRadioButton *radioButtonI;
    QPushButton *LoadPosition;
    QLabel *PositionX;
    QLabel *PositionY;
    QLabel *DataNumber;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1016, 482);
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
        spinBox->setMaximum(16);
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
        pn1 = new QLabel(Dialog);
        pn1->setObjectName(QStringLiteral("pn1"));
        pn1->setGeometry(QRect(900, 270, 47, 12));
        pn2 = new QLabel(Dialog);
        pn2->setObjectName(QStringLiteral("pn2"));
        pn2->setGeometry(QRect(960, 270, 47, 12));
        pn3 = new QLabel(Dialog);
        pn3->setObjectName(QStringLiteral("pn3"));
        pn3->setGeometry(QRect(900, 300, 47, 12));
        pn4 = new QLabel(Dialog);
        pn4->setObjectName(QStringLiteral("pn4"));
        pn4->setGeometry(QRect(960, 300, 47, 12));
        pn5 = new QLabel(Dialog);
        pn5->setObjectName(QStringLiteral("pn5"));
        pn5->setGeometry(QRect(900, 330, 47, 12));
        pn6 = new QLabel(Dialog);
        pn6->setObjectName(QStringLiteral("pn6"));
        pn6->setGeometry(QRect(960, 330, 47, 12));
        ndiv1 = new QLabel(Dialog);
        ndiv1->setObjectName(QStringLiteral("ndiv1"));
        ndiv1->setGeometry(QRect(900, 360, 47, 12));
        ndiv2 = new QLabel(Dialog);
        ndiv2->setObjectName(QStringLiteral("ndiv2"));
        ndiv2->setGeometry(QRect(960, 360, 47, 12));
        ndiv3 = new QLabel(Dialog);
        ndiv3->setObjectName(QStringLiteral("ndiv3"));
        ndiv3->setGeometry(QRect(900, 390, 47, 12));
        ndiv4 = new QLabel(Dialog);
        ndiv4->setObjectName(QStringLiteral("ndiv4"));
        ndiv4->setGeometry(QRect(960, 390, 47, 12));
        ndiv5 = new QLabel(Dialog);
        ndiv5->setObjectName(QStringLiteral("ndiv5"));
        ndiv5->setGeometry(QRect(900, 420, 47, 12));
        ndiv6 = new QLabel(Dialog);
        ndiv6->setObjectName(QStringLiteral("ndiv6"));
        ndiv6->setGeometry(QRect(960, 420, 47, 12));
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(720, 430, 301, 51));
        radioButtonH = new QRadioButton(groupBox);
        radioButtonH->setObjectName(QStringLiteral("radioButtonH"));
        radioButtonH->setGeometry(QRect(10, 20, 61, 16));
        radioButtonS = new QRadioButton(groupBox);
        radioButtonS->setObjectName(QStringLiteral("radioButtonS"));
        radioButtonS->setGeometry(QRect(70, 20, 71, 16));
        radioButtonI = new QRadioButton(groupBox);
        radioButtonI->setObjectName(QStringLiteral("radioButtonI"));
        radioButtonI->setGeometry(QRect(140, 20, 83, 16));
        LoadPosition = new QPushButton(Dialog);
        LoadPosition->setObjectName(QStringLiteral("LoadPosition"));
        LoadPosition->setGeometry(QRect(10, 430, 381, 51));
        LoadPosition->setFont(font);
        PositionX = new QLabel(Dialog);
        PositionX->setObjectName(QStringLiteral("PositionX"));
        PositionX->setGeometry(QRect(400, 430, 101, 41));
        PositionX->setFont(font);
        PositionY = new QLabel(Dialog);
        PositionY->setObjectName(QStringLiteral("PositionY"));
        PositionY->setGeometry(QRect(510, 430, 101, 41));
        PositionY->setFont(font);
        DataNumber = new QLabel(Dialog);
        DataNumber->setObjectName(QStringLiteral("DataNumber"));
        DataNumber->setGeometry(QRect(620, 430, 101, 41));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DataNumber->sizePolicy().hasHeightForWidth());
        DataNumber->setSizePolicy(sizePolicy);
        DataNumber->setFont(font);

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
        groupBox->setTitle(QApplication::translate("Dialog", "Type", 0));
        radioButtonH->setText(QApplication::translate("Dialog", "Health", 0));
        radioButtonS->setText(QApplication::translate("Dialog", "Symptom", 0));
        radioButtonI->setText(QApplication::translate("Dialog", "Incubation", 0));
        LoadPosition->setText(QApplication::translate("Dialog", "Load Position", 0));
        PositionX->setText(QApplication::translate("Dialog", "X", 0));
        PositionY->setText(QApplication::translate("Dialog", "Y", 0));
        DataNumber->setText(QApplication::translate("Dialog", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
