#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <opencv.hpp>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void initial(std::vector<cv::Mat> cutPic,std::vector<cv::Point> RefCorPoint );


private slots:
    bool eventFilter(QObject *obj,QEvent *event);//事件偵測(滑鼠)
    void on_spinBox_valueChanged(int arg1);

    void on_saveButton_clicked();


    void on_RGBButton_clicked();

    void on_xSpinBox_valueChanged(int arg1);

    void on_ySpinBox_valueChanged(int arg1);

private:
    Ui::Dialog *ui;
    void ShowOnLabel(cv::Mat mat,QLabel *k);
    void draw(std::vector<cv::Mat> &mat, int x, int y, QLabel *k);
    void show(std::vector<cv::Mat> &mat, int x, int y, QLabel *k);
    std::vector<cv::Mat> Omat;
    std::vector<cv::Point> CorPoint;
    std::vector<std::vector<int>> data;
    std::vector<cv::Mat> temp;

    //std::vector<cv::Mat> labelmat;

    cv::Mat res;


};

#endif // DIALOG_H
