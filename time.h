#ifndef TIME_H
#define TIME_H

#include <QMainWindow>
#include <QDateTime>
#include <QDebug>

class Time
{
private:
    QDateTime _qdt;
public:
    Time();
    Time(QDateTime qdt);
    ~Time();
    QString getTimeString();
    void setTime(QDateTime qdt);
    void updateTime(int addTime);
    QDateTime getQTime();
};

#endif // TIME_H
