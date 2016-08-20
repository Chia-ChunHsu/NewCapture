#include "timestring.h"
QString getTimeStr(QDateTime qdt)
{
    QString y = QString::number(qdt.date().year());
    QString m = QString::number(qdt.date().month());
    QString d = QString::number(qdt.date().day());
    QString t = qdt.time().toString();
    QString str = y+"/"+m+"/"+d+"/"+t;
    return str;
}

