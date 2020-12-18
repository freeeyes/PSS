#include "WorkThreadAI.h"

void CWorkThreadAI::Init(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTTimeoutCount, uint32 u4WTStopTime, uint8 u1WTReturnDataType, const char* pReturnData)
{
    m_u1WTAI             = u1AI;
    m_u4WTCheckTime      = u4WTCheckTime;
    m_u4WTTimeoutCount   = u4WTTimeoutCount;
    m_u4WTStopTime       = u4WTStopTime;
    m_u1WTReturnDataType = u1WTReturnDataType;
    m_u4DisposeTime      = u4DisposeTime;
    m_strWTReturnData    = pReturnData;
    m_u2ReturnDataLen    = (uint16)m_strWTReturnData.length();
}

bool CWorkThreadAI::SaveTimeout(uint16 u2CommandID, uint32 u4TimeCost)
{
    if(m_u1WTAI == 1)
    {
        //如果在处理时间内，则后面都不处理
        if(m_u4DisposeTime > u4TimeCost)
        {
            return false;
        }

        auto u8Now = CTimeStamp::Get_Time_use_second(CTimeStamp::Get_Time_Stamp());

        //如果开启AI功能
        bool blState = false;
        int nRet = Do_Command_Account(u2CommandID, u8Now, u4TimeCost, blState);

        if (1 == nRet)
        {
            return blState;
        }

        //如果不在监控列表中，添加一个监控列表
        auto pCheckCommandTime = std::make_shared<_CommandTime>();
        pCheckCommandTime->m_u2CommandID = u2CommandID;
        pCheckCommandTime->m_objTime.Init(m_u4WTTimeoutCount);
        shared_ptr<_CommandTimeout> pCommandTimeout = pCheckCommandTime->m_objTime.GetFreeData();

        if(nullptr != pCommandTimeout)
        {
            pCommandTimeout->m_u2CommandID = u2CommandID;
            pCommandTimeout->m_u8Second    = u8Now;
            pCommandTimeout->m_u4Timeout   = u4TimeCost;
        }

        pCheckCommandTime->m_objTime.Add();
        m_vecCommandTime.emplace_back(pCheckCommandTime);

        //如果是只需要1个包就关闭，那么在这里人到屏蔽列表中
        if(m_u4WTTimeoutCount <= 1)
        {
            //需要关闭了
            _CommandTimeout objCommandTimeout;
            objCommandTimeout.m_u2CommandID = u2CommandID;
            objCommandTimeout.m_u8Second = u8Now + m_u4WTStopTime;
            m_vecCommandTimeout.push_back(objCommandTimeout);
        }

        return true;
    }

    return false;
}

bool CWorkThreadAI::CheckCurrTimeout(uint16 u2CommandID, uint64 u8Now)
{
    if (m_u1WTAI != 1)
    {
        return false;
    }

    if(m_vecCommandTimeout.empty())
    {
        return false;
    }

    //如果需要检测才走循环
    for(vector<_CommandTimeout>::iterator b = m_vecCommandTimeout.begin(); b != m_vecCommandTimeout.end(); ++b)
    {
        const _CommandTimeout& objCommandTimeout = (_CommandTimeout& )(*b);

        if (objCommandTimeout.m_u2CommandID != u2CommandID)
        {
            continue;
        }

        if(objCommandTimeout.m_u8Second >= u8Now)
        {
            //在禁止时间内，返回当前命令不应被继续处理
            return true;
        }
        else
        {
            //已经到达禁止时间，从禁止列表中删除
            m_vecCommandTimeout.erase(b);
            return false;
        }
    }

	return false;
}

uint16 CWorkThreadAI::GetReturnDataLength() const
{
    return m_u2ReturnDataLen;
}

void CWorkThreadAI::Close()
{
    for(auto pCommandTime : m_vecCommandTime)
    {
        pCommandTime->m_objTime.Close();
    }

    m_vecCommandTime.clear();
    m_vecCommandTimeout.clear();

}

