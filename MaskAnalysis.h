#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "opencv.hpp"
#include <QString>
#include <QDebug>

class MaskAnalysis
{
private:
    cv::Mat _ResultSizeMat;
    std::vector<cv::Mat> _DataMat;
    std::vector<cv::Point> _RefCorPoint;
    cv::Mat _MaskMat;
public:
    MaskAnalysis();
    ~MaskAnalysis();
    void setRequest(cv::Mat ResultSizeMat,std::vector<cv::Mat> &DataMat,std::vector<cv::Point> &RefCorPoint);
    cv::Mat NDVI();
    float NDVIValue(int y, int x);
    void BuildMaskMat(int threshold1,int threshold2,std::vector<cv::Mat> &OriginalCutMat);
    cv::Mat getMaskMat();
};

#endif // ANALYSIS_H
