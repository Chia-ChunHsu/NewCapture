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
    FileAd= "";
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

void MainWindow::LoadFromFile(std::vector<cv::Mat> &Ref ,QString &File)
{
    if(FileAd=="")
    {
        FileAd = "D:/";
    }

    QStringList names = QFileDialog::getOpenFileNames(this,
                                                      "Select one or more files to open",
                                                      FileAd,
                                                      "Images (*.jpg)");
    if(names.size()!=4)
    {
        //Ref.clear();
        return;
    }
    File = names[0];
    qDebug()<<names[0];
    QStringList f = names[0].split("/");
    QStringList fn = names[3].split(".");
    FileAd.clear();
    FileNameAd.clear();
    for(int i=0;i<f.size()-2;i++)
    {
        FileAd=FileAd+f[i]+"/";
    }
    //FileAd=FileAd+f[f.size()-3];
    ui->FileName->setText(FileAd);
    FileNameAd = fn[0];

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
    //cv::imshow("WarpPic",WarpPic[0]);

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
                bool2 = ( tempWarp[one].at<cv::Vec3b>(j-y1,i-x1)[0]+tempWarp[one].at<cv::Vec3b>(j-y1,i-x1)[1]+tempWarp[one].at<cv::Vec3b>(j-y1,i-x1)[2])/3< threv2;
            }
            if(j-y1-20>0 && j-y2-20>0 && i-x1-20>0 && i-x2-20>0 && j-y1+20 <tempWarp[one].rows && i-x1+20<tempWarp[one].cols && j-y2+20 <tempWarp[two].rows && i-x2+20<tempWarp[two].cols )
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
    int erosion_size = 4;
    int dilation_elem = 0;
    int dilation_size = 4;//10

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
    QString refName;
    LoadFromFile(refPic ,refName);
    if(refPic.size()==0)
        return;
    ui->LoadPicButton->setEnabled(true);

    LoadPic(refPic,ui->RefLabel);
    std::vector<cv::Mat> WRef;

    RefCorPoint.clear();
    WRefMask.clear();

    StitchMethod(refPic,WRef,WRefMask,Refresult,RefCorPoint);

    for(int i=0;i<WRef.size();i++)
    {
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
    QString PicName;
    OPic.clear();
    LoadFromFile(OPic,PicName);
    if(OPic.size()==0)
        return;
    ui->FileName->setText(PicName);

        //=========================Normailize
        std::vector<int> DNm;
        DNm.clear();

        for(int n=0;n<OPic.size();n++)
        {
//            int total = 0;
//            int c = 0;
//            for(int i=0;i<OPic[n].cols;i++)
//            {
//                for(int j=0;j<OPic[n].rows;j++)
//                {

//                    //total = total + OPic[n].at<cv::Vec3b>(j,i)[0];
//                    if(OPic[n].at<cv::Vec3b>(j,i)[0] != 0)
//                    {
//                        total = total + OPic[n].at<cv::Vec3b>(j,i)[0];
//                        c++;
//                    }
//                }
//            }
//    //        DNm.push_back(total/c);
//            DNm.push_back(total/(OPic[n].cols * OPic[n].rows));
            DNm.push_back(127);
//                    qDebug()<< n<<" "<<DNm[n];
        }

        Pic.clear();
        int ***f;
        f = new int**[OPic.size()];
        for(int n=0;n<OPic.size();n++)
        {
            f[n] = new int*[OPic[n].cols];
            for(int i=0;i<OPic[n].cols;i++)
            {
                f[n][i] = new int[OPic[n].rows];
            }
        }
        qDebug()<<"1";

        for(int n=0;n<OPic.size();n++)
        {
            //Pic.push_back(OPic[n]);
            for(int i=0;i<OPic[n].cols;i++)
            {
                for(int j=0;j<OPic[n].rows;j++)
                {
                    if(OPic[n].at<cv::Vec3b>(j,i)[0] == 0)
                    {
                        f[n][i][j]=0;
                    }
                    else
                    {
                        f[n][i][j] = abs(OPic[n].at<cv::Vec3b>(j,i)[0] + (128 - DNm[n]));
                        //                    Pic[n].at<cv::Vec3b>(j,i)[1] = OPic[n].at<cv::Vec3b>(j,i)[1] + (128 - DNm[n]);
                        //                    Pic[n].at<cv::Vec3b>(j,i)[2] = OPic[n].at<cv::Vec3b>(j,i)[2] + (128 - DNm[n]);
                    }
                }
            }
        }
        qDebug()<<"2";
        //=================================
        //=======================Cut
        std::vector<cv::Mat> showMat;
        showMat.clear();
        for(int n=0;n<OPic.size();n++)
        {
            cv::Mat m = OPic[n];
            for(int i=0;i<OPic[n].cols;i++)
            {
                for(int j=0;j<OPic[n].rows;j++)
                {
                    if(OPic[n].at<cv::Vec3b>(j,i)[0]==0)
                    {

                    }
                    else
                    {
                        int k = f[n][i][j]+(128-WrefPic[n].at<cv::Vec3b>(j,i)[0]);

                        m.at<cv::Vec3b>(j,i)[0] = abs(k);
                        m.at<cv::Vec3b>(j,i)[1] = abs(k);
                        m.at<cv::Vec3b>(j,i)[2] = abs(k);

                    }
                }
            }


            showMat.push_back(m);
            Pic.push_back(m);
        }
        qDebug()<<"20";
        for(int n=0;n<OPic.size();n++)
        {
            for(int i=0;i<OPic[n].cols;i++)
            {
                delete [] f[n][i];
            }
            delete [] f[n];

        }
        delete [] f;
        qDebug()<<"21";
        qDebug()<<"3";
        //======================================


//    Pic.clear();
//    for(int i=0;i<OPic.size();i++)
//        Pic.push_back(OPic[i]);
    LoadPic(Pic,ui->PicLabel);
    for(int i=0;i<Pic.size();i++)
        cv::imwrite(QString::number(i).toStdString()+"_white.jpg",Pic[i]);
    WarpPic.clear();
    if(TransferWarp(Pic,WarpPic)!=1)
        return;
//    WarpWPic.clear();
//    if(TransferWarp(WrefPic,WarpWPic)!=1)
//        return;
    for(int i=0;i<WarpPic.size();i++)
    {
        cv::imwrite(QString::number(i).toStdString()+"_wwarp.jpg",WarpPic[i]);
    }
    qDebug()<<"pic size"<<Pic.size();
    ui->CutButton->setEnabled(true);

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
            if(NDVIMat.at<cv::Vec3b>(j,i)[1] == 255/*maskResult.at<cv::Vec3b>(j,i)[0] != 0*/)
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
                else if(value == 2)
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 0;
                    predict.at<cv::Vec3b>(j,i)[2] = 255;
                }
                else if(value == 3)
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 255;
                    predict.at<cv::Vec3b>(j,i)[1] = 255;
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

    cv::Mat cutP = predict.clone();

    for(int i=0;i<predict.cols;i++)
    {
        for(int j=0;j<predict.rows;j++)
        {
            if(NDVIMat.at<cv::Vec3b>(j,i)[1] == 255 && predict.at<cv::Vec3b>(j,i)[2] == 255 && s.at<float>(j*predict.cols+i) == 1)//Refresult.cols*j+i
            {
                cutP.at<cv::Vec3b>(j,i)[0]=0;
                cutP.at<cv::Vec3b>(j,i)[1]=255;
                cutP.at<cv::Vec3b>(j,i)[2]=0;
            }
            else
            {

            }
        }
    }
    cv::imshow("CutP",cutP);
