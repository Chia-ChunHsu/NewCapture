#ifndef PREDICTION_H
#define PREDICTION_H
#include "opencv.hpp"

class Prediction
{
private:
    cv::Mat _NDVIMat;
    std::vector<cv::Point> _CorPoint;
    std::vector<cv::Mat> _DataMat;
    int _FeatureNum;

public:
    Prediction();
    void Initial(cv::Mat NDVIMat,std::vector<cv::Point> &CorPoint,std::vector<cv::Mat> &DataMat ,int FeaturesNum);
    cv::Mat SVMResult();
    float returnSvmAns(int y, int x);
};

#endif // PREDICTION_H
