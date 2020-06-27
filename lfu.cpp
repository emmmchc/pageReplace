#include"lfu.h"
#include <algorithm>


void lfu::work(int a,double &per_time)
{
    //qDebug() << a;
    if(!is_check)//无快表模式
    {
        bool flag = true;
        int index = 0;
        for (QPair<int, int> pair : lfu_memory_page)
        {
            if(pair.first == a)
            {
                flag = false;
                break;
            }
            index++;
        }

        if(flag)//不在内存
        {
            lfu_lackpage++;
            //内存满，删去访问频率最小的那个，相等的频率按fifo删除
            if(lfu_memory_page.length()>=memory_num)
            {
                QPair<int,int> min_obj;
                min_obj = lfu_memory_page[0];
                for (QPair<int, int> pair : lfu_memory_page)
                {
                    if(pair.second<min_obj.second)
                    {
                        min_obj = pair;
                    }
                }
                //qDebug() << "删除的是：" << min_obj;
                lfu_memory_page.removeOne(min_obj);
            }

            lfu_memory_page.append(qMakePair(a, 1));
            //qDebug() << lfu_memory_page;
            lfu_total_time += 3*memory_access + miss_page_interrupt;
            per_time += 3*memory_access + miss_page_interrupt;
        }
        else
        {
            lfu_memory_page[index].second++;
            lfu_total_time += 2*memory_access;
            per_time += 2*memory_access;

        }
    }
    else //快表模式
    {
        if(!lfu_quick_table_page.contains(a))//不在快表
        {
            bool flag = true;
            int index = 0;
            for (QPair<int, int> pair : lfu_memory_page)
            {
                if(pair.first == a)
                {
                    lfu_total_time += 2*memory_access;
                    per_time += 2*memory_access;
                    flag = false;

                    break;
                }
                index++;
            }

            if(flag)//不在内存
            {
                lfu_lackpage++;
                //内存满，删去访问频率最小的那个，相等的频率按fifo删除
                if(lfu_memory_page.length()>=memory_num)
                {
                    QPair<int,int> min_obj;
                    min_obj = lfu_memory_page[0];
                    for (QPair<int, int> pair : lfu_memory_page)
                    {
                        if(pair.second<min_obj.second)
                        {
                            min_obj = pair;
                        }
                    }
                    //qDebug() << "删除的是：" << min_obj;
                    lfu_memory_page.removeOne(min_obj);
                }

                lfu_memory_page.append(qMakePair(a, 1));
                //qDebug() << lfu_memory_page;


                //快表置换用FIFO
                if(lfu_quick_table_page.size()<quick_table_num)
                {
                    lfu_quick_table_page.append(a);
                }
                else
                {
                    lfu_quick_table_page.removeFirst();
                    lfu_quick_table_page.append(a);
                }

                lfu_total_time += 3*memory_access + miss_page_interrupt;
                per_time += 3*memory_access + miss_page_interrupt;
            }
            else//在内存
            {
                lfu_memory_page[index].second++;
                lfu_total_time += 2*memory_access;
                per_time += 2*memory_access;

            }
        }
        else//在快表
        {
            int index = 0;
            for (QPair<int, int> pair : lfu_memory_page)
            {
                if(pair.first == a)
                {
                   lfu_memory_page[index].second++;
                   break;
                }
                index++;
            }
            lfu_total_time += quick_table_access + memory_access;
            per_time += quick_table_access + memory_access;
        }
    }
}

void lfu::run()
{
    for(lfu_i=0;lfu_i<taskSize;lfu_i++)
    {
        pause.lock();
        QString snumber="";
        double per_time = 0;
        work(task[lfu_i],per_time);
        QString str1="查找";
        QString s1 = QString::number(task[lfu_i], 10);
        snumber+=str1;
        snumber+=s1;
        snumber+=":";

        for(int j=0;j<memory_num;j++)
        {
            if(j<lfu_memory_page.length())
            {
                QString s = QString::number(lfu_memory_page[j].first, 10);
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

        emit lfu_updata(snumber,"textEdit");
        mwsleep(run_time);
    }
    QString s1 = QString::number(taskSize, 10);
    s1 = QString::number(lfu_lackpage, 10);
    emit lfu_updata(s1+"次","interrupt_label");
    double d=(double)lfu_lackpage/taskSize*100;
    s1 = QString::number(d, 10,2);
    emit lfu_updata(s1+"%","miss_page_label");
    emit lfu_updata(QString::number(lfu_total_time,10,2),"total_time_label");
    lfu_ave_time = (double)lfu_total_time/taskSize;
    emit lfu_updata(QString::number(lfu_ave_time,10,2),"ave_time_label");

}


