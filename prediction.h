#ifndef PREDICTION_H
#define PREDICTION_H
#include "opencv.hpp"
#include <QDebug>

class Prediction
{
private:
    cv::Mat _NDVIMat;
    std::vector<cv::Point> _CorPoint;
    std::vector<cv::Mat> _DataMat;
    std::vector<int> _FeatureNum;

public:
    Prediction();
    void Initial(cv::Mat NDVIMat,std::vector<cv::Point> &CorPoint,std::vector<cv::Mat> &DataMat ,std::vector<int> &FeaturesNum);
    cv::Mat SVMResult();
    float returnSvmAns(int y, int x);
};

#endif // PREDICTION_H
