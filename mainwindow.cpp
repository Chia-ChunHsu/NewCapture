#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->setInterval(1000);
    timer->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowOnLabel(cv::Mat mat, QLabel *k)
{
    if(mat.channels()==3)
    {
        //cv::cvtColor(mat,mat,CV_BGR2RGB);
        QImage qtemp = QImage((const unsigned char*)(mat.data),mat.cols,mat.rows,mat.step,QImage::Format_RGB888);
        k->clear();
        k->setPixmap(QPixmap::fromImage(qtemp.scaled(k->width(),k->height(),Qt::KeepAspectRatio)));
        k->show();
    }
    else if(mat.channels() == 1)
    {
        QVector<QRgb> colorTable;
        for(int i=0;i<256;i++)
        {
            colorTable.push_back(qRgb(i,i,i));
        }
        QImage qtemp = QImage((const unsigned char*)(mat.data),mat.cols,mat.rows,mat.step,QImage::Format_Indexed8);
        qtemp.setColorTable(colorTable);
        k->clear();
        k->setPixmap(QPixmap::fromImage(qtemp.scaled(k->width(),k->height(),Qt::KeepAspectRatio)));
        k->show();
    }
}

void MainWindow::LoadFromFile(std::vector<cv::Mat> &Ref)
{

    QStringList names = QFileDialog::getOpenFileNames(this,
                                                      "Select one or more files to open",
                                                      "/home",
                                                      "Images (*.jpg)");
    if(names.size()!=4)
    {
        //Ref.clear();
        return;
    }

    Ref.clear();
    for(int i=0;i<4;i++)
    {
        cv::Mat temp = cv::imread(names[i].toStdString().c_str());
        Ref.push_back(temp);
    }
    //ui->fileLabel->setText(name[0]);
}

void MainWindow::LoadPic(std::vector<cv::Mat> &Ref,QLabel *k)
{
    ShowOnLabel(Ref[0],k);
}

void MainWindow::StitchMethod(std::vector<cv::Mat> &Ref, std::vector<cv::Mat> &WRef, std::vector<cv::Mat> &mask, cv::Mat &RefResult, std::vector<cv::Point> &CorPoint)
{
    if(TS.Warp(Ref,WRef,mask,RefResult,CorPoint)!=1)
    {
        return;
    }
}

int MainWindow::TransferWarp(std::vector<cv::Mat> &Pic, std::vector<cv::Mat> &WarpPic)
{
    Thread_Stitch TSPic;
    if(TSPic.StLike(refPic,Pic,WarpPic,TS.getK(),TS.getCam())!=1)
        return 0;
    for(int i=0;i<4;i++)
    {
        cv::Size s = WRefMask[i].size();
        cv::resize(WarpPic[i],WarpPic[i],s);
    }
    for(int n=0;n<Pic.size();n++)
    {
        //cv::Mat temp = WarpPic[n].clone();
        for(int i=0;i<WarpPic[n].cols;i++)
        {
            for(int j=0;j<WarpPic[n].rows;j++)
            {
                if(WRefMask[n].at<uchar>(j,i)!=255)
                {
                    WarpPic[n].at<cv::Vec3b>(j,i)[0]=0;
                    WarpPic[n].at<cv::Vec3b>(j,i)[1]=0;
                    WarpPic[n].at<cv::Vec3b>(j,i)[2]=0;
                }
            }
        }
    }

    return 1;
}

