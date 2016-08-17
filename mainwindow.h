#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include "opencv.hpp"
#include "thread_stitch.h"
#include <QTime>
#include <string>
#include <QTimer>
#include <QDate>
#include <dialog.h>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ShowOnLabel(cv::Mat mat,QLabel *k);
    void LoadFromFile(std::vector<cv::Mat> &Ref ,QString &name);
    void LoadPic(std::vector<cv::Mat> &Ref,QLabel *k);
    void StitchMethod(std::vector<cv::Mat> &Ref,std::vector<cv::Mat> &WRef, std::vector<cv::Mat> &mask, cv::Mat &RefResult,std::vector<cv::Point> &CorPoint);
    int TransferWarp(std::vector<cv::Mat> &Pic,std::vector<cv::Mat> &WarpPic);
    void CutMask(int one,int two,cv::Mat &MaskResult);
    float predictresult(int y,int x);
//    float SVI_value(int y,int x);
    float NDVI_value(std::vector<cv::Mat> &m,int y,int x);
//    void Muti_value(int y,int x,float &div1,float &div2,float &div3,float &div4,float &div5,float &div6);
    void Div_value(std::vector<cv::Mat> &m,int y,int x,std::vector<float> &div);
//    void Pluspixel_value(std::vector<cv::Mat> &m,int y,int x,std::vector<int> &pixel);
    void MinusPixel_value(std::vector<cv::Mat> &m,int y,int x,std::vector<int> &pixel);

private slots:
    void on_LoadRefButton_clicked();

    void on_spinBoxRef_valueChanged(int arg1);

    void on_LoadPicButton_clicked();

    void on_spinBoxPic_valueChanged(int arg1);

    void on_CutButton_clicked();

    void on_PredictButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_ThresholdSlider1_sliderMoved(int position);

    void on_ThresholdSlider2_sliderMoved(int position);

    void on_ApplyButton_clicked();

    void update();

    void on_CaptureRefButton_clicked();

    void on_CapturePicButton_clicked();

    void on_ChooseButton_clicked();

    void on_SaveButton_clicked();

    void on_KnnPredictButtom_clicked();


    void on_pushButton_clicked();


    void on_LoadWRefButton_clicked();

    void on_RGBButtom_clicked();

    void on_NDVIButton_clicked();

    void on_Multi_Buttom_clicked();

    void on_TrainingButtom_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<cv::Mat> refPic;
    std::vector<cv::Mat> WrefPic;
    std::vector<cv::Mat> WarpWPic;
    std::vector<cv::Mat> Pic;
    std::vector<cv::Mat> OPic;
    std::vector<cv::Mat> WRefMask;
    cv::Mat Refresult;
    std::vector<cv::Mat> WarpPic;
    std::vector<cv::Mat> CutPic;
    Thread_Stitch TS;
    std::vector<cv::Point> RefCorPoint;
    cv::Mat maskResult;

    CvSVM svm;
    QDateTime Dtime;
    QTime currentTime;
    int time;

    QString currentFile;
    QString FirstFile;

    Dialog picdialog;

    cv::Mat RGB;

    QString FileAd;
    QString FileNameAd;

    cv::Mat NDVIMat;

    cv::Mat s;

    cv::Mat gk2;

    cv::Mat gdiv1;
    cv::Mat gdiv2;
    cv::Mat gdiv3;
    cv::Mat gdiv4;
    cv::Mat gdiv5;
    cv::Mat gdiv6;

    int flag;

    std::vector<int> Fnumber;
};

#endif // MAINWINDOW_H
