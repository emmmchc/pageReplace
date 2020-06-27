#include"fifo.h"

void fifo::work(int a,double &per_time)
{

        if(!is_check)//无快表模式
        {
            if(!fifo_memory_page.contains(a))//不在内存，产生缺页中断
            {
                fifo_lackpage++;
                if(fifo_memory_page.size()<memory_num)
                {
                    fifo_memory_page.append(a);
                }
                else
                {
                    fifo_memory_page.removeFirst();
                    fifo_memory_page.append(a);
                }
                fifo_total_time += 3*memory_access + miss_page_interrupt;
                per_time += 3*memory_access + miss_page_interrupt;
            }
            else
            {
                fifo_total_time += 2*memory_access;
                per_time += 2*memory_access;
            }
        }
        else //快表模式
        {
            if(!fifo_quick_table_page.contains(a))//不在快表
            {
                if(!fifo_memory_page.contains(a))//不在内存，产生缺页中断
                {
                    fifo_lackpage++;
                    if(fifo_memory_page.size()<memory_num)//内存块未满,直接调入
                    {
                        fifo_memory_page.append(a);
                    }
                    else
                    {
                        fifo_memory_page.removeFirst();
                        fifo_memory_page.append(a);
                    }

                    //快表置换用FIFO
                    if(fifo_quick_table_page.size()<quick_table_num)
                    {
                        fifo_quick_table_page.append(a);
                    }
                    else
                    {
                        fifo_quick_table_page.removeFirst();
                        fifo_quick_table_page.append(a);
                    }
                    fifo_total_time += 3*memory_access + miss_page_interrupt;
                    per_time += 3*memory_access + miss_page_interrupt;
                }
                else//在内存
                {
                    fifo_total_time += 2*memory_access;
                    per_time += 2*memory_access;
                }
            }
            else//在快表
            {
                fifo_total_time += quick_table_access + memory_access;
                per_time += quick_table_access + memory_access;;
            }
        }


}

void fifo::run()
{

        for(fifo_i=0;fifo_i<taskSize;fifo_i++)
        {
            pause.lock();
            QString snumber="";
            double per_time = 0;
            work(task[fifo_i],per_time);
            QString str1="查找";
            QString s1 = QString::number(task[fifo_i], 10);
            snumber+=str1;
            snumber+=s1;
            snumber+=":";

            for(int j=0;j<memory_num;j++)
            {
                if(j<fifo_memory_page.size())
                {
                    QString s = QString::number(fifo_memory_page[j], 10);
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
            emit fifo_updata(snumber,"textEdit");
            mwsleep(run_time);//睡眠2000ms即2s
        }
        QString s1 = QString::number(taskSize, 10);
        s1 = QString::number(fifo_lackpage, 10);
        emit fifo_updata(s1+"次","interrupt_label");
        double d=(double)fifo_lackpage/taskSize*100;
        s1 = QString::number(d, 10,2);
        emit fifo_updata(s1+"%","miss_page_label");
        emit fifo_updata(QString::number(fifo_total_time,10,2),"total_time_label");
        fifo_ave_time = (double)fifo_total_time/taskSize;
        emit fifo_updata(QString::number(fifo_ave_time,10,2),"ave_time_label");


}