void MainWindow::CutMask(int one,int two,cv::Mat &MaskResult)
{
    cv::Mat shadow;

    //RCapWarp.clear();
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    cv::Point m1(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
    for(int i=0;i<RefCorPoint.size();i++)
    {
        t1.x = std::min(t1.x,RefCorPoint[i].x);
        t1.y = std::min(t1.y,RefCorPoint[i].y);
        m1.x = std::max(m1.x,RefCorPoint[i].x);
        m1.y = std::max(m1.y,RefCorPoint[i].y);
    }
    cv::imwrite("ref.jpg",Refresult);
    cv::Size size(Refresult.cols,Refresult.rows);

    shadow.create(size,CV_MAKETYPE(Refresult.type(),3));
    shadow = cv::Scalar::all(0);

    std::vector<cv::Mat> tempWarp;
    tempWarp.clear();
    for(int i=0;i<WarpPic.size();i++)
    {
        tempWarp.push_back(WarpPic[i]);
    }

    int x1 = RefCorPoint[one].x-t1.x;
    int y1 = RefCorPoint[one].y-t1.y;

    int threv1 = ui->ThresholdSlider1->value();
    int threv2 = ui->ThresholdSlider2->value();

    int y2 = RefCorPoint[two].y-t1.y;
    int x2 = RefCorPoint[two].x-t1.x;

    for(int i = 0;i<shadow.cols;i++)
    {
        for(int j=0;j<shadow.rows;j++)
        {
            bool bool1 = false;
            bool bool2 = false;
            bool bool3 = false;
            if(j>=y2 && i >=x2 && j <tempWarp[two].rows+y2 && i <tempWarp[two].cols+x2)
            {
                bool1 = ( tempWarp[two].at<cv::Vec3b>(j-y2,i-x2)[0]+tempWarp[two].at<cv::Vec3b>(j-y2,i-x2)[1]+tempWarp[two].at<cv::Vec3b>(j-y2,i-x2)[2])/3 > threv1;
            }
            if(j>=y1 && i>=x1 && j<tempWarp[one].rows+y1 && i<tempWarp[one].cols+x1)
            {
                bool2 = ( tempWarp[one].at<cv::Vec3b>(j-y1,i-x1)[0]+tempWarp[one].at<cv::Vec3b>(j-y1,i-x1)[1]+tempWarp[one].at<cv::Vec3b>(j-y1,i-x1)[2])/3 < threv2;
            }
            if(j-y1-30>0 && j-y2-30>0 && i-x1-30>0 && i-x2-30>0 && j-y1+30 <tempWarp[one].rows && i-x1+30<tempWarp[one].cols && j-y2+30 <tempWarp[two].rows && i-x2+30<tempWarp[two].cols )
            {
                bool3 = true;
            }
            if(bool1==true  && bool2 == true && bool3 == true )
            {
                shadow.at<cv::Vec3b>(j,i)[0] = 255;
                shadow.at<cv::Vec3b>(j,i)[1] = 255;
                shadow.at<cv::Vec3b>(j,i)[2] = 255;
            }
            else
            {
                shadow.at<cv::Vec3b>(j,i)[0] = 0;
                shadow.at<cv::Vec3b>(j,i)[1] = 0;
                shadow.at<cv::Vec3b>(j,i)[2] = 0;
            }
        }
    }
    int erosion_elem = 0;
    int erosion_size = 5;
    int dilation_elem = 0;
    int dilation_size = 5;//10

    int erosion_type;
    if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = cv::MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }

    int dilation_type;
    if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = cv::MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }

    cv::Mat eroMat;
    cv::Mat dilMat;
    cv::Mat eroelement = getStructuringElement( erosion_type,
                                                cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                                cv::Point( erosion_size, erosion_size ) );
    cv::Mat dilelement = getStructuringElement( dilation_type,
                                                cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                                cv::Point( dilation_size, dilation_size ) );
    cv::erode(shadow,eroMat,eroelement);
    cv::dilate(eroMat,dilMat,dilelement);
    cv::imwrite("shadow.jpg",shadow);
    cv::imwrite("eroMat.jpg",eroMat);
    cv::imwrite("dilMat.jpg",dilMat);


    MaskResult = dilMat.clone();

    //=========================


    std::vector<cv::Mat> temp;
    temp.clear();
    for(int i=0;i<WarpPic.size();i++)
    {
        cv::Mat tMat = WarpPic[i].clone();
        temp.push_back(tMat);
    }

    cv::Mat tk;
    tk.create(dilMat.rows,dilMat.cols,CV_MAKETYPE(tk.type(),3));
    tk = cv::Scalar::all(0);

    int dy0 = -t1.y+RefCorPoint[0].y;
    int dx0 = -t1.x+RefCorPoint[0].x;
    int dy1 = -t1.y+RefCorPoint[1].y;
    int dx1 = -t1.x+RefCorPoint[1].x;
    int dy2 = -t1.y+RefCorPoint[2].y;
    int dx2 = -t1.x+RefCorPoint[2].x;
    int dy3 = -t1.y+RefCorPoint[3].y;
    int dx3 = -t1.x+RefCorPoint[3].x;

    for(int i=0;i<dilMat.cols;i++)
    {
        for(int j=0;j<dilMat.rows;j++)
        {
            if(dilMat.at<cv::Vec3b>(j,i)[0] != 255 && j-dy0>=0 && j-dy0<temp[0].rows && i-dx0>=0 && i-dx0<temp[0].cols)
            {
                temp[0].at<cv::Vec3b>(j-dy0,i-dx0)[0]=0;
                temp[0].at<cv::Vec3b>(j-dy0,i-dx0)[1]=0;
                temp[0].at<cv::Vec3b>(j-dy0,i-dx0)[2]=0;
            }

            if(dilMat.at<cv::Vec3b>(j,i)[0] != 255 && j-dy1>=0 && j-dy1<temp[1].rows && i-dx1>=0 && i-dx1<temp[1].cols)
            {
                temp[1].at<cv::Vec3b>(j-dy1,i-dx1)[0]=0;
                temp[1].at<cv::Vec3b>(j-dy1,i-dx1)[1]=0;
                temp[1].at<cv::Vec3b>(j-dy1,i-dx1)[2]=0;
            }
            if(dilMat.at<cv::Vec3b>(j,i)[0] != 255 && j-dy2>=0 && j-dy2<temp[2].rows && i-dx2>=0 && i-dx2<temp[2].cols)
            {

                temp[2].at<cv::Vec3b>(j-dy2,i-dx2)[0]=0;
                temp[2].at<cv::Vec3b>(j-dy2,i-dx2)[1]=0;
                temp[2].at<cv::Vec3b>(j-dy2,i-dx2)[2]=0;
            }
            if(dilMat.at<cv::Vec3b>(j,i)[0] != 255 && j-dy3>=0 && j-dy3<temp[3].rows && i-dx3>=0 && i-dx3<temp[3].cols)
            {
                temp[3].at<cv::Vec3b>(j-dy3,i-dx3)[0]=0;
                temp[3].at<cv::Vec3b>(j-dy3,i-dx3)[1]=0;
                temp[3].at<cv::Vec3b>(j-dy3,i-dx3)[2]=0;
            }

        }
    }
    for(int i=0;i<temp.size();i++)
    {
        cv::imwrite("temp"+QString::number(i+1).toStdString()+".jpg",temp[i]);
    }
    CutPic.clear();
    for(int i=0;i<temp.size();i++)
        CutPic.push_back(temp[i]);

    ui->RGBButtom->setEnabled(true);



}

