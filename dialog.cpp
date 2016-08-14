#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initial(std::vector<cv::Mat> cutPic, std::vector<cv::Point> RefCorPoint, QString File)
{
    this->open();
    ui->labelall->setGeometry(ui->labelall->x(),ui->labelall->y(),cutPic[0].cols,cutPic[0].rows);
    ShowOnLabel(cutPic[0],ui->labelall);
    Omat.clear();
    CorPoint.clear();
    temp.clear();
    //labelmat.clear();
    data.clear();
    dataName = File;
    Xposition.clear();
    Yposition.clear();
    temp.assign(cutPic.begin(),cutPic.end());
    Omat.assign(cutPic.begin(),cutPic.end());

    for(int i=0;i<cutPic.size()-13;i++)
    {
        CorPoint.push_back(RefCorPoint[i]);
    }
    //cv::Point p(RefCorPoint[0].x,RefCorPoint[0].y);
    for(int n=0;n<12;n++)
    {
        CorPoint.push_back(RefCorPoint[0]);
    }
    CorPoint.push_back(RefCorPoint[0]);
    qApp->installEventFilter(this);
    //qDebug()<<"0 "<<cutPic[4].cols<<" "<<cutPic[4].rows<<" "<<cutPic[0].cols;
    for(int i=0;i<CorPoint.size();i++)
    {
        qDebug()<<i<<" "<<temp[i].cols<<" "<<temp[i].rows<<" "<<CorPoint[i].x<<" "<<CorPoint[i].y;
    }
}

void Dialog::ShowOnLabel(cv::Mat mat, QLabel *k)
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

bool Dialog::eventFilter(QObject *obj, QEvent *event)
{
  QString str;
  QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

  if( event->type() == QMouseEvent::MouseMove && obj->isWidgetType()&& obj == ui->labelall)
  {
      QPointF pt = mouseEvent->localPos();
      event = static_cast<QMouseEvent*>(event);
      str = "MouseMove" + QString::number(pt.x()) +", " + QString::number(pt.y());
      setWindowTitle(str);                            // 顯示座標
      show(temp,pt.x(),pt.y(),ui->labelScale);
      return false;

  }else if(event->type() == QMouseEvent::MouseButtonPress && obj->isWidgetType() && obj == ui->labelall){
      QPointF pt = mouseEvent->localPos();
      str = "MouseButtonPress" + QString::number(pt.rx()) +", " + QString::number(pt.ry());
      setWindowTitle(str);                            // 顯示座標

      int x = pt.rx();
      int y = pt.ry();

      //draw(temp,x,y,ui->labelall);
      draw(temp,pt.x(),pt.y(),ui->labelall);
      show(temp,pt.x(),pt.y(),ui->labelScale);
  }


  return false;
}

void Dialog::draw(std::vector<cv::Mat> &m, int x, int y, QLabel *k)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<CorPoint.size();i++)
    {
        t1.x = std::min(t1.x,CorPoint[i].x);
        t1.y = std::min(t1.y,CorPoint[i].y);
    }

    for(int n=4;n<16;n++)
    {
        CorPoint[n]=t1;
    }

    std::vector<int> dy;
    std::vector<int> dx;

    for(int i=0;i<m.size();i++)
    {
        for(int spin=1;spin<=ui->spinBox->maximum();spin++)
        if(ui->spinBox->value()==spin)
        {
            dy.push_back((-t1.y+CorPoint[i].y)-(CorPoint[spin-1].y-CorPoint[0].y));
            dx.push_back((-t1.x+CorPoint[i].x)-(CorPoint[spin-1].x-CorPoint[0].x));
        }
    }

    std::vector<int> tempdata;
    tempdata.clear();


    std::vector<int> t;
    for(int i=0;i<m.size();i++)
    {
        if(i<16)
        {
            int n = m[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
            tempdata.push_back(n);
        }
        for(int i=0;i<3;i++)
        {
            int color = m[16].at<cv::Vec3b>(y-dy[16],x-dx[16])[i];
            t.push_back(color);
        }
        cv::Point p(x-dx[i],y-dy[i]);
        cv::circle(m[i],p,1,cv::Scalar(0,0,255),-1,8);
        cv::Mat t;
        cv::cvtColor(m[i],t,CV_BGR2RGB);
        cv::imwrite(QString::number(i).toStdString()+"point.jpg",m[i]);
    }
    ui->PositionX->setText(QString::number(x));
    ui->PositionY->setText(QString::number(y));

    Xposition.push_back(x);
    Yposition.push_back(y);

    data.push_back(tempdata);
    ui->DataNumber->setText(QString::number(data.size()));

//    ui->label->setText(QString::number(t[2]));
    RGBData.push_back(t);
    ShowOnLabel(temp[ui->spinBox->value()-1],k);
}

