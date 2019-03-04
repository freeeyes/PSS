#include "MessageDyeingManager.h"

CMessageDyeingManager::CMessageDyeingManager()
{
    m_u2CurrCommandCount = 0;
}

CMessageDyeingManager::~CMessageDyeingManager()
{

}

void CMessageDyeingManager::AddDyringIP(const char* pClientIP, uint16 u2MaxCount)
{
    m_objDyeingIP.m_u2MaxCount = u2MaxCount;
    m_objDyeingIP.m_u2CurrCount = 0;
    sprintf_safe(m_objDyeingIP.m_szIP, MAX_BUFF_20, "%s", pClientIP);
    m_objDyeingIP.m_blIsValid = true;
}

bool CMessageDyeingManager::AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount)
{
    if (m_u2CurrCommandCount > MAX_DYEING_COMMAND_COUNT)
    {
        return false;
    }

    for (int i = 0; i < MAX_DYEING_COMMAND_COUNT; i++)
    {
        if (m_objCommandList[i].m_blIsValid == false)
        {
            m_objCommandList[i].m_u2CommandID = u2CommandID;
            m_objCommandList[i].m_u2CurrCount = 0;
            m_objCommandList[i].m_u2MaxCount  = u2MaxCount;
            m_objCommandList[i].m_blIsValid = true;
            m_u2CurrCommandCount++;
            return true;
        }
    }

    return false;
}

void CMessageDyeingManager::GetDyeingCommand(vec_Dyeing_Command_list& objList)
{
    for (int i = 0; i < MAX_DYEING_COMMAND_COUNT; i++)
    {
        if (m_objCommandList[i].m_blIsValid == true)
        {
            objList.push_back(m_objCommandList[i]);
        }
    }
}

string CMessageDyeingManager::GetTraceID(const char* pClientIP, short sClintPort, uint16 u2CommandID)
{
    string strTraceID = "";

    //检验IP规则
    if (true == m_objDyeingIP.m_blIsValid && ACE_OS::strcmp(m_objDyeingIP.m_szIP, pClientIP) == 0)
    {
        m_objDyeingIP.m_u2CurrCount++;

        //判断是否达到最大个数包
        if (m_objDyeingIP.m_u2CurrCount >= m_objDyeingIP.m_u2MaxCount)
        {
            m_objDyeingIP.Clear();
        }

        strTraceID = CREATE_TRACE(pClientIP, sClintPort, u2CommandID);
        return strTraceID;
    }

    //检测命令规则
    if(m_u2CurrCommandCount > 0)
    {
        for (int i = 0; i < MAX_DYEING_COMMAND_COUNT; i++)
        {
            if (true == m_objCommandList[i].m_blIsValid && m_objCommandList[i].m_u2CommandID == u2CommandID)
            {
                m_objCommandList[i].m_u2CurrCount++;

                //判断是否达到最大个数包
                if (m_objCommandList[i].m_u2CurrCount >= m_objCommandList[i].m_u2MaxCount)
                {
                    m_objCommandList[i].Clear();
                    m_u2CurrCommandCount--;
                }

                strTraceID = CREATE_TRACE(pClientIP, sClintPort, u2CommandID);
                return strTraceID;
            }
        }
    }

    return strTraceID;
}