void MainWindow::on_LoadRefButton_clicked()
{
    LoadFromFile(refPic);
    if(refPic.size()==0)
        return;
    ui->LoadPicButton->setEnabled(true);

//    //=========================Normailize
//    std::vector<int> DNm;
//    DNm.clear();
//    for(int n=0;n<refPic.size();n++)
//    {
//        int total = 0;
//        for(int i=0;i<refPic[n].cols;i++)
//        {
//            for(int j=0;j<refPic[n].rows;j++)
//            {
//                total = total + refPic[n].at<cv::Vec3b>(j,i)[0];
//            }
//        }
//        DNm.push_back(total/(refPic[n].cols * refPic[n].rows));
//    }

//    std::vector<cv::Mat> temp;
//    temp.clear();
//    for(int n=0;n<refPic.size();n++)
//    {
//        temp.push_back(refPic[n]);
//        for(int i=0;i<refPic[n].cols;i++)
//        {
//            for(int j=0;j<refPic[n].rows;j++)
//            {
//                temp[n].at<cv::Vec3b>(j,i)[0] = refPic[n].at<cv::Vec3b>(j,i)[0] + (128 - DNm[n]);
//                temp[n].at<cv::Vec3b>(j,i)[1] = refPic[n].at<cv::Vec3b>(j,i)[1] + (128 - DNm[n]);
//                temp[n].at<cv::Vec3b>(j,i)[2] = refPic[n].at<cv::Vec3b>(j,i)[2] + (128 - DNm[n]);
//            }
//        }
//    }
//    //=================================
//    //=======================Cut
//    std::vector<cv::Mat> showMat;
//    showMat.clear();
//    for(int n=0;n<temp.size();n++)
//    {
//        cv::Mat m = temp[n];
//        for(int i=0;i<temp[n].cols;i++)
//        {
//            for(int j=0;j<temp[n].rows;j++)
//            {
//                int k = m.at<cv::Vec3b>(j,i)[0]+(128-WrefPic[n].at<cv::Vec3b>(j,i)[0]);
//                m.at<cv::Vec3b>(j,i)[0] = k;
//                m.at<cv::Vec3b>(j,i)[1] = k;
//                m.at<cv::Vec3b>(j,i)[2] = k;
//            }
//        }

//        showMat.push_back(m);
//        refPic[n] = m.clone();
//    }
//    //======================================

//    for(int n=0;n<refPic.size();n++)
//    {
//        for(int i=0;i<refPic[n].cols;i++)
//        {
//            for(int j=0;j<refPic[n].rows;j++)
//            {
//                if(refPic[n].at<cv::Vec3b>(j,i)[0]<130)
//                {
//                    refPic[n].at<cv::Vec3b>(j,i)[0] = 0;
//                    refPic[n].at<cv::Vec3b>(j,i)[1] = 0;
//                    refPic[n].at<cv::Vec3b>(j,i)[2] = 0;
//                }
//                else
//                {
//                    refPic[n].at<cv::Vec3b>(j,i)[0] = 255;
//                    refPic[n].at<cv::Vec3b>(j,i)[1] = 255;
//                    refPic[n].at<cv::Vec3b>(j,i)[2] = 255;
//                }
//            }
//        }
//    }

    LoadPic(refPic,ui->RefLabel);
    std::vector<cv::Mat> WRef;
    RefCorPoint.clear();



    StitchMethod(refPic,WRef,WRefMask,Refresult,RefCorPoint);
    for(int i=0;i<WRef.size();i++)
    {
        //cv::imshow(QString::number(i).toStdString(),WRef[i]);
        cv::imwrite(QString::number(i).toStdString()+"_warp.jpg",WRef[i]);
    }
    qDebug()<<"REF "<<refPic.size()<<"RefCorPoint "<<RefCorPoint.size();
}

