#include "svmdata.h"

SvmData::SvmData(std::vector<int> &_featuresChannel,int _featuresNum,double _C)
{
    featuresChannel.assign(_featuresChannel.begin(),_featuresChannel.end());
    featuresNum = _featuresNum;
    C = _C;
}

SvmData::~SvmData()
{

}

void SvmData::initial()//Call完這個Function SVM就訓練完了
{
    std::vector<float> featureData;
    std::vector<float> labels;
    LoadTraingData(featureData,labels);
    Training(featureData,labels);
}

void SvmData::LoadTraingData(std::vector<float> &featureData, std::vector<float> &labels)
{
    QString TraingDatafile = QFileDialog::getOpenFileName(this,tr("Training Data"),"C:/",tr("TraingData File(*.txt)"));
    if(TraingDatafile.isEmpty())
        return;

    featureData.clear();
    labels.clear();

    //讀取先前存好的SVM Model 所要訓練的特徵資料類
    QFile file1(TraingDatafile);
    file1.open(QIODevice::ReadOnly);
    QTextStream in1(&file1);

    while(!in1.atEnd())
    {
        QString str= in1.readLine();
        QStringList strList = str.split("\t");
        labels.push_back(strList[strList.length()-1].toFloat());
        for(int i=0;i<featuresChannel.size();i++)
        {
            featureData.push_back(strList[featuresChannel[i]].toFloat());
        }
    }
    file1.close();
    if(labels.size()*featuresNum!=featureData.size())
    {
        qDebug()<<"Features Size is not Match!";
        return;
    }
    QString ad = "FeatuerChannesl.txt";
    QFile File(ad);
    File.open(QIODevice::WriteOnly);
    QTextStream out(&File);
    for(int i=0;i<featuresChannel.size();i++)
    {
        out<< featuresChannel[i]<<",";
    }
    File.close();
}

void SvmData::Training(std::vector<float> &featureData, std::vector<float> &labels)
{
    //建立SVM訓練資料，分為兩筆，一筆為特徵資料，一筆是特徵資料對應到的類型
    cv::Mat trainingData(labels.size(),featuresChannel.size(),CV_32FC1);
    cv::Mat label(labels.size(),1,CV_32FC1);
    for(int i=0;i<featureData.size();i++)
    {
        trainingData.at<float>(i/featuresNum,i%featuresNum)=featureData[i];
    }
    for(int i=0;i<labels.size();i++)
    {
        label.at<float>(i,0)=labels[i];
    }

    //SVM 參數設定
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type =CvSVM::LINEAR;
    params.term_crit = cv::TermCriteria(CV_TERMCRIT_EPS, 100, 1e-6);
    params.C = C;

    CvSVM svm;
    svm.train(trainingData,label,cv::Mat(),cv::Mat(),params);
    svm.save("SVM.txt");
    qDebug()<<"SVM Model is Saved";
}
