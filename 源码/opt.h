#ifndef OPT_H
#define OPT_H

#include <QVector>
#include <qDebug>
#include <QThread>
#include "cglobal.h"
#include <QTextCodec>
#include <QTimer>
#include <QEventLoop>

namespace Ui {
class opt;
}

class opt:public QThread
{
        Q_OBJECT
protected:
    virtual void run();
public:
    void work(int a,double &per_time);
    QMutex pause;
signals:
    void opt_updata(QString sMessage,QString str);
};

#endif // OPT_H
