#include"opt.h"


void opt::work(int a,double &per_time)
{
    if(!is_check)//无快表模式
    {
        if(!opt_memory_page.contains(a))//不在内存，产生缺页中断
        {
            opt_lackpage++;

            if(opt_memory_page.size()<memory_num)
            {
                opt_memory_page.append(a);
            }
            else//遍历之后所有页号，与内存内页号相同则移到尾部，最后删除头部
            {
                QMap<int,bool> has_move;//移动过的不能再移动
                int cnt = 0;//移动次数要小于内存块数
                for(int j=opt_i+1;j<taskSize;j++)//从需要进入内存的页号的后一位开始
                {
                    if(opt_memory_page.contains(task[j]) && has_move[task[j]]!=true && cnt<memory_num-1)
                    {
                        opt_memory_page.removeOne(task[j]);
                        opt_memory_page.append(task[j]);
                        has_move.insert(task[j],true);
                        cnt++;
                    }
                }

                opt_memory_page.removeFirst();
                opt_memory_page.append(a);
            }

            opt_total_time += 3*memory_access + miss_page_interrupt;
            per_time += 3*memory_access + miss_page_interrupt;
        }
        else
        {
            opt_total_time += 2*memory_access;
            per_time += 2*memory_access;
        }
    }
    else //快表模式
    {
        if(!opt_quick_table_page.contains(a))//不在快表
        {
            if(!opt_memory_page.contains(a))//不在内存，产生缺页中断
            {
                opt_lackpage++;
                if(opt_memory_page.size()<memory_num)//内存块未满,直接调入
                {
                    opt_memory_page.append(a);
                }
                else
                {
                    QMap<int,bool> has_move;//移动过的不能再移动
                    int cnt = 0;//移动次数要小于内存块数
                    for(int j=opt_i+1;j<taskSize;j++)//从需要进入内存的页号的后一位开始
                    {
                        if(opt_memory_page.contains(task[j]) && has_move[task[j]]!=true && cnt<memory_num-1)
                        {
                            opt_memory_page.removeOne(task[j]);
                            opt_memory_page.append(task[j]);
                            has_move.insert(task[j],true);
                            cnt++;
                        }
                    }
                    opt_memory_page.removeFirst();
                    opt_memory_page.append(a);
                }

                //快表置换用FIFO
                if(opt_quick_table_page.size()<quick_table_num)
                {
                    opt_quick_table_page.append(a);
                }
                else
                {
                    opt_quick_table_page.removeFirst();
                    opt_quick_table_page.append(a);
                }
                opt_total_time += 3*memory_access + miss_page_interrupt;
                per_time += 3*memory_access + miss_page_interrupt;

            }
            else//在内存
            {
                opt_total_time += 2*memory_access;
                per_time += 2*memory_access;
            }
        }
        else//在快表
        {
            opt_total_time += quick_table_access + memory_access;
            per_time += quick_table_access + memory_access;
        }
    }
}

void opt::run()
{
    for(opt_i=0;opt_i<taskSize;opt_i++)
    {
        pause.lock();
        QString snumber="";
        double per_time = 0;
        work(task[opt_i],per_time);
        QString str1="查找";
        QString s1 = QString::number(task[opt_i], 10);
        snumber+=str1;
        snumber+=s1;
        snumber+=":";

        for(int j=0;j<memory_num;j++)
        {
            if(j<opt_memory_page.size())
            {
                QString s = QString::number(opt_memory_page[j], 10);
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
        emit opt_updata(snumber,"textEdit");
        mwsleep(run_time);
    }
    QString s1 = QString::number(taskSize, 10);
    s1 = QString::number(opt_lackpage, 10);
    emit opt_updata(s1+"次","interrupt_label");
    double d=(double)opt_lackpage/taskSize*100;
    s1 = QString::number(d, 10,2);
    emit opt_updata(s1+"%","miss_page_label");
    emit opt_updata(QString::number(opt_total_time,10,2),"total_time_label");
    opt_ave_time = (double)opt_total_time/taskSize;
    emit opt_updata(QString::number(opt_ave_time,10,2),"ave_time_label");

}


