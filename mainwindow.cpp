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
    flag =1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowOnLabel(cv::Mat mat, QLabel *k)
{
    if(mat.channels()==3)
    {
        cv::Mat t;
        cv::cvtColor(mat,t,CV_BGR2RGB);
        QImage qtemp = QImage((const unsigned char*)(t.data),t.cols,t.rows,t.step,QImage::Format_RGB888);
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
        FileAd = "D:/";//預設的開啟檔案位置
    }

    QStringList names = QFileDialog::getOpenFileNames(this,
                                                      "Select one or more files to open",
                                                      FileAd,
                                                      "Images (*.jpg)");
    if(names.size()!=4)
    {
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
    ui->FileName->setText(FileAd);
    FileNameAd = fn[0];
    Ref.clear();
    for(int i=0;i<4;i++)
    {
        cv::Mat temp = cv::imread(names[i].toStdString().c_str());
        Ref.push_back(temp);
    }
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

void MainWindow::on_LoadRefButton_clicked()
{
    QString refName;
    refPic.clear();
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

    //=========================Normailize  白校正程式
    std::vector<int> DNm;
    DNm.clear();

    for(int n=0;n<OPic.size();n++)
    {
        DNm.push_back(127);
    }

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

    for(int n=0;n<OPic.size();n++)
    {
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
                    f[n][i][j] = OPic[n].at<cv::Vec3b>(j,i)[0] + (128 - DNm[n]);
                }
            }
        }
    }
    std::vector<cv::Mat> showMat;
    showMat.clear();
    Pic.clear();
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
                    if(k<0)
                        k=0;

                    m.at<cv::Vec3b>(j,i)[0] = k;
                    m.at<cv::Vec3b>(j,i)[1] = k;
                    m.at<cv::Vec3b>(j,i)[2] = k;
                }
            }
        }

        showMat.push_back(m);
        Pic.push_back(m);
    }

    for(int n=0;n<OPic.size();n++)
    {
        for(int i=0;i<OPic[n].cols;i++)
        {
            delete [] f[n][i];
        }
        delete [] f[n];
    }
    delete [] f;
    //======================================白校正結束

    LoadPic(Pic,ui->PicLabel);
    for(int i=0;i<Pic.size();i++)
        cv::imwrite(QString::number(i).toStdString()+"_white.jpg",Pic[i]);
    WarpPic.clear();
    if(TransferWarp(Pic,WarpPic)!=1)
        return;

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
    analysisData.setRequest(Refresult,WarpPic,RefCorPoint);
    analysisData.BuildMaskMat(ui->ThresholdSlider1->value(),ui->ThresholdSlider2->value(),CutPic);
    maskResult = analysisData.getMaskMat();
    ShowOnLabel(maskResult,ui->CutLabel);
    ui->ChooseButton->setEnabled(true);
    ui->NDVIButton->setEnabled(true);
}

void MainWindow::on_PredictButton_clicked()
{
    cv::Mat predict;
    predict.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);

    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(NDVIMat.at<cv::Vec3b>(j,i)[1] == 255)
            {
                float value = predictresult(j,i);

                if(value == 0)//G
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
            }
        }
    }

    ShowOnLabel(predict,ui->FalseColorLabel);

    cv::imwrite(FileNameAd.toStdString()+"_pre.jpg",predict);
}

