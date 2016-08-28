#include "prediction.h"

Prediction::Prediction()
{

}

void Prediction::Initial(cv::Mat NDVIMat, std::vector<cv::Point> &CorPoint, std::vector<cv::Mat> &DataMat,std::vector<int> &FeaturesNum)
{
    _NDVIMat = NDVIMat;
    _CorPoint.assign(CorPoint.begin(),CorPoint.end());
    _DataMat.assign(DataMat.begin(),DataMat.end());
    _FeatureNum.assign(FeaturesNum.begin(),FeaturesNum.end());
}

cv::Mat Prediction::SVMResult()
{
    cv::Mat predict;
    predict.create(_NDVIMat.rows,_NDVIMat.cols,CV_MAKETYPE(predict.type(),3));
    predict = cv::Scalar::all(0);

    for(int i=0;i<_NDVIMat.cols;i++)
    {
        for(int j=0;j<_NDVIMat.rows;j++)
        {
            if(_NDVIMat.at<cv::Vec3b>(j,i)[1] == 255)
            {
                float value = returnSvmAns(j,i);

                if(value == 0)  //if ans = 0 , 健康部分(綠色)
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 255;
                    predict.at<cv::Vec3b>(j,i)[2] = 0;
                }
                else if(value == 1) // if ans = 1 ,潛伏期(黃色)
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 255;
                    predict.at<cv::Vec3b>(j,i)[2] = 255;
                }
                else if(value == 2) // if ans = 2 , 有病徵(紅色)
                {
                    predict.at<cv::Vec3b>(j,i)[0] = 0;
                    predict.at<cv::Vec3b>(j,i)[1] = 0;
                    predict.at<cv::Vec3b>(j,i)[2] = 255;
                }
            }
        }
    }
    return predict;

//    ShowOnLabel(predict,ui->FalseColorLabel);

    //    cv::imwrite(FileNameAd.toStdString()+"_pre.jpg",predict);
}

float Prediction::returnSvmAns(int y, int x)
{

//    int features = ui->FeaturesSpinBox->value() ;

    std::vector<cv::Point> Point;

    for(int i=0;i<4;i++)
    {
        Point.push_back(_CorPoint[i]);
    }

    cv::Point t1(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

    for(int i=0;i<4;i++)
    {
        t1.x = std::min(t1.x,_CorPoint[i].x);
        t1.y = std::min(t1.y,_CorPoint[i].y);
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

    std::vector<int> TResult; //全部16個Data量
    TResult.clear();

    std::vector<int> result;  //我們有興趣的Data，要拿來training的
    result.clear();


    for(int n=0;n<16;n++)
    {
        if(y-dy[n]>=0 && y-dy[n]<_DataMat[n].rows && x-dx[n]>=0 && x-dx[n]<_DataMat[n].cols)
        {
            int pn;
            pn = _DataMat[n].at<cv::Vec3b>(y-dy[n],x-dx[n])[0];
            TResult.push_back(pn);
        }
    }

    for(int i=0;i<_FeatureNum.size();i++)
    {
        result.push_back(TResult[_FeatureNum[i]]);
    }

    cv::Mat test(1,_FeatureNum.size(),CV_32FC1);

    if(result.size()!=_FeatureNum.size())
    {
        qDebug()<<"Wrong! Features Size is not match!";
        return 100;
    }

    for(int j=0;j<_FeatureNum.size();j++)
    {
        test.at<float>(0,j) = result[j];
    }

    CvSVM svm;
    svm.load("SVM.txt");
    float label = svm.predict(test);
    return label;
}

