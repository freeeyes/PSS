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
    //��ʱ����ָ����ҳ��
    CDataManager::GetInstance()->make_index_html();

    CDataManager::GetInstance()->make_detail_html();

    //���л��ڴ�
    CDataManager::GetInstance()->Serialization();

    return 0;
}

