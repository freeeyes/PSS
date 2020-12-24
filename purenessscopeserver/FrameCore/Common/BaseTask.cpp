#include "BaseTask.h"

int Task_Common_CloseMsgQueue(ACE_Task<ACE_MT_SYNCH>* pTask, ACE_Condition<ACE_Thread_Mutex>& cond, ACE_Thread_Mutex& mutex)
{
    // We can choose to process the message or to differ it into the message
    // queue, and process them into the svc() method. Chose the last option.
    int retval;

    ACE_Message_Block* mblk = nullptr;
    ACE_NEW_RETURN(mblk, ACE_Message_Block(0, ACE_Message_Block::MB_STOP), -1);

    // If queue is full, flush it before block in while
    if (pTask->msg_queue()->is_full() && pTask->msg_queue()->flush() == -1)
    {
        PSS_LOGGER_DEBUG("[CLogManager::CloseMsgQueue]put error flushing queue.");
    }

    mutex.acquire();

    while ((retval = pTask->putq(mblk)) == -1)
    {
        if (pTask->msg_queue()->state() != ACE_Message_Queue_Base::PULSED)
        {
            PSS_LOGGER_DEBUG("[CLogManager::CloseMsgQueue]put Queue not activated.");
            break;
        }
    }

    cond.wait();
    mutex.release();

    return retval;
}