//    QString saveResultFile;
//    if(FirstFile.isEmpty())
//        saveResultFile = QFileDialog::getSaveFileName(this, tr("Save File"),
//                                                      "untitled.jpg",
//                                                      tr("Images (*.jpg)"));
//    else
//        saveResultFile = FirstFile;
//    if(saveResultFile.isEmpty())
//    {
//        return;
//    }
//    else
//    {
        //cv::imwrite()
        cv::imwrite(FileNameAd.toStdString()+"_pre.jpg",predict);
        cv::imwrite(FileNameAd.toStdString()+"_cut_pre.jpg",cutP);
//    }

}

float MainWindow::predictresult(int y,int x)
{
    int features = 5;
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
    //result.push_back(QString::number(abs(CutPic[1].at<cv::Vec3b>(y-dy1,x-dx1)[0]-CutPic[2].at<cv::Vec3b>(y-dy2,x-dx2)[0])));
    result.push_back(QString::number(result[3].toFloat()/result[2].toFloat()));

    cv::Mat test(1,features,CV_32FC1);
    if(result.size()!=features)
        return 100;

    for(int j=0;j<features;j++)
    {
        test.at<float>(0,j) = result[j].toFloat();
    }

    svm.load("SVM.txt");
    float resultclass = svm.predict(test);
//    if(resultclass == 0)
//    {
//        CvSVM svm2;
//        svm2.load("SVM_HI.txt");
//        resultclass = svm2.predict(test);
//    }


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
        //RefCorPoint[0].x = RefCorPoint[0].x+10;
        //RefCorPoint[0].y = RefCorPoint[0].y-10;
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
            for(int j=0;j<6;j++)
            {
                QString str;
                in1>>str;

                if(!str.isNull())
                {
                    if(j==5)
                        l.push_back(str.toFloat());
                    else
                        s.push_back(str.toFloat());
                }
            }

        }
        file1.close();

        if(l.size()*5!=s.size())
        {
            qDebug()<<"label and features problem!";
            return;
        }
        cv::Mat trainingData(l.size(),5,CV_32FC1);
        cv::Mat label(l.size(),1,CV_32FC1);


        for(int i=0;i<s.size();i++)
        {
            trainingData.at<float>(i/5,i%5)=s[i];
        }
        for(int i=0;i<l.size();i++)
        {
            label.at<float>(i,0)=l[i];
        }

        //cv::Mat test(l.size(),4,CV_32FC1);
        cv::Mat *nearest;
        nearest = new cv::Mat(1,5,CV_32FC1);
        CvKNearest *knn;
        knn = new CvKNearest(trainingData,label,cv::Mat(),false,10);

    //    qDebug()<<"hello";

    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(NDVIMat.at<cv::Vec3b>(j,i)[1] == 255)
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
                result.push_back(QString::number(result[3].toFloat()/result[2].toFloat()));

                cv::Mat test(1,5,CV_32FC1);
                if(result.size()==5)
                {
                    for(int k=0;k<5;k++)
                    {
                        test.at<float>(0,k) = result[k].toFloat();
                    }

                    //====================
                    float value = knn->find_nearest(test,3,0,0,nearest,0);
//                    float value = test.at<float>(0,0) *(test.at<float>(0,2) + test.at<float>(0,3) - test.at<float>(0,1)*2 );
//                    float value = predictresult(j,i);
//                    float index = (test.at<float>(2,0)-test.at<float>(1,0))/(test.at<float>(1,0));
//                    float habs;
//                    float iabs;
//                    float sabs;
//                    habs = abs(-2.093*log(test.at<float>(2,0))-10.048-index);
//                    iabs = abs(-2.4591*log(test.at<float>(2,0))-11.154-index);
//                    sabs = abs(-2.0641*log(test.at<float>(2,0))-9.2553-index);
//                    //if(value > ui->upSlider->value())
                    if(value == 0)
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 0;
                    }
                    //else if(value < ui->downSlider->value())
                    else if(value ==2)
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 0;
                        predict.at<cv::Vec3b>(j,i)[2] = 255;
                    }
                    else if(value ==1 )
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 255;
                    }
                    else
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 255;
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
    QString WName;
    LoadFromFile(WrefPic ,WName);
    if(WrefPic.size()==0)
        return;
