#include "cglobal.h"
#include <QEventLoop>
#include <QTimer>

int page_low = 1;
int page_high = 9;
int page_list_low = 1;
int page_list_high = 9;

int run_time = 300;

int quick_table_num=2;//快表大小
int memory_num = 3;//内存块数
double memory_access=8;//内存存取时间
double quick_table_access=1;//快表存取时间
double miss_page_interrupt=20;//缺页中断处理时间
int taskSize = 20;//页面序列数量
int *task = new int[taskSize];//页面序列存放
bool is_check=false;//快表启用判断


int fifo_i = 0;
int fifo_lackpage=0;//缺页数
double fifo_ave_time=0;//平均时间
double fifo_total_time=0;//总时间
QList<int> fifo_memory_page;//内存现有页数存放
QList<int> fifo_quick_table_page ;//快表现有页数存放

int lru_i = 0;
int lru_lackpage=0;
double lru_ave_time=0;
double lru_total_time=0;
QList<int> lru_memory_page;
QList<int> lru_quick_table_page ;

int lfu_i = 0;
int lfu_lackpage=0;
double lfu_ave_time=0;
double lfu_total_time=0;
QList<QPair<int,int>> lfu_memory_page;
QList<int> lfu_quick_table_page ;

int opt_i = 0;
int opt_lackpage=0;
double opt_ave_time=0;
double opt_total_time=0;
QList<int> opt_memory_page;
QList<int> opt_quick_table_page ;

bool mutex_pause = false;

void mwsleep(int m)
{
    QEventLoop eventloop;
    QTimer::singleShot(m, &eventloop, SLOT(quit()));
    eventloop.exec();
}
