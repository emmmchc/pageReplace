#include"lru.h"


//将最久未使用的放到QList开头
void lru::work(int a,double &per_time)
{
    if(!is_check)//无快表模式
    {
        if(!lru_memory_page.contains(a))//不在内存，产生缺页中断
        {
            lru_lackpage++;
            if(lru_memory_page.size()<memory_num)
            {
                lru_memory_page.append(a);
            }
            else
            {
                lru_memory_page.removeFirst();
                lru_memory_page.append(a);
            }
            lru_total_time += 3*memory_access + miss_page_interrupt;
            per_time += 3*memory_access + miss_page_interrupt;

        }
        else//在内存中则找出这个元素并删除，在末尾添加这个元素，保证最近访问的置于末尾
        {
            lru_memory_page.removeOne(a);
            lru_memory_page.append(a);
            lru_total_time += 2*memory_access;
            per_time += 2*memory_access;
        }
    }
    else //快表模式
    {
        if(!lru_quick_table_page.contains(a))//不在快表
        {
            if(!lru_memory_page.contains(a))//不在内存，产生缺页中断
            {
                lru_lackpage++;
                if(lru_memory_page.size()<memory_num)//内存块未满,直接调入
                {
                    lru_memory_page.append(a);
                }
                else
                {
                    lru_memory_page.removeFirst();
                    lru_memory_page.append(a);
                }

                //快表置换用FIFO
                if(lru_quick_table_page.size()<quick_table_num)
                {
                    lru_quick_table_page.append(a);
                }
                else
                {
                    lru_quick_table_page.removeFirst();
                    lru_quick_table_page.append(a);
                }
                lru_total_time += 3*memory_access + miss_page_interrupt;
                per_time += 3*memory_access + miss_page_interrupt;
            }
            else//在内存
            {
                lru_memory_page.removeOne(a);
                lru_memory_page.append(a);
                lru_total_time += 2*memory_access;
                per_time += 2*memory_access;
            }
        }
        else//在快表
        {
            lru_memory_page.removeOne(a);
            lru_memory_page.append(a);
            lru_total_time += quick_table_access + memory_access;
            per_time += quick_table_access + memory_access;
        }
    }
}

void lru::run()
{
    for(lru_i=0;lru_i<taskSize;lru_i++)
    {
        pause.lock();
        QString snumber="";
        double per_time = 0;
        work(task[lru_i],per_time);
        QString str1="查找";
        QString s1 = QString::number(task[lru_i], 10);
        snumber+=str1;
        snumber+=s1;
        snumber+=":";

        for(int j=0;j<memory_num;j++)
        {
            if(j<lru_memory_page.size())
            {
                QString s = QString::number(lru_memory_page[j], 10);
                snumber+=s;
                snumber+=" ";
            }
            else
            {
                snumber+=" ";
            }
        }
        pause.unlock();

        snumber += "耗费时间:"+QString::number(per_time,10,2);
        emit lru_updata(snumber,"textEdit");
        mwsleep(run_time);
    }
    QString s1 = QString::number(taskSize, 10);
    s1 = QString::number(lru_lackpage, 10);
    emit lru_updata(s1+"次","interrupt_label");
    double d=(double)lru_lackpage/taskSize*100;
    s1 = QString::number(d, 10,2);
    emit lru_updata(s1+"%","miss_page_label");
    emit lru_updata(QString::number(lru_total_time,10,2),"total_time_label");
    lru_ave_time = (double)lru_total_time/taskSize;
    emit lru_updata(QString::number(lru_ave_time,10,2),"ave_time_label");
}