//    for(int n=0;n<WrefPic.size();n++)
//    {
//        int total = 0;
//        int count =0;
//        for(int i=0;i<WrefPic[n].cols;i++)
//        {
//            for(int j=0;j<WrefPic[n].rows;j++)
//            {
//                total = total + WrefPic[n].at<cv::Vec3b>(j,i)[0];
//                count ++;
//            }
//        }
//        qDebug()<<n<<" "<<total/count;
//    }
    ui->LoadRefButton->setEnabled(true);
}

void MainWindow::on_RGBButtom_clicked()
{
    QString Rgbname = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                   FileAd,
                                                   tr("Images (*.png *.xpm *.jpg)"));
    if(Rgbname.isEmpty())
        return;
    RGB = cv::imread(Rgbname.toStdString());

    Thread_Stitch Ts;
    std::vector<cv::Mat> change;
    for(int i=0;i<Pic.size();i++)
    {
        if(i==0)
            change.push_back(RGB);
        else
            change.push_back(Pic[i]);
    }
    std::vector<cv::Mat> warptemp;
    if(Ts.StLike(refPic,change,warptemp,TS.getK(),TS.getCam())!=1)
        return;

    cv::Size s = WRefMask[0].size();
    cv::resize(warptemp[0],warptemp[0],s);


    for(int i=0;i<warptemp[0].cols;i++)
    {
        for(int j=0;j<warptemp[0].rows;j++)
        {
            if(WRefMask[0].at<uchar>(j,i)!=255)
            {
                warptemp[0].at<cv::Vec3b>(j,i)[0]=0;
                warptemp[0].at<cv::Vec3b>(j,i)[1]=0;
                warptemp[0].at<cv::Vec3b>(j,i)[2]=0;
            }
        }
    }

    if(CutPic.size() == 11)
        CutPic.pop_back();
    CutPic.push_back(warptemp[0]);
    qDebug()<<CutPic.size();
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    cv::Point m1(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
    for(int i=0;i<RefCorPoint.size();i++)
    {
        t1.x = std::min(t1.x,RefCorPoint[i].x);
        t1.y = std::min(t1.y,RefCorPoint[i].y);
        m1.x = std::max(m1.x,RefCorPoint[i].x);
        m1.y = std::max(m1.y,RefCorPoint[i].y);
    }
    int dy0 = -t1.y+RefCorPoint[0].y;
    int dx0 = -t1.x+RefCorPoint[0].x;

    for(int i=0;i<maskResult.cols;i++)
    {
        for(int j=0;j<maskResult.rows;j++)
        {
            if(maskResult.at<cv::Vec3b>(j,i)[0] != 255 && j-dy0>=0 && j-dy0<CutPic[10].rows && i-dx0>=0 && i-dx0<CutPic[10].cols)
            {
                CutPic[10].at<cv::Vec3b>(j-dy0,i-dx0)[0]=0;
                CutPic[10].at<cv::Vec3b>(j-dy0,i-dx0)[1]=0;
                CutPic[10].at<cv::Vec3b>(j-dy0,i-dx0)[2]=0;
            }
        }
    }
    cv::imshow("RGB_Cut",CutPic[10]);

}

