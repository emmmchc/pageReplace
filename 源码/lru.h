#ifndef LRU_H
#define LRU_H

#include <QVector>
#include <qDebug>
#include <QThread>
#include "cglobal.h"
#include <QTextCodec>
#include <QTimer>
#include <QEventLoop>

namespace Ui {
class lru;
}

class lru:public QThread
{
    Q_OBJECT
protected:
    virtual void run();

public:
    void work(int a,double &per_time);
    QMutex pause;
signals:     //这里制造一个名为lru_updata的信号
    void lru_updata(QString sMessage,QString str);
};

typedef struct func
{
    int value;
    void (*func)();
}mapping_func;

#endif // LRU_H
