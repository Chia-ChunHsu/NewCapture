#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include "opencv.hpp"
#include <QString>

class DataAnalysis
{
private:
    std::vector<cv::Point> _CorPoint;
    std::vector<cv::Mat> _DataMat;
    std::vector<int> _dx;
    std::vector<int> _dy;
    std::vector<cv::Mat> _MinusMat;
    std::vector<cv::Mat> _DivisionMat;
    std::vector<cv::Mat> _AvgMat;
    cv::Mat _NDVIMat;
    
public:
    DataAnalysis();
    void Initial(std::vector<cv::Mat> &DataMat,std::vector<cv::Point> &CorPoint,cv::Mat NDVIMat);
    void DivisionMat();
    void MinusMat();
    void DivisionValue(std::vector<cv::Mat> &DataMat,int y, int x, std::vector<float> &Divisionvalue);
    void MinusValue(std::vector<cv::Mat> &DataMat,int y, int x, std::vector<int> &Minusvalue);
    void GetDataMat(std::vector<cv::Mat> &MinusMat,std::vector<cv::Mat> &DivMat);
};

#endif // DATAANALYSIS_H
