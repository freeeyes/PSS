#include "TimeEvent.h"

CTimeEventTask::CTimeEventTask()
{

}

CTimeEventTask::~CTimeEventTask()
{
    OUR_DEBUG((LM_INFO, "[CTimeEventTask::~CTimeEventTask].\n"));
}

int CTimeEventTask::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    //定时生成指定的页面
    CDataManager::GetInstance()->make_index_html();

    CDataManager::GetInstance()->make_detail_html();

    //序列化内存
    CDataManager::GetInstance()->Serialization();

    return 0;
}

