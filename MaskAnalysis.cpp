#include "MaskAnalysis.h"

MaskAnalysis::MaskAnalysis()
{

}

MaskAnalysis::~MaskAnalysis()
{

}

void MaskAnalysis::setRequest(cv::Mat ResultSizeMat, std::vector<cv::Mat> &DataMat, std::vector<cv::Point> &RefCorPoint)
{
    _ResultSizeMat = ResultSizeMat;
    _DataMat.assign(DataMat.begin(),DataMat.end());
    _RefCorPoint.assign(RefCorPoint.begin(),RefCorPoint.end());
}

cv::Mat MaskAnalysis::NDVI()
{
    cv::Mat predict;
    predict.create(_ResultSizeMat.rows,_ResultSizeMat.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);

    for(int i=0;i<_ResultSizeMat.cols;i++)
    {
        for(int j=0;j<_ResultSizeMat.rows;j++)
        {
            if(_MaskMat.at<cv::Vec3b>(j,i)[0] != 0)
            {
                float value = NDVIValue(j,i);

                if(value < 0.9)
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0; //B
                    predict.at<cv::Vec3b>(j,i)[1] = 0; //G
                    predict.at<cv::Vec3b>(j,i)[2] = 255;//R
                }
                else//葉子
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 255;//G
                    predict.at<cv::Vec3b>(j,i)[2] = 0;
                }
            }
        }
    }
    return predict;
}

float MaskAnalysis::NDVIValue(int y, int x)
{
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    for(int i=0;i<4;i++)
    {
        t1.x = std::min(t1.x,_RefCorPoint[i].x);
        t1.y = std::min(t1.y,_RefCorPoint[i].y);
    }

    std::vector<int> dx;
    std::vector<int> dy;

    for(int i=0;i<_RefCorPoint.size();i++)
    {
        dx.push_back(-t1.x+_RefCorPoint[i].x);
        dy.push_back(-t1.y+_RefCorPoint[i].y);
    }
    std::vector<int> result;
    for(int i=0;i<4;i++)
    {
        if(x-dx[i]>=0 && x-dx[i] <_DataMat[i].cols && y-dy[i] >=0 && y-dy[i]<_DataMat[i].rows)
        {
            int n = _DataMat[i].at<cv::Vec3b>(y-dy[i],x-dx[i])[0];
            result.push_back(n);
        }
    }
    if(result.size()!=4)
        return -1.0;
    if(result[2]+result[1] == 0)// NDVI 公式 分母不可為0
        return -1.0;
    float ans = (float(result[2])-float(result[1])*20/400)/(float(result[2])+float(result[1])*20/400);
    return ans;
}

void MaskAnalysis::BuildMaskMat(int threshold1, int threshold2,std::vector<cv::Mat> &OriginalCutMat )
{
    cv::Mat shadow;
    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

    for(int i=0;i<_RefCorPoint.size();i++)
    {
        t1.x = std::min(t1.x,_RefCorPoint[i].x);
        t1.y = std::min(t1.y,_RefCorPoint[i].y);
    }
    cv::Size size(_ResultSizeMat.cols,_ResultSizeMat.rows);

    shadow.create(size,CV_MAKETYPE(_ResultSizeMat.type(),3));
    shadow = cv::Scalar::all(0);

    std::vector<cv::Mat> WarpMat;
    WarpMat.clear();
    for(int i=0;i<_DataMat.size();i++)
        WarpMat.push_back(_DataMat[i]);

    std::vector<int> dx;
    std::vector<int> dy;
    for(int i=0;i<_RefCorPoint.size();i++)
    {
        dx.push_back(-t1.x+_RefCorPoint[i].x);
        dy.push_back(-t1.y+_RefCorPoint[i].y);
    }
    for(int i = 0;i<shadow.cols;i++)
    {
        for(int j=0;j<shadow.rows;j++)
        {
            bool bool1 = false;
            bool bool2 = false;
            bool bool3 = false;
            if(j>=dy[2] && i >=dx[2] && j <_DataMat[2].rows+dy[2] && i <_DataMat[2].cols+dx[2])
            {
                bool1 = (_DataMat[2].at<cv::Vec3b>(j-dy[2],i-dx[2])[0]+_DataMat[2].at<cv::Vec3b>(j-dy[2],i-dx[2])[1]+_DataMat[2].at<cv::Vec3b>(j-dy[2],i-dx[2])[2])/3 > threshold1;
            }
            if(j>=dy[0] && i>=dx[0] && j<_DataMat[0].rows+dy[0] && i<_DataMat[0].cols+dx[0])
            {
                bool2 = (_DataMat[0].at<cv::Vec3b>(j-dy[0],i-dx[0])[0]+_DataMat[0].at<cv::Vec3b>(j-dy[0],i-dx[0])[1]+_DataMat[0].at<cv::Vec3b>(j-dy[0],i-dx[0])[2])/3< threshold2;
            }
            if(j-dy[0]-20>0 && j-dy[2]-20>0 && i-dx[0]-20>0 && i-dx[2]-20>0 && j-dy[0]+20 <_DataMat[0].rows && i-dx[0]+20<_DataMat[0].cols && j-dy[2]+20 <_DataMat[2].rows && i-dx[2]+20<_DataMat[2].cols )
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
    int dilation_size = 4;

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
    _MaskMat = dilMat.clone();

    std::vector<cv::Mat> temp;
    temp.clear();
    for(int i=0;i<_DataMat.size();i++)
    {
        cv::Mat tMat = _DataMat[i].clone();
        temp.push_back(tMat);
    }

    cv::Mat tk;
    tk.create(dilMat.rows,dilMat.cols,CV_MAKETYPE(tk.type(),3));
    tk = cv::Scalar::all(0);

    for(int i=0;i<dilMat.cols;i++)
    {
        for(int j=0;j<dilMat.rows;j++)
        {
            for(int n=0;n<4;n++)
            {
                if(dilMat.at<cv::Vec3b>(j,i)[0]!=255 && j-dy[n]>=0  && j-dy[n]<temp[n].rows && i-dx[n]>=0 && i-dx[n]<temp[n].cols)
                {
                    temp[n].at<cv::Vec3b>(j-dy[n],i-dx[n])[0] = 0;
                    temp[n].at<cv::Vec3b>(j-dy[n],i-dx[n])[1] = 0;
                    temp[n].at<cv::Vec3b>(j-dy[n],i-dx[n])[2] = 0;
                }
            }
        }
    }
    for(int i=0;i<temp.size();i++)
    {
        cv::imwrite("WhiteCorrection_ROI"+QString::number(i).toStdString()+".jpg",temp[i]);
    }
    OriginalCutMat.clear();
    for(int i=0;i<temp.size();i++)
        OriginalCutMat.push_back(temp[i]);
}

cv::Mat MaskAnalysis::getMaskMat()
{
    return _MaskMat;
}

