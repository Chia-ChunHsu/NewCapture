#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    temp.clear();
    data.clear();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initial(std::vector<cv::Mat> cutPic, std::vector<cv::Point> RefCorPoint)
{
    this->open();
    ui->labelall->setGeometry(ui->labelall->x(),ui->labelall->y(),cutPic[0].cols,cutPic[0].rows);
    ShowOnLabel(cutPic[0],ui->labelall);
    Omat.clear();
    CorPoint.clear();
    temp.clear();
    //labelmat.clear();
    data.clear();
    for(int i=0;i<cutPic.size()-1;i++)
    {
        //cv::Mat t = cutPic[i].clone();
        temp.push_back(cutPic[i]);
        Omat.push_back(cutPic[i]);
        CorPoint.push_back(RefCorPoint[i]);

    }
    temp.push_back(cutPic[4]);
    Omat.push_back(cutPic[4]);
    CorPoint.push_back(RefCorPoint[0]);
    qApp->installEventFilter(this);
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

      draw(temp,x,y,ui->labelall);
      //draw(temp,pt.x(),pt.y(),ui->labelall);
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
    std::vector<int> dy;
    std::vector<int> dx;

    //int dy0 = -t1.y+RefCorPoint[0].y;
//    int dx0 = -t1.x+RefCorPoint[0].x;
//    int dy1 = -t1.y+RefCorPoint[1].y;
//    int dx1 = -t1.x+RefCorPoint[1].x-1;
//    int dy2 = -t1.y+RefCorPoint[2].y;
//    int dx2 = -t1.x+RefCorPoint[2].x;
//    int dy3 = -t1.y+RefCorPoint[3].y;
//    int dx3 = -t1.x+RefCorPoint[3].x-1;
    for(int i=0;i<m.size();i++)
    {
        if(ui->spinBox->value()==1)
        {
            dy.push_back(-t1.y+CorPoint[i].y);
            dx.push_back(-t1.x+CorPoint[i].x);
        }
        else if(ui->spinBox->value()==2)
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[1].y-CorPoint[0].y));
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[1].x-CorPoint[0].x));
        }
        else if(ui->spinBox->value()==3 )
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[2].y-CorPoint[0].y));
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[2].x-CorPoint[0].x));
        }
        else if(ui->spinBox->value()==4)
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[3].y-CorPoint[0].y));
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[3].x-CorPoint[0].x));
        }
        else if(ui->spinBox->value()==5)
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[4].y-CorPoint[0].y)-1);
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[4].x-CorPoint[0].x)+1);
        }
    }


    std::vector<int> tempdata;
    tempdata.clear();


    for(int i=0;i<m.size();i++)
    {
        if(i!=4)
        {
            int n = Omat[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
            tempdata.push_back(n);
            if(i==0)
                ui->label1->setText(QString::number(n));
            else if(i==1)
                ui->label2->setText(QString::number(n));
            else if(i==2)
                ui->label3->setText(QString::number(n));
            else if(i==3)
                ui->label4->setText(QString::number(n));
        }


        cv::Point p(x-dx[i],y-dy[i]);
        cv::circle(m[i],p,1,cv::Scalar(0,0,255),-1,8);
        //cv::Mat t;
        //cv::cvtColor(m[i],t,CV_BGR2RGB);
        cv::imwrite(QString::number(i).toStdString()+"point.jpg",m[i]);

    }

    data.push_back(tempdata);
    ShowOnLabel(temp[ui->spinBox->value()-1],k);

}

void Dialog::show(std::vector<cv::Mat> &m, int x, int y, QLabel *k)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<CorPoint.size()-1;i++)
    {
        t1.x = std::min(t1.x,CorPoint[i].x);
        t1.y = std::min(t1.y,CorPoint[i].y);
    }
    std::vector<int> dy;
    std::vector<int> dx;
    for(int i=0;i<m.size();i++)
    {
        if(ui->spinBox->value()==1||   ui->spinBox->value()==5)
        {
            dy.push_back(-t1.y+CorPoint[i].y);
            dx.push_back(-t1.x+CorPoint[i].x);
        }
        else if(ui->spinBox->value()==2)
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[1].y-CorPoint[0].y));
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[1].x-CorPoint[0].x));
        }
        else if(ui->spinBox->value()==3 )
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[2].y-CorPoint[0].y));
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[2].x-CorPoint[0].x));
        }
        else
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[3].y-CorPoint[0].y));
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[3].x-CorPoint[0].x));
            //qDebug()<<-t1.y+CorPoint[i].y-(CorPoint[3].y-CorPoint[0].y)<< -t1.x+CorPoint[i].x-(CorPoint[3].x-CorPoint[0].y);
        }

    }
    int gap = 30;
    for(int i=0;i<m.size()-1;i++)
    {
        if(x-gap-dx[i]<0 || x+gap-dx[i]>m[i].cols || y-gap-dy[i]<0 || y+gap-dy[i]>m[i].rows)
            return;
    }
    std::vector<cv::Mat> roi;
    roi.clear();
    std::vector<cv::Mat> t;
    t.clear();
    for(int i=0;i<m.size();i++)
    {
        cv::Mat kt=m[i];
        t.push_back(kt);
        int n = m[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
        if(i==0)
            ui->label1->setText(QString::number(n));
        else if(i==1)
            ui->label2->setText(QString::number(n));
        else if(i==2 )
            ui->label3->setText(QString::number(n));
        else if(i==3)
            ui->label4->setText(QString::number(n));
        cv::Mat tmp = t[i].clone();
        cv::Rect r = cv::Rect(x-gap-dx[i],y-gap-dy[i],2*gap,2*gap);
        cv::Mat troi = tmp(r);
        roi.push_back(troi);
        cv::circle(roi[i],cv::Point(gap,gap),1,cv::Scalar(0,0,255),-1,8);
    }

    ShowOnLabel(roi[ui->spinBox->value()-1],k);


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

    QFile file(name+".tab");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    //int tmp =  saveMat.at<cv::Vec3b>(0,i)[0];
    //out <<ui->L1->text()<<"\t"<<ui->L2->text()<<"\t"<<ui->L3->text()<<"\t"<<ui->L4->text();
    for(int i=0;i<data.size();i++)
    {
        for(int j=0;j<data[i].size();j++)
        {
            out<<data[i][j]<<"\t";
        }
        out<< "\n";
    }
    file.close();
}



void Dialog::on_RGBButton_clicked()
{

}

void Dialog::on_xSpinBox_valueChanged(int arg1)
{
    CorPoint[0].x = CorPoint[0].x+arg1;
}

void Dialog::on_ySpinBox_valueChanged(int arg1)
{
    CorPoint[0].y = CorPoint[0].y+arg1;
}
