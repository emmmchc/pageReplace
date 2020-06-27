#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QSqlDatabase>
#include <QSplashScreen>

#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));

    //连接数据库
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("os");
    db.setUserName("root");
    db.setPassword("123456");
    if (!db.open())
    {
        QMessageBox::critical(NULL,"连接结果", "连接数据库失败");
        return 0;
    }
    MainWindow w;
    w.setWindowTitle(QString("页面置换算法模拟"));
    w.show();
    return a.exec();
}