void MainWindow::on_spinBoxRef_valueChanged(int arg1)
{
    ShowOnLabel(refPic[arg1],ui->RefLabel);
}

void MainWindow::on_LoadPicButton_clicked()
{
    OPic.clear();
    LoadFromFile(OPic);
    if(OPic.size()==0)
        return;

    //=========================Normailize
    std::vector<int> DNm;
    DNm.clear();
    for(int n=0;n<OPic.size();n++)
    {
        int total = 0;
        for(int i=0;i<OPic[n].cols;i++)
        {
            for(int j=0;j<OPic[n].rows;j++)
            {
                total = total + OPic[n].at<cv::Vec3b>(j,i)[0];
            }
        }
        DNm.push_back(total/(OPic[n].cols * OPic[n].rows));
    }

    Pic.clear();
    for(int n=0;n<OPic.size();n++)
    {
        Pic.push_back(OPic[n]);
        for(int i=0;i<OPic[n].cols;i++)
        {
            for(int j=0;j<OPic[n].rows;j++)
            {
                Pic[n].at<cv::Vec3b>(j,i)[0] = OPic[n].at<cv::Vec3b>(j,i)[0] + (128 - DNm[n]);
                Pic[n].at<cv::Vec3b>(j,i)[1] = OPic[n].at<cv::Vec3b>(j,i)[1] + (128 - DNm[n]);
                Pic[n].at<cv::Vec3b>(j,i)[2] = OPic[n].at<cv::Vec3b>(j,i)[2] + (128 - DNm[n]);
            }
        }
    }
    //=================================
    //=======================Cut
    std::vector<cv::Mat> showMat;
    showMat.clear();
    for(int n=0;n<Pic.size();n++)
    {
        cv::Mat m = Pic[n];
        for(int i=0;i<Pic[n].cols;i++)
        {
            for(int j=0;j<Pic[n].rows;j++)
            {
                int k = m.at<cv::Vec3b>(j,i)[0]+(128-WrefPic[n].at<cv::Vec3b>(j,i)[0]);
                m.at<cv::Vec3b>(j,i)[0] = k;
                m.at<cv::Vec3b>(j,i)[1] = k;
                m.at<cv::Vec3b>(j,i)[2] = k;
            }
        }

        showMat.push_back(m);
        Pic[n] = m.clone();
    }
    //======================================




    LoadPic(Pic,ui->PicLabel);
    for(int i=0;i<Pic.size();i++)
        cv::imwrite(QString::number(i).toStdString()+"_white.jpg",Pic[i]);
    WarpPic.clear();
    if(TransferWarp(Pic,WarpPic)!=1)
        return;
    for(int i=0;i<WarpPic.size();i++)
    {
        cv::imwrite(QString::number(i).toStdString()+"_wwarp.jpg",WarpPic[i]);
    }
    qDebug()<<"pic size"<<Pic.size();
    ui->CutButton->setEnabled(true);
    ui->CutRadioButton->setEnabled(true);
    ui->OriginalRadioButton->setEnabled(true);
    ui->ThresholdSlider1->setEnabled(true);
    ui->ThresholdSlider2->setEnabled(true);
}

