#include "dataanalysis.h"

DataAnalysis::DataAnalysis()
{

}

void DataAnalysis::Initial(std::vector<cv::Mat> &DataMat, std::vector<cv::Point> &CorPoint,cv::Mat NDVIMat)
{
    _DataMat.assign(DataMat.begin(),DataMat.end());
    _CorPoint.assign(CorPoint.begin(),CorPoint.end());
    _NDVIMat = NDVIMat;
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
    {
        t1.x = std::min(t1.x,_CorPoint[i].x);
        t1.y = std::min(t1.y,_CorPoint[i].y);
    }
//    std::vector<int> dx;
//    std::vector<int> dy;

    _dx.clear();
    _dy.clear();

    for(int i=0;i<4;i++)
    {
        _dy.push_back(-t1.y+_CorPoint[i].y);
        _dx.push_back(-t1.x+_CorPoint[i].x);
    }
}

void DataAnalysis::DivisionMat()
{
    //因為相除時，不同顏色的影像顏色差異太大，因此我們將影像相都改到平均值為127處
    std::vector<cv::Mat> OriginalAvgMat;
    for(int n=0;n<4;n++)
    {
        int value[256]={0};
        int max_count = 0;
        int max_value=0;
        for(int i=0;i<_DataMat[n].cols;i++)
        {
            for(int j=0;j<_DataMat[n].rows;j++)
            {
                value[_DataMat[n].at<cv::Vec3b>(j,i)[0]]++;
                if(max_count < value[_DataMat[n].at<cv::Vec3b>(j,i)[0]] && _DataMat[n].at<cv::Vec3b>(j,i)[0]!=0)
                {
                    max_count = value[_DataMat[n].at<cv::Vec3b>(j,i)[0]];
                    max_value = _DataMat[n].at<cv::Vec3b>(j,i)[0];
                }
            }
        }
        cv::Mat atmp;// = CutPic[n];
        atmp.create(_DataMat[n].rows,_DataMat[n].cols,_DataMat[n].type());
        atmp = cv::Scalar::all(0);
        for(int i=0;i<_DataMat[n].cols;i++)
        {
            for(int j=0;j<_DataMat[n].rows;j++)
            {
                int new_value = _DataMat[n].at<cv::Vec3b>(j,i)[0]+(127-max_value);
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
        k.create(_NDVIMat.rows,_NDVIMat.cols,CV_MAKETYPE(CV_32F,1));
        k = cv::Scalar::all(0);
        division_normal.push_back(k);
    }
    for(int i=0;i<_NDVIMat.cols;i++)
    {
        for(int j=0;j<_NDVIMat.rows;j++)
        {
            if(_NDVIMat.at<cv::Vec3b>(j,i)[1] == 255)
            {
                std::vector<float>div;

                DivisionValue(OriginalAvgMat,j,i,div);

                for(int n=0;n<div.size();n++)
                {
                    division_normal[n].at<float>(_NDVIMat.cols*j+i)=div[n];
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
        _DivisionMat.push_back(normalbgr);
        cv::imwrite("division_normal_normalize_"+QString::number(n).toStdString()+".jpg",division_normal[n]);
    }
}

void DataAnalysis::MinusMat()
{
    //std::vector<cv::Mat> MinusMat;
    _MinusMat.clear();
    for(int n=0;n<6;n++)
    {
        cv::Mat temp;
        temp.create(_NDVIMat.rows,_NDVIMat.cols,_NDVIMat.type());
        temp = cv::Scalar::all(0);
        _MinusMat.push_back(temp);
    }

    for(int i=0;i<_NDVIMat.cols;i++)
    {
        for(int j=0;j<_NDVIMat.rows;j++)
        {
            if(_NDVIMat.at<cv::Vec3b>(j,i)[1] == 255)
            {
                std::vector<int> minus_pixel;
                //MinusPixel_value(CutPic,j,i,minus_pixel);
                MinusValue(_DataMat,j,i,minus_pixel);
                for(int n=0;n<6;n++)
                {
                    _MinusMat[n].at<cv::Vec3b>(j,i)[0]=minus_pixel[n];
                    _MinusMat[n].at<cv::Vec3b>(j,i)[1]=minus_pixel[n];
                    _MinusMat[n].at<cv::Vec3b>(j,i)[2]=minus_pixel[n];
                }
            }
        }
    }

    for(int n=0;n<6;n++)    //相減的影像正規化
    {
        cv::normalize(_MinusMat[n],_MinusMat[n],0,255,CV_MINMAX);
        cv::imwrite("MinusMat_"+QString::number(n).toStdString()+".jpg",_MinusMat[n]);
    }
}

void DataAnalysis::DivisionValue(std::vector<cv::Mat> &DataMat, int y, int x, std::vector<float> &Divisionvalue)
{
    std::vector<int> result;
    //集合四張圖片的數據值

    for(int i=0;i<4;i++)
    {
        if(x-_dx[i]>=0 && x-_dx[i]<DataMat[i].cols && y-_dy[i] >=0 && y-_dy[i]<DataMat[i].rows)
        {
            int n = DataMat[i].at<cv::Vec3b>(y-_dy[i],x-_dx[i])[0];
            result.push_back(n);
        }
    }
    float div1,div2,div3,div4,div5,div6;
    //div1=1/0 div2=2/0 div3=3/0 div4=2/1  div5=3/1 div6=3/2
    //div的定義 div1=700/568 div2=750/568 div3=830/568 div4=750/700 div5=830/700 div6=830/750
    if(result.size()!=4) //檢查有沒有四個值，不然就是有圖片不在範圍內
    {
        div1 = 0;
        div2 = 0;
        div3 = 0;
        div4 = 0;
        div5 = 0;
        div6 = 0;
        return;
    }
    if(result[0]== 0)   //避免分母為0的狀況
    {
        div1 = 1;
        div2 = 1;
        div3 = 1;
    }
    else
    {
        div1 = (float)result[1]/(float)result[0];
        div2 = (float)result[2]/(float)result[0];
        div3 = (float)result[3]/(float)result[0];
    }
    if(result[1]==0)
    {
        div4 = 1;
        div5 = 1;
    }
    else
    {
        div4 = (float)result[2]/(float)result[1];
        div5 = (float)result[3]/(float)result[1];
    }
    if(result[2]== 0)
        div6 = 1;
    else
        div6 = (float)result[3]/(float)result[2];

    Divisionvalue.push_back(div1);
    Divisionvalue.push_back(div2);
    Divisionvalue.push_back(div3);
    Divisionvalue.push_back(div4);
    Divisionvalue.push_back(div5);
    Divisionvalue.push_back(div6);
}

void DataAnalysis::MinusValue(std::vector<cv::Mat> &DataMat, int y, int x, std::vector<int> &Minusvalue)
{
    std::vector<int> result;
    for(int i=0;i<4;i++)
    {
        if(x-_dx[i]>=0 && x-_dx[i]<DataMat[i].cols && y-_dy[i] >=0 && y-_dy[i]<DataMat[i].rows)
        {
            int n = DataMat[i].at<cv::Vec3b>(y-_dy[i],x-_dx[i])[0];
            result.push_back(n);
        }
    }
    Minusvalue.clear();

    if(result.size()!=4 )
    {
        for(int n=0;n<6;n++)
            Minusvalue.push_back(0);
        return;
    }
    int v0 = abs(result[0]-result[1]);
    int v1 = abs(result[0]-result[2]);
    int v2 = abs(result[0]-result[3]);
    int v3 = abs(result[1]-result[2]);
    int v4 = abs(result[1]-result[3]);
    int v5 = abs(result[2]-result[3]);
    Minusvalue.push_back(v0);
    Minusvalue.push_back(v1);
    Minusvalue.push_back(v2);
    Minusvalue.push_back(v3);
    Minusvalue.push_back(v4);
    Minusvalue.push_back(v5);
}

void DataAnalysis::GetDataMat(std::vector<cv::Mat> &MinusMat, std::vector<cv::Mat> &DivMat)
{
    MinusMat.assign(_MinusMat.begin(),_MinusMat.end());
    DivMat.assign(_DivisionMat.begin(),_DivisionMat.end());
}

