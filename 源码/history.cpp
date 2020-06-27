#include "history.h"
#include "ui_history.h"

history::history(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::history)
{
    ui->setupUi(this);
    tabModel = new QSqlTableModel(this);
    ui->tableView->setModel(tabModel);
    tabModel->setTable("page_replace");
    tabModel->setHeaderData(1,Qt::Horizontal,"内存块数");
    tabModel->setHeaderData(2,Qt::Horizontal,"页面大小");
    tabModel->setHeaderData(3,Qt::Horizontal,"页号序列");
    tabModel->setHeaderData(4,Qt::Horizontal,"内存存取时间");
    tabModel->setHeaderData(5,Qt::Horizontal,"快表存取时间");
    tabModel->setHeaderData(6,Qt::Horizontal,"中断执行时间");
    tabModel->setHeaderData(7,Qt::Horizontal,"是否勾选快表");

    tabModel->setHeaderData(8,Qt::Horizontal,"FIFO总时间");
    tabModel->setHeaderData(9,Qt::Horizontal,"FIFO平均时间");
    tabModel->setHeaderData(10,Qt::Horizontal,"FIFO缺页率");
    tabModel->setHeaderData(11,Qt::Horizontal,"FIFO中断次数");
    tabModel->setHeaderData(12,Qt::Horizontal,"FIFO执行过程");

    tabModel->setHeaderData(13,Qt::Horizontal,"LRU总时间");
    tabModel->setHeaderData(14,Qt::Horizontal,"LRU平均时间");
    tabModel->setHeaderData(15,Qt::Horizontal,"LRU缺页率");
    tabModel->setHeaderData(16,Qt::Horizontal,"LRU中断次数");
    tabModel->setHeaderData(17,Qt::Horizontal,"LRU执行过程");

    tabModel->setHeaderData(18,Qt::Horizontal,"LFU总时间");
    tabModel->setHeaderData(19,Qt::Horizontal,"LFU平均时间");
    tabModel->setHeaderData(20,Qt::Horizontal,"LFU缺页率");
    tabModel->setHeaderData(21,Qt::Horizontal,"LFU中断次数");
    tabModel->setHeaderData(22,Qt::Horizontal,"LFU执行过程");

    tabModel->setHeaderData(23,Qt::Horizontal,"OPT总时间");
    tabModel->setHeaderData(24,Qt::Horizontal,"OPT平均时间");
    tabModel->setHeaderData(25,Qt::Horizontal,"OPT缺页率");
    tabModel->setHeaderData(26,Qt::Horizontal,"OPT中断次数");
    tabModel->setHeaderData(27,Qt::Horizontal,"OPT执行过程");

    tabModel->select();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotRowDoubleClicked(const QModelIndex &)));
}

history::~history()
{
    delete ui;
}

void history::slotRowDoubleClicked(const QModelIndex index)
{
    emit history_updata(index);
    this->close();
}
