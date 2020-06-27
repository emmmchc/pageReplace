#ifndef LFU_H
#define LFU_H

#include <QVector>
#include <qDebug>
#include <QThread>
#include "cglobal.h"
#include <QTextCodec>
#include <QTimer>
#include <QEventLoop>

namespace Ui {
class lfu;
}

class lfu:public QThread
{
    Q_OBJECT
protected:
    virtual void run();
public:
    void work(int a,double &per_time);
    QMutex pause;
signals:     //这里制造一个名为fifo_updata的信号
    void lfu_updata(QString sMessage,QString str);
};

#endif // LFU_H