void MainWindow::on_spinBoxPic_valueChanged(int arg1)
{
    ShowOnLabel(Pic[arg1],ui->PicLabel);
}

void MainWindow::on_CutButton_clicked()
{
    CutMask(ui->spinBoxT1->value(),ui->spinBoxT2->value(),maskResult);
    ShowOnLabel(maskResult,ui->CutLabel);

    ui->ChooseButton->setEnabled(true);
}

void MainWindow::on_PredictButton_clicked()
{
    cv::Mat predict;
    predict.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);
    //svm.load("SVM.txt");
    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(maskResult.at<cv::Vec3b>(j,i)[0] != 0)
            {
                float value = predictresult(j,i);
                if(value == 0)
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 255;
                    predict.at<cv::Vec3b>(j,i)[2] = 0;
                }
                else if(value == 1)
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 255;
                    predict.at<cv::Vec3b>(j,i)[2] = 255;
                }
                else
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 0;
                    predict.at<cv::Vec3b>(j,i)[2] = 255;
                }
            }
        }
    }
    //cv::imshow("Result Predict",predict);
    cv::Mat k ;
    cv::cvtColor(predict,k,CV_BGR2RGB);
    ShowOnLabel(k,ui->FalseColorLabel);
    cv::imshow("pre",predict);
    QString saveResultFile;
    if(FirstFile.isEmpty())
        saveResultFile = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                          "untitled.jpg",
                                                          tr("Images (*.jpg)"));
    else
        saveResultFile = FirstFile;
    if(saveResultFile.isEmpty())
    {
        return;
    }
    else
    {
        //cv::imwrite()
        cv::imwrite(saveResultFile.toStdString(),predict);
    }

}

