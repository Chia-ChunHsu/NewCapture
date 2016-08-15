/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *RefLabel;
    QSpinBox *spinBoxRef;
    QPushButton *CaptureRefButton;
    QPushButton *LoadRefButton;
    QLabel *label;
    QPushButton *LoadWRefButton;
    QWidget *tab_2;
    QLabel *PicLabel;
    QPushButton *CapturePicButton;
    QSpinBox *spinBoxPic;
    QPushButton *LoadPicButton;
    QWidget *tab_4;
    QLabel *CutLabel;
    QPushButton *CutButton;
    QSlider *ThresholdSlider1;
    QSpinBox *spinBox;
    QSlider *ThresholdSlider2;
    QSpinBox *spinBox_2;
    QSpinBox *spinBoxT1;
    QSpinBox *spinBoxT2;
    QPushButton *TestButtom;
    QWidget *tab_3;
    QLabel *FalseColorLabel;
    QPushButton *PredictButton;
    QSpinBox *FeaturesSpinBox;
    QDateTimeEdit *dateTimeEdit;
    QPushButton *ApplyButton;
    QPushButton *ChooseButton;
    QPushButton *RGBButtom;
    QPushButton *KnnPredictButtom;
    QLabel *FileName;
    QPushButton *NDVIButton;
    QPushButton *Multi_Buttom;
    QDoubleSpinBox *TCutdoubleSpinBox2;
    QDoubleSpinBox *TCutdoubleSpinBox4;
    QLabel *TempLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(623, 396);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 461, 261));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        RefLabel = new QLabel(tab);
        RefLabel->setObjectName(QStringLiteral("RefLabel"));
        RefLabel->setGeometry(QRect(10, 10, 288, 216));
        RefLabel->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        spinBoxRef = new QSpinBox(tab);
        spinBoxRef->setObjectName(QStringLiteral("spinBoxRef"));
        spinBoxRef->setGeometry(QRect(310, 10, 141, 41));
        QFont font;
        font.setPointSize(20);
        spinBoxRef->setFont(font);
        spinBoxRef->setMaximum(3);
        CaptureRefButton = new QPushButton(tab);
        CaptureRefButton->setObjectName(QStringLiteral("CaptureRefButton"));
        CaptureRefButton->setGeometry(QRect(310, 180, 141, 51));
        LoadRefButton = new QPushButton(tab);
        LoadRefButton->setObjectName(QStringLiteral("LoadRefButton"));
        LoadRefButton->setEnabled(false);
        LoadRefButton->setGeometry(QRect(310, 120, 141, 51));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(316, 60, 131, 20));
        LoadWRefButton = new QPushButton(tab);
        LoadWRefButton->setObjectName(QStringLiteral("LoadWRefButton"));
        LoadWRefButton->setGeometry(QRect(310, 80, 141, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        PicLabel = new QLabel(tab_2);
        PicLabel->setObjectName(QStringLiteral("PicLabel"));
        PicLabel->setGeometry(QRect(10, 10, 288, 216));
        PicLabel->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        CapturePicButton = new QPushButton(tab_2);
        CapturePicButton->setObjectName(QStringLiteral("CapturePicButton"));
        CapturePicButton->setGeometry(QRect(310, 160, 141, 71));
        spinBoxPic = new QSpinBox(tab_2);
        spinBoxPic->setObjectName(QStringLiteral("spinBoxPic"));
        spinBoxPic->setGeometry(QRect(310, 10, 141, 41));
        spinBoxPic->setFont(font);
        spinBoxPic->setMaximum(3);
        LoadPicButton = new QPushButton(tab_2);
        LoadPicButton->setObjectName(QStringLiteral("LoadPicButton"));
        LoadPicButton->setEnabled(false);
        LoadPicButton->setGeometry(QRect(310, 102, 141, 51));
        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        CutLabel = new QLabel(tab_4);
        CutLabel->setObjectName(QStringLiteral("CutLabel"));
        CutLabel->setGeometry(QRect(10, 10, 288, 216));
        CutLabel->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        CutButton = new QPushButton(tab_4);
        CutButton->setObjectName(QStringLiteral("CutButton"));
        CutButton->setEnabled(false);
        CutButton->setGeometry(QRect(310, 130, 141, 51));
        ThresholdSlider1 = new QSlider(tab_4);
        ThresholdSlider1->setObjectName(QStringLiteral("ThresholdSlider1"));
        ThresholdSlider1->setEnabled(false);
        ThresholdSlider1->setGeometry(QRect(310, 40, 101, 19));
        ThresholdSlider1->setMaximum(255);
        ThresholdSlider1->setValue(91);
        ThresholdSlider1->setOrientation(Qt::Horizontal);
        spinBox = new QSpinBox(tab_4);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(410, 40, 42, 22));
        spinBox->setMaximum(255);
        spinBox->setValue(91);
        ThresholdSlider2 = new QSlider(tab_4);
        ThresholdSlider2->setObjectName(QStringLiteral("ThresholdSlider2"));
        ThresholdSlider2->setEnabled(false);
        ThresholdSlider2->setGeometry(QRect(310, 100, 101, 19));
        ThresholdSlider2->setMaximum(255);
        ThresholdSlider2->setValue(126);
        ThresholdSlider2->setOrientation(Qt::Horizontal);
        spinBox_2 = new QSpinBox(tab_4);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setGeometry(QRect(410, 100, 42, 22));
        spinBox_2->setMaximum(255);
        spinBox_2->setValue(126);
        spinBoxT1 = new QSpinBox(tab_4);
        spinBoxT1->setObjectName(QStringLiteral("spinBoxT1"));
        spinBoxT1->setGeometry(QRect(310, 10, 51, 22));
        spinBoxT1->setMaximum(3);
        spinBoxT2 = new QSpinBox(tab_4);
        spinBoxT2->setObjectName(QStringLiteral("spinBoxT2"));
        spinBoxT2->setGeometry(QRect(310, 70, 51, 22));
        spinBoxT2->setMaximum(3);
        spinBoxT2->setValue(2);
        TestButtom = new QPushButton(tab_4);
        TestButtom->setObjectName(QStringLiteral("TestButtom"));
        TestButtom->setGeometry(QRect(310, 190, 141, 41));
        tabWidget->addTab(tab_4, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        FalseColorLabel = new QLabel(tab_3);
        FalseColorLabel->setObjectName(QStringLiteral("FalseColorLabel"));
        FalseColorLabel->setGeometry(QRect(10, 10, 288, 216));
        FalseColorLabel->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        PredictButton = new QPushButton(tab_3);
        PredictButton->setObjectName(QStringLiteral("PredictButton"));
        PredictButton->setGeometry(QRect(310, 180, 141, 51));
        FeaturesSpinBox = new QSpinBox(tab_3);
        FeaturesSpinBox->setObjectName(QStringLiteral("FeaturesSpinBox"));
        FeaturesSpinBox->setGeometry(QRect(310, 120, 141, 51));
        QFont font1;
        font1.setPointSize(16);
        FeaturesSpinBox->setFont(font1);
        FeaturesSpinBox->setMaximum(20);
        FeaturesSpinBox->setValue(12);
        tabWidget->addTab(tab_3, QString());
        dateTimeEdit = new QDateTimeEdit(centralWidget);
        dateTimeEdit->setObjectName(QStringLiteral("dateTimeEdit"));
        dateTimeEdit->setGeometry(QRect(220, 10, 171, 22));
        QFont font2;
        font2.setPointSize(11);
        dateTimeEdit->setFont(font2);
        ApplyButton = new QPushButton(centralWidget);
        ApplyButton->setObjectName(QStringLiteral("ApplyButton"));
        ApplyButton->setGeometry(QRect(390, 10, 81, 23));
        QFont font3;
        font3.setPointSize(10);
        ApplyButton->setFont(font3);
        ChooseButton = new QPushButton(centralWidget);
        ChooseButton->setObjectName(QStringLiteral("ChooseButton"));
        ChooseButton->setEnabled(false);
        ChooseButton->setGeometry(QRect(10, 300, 301, 41));
        RGBButtom = new QPushButton(centralWidget);
        RGBButtom->setObjectName(QStringLiteral("RGBButtom"));
        RGBButtom->setEnabled(false);
        RGBButtom->setGeometry(QRect(320, 300, 151, 41));
        RGBButtom->setFont(font1);
        KnnPredictButtom = new QPushButton(centralWidget);
        KnnPredictButtom->setObjectName(QStringLiteral("KnnPredictButtom"));
        KnnPredictButtom->setGeometry(QRect(480, 170, 141, 21));
        FileName = new QLabel(centralWidget);
        FileName->setObjectName(QStringLiteral("FileName"));
        FileName->setGeometry(QRect(10, 280, 461, 16));
        NDVIButton = new QPushButton(centralWidget);
        NDVIButton->setObjectName(QStringLiteral("NDVIButton"));
        NDVIButton->setEnabled(false);
        NDVIButton->setGeometry(QRect(480, 30, 141, 61));
        Multi_Buttom = new QPushButton(centralWidget);
        Multi_Buttom->setObjectName(QStringLiteral("Multi_Buttom"));
        Multi_Buttom->setEnabled(false);
        Multi_Buttom->setGeometry(QRect(480, 100, 141, 61));
        Multi_Buttom->setToolTipDuration(6);
        TCutdoubleSpinBox2 = new QDoubleSpinBox(centralWidget);
        TCutdoubleSpinBox2->setObjectName(QStringLiteral("TCutdoubleSpinBox2"));
        TCutdoubleSpinBox2->setGeometry(QRect(1000, 260, 62, 22));
        TCutdoubleSpinBox2->setMaximum(255);
        TCutdoubleSpinBox2->setValue(40);
        TCutdoubleSpinBox4 = new QDoubleSpinBox(centralWidget);
        TCutdoubleSpinBox4->setObjectName(QStringLiteral("TCutdoubleSpinBox4"));
        TCutdoubleSpinBox4->setGeometry(QRect(1000, 320, 62, 22));
        TCutdoubleSpinBox4->setMaximum(255);
        TCutdoubleSpinBox4->setValue(140);
        TempLabel = new QLabel(centralWidget);
        TempLabel->setObjectName(QStringLiteral("TempLabel"));
        TempLabel->setGeometry(QRect(490, 240, 128, 96));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 623, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        RefLabel->setText(QApplication::translate("MainWindow", "Reference", 0));
        CaptureRefButton->setText(QApplication::translate("MainWindow", "Capture Reference", 0));
        LoadRefButton->setText(QApplication::translate("MainWindow", "Load Reference", 0));
        label->setText(QApplication::translate("MainWindow", "Time", 0));
        LoadWRefButton->setText(QApplication::translate("MainWindow", "Load White Ref", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Step 1", 0));
        PicLabel->setText(QApplication::translate("MainWindow", "Sample Pictures", 0));
        CapturePicButton->setText(QApplication::translate("MainWindow", "Capture Pictures", 0));
        LoadPicButton->setText(QApplication::translate("MainWindow", "Load Pictures", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Step 2", 0));
        CutLabel->setText(QApplication::translate("MainWindow", "Leaf Mask", 0));
        CutButton->setText(QApplication::translate("MainWindow", "Cut", 0));
        TestButtom->setText(QApplication::translate("MainWindow", "Test", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Step 3", 0));
        FalseColorLabel->setText(QApplication::translate("MainWindow", "Prediction", 0));
        PredictButton->setText(QApplication::translate("MainWindow", "Predict", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Step 4", 0));
        dateTimeEdit->setDisplayFormat(QApplication::translate("MainWindow", "yyyy/M/d AP hh:mm::ss", 0));
        ApplyButton->setText(QApplication::translate("MainWindow", "Apply", 0));
        ChooseButton->setText(QApplication::translate("MainWindow", "Choose Point", 0));
        RGBButtom->setText(QApplication::translate("MainWindow", "Open RGB", 0));
        KnnPredictButtom->setText(QApplication::translate("MainWindow", "Predict by KNN", 0));
        FileName->setText(QApplication::translate("MainWindow", "File", 0));
        NDVIButton->setText(QApplication::translate("MainWindow", "NDVI", 0));
        Multi_Buttom->setText(QApplication::translate("MainWindow", "Analize Data", 0));
        TempLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