void MainWindow::on_TestButtom_clicked()
{
    //=========================Normailize
    std::vector<int> DNm;
    DNm.clear();
    for(int n=0;n<OPic.size();n++)
    {
        int total = 0;
        int count = 0;
        for(int i=0;i<OPic[n].cols;i++)
        {
            for(int j=0;j<OPic[n].rows;j++)
            {
                if(CutPic[n].at<cv::Vec3b>(j,i)[0] !=0)
                {
                    total = total + OPic[n].at<cv::Vec3b>(j,i)[0];
                    count++;
                }
            }
        }
        DNm.push_back(total/count);

    }
    std::vector<cv::Mat> testPic;
    testPic.clear();
    for(int n=0;n<OPic.size();n++)
    {
        testPic.push_back(OPic[n]);
        for(int i=0;i<OPic[n].cols;i++)
        {
            for(int j=0;j<OPic[n].rows;j++)
            {
                if(CutPic[n].at<cv::Vec3b>(j,i)[0] !=0)
                {
                    testPic[n].at<cv::Vec3b>(j,i)[0] = OPic[n].at<cv::Vec3b>(j,i)[0] + (128 - DNm[n]);
                    testPic[n].at<cv::Vec3b>(j,i)[1] = OPic[n].at<cv::Vec3b>(j,i)[1] + (128 - DNm[n]);
                    testPic[n].at<cv::Vec3b>(j,i)[2] = OPic[n].at<cv::Vec3b>(j,i)[2] + (128 - DNm[n]);
                }
            }
        }
    }
    //=================================
    //=======================Cut
    std::vector<cv::Mat> showMat;
    showMat.clear();
    for(int n=0;n<testPic.size();n++)
    {
        cv::Mat m = testPic[n];
        for(int i=0;i<testPic[n].cols;i++)
        {
            for(int j=0;j<testPic[n].rows;j++)
            {
                int k = m.at<cv::Vec3b>(j,i)[0]+(128-WrefPic[n].at<cv::Vec3b>(j,i)[0]);
                m.at<cv::Vec3b>(j,i)[0] = k;
                m.at<cv::Vec3b>(j,i)[1] = k;
                m.at<cv::Vec3b>(j,i)[2] = k;
            }
        }
        testPic[n] = m.clone();
        cv::imshow(QString::number(n).toStdString(),testPic[n]);
        //showMat.push_back(m);
        //Pic[n] = m.clone();
    }
}