float MainWindow::predictresult(int y,int x)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<RefCorPoint.size();i++)
    {
        t1.x = std::min(t1.x,RefCorPoint[i].x);
        t1.y = std::min(t1.y,RefCorPoint[i].y);
    }

    int dy0 = -t1.y+RefCorPoint[0].y;
    int dx0 = -t1.x+RefCorPoint[0].x;
    int dy1 = -t1.y+RefCorPoint[1].y;
    int dx1 = -t1.x+RefCorPoint[1].x;
    int dy2 = -t1.y+RefCorPoint[2].y;
    int dx2 = -t1.x+RefCorPoint[2].x;
    int dy3 = -t1.y+RefCorPoint[3].y;
    int dx3 = -t1.x+RefCorPoint[3].x;


    int cutsize = 1;
    std::vector<QString> result;
    if(x-dx0-cutsize>1 && x-dx0+cutsize <CutPic[0].cols-1 && y-dy0-cutsize >1 && y-dy0+cutsize<CutPic[0].rows-1)
    {
        int n = CutPic[0].at<cv::Vec3b>(y-dy0,x-dx0)[0];
        result.push_back(QString::number(n));
    }
    if(x-dx1-cutsize>1 && x-dx1+cutsize <CutPic[1].cols-1 && y-dy1-cutsize >1 && y-dy1+cutsize<CutPic[1].rows-1)
    {
        int n = CutPic[1].at<cv::Vec3b>(y-dy1,x-dx1)[0];
        result.push_back(QString::number(n));
    }
    if(x-dx2-cutsize>1 && x-dx2+cutsize <CutPic[2].cols-1 && y-dy2-cutsize >1 && y-dy2+cutsize<CutPic[2].rows-1)
    {
        int n = CutPic[2].at<cv::Vec3b>(y-dy2,x-dx2)[0];
        result.push_back(QString::number(n));
    }
    if(x-dx3-cutsize>1 && x-dx3+cutsize <CutPic[3].cols-1 && y-dy3-cutsize >1 && y-dy3+cutsize<CutPic[3].rows-1)
    {
        int n = CutPic[3].at<cv::Vec3b>(y-dy3,x-dx3)[0];
        result.push_back(QString::number(n));
    }
    cv::Mat test(1,4,CV_32FC1);
    if(result.size()!=4)
        return 100;

    for(int j=0;j<4;j++)
    {
        test.at<float>(0,j) = result[j].toFloat();
    }

    svm.load("SVM.txt");
    float resultclass = svm.predict(test);

    float finalresult = resultclass;



    return finalresult;

}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->ThresholdSlider1->setValue(arg1);
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    ui->ThresholdSlider2->setValue(arg1);
}

void MainWindow::on_ThresholdSlider1_sliderMoved(int position)
{
    ui->spinBox->setValue(position);
}

void MainWindow::on_ThresholdSlider2_sliderMoved(int position)
{
    ui->spinBox_2->setValue(position);
}

void MainWindow::on_ApplyButton_clicked()
{
    QDateTime ctime = ui->dateTimeEdit->dateTime();
    //Dtime = ui->dateTimeEdit->dateTime();
    QString year = QString::number(ctime.date().year());
    QString month = QString::number(ctime.date().month());
    QString day = QString::number(ctime.date().day());
    QString time = ctime.time().toString();
//    QString hour = QString::number(Dtime.time().hour());
//    QString min = QString::number(Dtime.time().minute());
//    QString sec = QString::number(Dtime.time().second());
    ui->label->setText(year+"/"+month+"/"+day+" "+time);

    //currentTime = Dtime.time();
    Dtime = ctime;
    currentFile = year+"/"+month+"/"+day+" "+time;


}

void MainWindow::update()
{

    //qDebug() << "update";
   ui->dateTimeEdit->setDate(Dtime.date());
   Dtime = Dtime.addSecs(1);
   QString year = QString::number(Dtime.date().year());
   QString month = QString::number(Dtime.date().month());
   QString day = QString::number(Dtime.date().day());
   QString time = Dtime.time().toString();
   ui->label->setText(year+"/"+month+"/"+day+" "+time);
   //ui->dateTimeEdit->setTime(currentTime);

}

