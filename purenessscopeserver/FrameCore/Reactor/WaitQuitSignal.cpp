/*
 * WaitQuitSignal.cpp
 *
 * Linux�µ��߳�ʵ����������������(light weighted process),�߳�����ʱ�����ɶ�Ӧ�Ľ��̿��ƽṹ��
 * ֻ�Ǹýṹ�븸�̵߳Ľ��̿��ƽṹ������ͬһ�������ڴ�ռ䡣 ͬʱ���̵߳Ľ��̿��ƽṹ���Ӹ��̣߳����̣�
 * �����Ƶõ�ͬ���Ľ�����Ϣ������ļ��б���ź���������ȡ����������������߳�����֮���޸����ź�������
 * �룬�˿����߳�ʹ�õ������߳�ԭ�еĽ�����Ϣ��������߳���Ȼ���SIGINT��SIGTERM�źŽ��з�Ӧ����˵�
 * ������Ctrl+C������ SIGINT�źŵ�ʱ�������̲�������źţ����ӽ��̣��̣߳������Ĭ�ϴ������˳���
 * �ӽ����˳���ͬʱ���򸸽��̣��̣߳�����SIGCHLD�źţ���ʾ�ӽ����˳������ڸ��ź�û�б���������˻ᵼ��
 * �����̣��̣߳�Ҳ�����˳���������ǰ�����������������������һ����������������߳�����ǰ�����ź����ã�
 * �������߳��ڲ������ź����á�
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
    sigemptyset(&m_wait_mask);
    sigaddset(&m_wait_mask, SIGINT);
    sigaddset(&m_wait_mask, SIGQUIT);
    sigaddset(&m_wait_mask, SIGTERM);
    sigaddset(&m_wait_mask, SIGKILL);
    pthread_sigmask(SIG_BLOCK, &m_wait_mask, nullptr);

    m_time.tv_sec  = 0;
    m_time.tv_nsec = 0;
}
bool WaitQuitSignal::wait(bool& blFlag)
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

    return blFlag;
}
#endif