void MainWindow::on_upSlider_sliderMoved(int position)
{
    cv::Mat predict;
    predict.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);

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
                    //float value = knn->find_nearest(test,3,0,0,nearest,0);
                    float value = test.at<float>(0,0) *(test.at<float>(0,2) + test.at<float>(0,3) - test.at<float>(0,1)*2 );
                    //float value = predictresult(j,i);
                    ui->up_label->setText(QString::number(ui->upSlider->value()));
                    ui->down_label->setText(QString::number(ui->downSlider->value()));
                    ui->label_750->setText(QString::number(ui->Slider750->value()));
                    if(value > ui->upSlider->value() )
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 0;
                    }
                    else if(value < ui->downSlider->value())
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 0;
                        predict.at<cv::Vec3b>(j,i)[2] = 255;
                    }
                    else if(test.at<float>(0,2) >ui->Slider750->value())
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 0;
                    }
                    else
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
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
}

void MainWindow::on_downSlider_sliderMoved(int position)
{
    cv::Mat predict;
    predict.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);

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
                    //float value = knn->find_nearest(test,3,0,0,nearest,0);
                    float value = test.at<float>(0,0) *(test.at<float>(0,2) + test.at<float>(0,3) - test.at<float>(0,1)*2 );
                    //float value = predictresult(j,i);
                    ui->up_label->setText(QString::number(ui->upSlider->value()));
                    ui->down_label->setText(QString::number(ui->downSlider->value()));
                    ui->label_750->setText(QString::number(ui->Slider750->value()));

                    if(value > ui->upSlider->value())
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 0;
                    }
                    else if(value < ui->downSlider->value())
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 0;
                        predict.at<cv::Vec3b>(j,i)[2] = 255;
                    }
                    else if(test.at<float>(0,2) >ui->Slider750->value())
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 0;
                    }
                    else
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
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

}

void MainWindow::on_Slider750_sliderMoved(int position)
{
    cv::Mat predict;
    predict.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);

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
                    //float value = knn->find_nearest(test,3,0,0,nearest,0);
                    float value = test.at<float>(0,0) *(test.at<float>(0,2) + test.at<float>(0,3) - test.at<float>(0,1)*2 );
                    //float value = predictresult(j,i);
                    ui->up_label->setText(QString::number(ui->upSlider->value()));
                    ui->down_label->setText(QString::number(ui->downSlider->value()));
                    ui->label_750->setText(QString::number(ui->Slider750->value()));

                    if(value > ui->upSlider->value())
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 0;
                    }
                    else if(value < ui->downSlider->value())
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 0;
                        predict.at<cv::Vec3b>(j,i)[2] = 255;
                    }
                    else if(test.at<float>(0,2) >ui->Slider750->value())
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
                        predict.at<cv::Vec3b>(j,i)[2] = 0;
                    }
                    else
                    {
                        predict.at<cv::Vec3b>(j,i)[0] = 0;
                        predict.at<cv::Vec3b>(j,i)[1] = 255;
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
}


