#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QTextCodec>
#include <QEventLoop>
#include <QDateTime>
#include <QVector>
#include <QString>
#include <QSqlTableModel>
#include <QSqlError>
#include <QModelIndex>
#include "cglobal.h"
#include "fifo.h"
#include "lru.h"
#include "lfu.h"
#include "opt.h"
#include "history.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public:
        int low = 1;
        int high = 9;
        fifo *fifo1;
        lru *lru1;
        lfu *lfu1;
        opt *opt1;
        history *history1;
        QSqlTableModel *tabModel;//数据模型
private slots:
        void on_taskSize_btn_clicked();
        void on_task_btn_clicked();
        void on_start_btn_clicked();
        void on_clear_btn_clicked();

        void on_stop_continue_btn_clicked();
        void on_stop_btn_clicked();

        void on_quick_table_check_clicked(bool checked);
        void on_quick_table_num_edit_textChanged();

        void on_save_btn_clicked();

        void on_open_btn_clicked();

        void on_result_btn_clicked();

public slots:
        void fifo_updata(QString sMessage,QString str);
        void lru_updata(QString sMessage,QString str);
        void lfu_updata(QString sMessage,QString str);
        void opt_updata(QString sMessage,QString str);
        void history_updata(QModelIndex index);
};

#endif // MAINWINDOW_H