void MainWindow::on_CaptureRefButton_clicked()
{

}

void MainWindow::on_CapturePicButton_clicked()
{
    FirstFile = currentFile;
}

void MainWindow::on_ChooseButton_clicked()
{
    if(!CutPic.empty())
    {
        picdialog.initial(CutPic,RefCorPoint);
    }
    return;
}

void MainWindow::on_SaveButton_clicked()
{

}

void MainWindow::on_KnnPredictButtom_clicked()
{
    cv::Mat predict;
    predict.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);
    //svm.load("SVM.txt");//_______________________________

    QString test_file = QFileDialog::getOpenFileName(this,tr("Train Data"),".",tr("Data File(*.txt)"));
    if(test_file.isEmpty())
    {
        qDebug()<<"test_file is empty!";
        return;
    }


    std::vector<float> s;
    std::vector<float> l;

    s.clear();
    l.clear();
    QFile file1(test_file);
    file1.open(QIODevice::ReadOnly);
    QTextStream in1(&file1);
    while(!in1.atEnd())
    {
        for(int j=0;j<5;j++)
        {
            QString str;
            in1>>str;

            if(!str.isNull())
            {
                if(j==4)
                    l.push_back(str.toFloat());
                else
                    s.push_back(str.toFloat());
            }
        }

    }
    file1.close();

    if(l.size()*4!=s.size())
    {
        qDebug()<<"label and features problem!";
        return;
    }
    cv::Mat trainingData(l.size(),4,CV_32FC1);
    cv::Mat label(l.size(),1,CV_32FC1);


    for(int i=0;i<s.size();i++)
    {
        trainingData.at<float>(i/4,i%4)=s[i];
    }
    for(int i=0;i<l.size();i++)
    {
        label.at<float>(i,0)=l[i];
    }

    //cv::Mat test(l.size(),4,CV_32FC1);
    cv::Mat *nearest;
    nearest = new cv::Mat(1,3,CV_32FC1);
    CvKNearest *knn;
    knn = new CvKNearest(trainingData,label,cv::Mat(),false,10);

    qDebug()<<"hello";
    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(maskResult.at<cv::Vec3b>(j,i)[0] != 0)
            {
                cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
                for(int f=0;f<RefCorPoint.size();f++)
                {
                    t1.x = std::min(t1.x,RefCorPoint[f].x);
                    t1.y = std::min(t1.y,RefCorPoint[f].y);
                }

                int dy0 = -t1.y+RefCorPoint[0].y;
                int dx0 = -t1.x+RefCorPoint[0].x;
                int dy1 = -t1.y+RefCorPoint[1].y;
                int dx1 = -t1.x+RefCorPoint[1].x;
                int dy2 = -t1.y+RefCorPoint[2].y;
                int dx2 = -t1.x+RefCorPoint[2].x;
                int dy3 = -t1.y+RefCorPoint[3].y;
                int dx3 = -t1.x+RefCorPoint[3].x;


                int cutsize = 1;
                std::vector<QString> result;
                if(i-dx0-cutsize>1 && i-dx0+cutsize <CutPic[0].cols-1 && j-dy0-cutsize >1 && j-dy0+cutsize<CutPic[0].rows-1)
                {
                    int n = CutPic[0].at<cv::Vec3b>(j-dy0,i-dx0)[0];
                    result.push_back(QString::number(n));
                }
                if(i-dx1-cutsize>1 && i-dx1+cutsize <CutPic[1].cols-1 && j-dy1-cutsize >1 && j-dy1+cutsize<CutPic[1].rows-1)
                {
                    int n = CutPic[1].at<cv::Vec3b>(j-dy1,i-dx1)[0];
                    result.push_back(QString::number(n));
                }
                if(i-dx2-cutsize>1 && i-dx2+cutsize <CutPic[2].cols-1 && j-dy2-cutsize >1 && j-dy2+cutsize<CutPic[2].rows-1)
                {
                    int n = CutPic[2].at<cv::Vec3b>(j-dy2,i-dx2)[0];
                    result.push_back(QString::number(n));
                }
                if(i-dx3-cutsize>1 && i-dx3+cutsize <CutPic[3].cols-1 && j-dy3-cutsize >1 && j-dy3+cutsize<CutPic[3].rows-1)
                {
                    int n = CutPic[3].at<cv::Vec3b>(j-dy3,i-dx3)[0];
                    result.push_back(QString::number(n));
                }
                cv::Mat test(1,4,CV_32FC1);
                if(result.size()==4)
                {
                    for(int k=0;k<4;k++)
                    {
                        test.at<float>(0,k) = result[k].toFloat();
                    }

                    //====================
                    float value = knn->find_nearest(test,3,0,0,nearest,0);
                    //float value = predictresult(j,i);
                    if(value ==0)
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 0;
                    }
                    else if(value == 1)
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 255;
                    }
                    else
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 0;
                        predict.at<cv::Vec3b>(j,i)[2] = 255;
                    }
                }
            }
        }
    }
    //cv::imshow("Result Predict",predict);
    cv::Mat k ;
    cv::cvtColor(predict,k,CV_BGR2RGB);
    ShowOnLabel(k,ui->FalseColorLabel);
    cv::imshow("predict",predict);
    qDebug()<<"O.K.";
