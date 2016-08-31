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
    FeatureNum  = 16 ;
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
    NStitch TSPic;
    if(TSPic.StLike(refPic,Pic,WarpPic,TS.getK(),TS.getCam())!=1)//套用原本的Warping Mask + 相對位置
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
                if(WRefMask[n].at<uchar>(j,i)!=255) //如果不是Warping原本的區塊，則扔掉不用
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
        cv::imwrite(QString::number(i).toStdString()+"_warp.jpg",WRef[i]);

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
        DNm.push_back(127);//直接假設平均灰階直為127
    }

    int ***f;//
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
    //================
//    std::vector<cv::Mat> showMat;
//    showMat.clear();
    Pic.clear();
    for(int n=0;n<OPic.size();n++)
    {
        cv::Mat m = OPic[n];
        for(int i=0;i<OPic[n].cols;i++)
        {
            for(int j=0;j<OPic[n].rows;j++)
            {
                if(OPic[n].at<cv::Vec3b>(j,i)[0]==0)//圖片本來就是黑的，不用做計算
                {
                }
                else
                {
                    int k = f[n][i][j]+(128-WrefPic[n].at<cv::Vec3b>(j,i)[0]);//WrefPic = WhiteReferencePicture 
                    if(k<0)
                        k=0;

                    m.at<cv::Vec3b>(j,i)[0] = k;
                    m.at<cv::Vec3b>(j,i)[1] = k;
                    m.at<cv::Vec3b>(j,i)[2] = k;
                }
            }
        }
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

    ui->CutButton->setEnabled(true);//切除背景的按鈕

    ui->ThresholdSlider1->setEnabled(true); //設定背景的區域範圍的bar
    ui->ThresholdSlider2->setEnabled(true); //
    ui->spinBox0->setEnabled(true);
    ui->spinBox1->setEnabled(true);
}

void MainWindow::on_spinBoxPic_valueChanged(int arg1)
{
    ShowOnLabel(Pic[arg1],ui->PicLabel);
}

void MainWindow::on_CutButton_clicked() //將葉片區塊切出來
{
    analysisData.setRequest(Refresult,WarpPic,RefCorPoint);
    analysisData.BuildMaskMat(ui->ThresholdSlider1->value(),ui->ThresholdSlider2->value(),CutPic);
    maskResult = analysisData.getMaskMat();
    ShowOnLabel(maskResult,ui->CutLabel);
    ui->ChooseButton->setEnabled(true);
    ui->NDVIButton->setEnabled(true);
}

void MainWindow::on_PredictButton_clicked() //預測按鈕，後面會call prediction.cpp的程式
{
    std::vector<int> Fnum;
    Features(Fnum);
    if(Fnum.size()==0)
        return;
    ui->FeaturesSpinBox->setValue(Fnum.size()); //顯示到底現在用了多少個Features 訓練
    Prediction preResult;
    preResult.Initial(NDVIMat,RefCorPoint,CutPic,Fnum);
    cv::Mat predict = preResult.SVMResult();
    ShowOnLabel(predict,ui->FalseColorLabel);
    cv::imwrite(FileNameAd.toStdString()+"_pre.jpg",predict);
}

void MainWindow::on_spinBox0_valueChanged(int arg1)
{
    ui->ThresholdSlider1->setValue(arg1);
}

void MainWindow::on_spinBox1_valueChanged(int arg1)
{
    ui->ThresholdSlider2->setValue(arg1);
}

void MainWindow::on_ThresholdSlider1_sliderMoved(int position)
{
    ui->spinBox0->setValue(position);
}

void MainWindow::on_ThresholdSlider2_sliderMoved(int position)
{
    ui->spinBox1->setValue(position);
}

void MainWindow::on_ApplyButton_clicked()   //將時間改正到對的時間點
{
    QDateTime ctime = ui->dateTimeEdit->dateTime();
    t.setTime(ctime);
    currentFile = t.getTimeString();
    ui->Timelabel->setText(t.getTimeString());
}

