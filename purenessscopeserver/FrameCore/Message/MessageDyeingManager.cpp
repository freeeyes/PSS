#include "MessageDyeingManager.h"

void CMessageDyeingManager::AddDyringIP(const char* pClientIP, uint16 u2MaxCount)
{
    m_objDyeingIP.m_u2MaxCount  = u2MaxCount;
    m_objDyeingIP.m_u2CurrCount = 0;
    m_objDyeingIP.m_strIP       = pClientIP;
    m_objDyeingIP.m_blIsValid   = true;
}

bool CMessageDyeingManager::AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount)
{
    if (m_u2CurrCommandCount > MAX_DYEING_COMMAND_COUNT)
    {
        return false;
    }

    for (_Dyeing_Command& dyeing_command : m_objCommandList)
    {
        if (dyeing_command.m_blIsValid == false)
        {
            dyeing_command.m_u2CommandID = u2CommandID;
            dyeing_command.m_u2CurrCount = 0;
            dyeing_command.m_u2MaxCount  = u2MaxCount;
            dyeing_command.m_blIsValid = true;
            m_u2CurrCommandCount++;
            return true;
        }
    }

    return false;
}

void CMessageDyeingManager::GetDyeingCommand(vec_Dyeing_Command_list& objList) const
{
    for (auto dyeing_command : m_objCommandList)
    {
        if (dyeing_command.m_blIsValid == true)
        {
            objList.push_back(dyeing_command);
        }
    }
}

string CMessageDyeingManager::GetTraceID(const char* pClientIP, short sClintPort, uint16 u2CommandID)
{
    string strTraceID = "";

    //检验IP规则
    if (true == m_objDyeingIP.m_blIsValid && m_objDyeingIP.m_strIP == (string)pClientIP)
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
        for (auto dyeing_command : m_objCommandList)
        {
            if (false == dyeing_command.m_blIsValid || dyeing_command.m_u2CommandID != u2CommandID)
            {
                continue;
            }

            dyeing_command.m_u2CurrCount++;

            //判断是否达到最大个数包
            if (dyeing_command.m_u2CurrCount >= dyeing_command.m_u2MaxCount)
            {
                dyeing_command.Clear();
                m_u2CurrCommandCount--;
            }

            strTraceID = CREATE_TRACE(pClientIP, sClintPort, u2CommandID);
            return strTraceID;
        }
    }

    return strTraceID;
}

