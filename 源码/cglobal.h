#ifndef CGLOBAL_H
#define CGLOBAL_H

#include <QVector>
#include <QList>
#include <QMutex>
extern int page_low;
extern int page_high;
extern int page_list_low;
extern int page_list_high;

extern int run_time;

extern int quick_table_num;
extern int memory_num;
extern double memory_access;
extern double quick_table_access;
extern double miss_page_interrupt;
extern int taskSize;
extern int *task;
extern bool is_check;

extern int fifo_i;
extern int fifo_lackpage;
extern double fifo_ave_time;
extern double fifo_total_time;
extern QList<int> fifo_memory_page ;
extern QList<int> fifo_quick_table_page ;

extern int lru_i;
extern int lru_lackpage;
extern double lru_ave_time;
extern double lru_total_time;
extern QList<int> lru_memory_page ;
extern QList<int> lru_quick_table_page ;

extern int lfu_i;
extern int lfu_lackpage;
extern double lfu_ave_time;
extern double lfu_total_time;
extern QList<QPair<int,int>> lfu_memory_page ;
extern QList<int> lfu_quick_table_page ;

extern int opt_i;
extern int opt_lackpage;
extern double opt_ave_time;
extern double opt_total_time;
extern QList<int> opt_memory_page ;
extern QList<int> opt_quick_table_page ;

extern bool mutex_pause;

void mwsleep(int m);
#endif // CGLOBAL_H