void CWorkThreadAI::ReSet(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime)
{
    m_u1WTAI             = u1AI;
    m_u4WTCheckTime      = u4WTCheckTime;
    m_u4DisposeTime      = u4DisposeTime;
    m_u4WTStopTime       = u4WTStopTime;
}

void CWorkThreadAI::GetAIInfo(_WorkThreadAIInfo& objWorkThreadAIInfo) const
{
    objWorkThreadAIInfo.m_u1WTAI           = m_u1WTAI;
    objWorkThreadAIInfo.m_u4DisposeTime    = m_u4DisposeTime;
    objWorkThreadAIInfo.m_u4WTCheckTime    = m_u4WTCheckTime;
    objWorkThreadAIInfo.m_u4WTStopTime     = m_u4WTStopTime;
    objWorkThreadAIInfo.m_u4WTTimeoutCount = m_u4WTTimeoutCount;
}

void CWorkThreadAI::GetAllTimeout(uint32 u4ThreadID, vecCommandTimeout& objTimeout) const
{
    for(auto pCommandTime : m_vecCommandTime)
    {
        if (nullptr == pCommandTime)
        {
            continue;
        }

		for (int j = pCommandTime->m_objTime.GetCount() - 1; j >= 0; j--)
		{
			_CommandTimeout objData;

			if (pCommandTime->m_objTime.GetLinkData(j)->m_u8Second > 0)
			{
				objData.m_u4ThreadID = u4ThreadID;
				objData.m_u2CommandID = pCommandTime->m_objTime.GetLinkData(j)->m_u2CommandID;
				objData.m_u8Second = pCommandTime->m_objTime.GetLinkData(j)->m_u8Second;
				objData.m_u4Timeout = pCommandTime->m_objTime.GetLinkData(j)->m_u4Timeout;
				objTimeout.push_back(objData);
			}
		}
    }
}

void CWorkThreadAI::GetAllForbiden(uint32 u4ThreadID, vecCommandTimeout& objForbiden) const
{
    for(const auto& objCommandTimeout : m_vecCommandTimeout)
    {
        _CommandTimeout objData;
        objData.m_u4ThreadID  = u4ThreadID;
        objData.m_u2CommandID = objCommandTimeout.m_u2CommandID;
        objData.m_u8Second    = objCommandTimeout.m_u8Second;
        objForbiden.push_back(objData);
    }
}

int CWorkThreadAI::Do_Command_Account(uint16 u2CommandID, uint64 u8Now, uint32 u4TimeCost, bool& blRet)
{
    for (auto pCommandTime : m_vecCommandTime)
    {
        if (u2CommandID != pCommandTime->m_u2CommandID)
        {
            continue;
        }

        //首先添加一个到环里面
        shared_ptr<_CommandTimeout> pCommandTimeout = pCommandTime->m_objTime.GetFreeData();

        if (nullptr != pCommandTimeout)
        {
            pCommandTimeout->m_u2CommandID = u2CommandID;
            pCommandTimeout->m_u8Second = u8Now;
            pCommandTimeout->m_u4Timeout = u4TimeCost;
        }

        pCommandTime->m_objTime.Add();

        //在判断当前环里面最后一个时间是否在间隔时间内
        const shared_ptr<_CommandTimeout> pCommandLastTimeout = pCommandTime->m_objTime.GetLinkData(m_u4WTTimeoutCount - 1);

        if (nullptr != pCommandLastTimeout)
        {
            if (u8Now - pCommandLastTimeout->m_u8Second <= m_u4WTCheckTime)
            {
                //需要关闭了
                _CommandTimeout objCommandTimeout;
                objCommandTimeout.m_u2CommandID = u2CommandID;
                objCommandTimeout.m_u8Second = u8Now + m_u4WTStopTime;
                m_vecCommandTimeout.push_back(objCommandTimeout);

                blRet = true;
                return 1;
            }
            else
            {
                blRet = false;
                return 1;
            }
        }
        else
        {
            blRet = false;
            return 1;
        }
    }

    return 0;
}
