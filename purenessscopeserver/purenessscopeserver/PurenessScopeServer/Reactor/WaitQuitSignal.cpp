/*
 * WaitQuitSignal.cpp
 *
 * Linux下的线程实质上是轻量级进程(light weighted process),线程生成时会生成对应的进程控制结构，
 * 只是该结构与父线程的进程控制结构共享了同一个进程内存空间。 同时新线程的进程控制结构将从父线程（进程）
 * 处复制得到同样的进程信息，如打开文件列表和信号阻塞掩码等。由于我们是在子线程生成之后修改了信号阻塞掩
 * 码，此刻子线程使用的是主线程原有的进程信息，因此子线程仍然会对SIGINT和SIGTERM信号进行反应，因此当
 * 我们用Ctrl+C发出了 SIGINT信号的时候，主进程不处理该信号，而子进程（线程）会进行默认处理，即退出。
 * 子进程退出的同时会向父进程（线程）发送SIGCHLD信号，表示子进程退出，由于该信号没有被阻塞，因此会导致
 * 主进程（线程）也立刻退出，出现了前述的运行情况。因而该问题的一个解决方法是在子线程生成前进行信号设置，
 * 或在子线程内部进行信号设置。
 *  Created on: Aug 14, 2011
 *      Author: xian0617
 */

#ifndef WIN32
#include <iostream>
#include "WaitQuitSignal.h"
#include "ServerManager.h"

sigset_t WaitQuitSignal::m_wait_mask;
struct timespec WaitQuitSignal::m_time;

void WaitQuitSignal::init()
{
    try
    {
        sigemptyset(&m_wait_mask);
        sigaddset(&m_wait_mask, SIGINT);
        sigaddset(&m_wait_mask, SIGQUIT);
        sigaddset(&m_wait_mask, SIGTERM);
        sigaddset(&m_wait_mask, SIGKILL);
        pthread_sigmask(SIG_BLOCK, &m_wait_mask, 0);
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    m_time.tv_sec=0;
    m_time.tv_nsec =0;
}
bool WaitQuitSignal::wait(bool& blFlag)
{
    try
    {
        siginfo_t sig ;

        switch(sigtimedwait(&m_wait_mask,&sig,&m_time))
        {
        case SIGKILL:
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
            App_ServerManager::instance()->Close();
            blFlag = false;
            break;

        default:
            break;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return blFlag;
}
#endif
