#include "analysis.h"

Analysis::Analysis(cv::Mat ResultSizeMat,std::vector<cv::Mat> &DataMat,std::vector<cv::Point> &RefCorPoint)
{
    _DataMat.assign(DataMat.begin(),DataMat.end());
    _RefCorPoint.assign(RefCorPoint.begin(),RefCorPoint.end());
    _ResultSizeMat = ResultSizeMat;
}

Analysis::~Analysis()
{

}

cv::Mat Analysis::NDVI()
{
    cv::Mat predict;
    predict.create(_ResultSizeMat.rows,_ResultSizeMat.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);

    for(int i=0;i<_ResultSizeMat.cols;i++)
    {
        for(int j=0;j<_ResultSizeMat.rows;j++)
        {
            if(maskResult.at<cv::Vec3b>(j,i)[0] != 0)
            {
                float value = NDVI_value(j,i);

                if(value < 0.9)
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
    return predict;
//    ShowOnLabel(predict,ui->TempLabel);
//    NDVIMat = predict;//.clone();
//    ui->Multi_Buttom->setEnabled(true);
//    cv::imwrite("NDVI.jpg",NDVIMat);
}

float Analysis::NDVIValue(int x, int y)
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


//    if(x-dx0>=0 && x-dx0 <m[0].cols && y-dy0 >=0 && y-dy0<m[0].rows)
//    {
//        int n = m[0].at<cv::Vec3b>(y-dy0,x-dx0)[0];
//        result.push_back(n);
//    }
//    if(x-dx1>=0 && x-dx1 <m[1].cols && y-dy1 >=0 && y-dy1<m[1].rows)
//    {
//        int n = m[1].at<cv::Vec3b>(y-dy1,x-dx1)[0];
//        result.push_back(n);
//    }
//    if(x-dx2>=0 && x-dx2 <m[2].cols && y-dy2>=0 && y-dy2<m[2].rows)
//    {
//        int n = m[2].at<cv::Vec3b>(y-dy2,x-dx2)[0];
//        result.push_back(n);
//    }
//    if(x-dx3>=0 && x-dx3<m[3].cols && y-dy3>=0 && y-dy3<m[3].rows)
//    {
//        int n = m[3].at<cv::Vec3b>(y-dy3,x-dx3)[0];
//        result.push_back(n);
//    }

    if(result.size()!=4)
        return -1.0;

    if(result[2]+result[1] == 0)
        return -1.0;
    float ans = (float(result[2])-float(result[1])*20/400)/(float(result[2])+float(result[1])*20/400);

    return ans;
}