void Dialog::show(std::vector<cv::Mat> &mat, int x, int y, QLabel *k)
{
    //尋找四張多光譜中的最小值數值
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<CorPoint.size();i++)
    {
        t1.x = std::min(t1.x,CorPoint[i].x);
        t1.y = std::min(t1.y,CorPoint[i].y);
    }

    for(int n=4;n<16;n++)
    {
        CorPoint[n]=t1;
    }

    std::vector<int> dx;//記錄各張圖片的相對位置關係
    std::vector<int> dy;

    //qDebug()<<"mat size = "<<mat.size();

    for(int n=0;n<mat.size();n++)
    {
        for(int spin = 1;spin<=ui->spinBox->maximum();spin++)
        {
            if(ui->spinBox->value()==spin)
            {
                dx.push_back(-t1.x+CorPoint[n].x-(CorPoint[spin-1].x-CorPoint[0].x));
                dy.push_back(-t1.y+CorPoint[n].y-(CorPoint[spin-1].y-CorPoint[0].y));
            }
        }
    }
    int gap = 30;
    for(int i=0;i<mat.size();i++)
    {
        if(x-gap-dx[i]<0 || x+gap-dx[i]>=mat[i].cols || y-gap-dy[i]<0 || y+gap-dy[i]>=mat[i].rows)
        {
            return;
        }
    }

    std::vector<cv::Mat> roi;
    roi.clear();

    std::vector<cv::Mat> show_roi;
    show_roi.clear();

    for(int n=0;n<mat.size();n++)
    {
        cv::Mat kt = mat[n]; //複製一張原圖
        //範圍設定 r
        cv::Rect r = cv::Rect(x-gap-dx[n],y-gap-dy[n],2*gap,2*gap);

        cv::Mat troi = kt(r);

        roi.push_back(troi);
        cv::Mat temp = roi[n].clone();
        show_roi.push_back(temp);

        //計算各張圖的灰階值
        int pn;

        pn= mat[n].at<cv::Vec3b>(y-dy[n],x-dx[n])[0];

        if(n==0)
            ui->label1->setText(QString::number(pn));
        else if(n==1)
            ui->label2->setText(QString::number(pn));
        else if(n==2)
            ui->label3->setText(QString::number(pn));
        else if(n==3)
            ui->label4->setText(QString::number(pn));
        else if(n==4)
            ui->pn1->setText(QString::number(pn));
        else if(n==5)
            ui->pn2->setText(QString::number(pn));
        else if(n==6)
            ui->pn3->setText(QString::number(pn));
        else if(n==7)
            ui->pn4->setText(QString::number(pn));
        else if(n==8)
            ui->pn5->setText(QString::number(pn));
        else if(n==9)
            ui->pn6->setText(QString::number(pn));
        else if(n==10)
            ui->ndiv1->setText(QString::number(pn));
        else if(n==11)
            ui->ndiv2->setText(QString::number(pn));
        else if(n==12)
            ui->ndiv3->setText(QString::number(pn));
        else if(n==13)
            ui->ndiv4->setText(QString::number(pn));
        else if(n==14)
            ui->ndiv5->setText(QString::number(pn));
        else if(n==15)
            ui->ndiv6->setText(QString::number(pn));

        cv::circle(show_roi[n],cv::Point(gap,gap),1,cv::Scalar(0,0,255),-1,8);
    }

    ShowOnLabel(show_roi[ui->spinBox->value()-1],k);
}


void Dialog::on_spinBox_valueChanged(int arg1)
{
    ui->labelall->setGeometry(ui->labelall->x(),ui->labelall->y(),Omat[arg1-1].cols,Omat[arg1-1].rows);
    ShowOnLabel(temp[arg1-1],ui->labelall);
}

void Dialog::on_saveButton_clicked()
{
    QString name = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                "D:/Data/MultiSpectral_Device_Data/0903_0904_set/0903_0904trainData/",
                                                tr("Data "));

    QStringList tName = dataName.split("/");
    QStringList FName = tName[tName.size()-1].split(".");
    QString SubName;
    if(ui->radioButtonH->isChecked() == true)
        SubName = "H";
    else if(ui->radioButtonI->isChecked() == true)
        SubName = "I";
    else
        SubName = "S";

    QFile file(name+"_"+FName[0]+"-"+SubName+".tab");
    QFile fileXY(name+"_"+FName[0]+"_"+SubName+"_XY.tab");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    fileXY.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QTextStream outXY(&fileXY);

    for(int i=0;i<data.size();i++)
    {
        for(int j=0;j<data[i].size();j++)
        {
            out<<data[i][j]<<"\t";
        }
        out<< "\n";
    }
    file.close();
    if(Yposition.size()!=Xposition.size())
    {
        qDebug()<<"XY Position Size isn't Match";
        return;
    }
    for(int i=0;i<Xposition.size();i++)
        outXY<<Xposition[i]<<"\t"<<Yposition[i]<<"\n";
    fileXY.close();
}


void Dialog::on_LoadPosition_clicked()
{
    QString XYPositionFile = QFileDialog::getOpenFileName(this, tr("Open File"),                                                       "/",
                                                          tr("Images (*.tab)"));
    QFile XYFile(XYPositionFile);
    QTextStream input(&XYFile);
    if(!XYFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"File Open Failed";
        return;
    }

    do
    {
        int x,y;
        input>>x>>y;
        //input>>y;
        qDebug()<<x<<y;
        //draw(temp,x,y,ui->labelall);
    }while(!input.atEnd());
    XYFile.close();
}
