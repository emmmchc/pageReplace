#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("logo.ico"));

    task = new int[taskSize];
    task[0] = -1;
    ui->memory_num_edit->setText(QString::number(memory_num,10));
    ui->taskSize_edit->setText(QString::number(taskSize,10));
    ui->memory_access_edit->setText(QString::number(memory_access,10,2));
    ui->quick_table_access_edit->setText(QString::number(quick_table_access,10,2));
    ui->miss_page_interrupt_edit->setText(QString::number(miss_page_interrupt,10,2));
    ui->quick_table_num_edit->setText(QString::number(quick_table_num,10));
    ui->quick_table_num_edit->setEnabled(false);
    on_task_btn_clicked();
    fifo1 = new fifo();
    lru1 = new lru();
    lfu1 = new lfu();
    opt1 = new opt();
    history1 = new history(this);
    tabModel = new QSqlTableModel(this);
    connect(fifo1, SIGNAL(fifo_updata(QString,QString)),this , SLOT(fifo_updata(QString,QString)),Qt::QueuedConnection);
    connect(lru1, SIGNAL(lru_updata(QString,QString)),this , SLOT(lru_updata(QString,QString)),Qt::QueuedConnection);
    connect(lfu1, SIGNAL(lfu_updata(QString,QString)),this , SLOT(lfu_updata(QString,QString)),Qt::QueuedConnection);
    connect(opt1, SIGNAL(opt_updata(QString,QString)),this , SLOT(opt_updata(QString,QString)),Qt::QueuedConnection);
    connect(history1, SIGNAL(history_updata(QModelIndex)),this , SLOT(history_updata(QModelIndex)),Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//页面数量随机
void MainWindow::on_taskSize_btn_clicked()
{
    if(!ui->taskSize_low_edit->text().isEmpty())
        page_low = ui->taskSize_low_edit->text().toInt();
    else
        page_low = 1;
    if(!ui->taskSize_high_edit->text().isEmpty())
        page_high = ui->taskSize_high_edit->text().toInt();
    else
        page_high = 9;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    taskSize = qrand()%(page_high-page_low) + page_low;
    ui->taskSize_edit->setText(QString::number(taskSize,10));
}

//页号序列随机(长度取决于页面数量)
void MainWindow::on_task_btn_clicked()
{
    if(!ui->task_low_edit->text().isEmpty())
        page_list_low = ui->task_low_edit->text().toInt();
    else
        page_list_low = 1;
    if(!ui->task_high_edit->text().isEmpty())
        page_list_high = ui->task_high_edit->text().toInt();
    else
        page_list_high = 9;
    taskSize = ui->taskSize_edit->text().toInt();
    QString list = "";
    task = new int[taskSize];
    qsrand(time(0));
    int i=0;
    for(;i<taskSize-1;i++)
    {
        task[i] = qrand()%(page_list_high-page_list_low) + page_list_low;
        list += QString::number(task[i],10)+",";
    }
    task[i] = qrand()%(page_list_high-page_list_low) + page_list_low;
    list += QString::number(task[i],10);
    ui->task_edit->setText(list);
}

void MainWindow::on_start_btn_clicked()
{
    on_clear_btn_clicked();
    if(ui->quick_table_check->isChecked() && quick_table_num>=memory_num)
    {
        QMessageBox::information(NULL, "警告", "快表大小应该小于内存大小");
        return;
    }
    memory_num = ui->memory_num_edit->text().toInt();
    memory_access = ui->memory_access_edit->text().toDouble();
    quick_table_access = ui->quick_table_access_edit->text().toDouble();
    miss_page_interrupt = ui->miss_page_interrupt_edit->text().toDouble();
    is_check = ui->quick_table_check->isChecked();
    QString str = ui->task_edit->text();
    QStringList lst = str.split(",");

    run_time = ui->run_time_edit->text().toInt();

    if(ui->run_time_edit->text().isEmpty())
    {
        run_time = 300;
    }

    taskSize = lst.size();
    task = new int[taskSize];
    for(int k=0;k<taskSize;k++)
    {
        QString str = lst[k];
        task[k] = str.toInt();
    }
    fifo1->start();
    lru1->start();
    lfu1->start();
    opt1->start();
}

void MainWindow::on_clear_btn_clicked()
{
    task = new int[taskSize];

    fifo_lackpage=0;
    fifo_ave_time=0;
    fifo_total_time=0;

    fifo_memory_page.clear();
    fifo_quick_table_page.clear();
    ui->fifo_textEdit->setText("");
    ui->fifo_interrupt_label->setText("");
    ui->fifo_miss_page_label->setText("");
    ui->fifo_total_time_label->setText("");
    ui->fifo_ave_time_label->setText("");

    lru_lackpage=0;
    lru_ave_time=0;
    lru_total_time=0;


    lru_memory_page.clear();
    lru_quick_table_page.clear();
    ui->lru_textEdit->setText("");
    ui->lru_interrupt_label->setText("");
    ui->lru_miss_page_label->setText("");
    ui->lru_total_time_label->setText("");
    ui->lru_ave_time_label->setText("");

    lfu_lackpage=0;
    lfu_ave_time=0;
    lfu_total_time=0;

    lfu_memory_page.clear();
    lfu_quick_table_page.clear();
    ui->lfu_textEdit->setText("");
    ui->lfu_interrupt_label->setText("");
    ui->lfu_miss_page_label->setText("");
    ui->lfu_total_time_label->setText("");
    ui->lfu_ave_time_label->setText("");

    opt_lackpage=0;
    opt_ave_time=0;
    opt_total_time=0;

    opt_memory_page.clear();
    opt_quick_table_page.clear();
    ui->opt_textEdit->setText("");
    ui->opt_interrupt_label->setText("");
    ui->opt_miss_page_label->setText("");
    ui->opt_total_time_label->setText("");
    ui->opt_ave_time_label->setText("");

}

void MainWindow::fifo_updata(QString sMessage,QString str)
{
    if(str==QString::fromLocal8Bit("textEdit"))
        ui->fifo_textEdit->append(sMessage);
    else if(str==QString::fromLocal8Bit("interrupt_label"))
        ui->fifo_interrupt_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("miss_page_label"))
        ui->fifo_miss_page_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("total_time_label"))
        ui->fifo_total_time_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("ave_time_label"))
        ui->fifo_ave_time_label->setText(sMessage);
}