void MainWindow::on_SVI_Button_clicked()
{
    cv::Mat predict;
    predict.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);
    //svm.load("SVM.txt");
    qDebug()<<"000";
    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(maskResult.at<cv::Vec3b>(j,i)[0] != 0)
            {
                float value = SVI_value(j,i);
                if(value < ui->doubleSpinBox->value())
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 0;
                    predict.at<cv::Vec3b>(j,i)[2] = 255;
                }
//                else if(value <0.02) //shadow
//                {
//                    predict.at<cv::Vec3b>(j,i)[0] = 0;
//                    predict.at<cv::Vec3b>(j,i)[1] = 255;
//                    predict.at<cv::Vec3b>(j,i)[2] = 255;
//                }
                else
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 255;
                    predict.at<cv::Vec3b>(j,i)[2] = 0;
                }
            }
        }
    }
    //cv::imshow("Result Predict",predict);
//    cv::Mat k ;
//    cv::cvtColor(predict,k,CV_BGR2RGB);
//    ShowOnLabel(k,ui->FalseColorLabel);
    cv::imshow("svi",predict);
}

float MainWindow::SVI_value(int y,int x)
{
    //qDebug()<<"001";
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
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
    //result.push_back(QString::number(abs(CutPic[1].at<cv::Vec3b>(y-dy1,x-dx1)[0]-CutPic[2].at<cv::Vec3b>(y-dy2,x-dx2)[0])));

    if(result.size()!=4)
        return -1.0;
    //qDebug()<<result[2].toFloat()<<" "<<result[1].toFloat();

    if(result[2].toFloat()+result[1].toFloat() == 0)
            return -1.0;
    float ans = (result[2].toFloat()-result[1].toFloat()*20/400)/(result[2].toFloat()+result[1].toFloat()*20/400)*result[2].toFloat();

    //qDebug()<<(result[2].toFloat()-result[1].toFloat())<<" "<<(result[2].toFloat()+result[1].toFloat())<<" "<<ans;



    return ans;
}

float MainWindow::NDVI_value(int y, int x)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
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

    if(result.size()!=4)
        return -1.0;

    if(result[2].toFloat()+result[1].toFloat() == 0)
            return -1.0;
    float ans = (result[2].toFloat()-result[1].toFloat()*20/400)/(result[2].toFloat()+result[1].toFloat()*20/400);


    return ans;
}

void MainWindow::Muti_value(int y,int x,float &div1,float &div2,float &div3,float &div4,float &div5,float &div6)//div1=1/0 div2=2/0 div3=3/0 div4=2/1  div5=3/1 div6=3/2
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
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

    if(result.size()!=4)
    {
        div1 = 0;
        div2 = 0;
        return;
    }
    //div1=1/0 div2=2/0 div3=3/0 div4=2/1  div5=3/1 div6=3/2
    if(result[0]== 0)
    {
        div1 = 1;
        div2 = 1;
        div3 = 1;
    }
    else
    {
        div1 = result[1].toFloat()/result[0].toFloat();
        div2 = result[2].toFloat()/result[0].toFloat();
        div3 = result[3].toFloat()/result[0].toFloat();
    }
    if(result[1]==0)
    {
        div4 = 1;
        div5 = 1;
    }
    else
    {
        div4 = result[2].toFloat()/result[1].toFloat();
        div5 = result[3].toFloat()/result[1].toFloat();
    }
    if(result[2]== 0)
    {
        div6 = 1;
    }
    else
    {
        div6 = result[3].toFloat()/result[2].toFloat();
    }
}