//    QString saveResultFile;
//    if(FirstFile.isEmpty())
//        saveResultFile = QFileDialog::getSaveFileName(this, tr("Save File"),
//                                                          "untitled.jpg",
//                                                          tr("Images (*.jpg)"));
//    else
//        saveResultFile = FirstFile;
//    if(saveResultFile.isEmpty())
//    {
//        return;
//    }
//    else
//    {
//        //cv::imwrite()
//        cv::imwrite(saveResultFile.toStdString(),predict);
//    }
}



void MainWindow::on_pushButton_clicked()
{
    cv::VideoCapture cap(0);
    cap.open(0);
    cv::Mat m;
    for(int i=0;i<10000;i++)
    {
        cap>>m;

    }
    cv::imshow("m",m);
    cap.release();

}

void MainWindow::on_LoadWRefButton_clicked()
{
    LoadFromFile(WrefPic);
    if(WrefPic.size()==0)
        return;
    ui->LoadRefButton->setEnabled(true);
}



void MainWindow::on_RGBButtom_clicked()
{
    QString Rgbname = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                   "/home",
                                                   tr("Images (*.png *.xpm *.jpg)"));
    if(Rgbname.isEmpty())
        return;
    RGB = cv::imread(Rgbname.toStdString());

    Thread_Stitch Ts;
    std::vector<cv::Mat> change;
    for(int i=0;i<Pic.size();i++)
    {
        if(i==2)
            change.push_back(RGB);
        else
            change.push_back(Pic[i]);
    }
    std::vector<cv::Mat> warptemp;
    if(Ts.StLike(refPic,change,warptemp,TS.getK(),TS.getCam())!=1)
        return;
    for(int i=0;i<4;i++)
    {
        cv::Size s = WRefMask[i].size();
        cv::resize(warptemp[i],warptemp[i],s);
    }

        //cv::Mat temp = WarpPic[n].clone();
        for(int i=0;i<warptemp[2].cols;i++)
        {
            for(int j=0;j<warptemp[2].rows;j++)
            {
                if(WRefMask[2].at<uchar>(j,i)!=255)
                {
                    warptemp[2].at<cv::Vec3b>(j,i)[0]=0;
                    warptemp[2].at<cv::Vec3b>(j,i)[1]=0;
                    warptemp[2].at<cv::Vec3b>(j,i)[2]=0;
                }
            }
        }
        if(CutPic.size() == 5)
            CutPic.pop_back();
        CutPic.push_back(warptemp[2]);
        qDebug()<<CutPic.size();
        cv::imshow("test",CutPic[4]);

}
