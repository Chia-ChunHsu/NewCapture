#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "opencv.hpp"

class Analysis
{
private:
    cv::Mat _ResultSizeMat;
    std::vector<cv::Mat> _DataMat;
    std::vector<cv::Point> _RefCorPoint;

public:
    Analysis(cv::Mat ResultSizeMat,std::vector<cv::Mat> &DataMat,std::vector<cv::Point> &RefCorPoint);
    ~Analysis();
    cv::Mat NDVI();
    float NDVIValue(int x, int y);
};

#endif // ANALYSIS_H