void MainWindow::Div_value(int y,int x,std::vector<float> &div)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
    {
        t1.x = std::min(t1.x,RefCorPoint[i].x);
        t1.y = std::min(t1.y,RefCorPoint[i].y);
    }

    std::vector<int> dy;
    std::vector<int> dx;

    for(int i=0;i<RefCorPoint.size();i++)
    {
        dy.push_back(-t1.y+RefCorPoint[i].y);
        dx.push_back(-t1.x+RefCorPoint[i].x);
    }

    int cutsize = 1;

    std::vector<QString> result;

    for(int i=0;i<RefCorPoint.size();i++)
    {
        if(x-dx[i]-cutsize>1 && x-dx[i]+cutsize <CutPic[i].cols-1 && y-dy[i]-cutsize >1 && y-dy[i]+cutsize<CutPic[i].rows-1)
        {
            int n = CutPic[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
            result.push_back(QString::number(n));
        }
    }

    float div1,div2,div3,div4,div5,div6;
    if(result.size()!=4)
    {
        div1 = 0;
        div2 = 0;
        return;
    }
    //div1=1/0 div2=2/0 div3=3/0 div4=2/1  div5=3/1 div6=3/2
    if(result[0]== 0)
    {
        div1 = 1;
        div2 = 1;
        div3 = 1;
    }
    else
    {
        div1 = result[1].toFloat()/result[0].toFloat();
        div2 = result[2].toFloat()/result[0].toFloat();
        div3 = result[3].toFloat()/result[0].toFloat();
    }
    if(result[1]==0)
    {
        div4 = 1;
        div5 = 1;
    }
    else
    {
        div4 = result[2].toFloat()/result[1].toFloat();
        div5 = result[3].toFloat()/result[1].toFloat();
    }
    if(result[2]== 0)
    {
        div6 = 1;
    }
    else
    {
        div6 = result[3].toFloat()/result[2].toFloat();
    }
    div.push_back(div1);
    div.push_back(div2);
    div.push_back(div3);
    div.push_back(div4);
    div.push_back(div5);
    div.push_back(div6);
}

void MainWindow::on_NDVIButton_clicked()
{
    cv::Mat predict;
    predict.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);

    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(maskResult.at<cv::Vec3b>(j,i)[0] != 0)
            {
                float value = NDVI_value(j,i);
                //ui->MultidoubleSpinBox_2
                if(value < ui->NDVIdoubleSpinBox->value())
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 0;
                    predict.at<cv::Vec3b>(j,i)[2] = 255;
                }
                else
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 255;
                    predict.at<cv::Vec3b>(j,i)[2] = 0;
                }
            }
        }
    }
    cv::imshow("Multi",predict);
    NDVIMat = predict.clone();
}

void MainWindow::on_Multi_Buttom_clicked()
{
    std::vector<cv::Mat> division;
    for(int i=0;i<6;i++)
    {
        cv::Mat temp;
        temp.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(CV_32F,1));
        temp = cv::Scalar::all(0);
        division.push_back(temp);
    }

    //svm.load("SVM.txt");
    qDebug()<<"000";
    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(NDVIMat.at<cv::Vec3b>(j,i)[1] == 255)
            {
                //div1=1/0 div2=2/0 div3=3/0 div4=2/1  div5=3/1 div6=3/2
                std::vector<float>div;
                //Muti_value(j,i,div1,div2,div3,div4,div5,div6);
                Div_value(j,i,div);

                for(int n=0;n<6;n++)
                {
                    division[n].at<float>(Refresult.cols*j+i)=div[n];
                }
            }
        }
    }
    std::vector<cv::Mat> dst;

    for(int n=0;n<division.size();n++)
    {
        cv::normalize(division[n],division[n],0,255,CV_MINMAX,CV_8UC3);//注意這邊還是只有one channel
        cv::Mat temp;
        cv::cvtColor(division[n],temp,CV_GRAY2BGR);
        dst.push_back(temp);
        cv::imshow(QString::number(n).toStdString()+"_dst",temp);
        qDebug()<<n<<" "<<temp.type();
        CutPic.push_back(temp);
    }
}

bool MainWindow::ShadowPlace(int y, int x)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
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

    if(result.size()!=4)
    {
        return false;
    }
        //return;

    if(result[0].toFloat() < ui->TCutdoubleSpinBox1->value()  &&  result[1].toFloat() <ui->TCutdoubleSpinBox2->value()&&  result[2].toFloat() < ui->TCutdoubleSpinBox3->value()&&  result[3].toFloat() <ui->TCutdoubleSpinBox4->value())
    {
        return true;
    }
    else
    {
        return false;
    }

}