void MainWindow::lru_updata(QString sMessage, QString str)
{
    if(str==QString::fromLocal8Bit("textEdit"))
        ui->lru_textEdit->append(sMessage);
    else if(str==QString::fromLocal8Bit("interrupt_label"))
        ui->lru_interrupt_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("miss_page_label"))
        ui->lru_miss_page_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("total_time_label"))
        ui->lru_total_time_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("ave_time_label"))
        ui->lru_ave_time_label->setText(sMessage);
}

void MainWindow::lfu_updata(QString sMessage, QString str)
{
    if(str==QString::fromLocal8Bit("textEdit"))
        ui->lfu_textEdit->append(sMessage);
    else if(str==QString::fromLocal8Bit("interrupt_label"))
        ui->lfu_interrupt_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("miss_page_label"))
        ui->lfu_miss_page_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("total_time_label"))
        ui->lfu_total_time_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("ave_time_label"))
        ui->lfu_ave_time_label->setText(sMessage);
}

void MainWindow::opt_updata(QString sMessage, QString str)
{
    if(str==QString::fromLocal8Bit("textEdit"))
        ui->opt_textEdit->append(sMessage);
    else if(str==QString::fromLocal8Bit("interrupt_label"))
        ui->opt_interrupt_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("miss_page_label"))
        ui->opt_miss_page_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("total_time_label"))
        ui->opt_total_time_label->setText(sMessage);
    else if(str==QString::fromLocal8Bit("ave_time_label"))
        ui->opt_ave_time_label->setText(sMessage);
}

