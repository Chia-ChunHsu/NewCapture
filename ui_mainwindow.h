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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
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
    QAction *actionClose;
    QAction *actionTraining_Features;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab_5;
    QPushButton *ApplyButton;
    QDateTimeEdit *dateTimeEdit;
    QWidget *tab;
    QLabel *RefLabel;
    QSpinBox *spinBoxRef;
    QPushButton *CaptureRefButton;
    QPushButton *LoadRefButton;
    QLabel *Timelabel;
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
    QSpinBox *spinBox0;
    QSlider *ThresholdSlider2;
    QSpinBox *spinBox1;
    QSpinBox *spinBoxT1;
    QSpinBox *spinBoxT2;
    QWidget *tab_3;
    QLabel *FalseColorLabel;
    QSpinBox *FeaturesSpinBox;
    QGroupBox *groupBox;
    QCheckBox *checkBox1;
    QCheckBox *checkBox6;
    QCheckBox *checkBox11;
    QCheckBox *checkBox2;
    QCheckBox *checkBox7;
    QCheckBox *checkBox8;
    QCheckBox *checkBox9;
    QCheckBox *checkBox13;
    QCheckBox *checkBox12;
    QCheckBox *checkBox14;
    QCheckBox *checkBox0;
    QCheckBox *checkBox10;
    QCheckBox *checkBox3;
    QCheckBox *checkBox15;
    QCheckBox *checkBox4;
    QCheckBox *checkBox5;
    QPushButton *ChooseButton;
    QPushButton *RGBButtom;
    QLabel *FileName;
    QPushButton *NDVIButton;
    QPushButton *DataAna_Buttom;
    QDoubleSpinBox *TCutdoubleSpinBox2;
    QDoubleSpinBox *TCutdoubleSpinBox4;
    QLabel *TempLabel;
    QPushButton *TrainingButtom;
    QPushButton *PredictButton;
    QDoubleSpinBox *C_spinBox;
    QLabel *label;
    QCheckBox *RecentTrainCheckBox;
    QCheckBox *RGBorNotcheckBox;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(629, 394);
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionTraining_Features = new QAction(MainWindow);
        actionTraining_Features->setObjectName(QStringLiteral("actionTraining_Features"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 461, 261));
        QFont font;
        font.setPointSize(10);
        tabWidget->setFont(font);
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        ApplyButton = new QPushButton(tab_5);
        ApplyButton->setObjectName(QStringLiteral("ApplyButton"));
        ApplyButton->setGeometry(QRect(210, 90, 111, 41));
        QFont font1;
        font1.setPointSize(9);
        ApplyButton->setFont(font1);
        dateTimeEdit = new QDateTimeEdit(tab_5);
        dateTimeEdit->setObjectName(QStringLiteral("dateTimeEdit"));
        dateTimeEdit->setGeometry(QRect(10, 90, 191, 41));
        QFont font2;
        font2.setPointSize(11);
        dateTimeEdit->setFont(font2);
        dateTimeEdit->setDateTime(QDateTime(QDate(2016, 7, 14), QTime(0, 0, 0)));
        tabWidget->addTab(tab_5, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        RefLabel = new QLabel(tab);
        RefLabel->setObjectName(QStringLiteral("RefLabel"));
        RefLabel->setGeometry(QRect(10, 10, 288, 216));
        RefLabel->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        spinBoxRef = new QSpinBox(tab);
        spinBoxRef->setObjectName(QStringLiteral("spinBoxRef"));
        spinBoxRef->setGeometry(QRect(310, 10, 141, 41));
        QFont font3;
        font3.setPointSize(20);
        spinBoxRef->setFont(font3);
        spinBoxRef->setMaximum(3);
        CaptureRefButton = new QPushButton(tab);
        CaptureRefButton->setObjectName(QStringLiteral("CaptureRefButton"));
        CaptureRefButton->setEnabled(false);
        CaptureRefButton->setGeometry(QRect(310, 180, 141, 51));
        LoadRefButton = new QPushButton(tab);
        LoadRefButton->setObjectName(QStringLiteral("LoadRefButton"));
        LoadRefButton->setEnabled(false);
        LoadRefButton->setGeometry(QRect(310, 120, 141, 51));
        Timelabel = new QLabel(tab);
        Timelabel->setObjectName(QStringLiteral("Timelabel"));
        Timelabel->setGeometry(QRect(316, 60, 131, 20));
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
        CapturePicButton->setEnabled(false);
        CapturePicButton->setGeometry(QRect(310, 180, 141, 51));
        spinBoxPic = new QSpinBox(tab_2);
        spinBoxPic->setObjectName(QStringLiteral("spinBoxPic"));
        spinBoxPic->setGeometry(QRect(310, 10, 141, 41));
        spinBoxPic->setFont(font3);
        spinBoxPic->setMaximum(3);
        LoadPicButton = new QPushButton(tab_2);
        LoadPicButton->setObjectName(QStringLiteral("LoadPicButton"));
        LoadPicButton->setEnabled(false);
        LoadPicButton->setGeometry(QRect(310, 102, 141, 71));
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
        CutButton->setGeometry(QRect(310, 130, 141, 91));
        QFont font4;
        font4.setPointSize(12);
        CutButton->setFont(font4);
        ThresholdSlider1 = new QSlider(tab_4);
        ThresholdSlider1->setObjectName(QStringLiteral("ThresholdSlider1"));
        ThresholdSlider1->setEnabled(false);
        ThresholdSlider1->setGeometry(QRect(310, 40, 101, 19));
        ThresholdSlider1->setMaximum(255);
        ThresholdSlider1->setValue(91);
        ThresholdSlider1->setOrientation(Qt::Horizontal);
        spinBox0 = new QSpinBox(tab_4);
        spinBox0->setObjectName(QStringLiteral("spinBox0"));
        spinBox0->setEnabled(false);
        spinBox0->setGeometry(QRect(410, 40, 42, 22));
        spinBox0->setMaximum(255);
        spinBox0->setValue(91);
        ThresholdSlider2 = new QSlider(tab_4);
        ThresholdSlider2->setObjectName(QStringLiteral("ThresholdSlider2"));
        ThresholdSlider2->setEnabled(false);
        ThresholdSlider2->setGeometry(QRect(310, 100, 101, 19));
        ThresholdSlider2->setMaximum(255);
        ThresholdSlider2->setValue(126);
        ThresholdSlider2->setOrientation(Qt::Horizontal);
        spinBox1 = new QSpinBox(tab_4);
        spinBox1->setObjectName(QStringLiteral("spinBox1"));
        spinBox1->setEnabled(false);
        spinBox1->setGeometry(QRect(410, 100, 42, 22));
        spinBox1->setMaximum(255);
        spinBox1->setValue(126);
        spinBoxT1 = new QSpinBox(tab_4);
        spinBoxT1->setObjectName(QStringLiteral("spinBoxT1"));
        spinBoxT1->setEnabled(false);
        spinBoxT1->setGeometry(QRect(310, 10, 51, 22));
        spinBoxT1->setMaximum(3);
        spinBoxT2 = new QSpinBox(tab_4);
        spinBoxT2->setObjectName(QStringLiteral("spinBoxT2"));
        spinBoxT2->setEnabled(false);
        spinBoxT2->setGeometry(QRect(310, 70, 51, 22));
        spinBoxT2->setMaximum(3);
        spinBoxT2->setValue(2);
        tabWidget->addTab(tab_4, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        FalseColorLabel = new QLabel(tab_3);
        FalseColorLabel->setObjectName(QStringLiteral("FalseColorLabel"));
        FalseColorLabel->setGeometry(QRect(10, 10, 288, 216));
        FalseColorLabel->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        FeaturesSpinBox = new QSpinBox(tab_3);
        FeaturesSpinBox->setObjectName(QStringLiteral("FeaturesSpinBox"));
        FeaturesSpinBox->setEnabled(false);
        FeaturesSpinBox->setGeometry(QRect(310, 190, 141, 41));
        FeaturesSpinBox->setFont(font4);
        FeaturesSpinBox->setMaximum(20);
        FeaturesSpinBox->setValue(0);
        groupBox = new QGroupBox(tab_3);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(310, 0, 141, 181));
        checkBox1 = new QCheckBox(groupBox);
        checkBox1->setObjectName(QStringLiteral("checkBox1"));
        checkBox1->setGeometry(QRect(10, 40, 73, 16));
        checkBox6 = new QCheckBox(groupBox);
        checkBox6->setObjectName(QStringLiteral("checkBox6"));
        checkBox6->setGeometry(QRect(10, 140, 73, 16));
        checkBox11 = new QCheckBox(groupBox);
        checkBox11->setObjectName(QStringLiteral("checkBox11"));
        checkBox11->setGeometry(QRect(80, 80, 73, 16));
        checkBox2 = new QCheckBox(groupBox);
        checkBox2->setObjectName(QStringLiteral("checkBox2"));
        checkBox2->setGeometry(QRect(10, 60, 73, 16));
        checkBox7 = new QCheckBox(groupBox);
        checkBox7->setObjectName(QStringLiteral("checkBox7"));
        checkBox7->setGeometry(QRect(10, 160, 73, 16));
        checkBox8 = new QCheckBox(groupBox);
        checkBox8->setObjectName(QStringLiteral("checkBox8"));
        checkBox8->setGeometry(QRect(80, 20, 73, 16));
        checkBox9 = new QCheckBox(groupBox);
        checkBox9->setObjectName(QStringLiteral("checkBox9"));
        checkBox9->setGeometry(QRect(80, 40, 73, 16));
        checkBox13 = new QCheckBox(groupBox);
        checkBox13->setObjectName(QStringLiteral("checkBox13"));
        checkBox13->setGeometry(QRect(80, 120, 73, 16));
        checkBox12 = new QCheckBox(groupBox);
        checkBox12->setObjectName(QStringLiteral("checkBox12"));
        checkBox12->setGeometry(QRect(80, 100, 73, 16));
        checkBox14 = new QCheckBox(groupBox);
        checkBox14->setObjectName(QStringLiteral("checkBox14"));
        checkBox14->setGeometry(QRect(80, 140, 73, 16));
        checkBox0 = new QCheckBox(groupBox);
        checkBox0->setObjectName(QStringLiteral("checkBox0"));
        checkBox0->setGeometry(QRect(10, 20, 73, 16));
        checkBox10 = new QCheckBox(groupBox);
        checkBox10->setObjectName(QStringLiteral("checkBox10"));
        checkBox10->setGeometry(QRect(80, 60, 73, 16));
        checkBox3 = new QCheckBox(groupBox);
        checkBox3->setObjectName(QStringLiteral("checkBox3"));
        checkBox3->setGeometry(QRect(10, 80, 73, 16));
        checkBox15 = new QCheckBox(groupBox);
        checkBox15->setObjectName(QStringLiteral("checkBox15"));
        checkBox15->setGeometry(QRect(80, 160, 73, 16));
        checkBox4 = new QCheckBox(groupBox);
        checkBox4->setObjectName(QStringLiteral("checkBox4"));
        checkBox4->setGeometry(QRect(10, 100, 73, 16));
        checkBox5 = new QCheckBox(groupBox);
        checkBox5->setObjectName(QStringLiteral("checkBox5"));
        checkBox5->setGeometry(QRect(10, 120, 73, 16));
        tabWidget->addTab(tab_3, QString());
        ChooseButton = new QPushButton(centralWidget);
        ChooseButton->setObjectName(QStringLiteral("ChooseButton"));
        ChooseButton->setEnabled(false);
        ChooseButton->setGeometry(QRect(10, 300, 201, 41));
        ChooseButton->setFont(font4);
        RGBButtom = new QPushButton(centralWidget);
        RGBButtom->setObjectName(QStringLiteral("RGBButtom"));
        RGBButtom->setEnabled(false);
        RGBButtom->setGeometry(QRect(330, 300, 141, 41));
        RGBButtom->setFont(font4);
        FileName = new QLabel(centralWidget);
        FileName->setObjectName(QStringLiteral("FileName"));
        FileName->setGeometry(QRect(10, 280, 371, 16));
        NDVIButton = new QPushButton(centralWidget);
        NDVIButton->setObjectName(QStringLiteral("NDVIButton"));
        NDVIButton->setEnabled(false);
        NDVIButton->setGeometry(QRect(480, 10, 141, 41));
        NDVIButton->setFont(font4);
        DataAna_Buttom = new QPushButton(centralWidget);
        DataAna_Buttom->setObjectName(QStringLiteral("DataAna_Buttom"));
        DataAna_Buttom->setEnabled(false);
        DataAna_Buttom->setGeometry(QRect(480, 50, 141, 41));
        DataAna_Buttom->setFont(font4);
        DataAna_Buttom->setToolTipDuration(6);
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
        TempLabel->setGeometry(QRect(490, 250, 128, 96));
        TrainingButtom = new QPushButton(centralWidget);
        TrainingButtom->setObjectName(QStringLiteral("TrainingButtom"));
        TrainingButtom->setGeometry(QRect(480, 90, 141, 41));
        TrainingButtom->setFont(font4);
        PredictButton = new QPushButton(centralWidget);
        PredictButton->setObjectName(QStringLiteral("PredictButton"));
        PredictButton->setEnabled(false);
        PredictButton->setGeometry(QRect(480, 200, 141, 41));
        PredictButton->setFont(font4);
        C_spinBox = new QDoubleSpinBox(centralWidget);
        C_spinBox->setObjectName(QStringLiteral("C_spinBox"));
        C_spinBox->setGeometry(QRect(520, 160, 101, 31));
        C_spinBox->setFont(font4);
        C_spinBox->setMaximum(512);
        C_spinBox->setValue(8);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(490, 160, 21, 31));
        label->setFont(font);
        RecentTrainCheckBox = new QCheckBox(centralWidget);
        RecentTrainCheckBox->setObjectName(QStringLiteral("RecentTrainCheckBox"));
        RecentTrainCheckBox->setGeometry(QRect(490, 140, 131, 16));
        RGBorNotcheckBox = new QCheckBox(centralWidget);
        RGBorNotcheckBox->setObjectName(QStringLiteral("RGBorNotcheckBox"));
        RGBorNotcheckBox->setGeometry(QRect(220, 310, 91, 21));
        RGBorNotcheckBox->setFont(font4);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 629, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0));
        actionTraining_Features->setText(QApplication::translate("MainWindow", "Analysis", 0));
        ApplyButton->setText(QApplication::translate("MainWindow", "Apply", 0));
        dateTimeEdit->setDisplayFormat(QApplication::translate("MainWindow", "yyyy/M/d AP hh:mm::ss", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "Setting", 0));
        RefLabel->setText(QApplication::translate("MainWindow", "Reference", 0));
        CaptureRefButton->setText(QApplication::translate("MainWindow", "Capture Reference", 0));
        LoadRefButton->setText(QApplication::translate("MainWindow", "Load Reference", 0));
        Timelabel->setText(QApplication::translate("MainWindow", "Time", 0));
        LoadWRefButton->setText(QApplication::translate("MainWindow", "Load White Ref", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Step 1", 0));
        PicLabel->setText(QApplication::translate("MainWindow", "Sample Pictures", 0));
        CapturePicButton->setText(QApplication::translate("MainWindow", "Capture Pictures", 0));
        LoadPicButton->setText(QApplication::translate("MainWindow", "Load Pictures", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Step 2", 0));
        CutLabel->setText(QApplication::translate("MainWindow", "Leaf Mask", 0));
        CutButton->setText(QApplication::translate("MainWindow", "Cut", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Step 3", 0));
        FalseColorLabel->setText(QApplication::translate("MainWindow", "Prediction", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Features", 0));
        checkBox1->setText(QApplication::translate("MainWindow", "700", 0));
        checkBox6->setText(QApplication::translate("MainWindow", "568-830", 0));
        checkBox11->setText(QApplication::translate("MainWindow", "750/568", 0));
        checkBox2->setText(QApplication::translate("MainWindow", "750", 0));
        checkBox7->setText(QApplication::translate("MainWindow", "700-750", 0));
        checkBox8->setText(QApplication::translate("MainWindow", "700-830", 0));
        checkBox9->setText(QApplication::translate("MainWindow", "750-830", 0));
        checkBox13->setText(QApplication::translate("MainWindow", "750/700", 0));
        checkBox12->setText(QApplication::translate("MainWindow", "830/568", 0));
        checkBox14->setText(QApplication::translate("MainWindow", "830/700", 0));
        checkBox0->setText(QApplication::translate("MainWindow", "568", 0));
        checkBox10->setText(QApplication::translate("MainWindow", "700/568", 0));
        checkBox3->setText(QApplication::translate("MainWindow", "830", 0));
        checkBox15->setText(QApplication::translate("MainWindow", "830/750", 0));
        checkBox4->setText(QApplication::translate("MainWindow", "568-700", 0));
        checkBox5->setText(QApplication::translate("MainWindow", "568-750", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Step 4", 0));
        ChooseButton->setText(QApplication::translate("MainWindow", "Choose Point", 0));
        RGBButtom->setText(QApplication::translate("MainWindow", "Open RGB", 0));
        FileName->setText(QApplication::translate("MainWindow", "File", 0));
        NDVIButton->setText(QApplication::translate("MainWindow", "NDVI", 0));
        DataAna_Buttom->setText(QApplication::translate("MainWindow", "Analize Data", 0));
        TempLabel->setText(QString());
        TrainingButtom->setText(QApplication::translate("MainWindow", "New SVM Training", 0));
        PredictButton->setText(QApplication::translate("MainWindow", "Predict", 0));
        label->setText(QApplication::translate("MainWindow", "  C", 0));
        RecentTrainCheckBox->setText(QApplication::translate("MainWindow", "Use Training Record", 0));
        RGBorNotcheckBox->setText(QApplication::translate("MainWindow", "With RGB", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
