
#ifndef FIFO_H
#define FIFO_H

#include <QVector>
#include <qDebug>
#include <QThread>
#include "cglobal.h"
#include <QTextCodec>
#include <QTimer>
#include <QEventLoop>
namespace Ui {
class fifo;
}

class fifo:public QThread
{
    Q_OBJECT
protected:
    virtual void run();

public:
    void work(int a,double &per_time);
    QMutex pause;
signals:     //这里制造一个名为fifo_updata的信号
    void fifo_updata(QString sMessage,QString str);
};

#endif // FIFO_H