void MainWindow::history_updata(QModelIndex index)
{
    int row_count = index.row();//获取点击所在行
    tabModel->setTable("page_replace");
    tabModel->select();
    QModelIndex index_memory_n  = tabModel->index(row_count,tabModel->fieldIndex("memory_n"), QModelIndex());
    ui->memory_num_edit->setText(tabModel->data(index_memory_n).toString());

    QModelIndex index_page_n  = tabModel->index(row_count,tabModel->fieldIndex("page_n"), QModelIndex());
    ui->taskSize_edit->setText(tabModel->data(index_page_n).toString());

    QModelIndex index_task_list  = tabModel->index(row_count,tabModel->fieldIndex("page_list"), QModelIndex());
    ui->task_edit->setText(tabModel->data(index_task_list).toString());

    QModelIndex index_memory_t  = tabModel->index(row_count,tabModel->fieldIndex("memory_t"), QModelIndex());
    ui->memory_access_edit->setText(tabModel->data(index_memory_t).toString());

    QModelIndex index_quick_table_t  = tabModel->index(row_count,tabModel->fieldIndex("quick_table_t"), QModelIndex());
    ui->quick_table_access_edit->setText(tabModel->data(index_quick_table_t).toString());

    QModelIndex index_interrupt_t  = tabModel->index(row_count,tabModel->fieldIndex("interrupt_t"), QModelIndex());
    ui->miss_page_interrupt_edit->setText(tabModel->data(index_interrupt_t).toString());

    QModelIndex index_is_check  = tabModel->index(row_count,tabModel->fieldIndex("is_check"), QModelIndex());
    int ck = tabModel->data(index_is_check).toInt();
    if(ck==1)
        ui->quick_table_check->setChecked(true);
    else
        ui->quick_table_check->setChecked(false);

    QModelIndex index_fifo_total_t  = tabModel->index(row_count,tabModel->fieldIndex("fifo_total_t"), QModelIndex());
    ui->fifo_total_time_label->setText(tabModel->data(index_fifo_total_t).toString());

    QModelIndex index_fifo_ave_t  = tabModel->index(row_count,tabModel->fieldIndex("fifo_ave_t"), QModelIndex());
    ui->fifo_ave_time_label->setText(tabModel->data(index_fifo_ave_t).toString());

    QModelIndex index_fifo_miss_page  = tabModel->index(row_count,tabModel->fieldIndex("fifo_miss_page"), QModelIndex());
    ui->fifo_miss_page_label->setText(tabModel->data(index_fifo_miss_page).toString()+"%");

    QModelIndex index_fifo_interrupt_n  = tabModel->index(row_count,tabModel->fieldIndex("fifo_interrupt_n"), QModelIndex());
    ui->fifo_interrupt_label->setText(tabModel->data(index_fifo_interrupt_n).toString()+"次");

    QModelIndex index_fifo_text  = tabModel->index(row_count,tabModel->fieldIndex("fifo_text"), QModelIndex());
    ui->fifo_textEdit->setText(tabModel->data(index_fifo_text).toString());


    QModelIndex index_lru_total_t  = tabModel->index(row_count,tabModel->fieldIndex("lru_total_t"), QModelIndex());
    ui->lru_total_time_label->setText(tabModel->data(index_lru_total_t).toString());

    QModelIndex index_lru_ave_t  = tabModel->index(row_count,tabModel->fieldIndex("lru_ave_t"), QModelIndex());
    ui->lru_ave_time_label->setText(tabModel->data(index_lru_ave_t).toString());

    QModelIndex index_lru_miss_page  = tabModel->index(row_count,tabModel->fieldIndex("lru_miss_page"), QModelIndex());
    ui->lru_miss_page_label->setText(tabModel->data(index_lru_miss_page).toString()+"%");

    QModelIndex index_lru_interrupt_n  = tabModel->index(row_count,tabModel->fieldIndex("lru_interrupt_n"), QModelIndex());
    ui->lru_interrupt_label->setText(tabModel->data(index_lru_interrupt_n).toString()+"次");

    QModelIndex index_lru_text  = tabModel->index(row_count,tabModel->fieldIndex("lru_text"), QModelIndex());
    ui->lru_textEdit->setText(tabModel->data(index_lru_text).toString());


    QModelIndex index_lfu_total_t  = tabModel->index(row_count,tabModel->fieldIndex("lfu_total_t"), QModelIndex());
    ui->lfu_total_time_label->setText(tabModel->data(index_lfu_total_t).toString());

    QModelIndex index_lfu_ave_t  = tabModel->index(row_count,tabModel->fieldIndex("lfu_ave_t"), QModelIndex());
    ui->lfu_ave_time_label->setText(tabModel->data(index_lfu_ave_t).toString());

    QModelIndex index_lfu_miss_page  = tabModel->index(row_count,tabModel->fieldIndex("lfu_miss_page"), QModelIndex());
    ui->lfu_miss_page_label->setText(tabModel->data(index_lfu_miss_page).toString()+"%");

    QModelIndex index_lfu_interrupt_n  = tabModel->index(row_count,tabModel->fieldIndex("lfu_interrupt_n"), QModelIndex());
    ui->lfu_interrupt_label->setText(tabModel->data(index_lfu_interrupt_n).toString()+"次");

    QModelIndex index_lfu_text  = tabModel->index(row_count,tabModel->fieldIndex("lfu_text"), QModelIndex());
    ui->lfu_textEdit->setText(tabModel->data(index_lfu_text).toString());


    QModelIndex index_opt_total_t  = tabModel->index(row_count,tabModel->fieldIndex("opt_total_t"), QModelIndex());
    ui->opt_total_time_label->setText(tabModel->data(index_opt_total_t).toString());

    QModelIndex index_opt_ave_t  = tabModel->index(row_count,tabModel->fieldIndex("opt_ave_t"), QModelIndex());
    ui->opt_ave_time_label->setText(tabModel->data(index_opt_ave_t).toString());

    QModelIndex index_opt_miss_page  = tabModel->index(row_count,tabModel->fieldIndex("opt_miss_page"), QModelIndex());
    ui->opt_miss_page_label->setText(tabModel->data(index_opt_miss_page).toString()+"%");

    QModelIndex index_opt_interrupt_n  = tabModel->index(row_count,tabModel->fieldIndex("opt_interrupt_n"), QModelIndex());
    ui->opt_interrupt_label->setText(tabModel->data(index_opt_interrupt_n).toString()+"次");

    QModelIndex index_opt_text  = tabModel->index(row_count,tabModel->fieldIndex("opt_text"), QModelIndex());
    ui->opt_textEdit->setText(tabModel->data(index_opt_text).toString());


}