void MainWindow::update()   //時間update
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
        picdialog.initial(CutPic,RefCorPoint,ui->FileName->text(),FeatureNum);
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

void MainWindow::on_RGBButtom_clicked() //Load RGB 圖片
{
    QString Rgbname = QFileDialog::getOpenFileName(this, tr("Open RGB File"),
                                                   FileAd,
                                                   tr("Images (*.jpg)"));
    if(Rgbname.isEmpty())
        return;
    RGB = cv::imread(Rgbname.toStdString());

    NStitch Ts;
    std::vector<cv::Mat> change;
    for(int i=0;i<Pic.size();i++)
    {
        if(i==0)
            change.push_back(RGB);          //因為原本是四張圖片一起Warping，現在只有一張跟568nm一樣視角的RGB圖，假設成一張RGB圖跟另外三張多光譜圖一起Warping
        else
            change.push_back(Pic[i]);
    }
    std::vector<cv::Mat> warptemp;
    if(Ts.StLike(refPic,change,warptemp,TS.getK(),TS.getCam())!=1)  //RGB圖也要跟著Warping
        return;

    cv::Size s = WRefMask[0].size();        //我們只需要RGB的Warping就好了
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

    if(CutPic.size() == 17)     //如果已經有RGB圖片了
        CutPic.pop_back();      //刪掉原本的
    CutPic.push_back(warptemp[0]);
//    qDebug()<<CutPic.size();
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

    for(int i=0;i<maskResult.cols;i++)  //RGB加入扣除葉片區塊的遮罩
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


void MainWindow::Features(std::vector<int> &Fnum)  //這邊是在確定到底使用者勾了哪幾個Features 要拿來訓練跟預測
{
    Fnum.clear();
    if(ui->checkBox0->isChecked()==true)
        Fnum.push_back(0);
    if(ui->checkBox1->isChecked()==true)
        Fnum.push_back(1);
    if(ui->checkBox2->isChecked()==true)
        Fnum.push_back(2);
    if(ui->checkBox3->isChecked()==true)
        Fnum.push_back(3);
    if(ui->checkBox4->isChecked()==true)
        Fnum.push_back(4);
    if(ui->checkBox5->isChecked()==true)
        Fnum.push_back(5);
    if(ui->checkBox6->isChecked()==true)
        Fnum.push_back(6);
    if(ui->checkBox7->isChecked()==true)
        Fnum.push_back(7);
    if(ui->checkBox8->isChecked()==true)
        Fnum.push_back(8);
    if(ui->checkBox9->isChecked()==true)
        Fnum.push_back(9);
    if(ui->checkBox10->isChecked()==true)
        Fnum.push_back(10);
    if(ui->checkBox11->isChecked()==true)
        Fnum.push_back(11);
    if(ui->checkBox12->isChecked()==true)
        Fnum.push_back(12);
    if(ui->checkBox13->isChecked()==true)
        Fnum.push_back(13);
    if(ui->checkBox14->isChecked()==true)
        Fnum.push_back(14);
    if(ui->checkBox15->isChecked()==true)
        Fnum.push_back(15);
}

void MainWindow::on_NDVIButton_clicked()    //找出NDVI的區塊，如此一來可以避免掉預測更多多於的葉片部分
{
    cv::Mat NDVI_Result=analysisData.NDVI();
    ShowOnLabel(NDVI_Result,ui->TempLabel);
    NDVIMat = NDVI_Result;
    ui->DataAna_Buttom->setEnabled(true);
    cv::imwrite("NDVI.jpg",NDVIMat);
}

void MainWindow::on_DataAna_Buttom_clicked()    //分析數據，分析減法+除法的圖片M1~M6+D1~D6
{
    DataAnalysis dataAna;
    dataAna.Initial(CutPic,RefCorPoint,NDVIMat);
    dataAna.MinusMat();
    dataAna.DivisionMat();
    std::vector<cv::Mat> minusMat;
    std::vector<cv::Mat> divMat;
    dataAna.GetDataMat(minusMat,divMat);
    std::vector<cv::Mat> TempData;      //暫存空間，要放置原圖影像
    TempData.clear();
    for(int i=0;i<4;i++)
    {
        TempData.push_back(CutPic[i]);  //先把原始影像加入
    }
    CutPic.clear();                     //為了確保接下來分析圖片CutPic裡面的圖片順序是對的，先全部清空
    for(int i=0;i<TempData.size();i++)
        CutPic.push_back(TempData[i]);  //加入原圖
    for(int i=0;i<minusMat.size();i++)
        CutPic.push_back(minusMat[i]);  //加入M1~M6
    for(int i=0;i<divMat.size();i++)
        CutPic.push_back(divMat[i]);    //加入D1~D6

    qDebug()<<"CutPic Size without RGB = "<<CutPic.size();

    if(ui->RGBorNotcheckBox->isChecked()==true) //決定要不要把RGB圖片放進來
    {
        ui->RGBButtom->setEnabled(true);
        FeatureNum  = 17 ;
    }
    else
    {
        ui->RGBButtom->setEnabled(false);
        FeatureNum  = 16 ;
        ui->ChooseButton->setEnabled(true);
    }
    ui->PredictButton->setEnabled(true);
}

void MainWindow::on_TrainingButtom_clicked()
{
    std::vector<int> FNum;
    Features(FNum);

    if(FNum.size()==0)
        return;
    ui->FeaturesSpinBox->setValue(FNum.size());
    double C_param = ui->C_spinBox->value();
    SvmData svm(FNum,FNum.size(),C_param);
    svm.initial();
}

void MainWindow::on_RecentTrainCheckBox_clicked()   //拿以前的training 好的model來用
{
    if(ui->RecentTrainCheckBox->isChecked() == true ) //如果要拿以前的training model
    {
        ui->TrainingButtom->setEnabled(false);
        QFile file("FeatuerChannesl.txt");  //這邊是Load到底之前用了哪些features
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&file);
        QString line;
        while(!in.atEnd())
        {
            line = in.readLine();
        }
        QStringList f = line.split(",");
        for(int i=0;i<f.size();i++)
        {
            float k = f[i].toFloat();
            if(k==0)
                ui->checkBox0->setChecked(true);
            else if(k==1)
                ui->checkBox1->setChecked(true);
            else if(k==2)
                ui->checkBox2->setChecked(true);
            else if(k==3)
                ui->checkBox3->setChecked(true);
            else if(k==4)
                ui->checkBox4->setChecked(true);
            else if(k==5)
                ui->checkBox5->setChecked(true);
            else if(k==6)
                ui->checkBox6->setChecked(true);
            else if(k==7)
                ui->checkBox7->setChecked(true);
            else if(k==8)
                ui->checkBox8->setChecked(true);
            else if(k==9)
                ui->checkBox9->setChecked(true);
            else if(k==10)
                ui->checkBox10->setChecked(true);
            else if(k==11)
                ui->checkBox11->setChecked(true);
            else if(k==12)
                ui->checkBox12->setChecked(true);
            else if(k==13)
                ui->checkBox13->setChecked(true);
            else if(k==14)
                ui->checkBox14->setChecked(true);
            else if(k==15)
                ui->checkBox15->setChecked(true);

        }
        ui->groupBox->setEnabled(false);
        ui->PredictButton->setEnabled(true);

    }
    else    //如果不要拿以前的，要重新training
    {
        ui->groupBox->setEnabled(true);
        ui->TrainingButtom->setEnabled(true);
    }
}

void MainWindow::on_RGBorNotcheckBox_clicked()  //RGB CheckBox 偵測
{
    if(ui->RGBorNotcheckBox->isChecked()==true)
    {
        ui->RGBButtom->setEnabled(true);
        FeatureNum  = 17 ;
    }
    else
    {
        ui->RGBButtom->setEnabled(false);
        FeatureNum  = 16 ;
        ui->ChooseButton->setEnabled(true);
    }
}

