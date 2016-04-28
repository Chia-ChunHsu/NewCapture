#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    data.clear();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initial(std::vector<cv::Mat> &cutPic, std::vector<cv::Point> &RefCorPoint)
{
    this->open();
    ui->labelall->setGeometry(ui->labelall->x(),ui->labelall->y(),cutPic[0].cols,cutPic[0].rows);
    ShowOnLabel(cutPic[0],ui->labelall);
    mat.clear();
    CorPoint.clear();
    //temp.clear();
    for(int i=0;i<cutPic.size();i++)
    {
        cv::Mat t = cutPic[i].clone();
        mat.push_back(t);
        CorPoint.push_back(RefCorPoint[i]);
        //temp.push_back(t);
    }

//    std::copy(cutPic.begin(),cutPic.end(),mat.begin());
//    std::copy(RefCorPoint.begin(),RefCorPoint.end(),CorPoint.begin());
//    std::copy(cutPic.begin(),cutPic.end(),temp.begin());
    qApp->installEventFilter(this);
}

void Dialog::ShowOnLabel(cv::Mat mat, QLabel *k)
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

bool Dialog::eventFilter(QObject *obj, QEvent *event)
{
  QString str;
  QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

  //mouseEvent->localPos()
  //int x;
  //int y;
  if( event->type() == QMouseEvent::MouseMove && obj->isWidgetType()&& obj == ui->labelall)
  {
      QPointF pt = mouseEvent->localPos();
      event = static_cast<QMouseEvent*>(event);
      str = "MouseMove" + QString::number(pt.x()) +", " + QString::number(pt.y());
      setWindowTitle(str);                            // 顯示座標
      show(mat,pt.x(),pt.y(),ui->labelScale);
      return false;

  }else if(event->type() == QMouseEvent::MouseButtonPress && obj->isWidgetType() && obj == ui->labelall){
      QPointF pt = mouseEvent->localPos();
      str = "MouseButtonPress" + QString::number(pt.rx()) +", " + QString::number(pt.ry());
      setWindowTitle(str);                            // 顯示座標

      int x = pt.rx();
      int y = pt.ry();

      draw(mat,x,y,ui->labelall);
      show(mat,pt.x(),pt.y(),ui->labelScale);
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
        else if(ui->spinBox->value()==3)
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[2].y-CorPoint[0].y));
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[2].x-CorPoint[0].x));
        }
        else if(ui->spinBox->value()==4)
        {
            dy.push_back(-t1.y+CorPoint[i].y-(CorPoint[3].y-CorPoint[0].y));
            dx.push_back(-t1.x+CorPoint[i].x-(CorPoint[3].x-CorPoint[0].x));
        }

    }
    //temp = m;

    std::vector<int> tempdata;
    tempdata.clear();
    //std::vector<cv::Mat> t;
//    for(int i=0;i<temp.size();i++)
//        temp[i]
    for(int i=0;i<m.size();i++)
    {
       // cv::Mat kt = mat[i].clone();
        //temp.push_back(kt);
        int n = m[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
        tempdata.push_back(n);
        if(i==0)
            ui->label1->setText(QString::number(n));
        else if(i==1)
            ui->label2->setText(QString::number(n));
        else if(i==2)
            ui->label3->setText(QString::number(n));
        else if(i==3)
            ui->label4->setText(QString::number(n));
        cv::Point p(x-dx[i],y-dy[i]);
        cv::Point textp(x-dx[i],y-10-dy[i]);
        cv::circle(temp[i],p,1,cv::Scalar(255,0,0),-1,8);
        cv::putText(temp[i],QString::number(data.size()+1).toStdString(),textp,cv::FONT_HERSHEY_COMPLEX,0.4,cv::Scalar(255,0,0));

    }
    //cv::imshow("temp",temp[0]);
    //cv::imshow("cut0",t[0]);
    //cv::imshow("cutMat",mat[0]);
    data.push_back(tempdata);
    ShowOnLabel(temp[ui->spinBox->value()-1],k);
}

void Dialog::show(std::vector<cv::Mat> &m, int x, int y, QLabel *k)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<CorPoint.size();i++)
    {
        t1.x = std::min(t1.x,CorPoint[i].x);
        t1.y = std::min(t1.y,CorPoint[i].y);
    }
    std::vector<int> dy;
    std::vector<int> dx;
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
        else if(ui->spinBox->value()==3)
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
    for(int i=0;i<m.size();i++)
    {
        if(x-gap-dx[i]<0 || x+gap-dx[i]>m[i].cols || y-gap-dy[i]<0 || y+gap-dy[i]>mat[i].rows)
            return;
    }
    std::vector<cv::Mat> roi;
    roi.clear();
    std::vector<cv::Mat> t;
    for(int i=0;i<m.size();i++)
    {
        cv::Mat kt=mat[i].clone();
        temp.push_back(kt);
        int n = m[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
        if(i==0)
            ui->label1->setText(QString::number(n));
        else if(i==1)
            ui->label2->setText(QString::number(n));
        else if(i==2)
            ui->label3->setText(QString::number(n));
        else if(i==3)
            ui->label4->setText(QString::number(n));
        cv::Mat tmp = temp[i].clone();
        cv::Rect r = cv::Rect(x-gap-dx[i],y-gap-dy[i],2*gap,2*gap);
        cv::Mat troi = tmp(r);
        roi.push_back(troi);
        cv::circle(roi[i],cv::Point(gap,gap),1,cv::Scalar(255,0,0),-1,8);
    }

    ShowOnLabel(roi[ui->spinBox->value()-1],k);
}

void Dialog::on_spinBox_valueChanged(int arg1)
{
    ui->labelall->setGeometry(ui->labelall->x(),ui->labelall->y(),mat[arg1-1].cols,mat[arg1-1].rows);
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
