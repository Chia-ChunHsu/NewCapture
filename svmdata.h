#ifndef SVMDATA_H
#define SVMDATA_H
#include "opencv.hpp"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>


class SvmData :public QWidget
{
private:
    std::vector<int> featuresChannel;
    int featuresNum;
    double C;
    //std::vector<int> &featuresChannel,int featuresNum,double C
public:
    SvmData(std::vector<int> &featuresChannel,int featuresNum,double C);
    ~SvmData();
    void initial();
    void LoadTraingData(std::vector<float> &featureData, std::vector<float> &labels);
    void Training(std::vector<float> &features, std::vector<float> &labels);
};

#endif // SVMDATA_H
