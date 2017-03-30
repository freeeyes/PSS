/*
 * WaitQuitSignal.h
 *
 *  Created on: Aug 14, 2011
 *      Author: xian0617
 */

#ifndef WAITQUITSIGNAL_H_
#define WAITQUITSIGNAL_H_

#ifndef WIN32
#include <signal.h>
#include <time.h>
#include <pthread.h>

class WaitQuitSignal
{
public:
    static void init();
    static bool wait(bool& flag);
private:
    static sigset_t m_wait_mask;
    static struct timespec m_time;
};

#endif
#endif /* WAITQUITSIGNAL_H_ */