float MainWindow::predictresult(int y,int x)
{
    int features = ui->FeaturesSpinBox->value() ;

    std::vector<cv::Point> Point;

    for(int i=0;i<4;i++)
    {
        Point.push_back(RefCorPoint[i]);
    }

    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

    for(int i=0;i<4;i++)
    {
        t1.x = std::min(t1.x,RefCorPoint[i].x);
        t1.y = std::min(t1.y,RefCorPoint[i].y);
    }

    for(int n=4;n<16;n++)
    {
        Point.push_back(t1);
    }

    std::vector<int> dx;
    std::vector<int> dy;
    for(int n=0;n<16;n++)
    {
        dx.push_back(-t1.x+Point[n].x);
        dy.push_back(-t1.y+Point[n].y);
    }

    std::vector<int> TResult;
    TResult.clear();

    std::vector<int> result;
    result.clear();


    for(int n=0;n<16;n++)
    {
        if(y-dy[n]>=0 && y-dy[n]<CutPic[n].rows && x-dx[n]>=0 && x-dx[n]<CutPic[n].cols)
        {
            int pn;
            pn = CutPic[n].at<cv::Vec3b>(y-dy[n],x-dx[n])[0];
            TResult.push_back(pn);
        }
    }
    for(int i=0;i<Fnumber.size();i++)
    {
        result.push_back(TResult[Fnumber[i]]);
    }

    cv::Mat test(1,features,CV_32FC1);

    if(result.size()!=features)
    {
        qDebug()<<"Wrong!";
        return 100;
    }

    for(int j=0;j<features;j++)
    {
        test.at<float>(0,j) = result[j];
    }

    CvSVM svm;
    svm.load("SVM.txt");
    float label = svm.predict(test);
    return label;
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
    t.setTime(ctime);
    currentFile = t.getTimeString();
    ui->Timelabel->setText(t.getTimeString());
}

void MainWindow::update()
{
    t.updateTime(1);
    ui->Timelabel->setText(t.getTimeString());
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
        picdialog.initial(CutPic,RefCorPoint,ui->FileName->text());
    }
    return;
}

void MainWindow::on_LoadWRefButton_clicked()
{
    QString WName;
    WrefPic.clear();
    LoadFromFile(WrefPic ,WName);
    if(WrefPic.size()==0)
        return;

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

    if(CutPic.size() == 17)
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
            if(maskResult.at<cv::Vec3b>(j,i)[0] != 255 && j-dy0>=0 && j-dy0<CutPic[16].rows && i-dx0>=0 && i-dx0<CutPic[16].cols)
            {
                CutPic[16].at<cv::Vec3b>(j-dy0,i-dx0)[0]=0;
                CutPic[16].at<cv::Vec3b>(j-dy0,i-dx0)[1]=0;
                CutPic[16].at<cv::Vec3b>(j-dy0,i-dx0)[2]=0;
            }
        }
    }
    qDebug()<<CutPic[16].channels();
    ShowOnLabel(CutPic[16],ui->TempLabel);
}