void MainWindow::on_stop_continue_btn_clicked()
{
    if(ui->stop_continue_btn->text()=="暂停执行")
    {
        fifo1->pause.lock();
        lru1->pause.lock();
        lfu1->pause.lock();
        opt1->pause.lock();
        ui->stop_continue_btn->setText("继续执行");
    }
    else
    {
        fifo1->pause.unlock();
        lru1->pause.unlock();
        lfu1->pause.unlock();
        opt1->pause.unlock();
        ui->stop_continue_btn->setText("暂停执行");
    }
}


void MainWindow::on_stop_btn_clicked()
{
    fifo1->terminate();
    lru1->terminate();
    lfu1->terminate();
    opt1->terminate();
}


void MainWindow::on_quick_table_check_clicked(bool checked)
{
    if(checked)
        ui->quick_table_num_edit->setEnabled(true);
    else
        ui->quick_table_num_edit->setEnabled(false);
}

void MainWindow::on_quick_table_num_edit_textChanged()
{
    quick_table_num = ui->quick_table_num_edit->text().toInt();
}

void MainWindow::on_save_btn_clicked()
{
    tabModel->setTable("page_replace");
    tabModel->select();

    int row_count = tabModel->rowCount();

    tabModel->insertRow(row_count);

    QModelIndex index_id  = tabModel->index(row_count,tabModel->fieldIndex("id"), QModelIndex());
    tabModel->setData(index_id,row_count+1);

    QModelIndex index_memory_n  = tabModel->index(row_count,tabModel->fieldIndex("memory_n"), QModelIndex());
    tabModel->setData(index_memory_n,ui->memory_num_edit->text().toInt());

    QModelIndex index_page_n  = tabModel->index(row_count,tabModel->fieldIndex("page_n"), QModelIndex());
    tabModel->setData(index_page_n,ui->taskSize_edit->text().toInt());

    QModelIndex index_task_list  = tabModel->index(row_count,tabModel->fieldIndex("page_list"), QModelIndex());
    tabModel->setData(index_task_list,ui->task_edit->text());

    QModelIndex index_memory_t  = tabModel->index(row_count,tabModel->fieldIndex("memory_t"), QModelIndex());
    tabModel->setData(index_memory_t,ui->memory_access_edit->text().toDouble());

    QModelIndex index_quick_table_t  = tabModel->index(row_count,tabModel->fieldIndex("quick_table_t"), QModelIndex());
    tabModel->setData(index_quick_table_t,ui->quick_table_access_edit->text().toDouble());

    QModelIndex index_interrupt_t  = tabModel->index(row_count,tabModel->fieldIndex("interrupt_t"), QModelIndex());
    tabModel->setData(index_interrupt_t,ui->miss_page_interrupt_edit->text().toDouble());

    QModelIndex index_is_check  = tabModel->index(row_count,tabModel->fieldIndex("is_check"), QModelIndex());
    if(ui->quick_table_check->isChecked()==true)
        tabModel->setData(index_is_check,1);
    else
        tabModel->setData(index_is_check,0);

    QModelIndex index_fifo_total_t  = tabModel->index(row_count,tabModel->fieldIndex("fifo_total_t"), QModelIndex());
    tabModel->setData(index_fifo_total_t,ui->fifo_total_time_label->text().toDouble());

    QModelIndex index_fifo_ave_t  = tabModel->index(row_count,tabModel->fieldIndex("fifo_ave_t"), QModelIndex());
    tabModel->setData(index_fifo_ave_t,ui->fifo_ave_time_label->text().toDouble());

    QModelIndex index_fifo_miss_page  = tabModel->index(row_count,tabModel->fieldIndex("fifo_miss_page"), QModelIndex());
    tabModel->setData(index_fifo_miss_page,ui->fifo_miss_page_label->text().replace("%","").toDouble());

    QModelIndex index_fifo_interrupt_n  = tabModel->index(row_count,tabModel->fieldIndex("fifo_interrupt_n"), QModelIndex());
    tabModel->setData(index_fifo_interrupt_n,ui->fifo_interrupt_label->text().replace("次","").toInt());

    QModelIndex index_fifo_text  = tabModel->index(row_count,tabModel->fieldIndex("fifo_text"), QModelIndex());
    tabModel->setData(index_fifo_text,ui->fifo_textEdit->toPlainText());


    QModelIndex index_lru_total_t  = tabModel->index(row_count,tabModel->fieldIndex("lru_total_t"), QModelIndex());
    tabModel->setData(index_lru_total_t,ui->lru_total_time_label->text().toDouble());

    QModelIndex index_lru_ave_t  = tabModel->index(row_count,tabModel->fieldIndex("lru_ave_t"), QModelIndex());
    tabModel->setData(index_lru_ave_t,ui->lru_ave_time_label->text().toDouble());

    QModelIndex index_lru_miss_page  = tabModel->index(row_count,tabModel->fieldIndex("lru_miss_page"), QModelIndex());
    tabModel->setData(index_lru_miss_page,ui->lru_miss_page_label->text().replace("%","").toDouble());

    QModelIndex index_lru_interrupt_n  = tabModel->index(row_count,tabModel->fieldIndex("lru_interrupt_n"), QModelIndex());
    tabModel->setData(index_lru_interrupt_n,ui->lru_interrupt_label->text().replace("次","").toInt());

    QModelIndex index_lru_text  = tabModel->index(row_count,tabModel->fieldIndex("lru_text"), QModelIndex());
    tabModel->setData(index_lru_text,ui->lru_textEdit->toPlainText());




    QModelIndex index_lfu_total_t  = tabModel->index(row_count,tabModel->fieldIndex("lfu_total_t"), QModelIndex());
    tabModel->setData(index_lfu_total_t,ui->lfu_total_time_label->text().toDouble());

    QModelIndex index_lfu_ave_t  = tabModel->index(row_count,tabModel->fieldIndex("lfu_ave_t"), QModelIndex());
    tabModel->setData(index_lfu_ave_t,ui->lfu_ave_time_label->text().toDouble());

    QModelIndex index_lfu_miss_page  = tabModel->index(row_count,tabModel->fieldIndex("lfu_miss_page"), QModelIndex());
    tabModel->setData(index_lfu_miss_page,ui->lfu_miss_page_label->text().replace("%","").toDouble());

    QModelIndex index_lfu_interrupt_n  = tabModel->index(row_count,tabModel->fieldIndex("lfu_interrupt_n"), QModelIndex());
    tabModel->setData(index_lfu_interrupt_n,ui->lfu_interrupt_label->text().replace("次","").toInt());

    QModelIndex index_lfu_text  = tabModel->index(row_count,tabModel->fieldIndex("lfu_text"), QModelIndex());
    tabModel->setData(index_lfu_text,ui->lfu_textEdit->toPlainText());



    QModelIndex index_opt_total_t  = tabModel->index(row_count,tabModel->fieldIndex("opt_total_t"), QModelIndex());
    tabModel->setData(index_opt_total_t,ui->opt_total_time_label->text().toDouble());

    QModelIndex index_opt_ave_t  = tabModel->index(row_count,tabModel->fieldIndex("opt_ave_t"), QModelIndex());
    tabModel->setData(index_opt_ave_t,ui->opt_ave_time_label->text().toDouble());

    QModelIndex index_opt_miss_page  = tabModel->index(row_count,tabModel->fieldIndex("opt_miss_page"), QModelIndex());
    tabModel->setData(index_opt_miss_page,ui->opt_miss_page_label->text().replace("%","").toDouble());

    QModelIndex index_opt_interrupt_n  = tabModel->index(row_count,tabModel->fieldIndex("opt_interrupt_n"), QModelIndex());
    tabModel->setData(index_opt_interrupt_n,ui->opt_interrupt_label->text().replace("次","").toInt());

    QModelIndex index_opt_text  = tabModel->index(row_count,tabModel->fieldIndex("opt_text"), QModelIndex());
    tabModel->setData(index_opt_text,ui->opt_textEdit->toPlainText());


    // 开始事务操作
    tabModel->database().transaction();
    if (tabModel->submitAll() && tabModel->rowCount()!=0)
    {
        QMessageBox::information(this, "提交信息", "保存成功！",QMessageBox::Ok);
        tabModel->database().commit(); //提交
    }
    else
    {
        QMessageBox::critical(this, "错误信息","提交错误,错误信息\n"+tabModel->lastError().text(),QMessageBox::Ok,QMessageBox::NoButton);
        tabModel->database().rollback(); //回滚
    }
}

void MainWindow::on_open_btn_clicked()
{
       history1->setModal(true);
       history1->show();
}

void MainWindow::on_result_btn_clicked()
{
    fifo1->exit();
    lru1->exit();
    lfu1->exit();
    opt1->exit();
}
