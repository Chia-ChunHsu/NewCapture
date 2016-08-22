#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include "opencv.hpp"

class DataAnalysis
{
public:
    DataAnalysis();
    void Initial();
    void DivisionValue(std::vector<cv::Mat> &DataMat,int y, int x, std::vector<float> &DivisionMat);
    void MinusValue(std::vector<cv::Mat> &);
};

#endif // DATAANALYSIS_H