void MainWindow::Div_value(std::vector<cv::Mat> &m,int y,int x,std::vector<float> &div)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
    {
        t1.x = std::min(t1.x,RefCorPoint[i].x);
        t1.y = std::min(t1.y,RefCorPoint[i].y);
    }

    std::vector<int> dy;
    std::vector<int> dx;

    for(int i=0;i<4;i++)
    {
        dy.push_back(-t1.y+RefCorPoint[i].y);
        dx.push_back(-t1.x+RefCorPoint[i].x);
    }

    std::vector<QString> result;

    for(int i=0;i<4;i++)
    {
        if(x-dx[i]>=0 && x-dx[i]<m[i].cols && y-dy[i] >=0 && y-dy[i]<m[i].rows)
        {
            int n = m[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
            result.push_back(QString::number(n));
        }
    }

    float div1,div2,div3,div4,div5,div6;
    if(result.size()!=4)
    {
        div1 = 0;
        div2 = 0;
        div3 = 0;
        div4 = 0;
        div5 = 0;
        div6 = 0;

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

void MainWindow::MinusPixel_value(std::vector<cv::Mat> &m,int y,int x,std::vector<int> &pixel)
{
    pixel.clear();
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
    {
        t1.x = std::min(t1.x,RefCorPoint[i].x);
        t1.y = std::min(t1.y,RefCorPoint[i].y);
    }

    std::vector<int> dy;
    std::vector<int> dx;

    for(int i=0;i<4;i++)
    {
        dy.push_back(-t1.y+RefCorPoint[i].y);
        dx.push_back(-t1.x+RefCorPoint[i].x);
    }

    std::vector<int> result;
    for(int i=0;i<4;i++)
    {
        if(x-dx[i]>=0 && x-dx[i]<m[i].cols && y-dy[i] >=0 && y-dy[i]<m[i].rows)
        {
            int n = m[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
            result.push_back(n);
        }
    }

    if(result.size()!=4 )
    {
        for(int n=0;n<6;n++)
            pixel.push_back(0);
        return;
    }

    int v0 = abs(result[0]-result[1]);
    int v1 = abs(result[0]-result[2]);
    int v2 = abs(result[0]-result[3]);
    int v3 = abs(result[1]-result[2]);
    int v4 = abs(result[1]-result[3]);
    int v5 = abs(result[2]-result[3]);

    pixel.push_back(v0);
    pixel.push_back(v1);
    pixel.push_back(v2);
    pixel.push_back(v3);
    pixel.push_back(v4);
    pixel.push_back(v5);
}

void MainWindow::on_NDVIButton_clicked()
{
    cv::Mat NDVI_Result=analysisData.NDVI();
    ShowOnLabel(NDVI_Result,ui->TempLabel);
    NDVIMat = NDVI_Result;
    ui->Multi_Buttom->setEnabled(true);
    cv::imwrite("NDVI.jpg",NDVIMat);
}

void MainWindow::on_Multi_Buttom_clicked()
{
    qDebug()<<"Initial Size "<<CutPic.size();

    std::vector<cv::Mat> MinusMat;//相減結果初始化
    for(int n=0;n<6;n++)
    {
        cv::Mat temp;
        temp.create(Refresult.rows,Refresult.cols,Refresult.type());
        temp = cv::Scalar::all(0);
        MinusMat.push_back(temp);
    }

    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(NDVIMat.at<cv::Vec3b>(j,i)[1] == 255)
            {
                //                std::vector<float>div;
                std::vector<int> minus_pixel;
                //                Div_value(CutPic,j,i,div);
                MinusPixel_value(CutPic,j,i,minus_pixel);
                for(int n=0;n<6;n++)
                {
                    //                    division[n].at<float>(Refresult.cols*j+i)=div[n];
                    MinusMat[n].at<cv::Vec3b>(j,i)[0]=minus_pixel[n];
                    MinusMat[n].at<cv::Vec3b>(j,i)[1]=minus_pixel[n];
                    MinusMat[n].at<cv::Vec3b>(j,i)[2]=minus_pixel[n];
                }
            }
        }
    }

    for(int n=0;n<6;n++)
    {
        cv::normalize(MinusMat[n],MinusMat[n],0,255,CV_MINMAX);
        cv::imwrite("MinusMat_"+QString::number(n).toStdString()+".jpg",MinusMat[n]);
    }

    if(CutPic.size()>4)
    {
        for(int n=0;n<6;n++)
            CutPic[n+4]=MinusMat[n];
    }
    else
    {
        for(int n=0;n<6;n++)
            CutPic.push_back(MinusMat[n]);
    }
    qDebug()<<"After Add Minus size ="<<CutPic.size();

    std::vector<cv::Mat> OriginalAvgMat;
    for(int n=0;n<4;n++)
    {
        int value[256]={0};
        int max_count = 0;
        int max_value=0;
        for(int i=0;i<CutPic[n].cols;i++)
        {
            for(int j=0;j<CutPic[n].rows;j++)
            {
                value[CutPic[n].at<cv::Vec3b>(j,i)[0]]++;
                if(max_count < value[CutPic[n].at<cv::Vec3b>(j,i)[0]] && CutPic[n].at<cv::Vec3b>(j,i)[0]!=0)
                {
                    max_count = value[CutPic[n].at<cv::Vec3b>(j,i)[0]];
                    max_value = CutPic[n].at<cv::Vec3b>(j,i)[0];
                }
            }
        }
        cv::Mat atmp;// = CutPic[n];
        atmp.create(CutPic[n].rows,CutPic[n].cols,CutPic[n].type());
        atmp = cv::Scalar::all(0);
        for(int i=0;i<CutPic[n].cols;i++)
        {
            for(int j=0;j<CutPic[n].rows;j++)
            {
                int new_value = CutPic[n].at<cv::Vec3b>(j,i)[0]+(127-max_value);
                if(new_value > 255)
                    new_value = 255;
                else if(new_value <0)
                    new_value = 0;
                atmp.at<cv::Vec3b>(j,i)[0] = new_value;
                atmp.at<cv::Vec3b>(j,i)[1] = new_value;
                atmp.at<cv::Vec3b>(j,i)[2] = new_value;
            }
        }
        OriginalAvgMat.push_back(atmp);
        cv::imwrite("normal_avg127_"+QString::number(n).toStdString()+".jpg",atmp);
    }
    std::vector<cv::Mat> division_normal;
    for(int i=0;i<6;i++)
    {
        cv::Mat k;
        k.create(Refresult.rows,Refresult.cols,CV_MAKETYPE(CV_32F,1));
        k = cv::Scalar::all(0);
        division_normal.push_back(k);
    }
    for(int i=0;i<Refresult.cols;i++)
    {
        for(int j=0;j<Refresult.rows;j++)
        {
            if(NDVIMat.at<cv::Vec3b>(j,i)[1] == 255)
            {
                std::vector<float>div;

                Div_value(OriginalAvgMat,j,i,div);

                for(int n=0;n<div.size();n++)
                {
                    division_normal[n].at<float>(Refresult.cols*j+i)=div[n];
                }
            }
        }
    }
    std::vector<cv::Mat> normalDivMat;
    for(int n=0;n<division_normal.size();n++)
    {
        cv::normalize(division_normal[n],division_normal[n],0,255,CV_MINMAX,CV_8U);//注意這邊還是只有one channel
        cv::Mat normalbgr;
        cv::cvtColor(division_normal[n],normalbgr,CV_GRAY2BGR);
        normalDivMat.push_back(normalbgr);
        cv::imwrite("division_normal_normalize_"+QString::number(n).toStdString()+".jpg",division_normal[n]);
    }

    if(CutPic.size()>10)
    {
        for(int n=0;n<normalDivMat.size();n++)
            CutPic[n+10]=normalDivMat[n];
    }
    else
    {
        for(int n=0;n<normalDivMat.size();n++)
            CutPic.push_back(normalDivMat[n]);
    }
    qDebug()<<"After Add Division size ="<<CutPic.size();

    qDebug()<<"Final CutPic Size = "<<CutPic.size();

    ui->RGBButtom->setEnabled(true);
}

void MainWindow::on_TrainingButtom_clicked()
{
    int features =0;
    Fnumber.clear();
    if(ui->checkBox0->isChecked()==true)
    {
        Fnumber.push_back(0);
        features++;
    }
    if(ui->checkBox1->isChecked()==true)
    {
        Fnumber.push_back(1);
        features++;
    }
    if(ui->checkBox2->isChecked()==true)
    {
        Fnumber.push_back(2);
        features++;
    }
    if(ui->checkBox3->isChecked()==true)
    {
        Fnumber.push_back(3);
        features++;
    }
    if(ui->checkBox4->isChecked()==true)
    {
        Fnumber.push_back(4);
        features++;
    }
    if(ui->checkBox5->isChecked()==true)
    {
        Fnumber.push_back(5);
        features++;
    }
    if(ui->checkBox6->isChecked()==true)
    {
        Fnumber.push_back(6);
        features++;
    }
    if(ui->checkBox7->isChecked()==true)
    {
        Fnumber.push_back(7);
        features++;
    }
    if(ui->checkBox8->isChecked()==true)
    {
        Fnumber.push_back(8);
        features++;
    }
    if(ui->checkBox9->isChecked()==true)
    {
        Fnumber.push_back(9);
        features++;
    }
    if(ui->checkBox10->isChecked()==true)
    {
        Fnumber.push_back(10);
        features++;
    }
    if(ui->checkBox11->isChecked()==true)
    {
        Fnumber.push_back(11);
        features++;
    }
    if(ui->checkBox12->isChecked()==true)
    {
        Fnumber.push_back(12);
        features++;
    }
    if(ui->checkBox13->isChecked()==true)
    {
        Fnumber.push_back(13);
        features++;
    }
    if(ui->checkBox14->isChecked()==true)
    {
        Fnumber.push_back(14);
        features++;
    }
    if(ui->checkBox15->isChecked()==true)
    {
        Fnumber.push_back(15);
        features++;
    }
    if(features==0)
        return;
    ui->FeaturesSpinBox->setValue(features);
    double C_param = ui->C_spinBox->value();
    SvmData svm(Fnumber,features,C_param);
    svm.initial();
}

void MainWindow::on_RecentTrainCheckBox_clicked()
{
    if(ui->RecentTrainCheckBox->isChecked() == true )
        ui->TrainingButtom->setEnabled(false);
    else
        ui->TrainingButtom->setEnabled(true);
}
