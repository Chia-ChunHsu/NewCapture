#include "time.h"

Time::Time()
{
    _qdt = QDateTime();
}

Time::Time(QDateTime qdt)
{
    _qdt = qdt;
}

Time::~Time()
{
}

QString Time::getTimeString()
{
    QString result;
    QString year = QString::number(_qdt.date().year());
    QString month = QString::number(_qdt.date().month());
    QString day = QString::number(_qdt.date().day());
    QString time = _qdt.time().toString();
    result = year+"/"+month+"/"+day+" "+time;
    return result;
}

void Time::setTime(QDateTime qdt)
{
    _qdt = qdt;
}

void Time::updateTime(int addTime)
{
    QDateTime TempTime;
    TempTime = _qdt.addSecs(addTime);
    _qdt = TempTime;
}

QDateTime Time::getQTime()
{
    return _qdt;
}

