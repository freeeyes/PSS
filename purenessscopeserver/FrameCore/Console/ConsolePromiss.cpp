#include "ConsolePromiss.h"

void CConsolePromissions::Init(const char* pFileName)
{
    CXmlOpeation m_ConcoleConfig;

    //初始化队列
    Close();

    if (false == m_ConcoleConfig.Init(pFileName))
    {
        OUR_DEBUG((LM_INFO, "[CConsolePromissions::Init]File Read Error = %s.\n", pFileName));
        return;
    }

    TiXmlElement* pNextTiXmlCommandName = nullptr;
    TiXmlElement* pNextTiXmlUser        = nullptr;

    while (true)
    {
        const char* pCommandNameData = nullptr;
        const char* pUserData        = nullptr;

        auto pConsole_Command_Info = std::make_shared<_Console_Command_Info>();
        pCommandNameData = m_ConcoleConfig.GetData("CommandInfo", "CommandName", pNextTiXmlCommandName);
        pUserData = m_ConcoleConfig.GetData("CommandInfo", "User", pNextTiXmlUser);

        if (nullptr == pCommandNameData || nullptr == pUserData)
        {
            break;
        }

        pConsole_Command_Info->m_strCommand = pCommandNameData;
        pConsole_Command_Info->m_strUser    = pUserData;

        m_objHashCommandList[pConsole_Command_Info->m_strCommand] = pConsole_Command_Info;
    }
}

void CConsolePromissions::Close()
{
    //清理配置列表
    vector<_Console_Command_Info*> vecConsoleCommandList;

    m_objHashCommandList.clear();
}

int CConsolePromissions::Check_Promission(const char* pCommandName, const char* pUser)
{
    auto f = m_objHashCommandList.find(pCommandName);
    
    if (m_objHashCommandList.end() == f)
    {
        OUR_DEBUG((LM_INFO, "[CConsolePromissions::Check_Promission](%s)CommandName is nullptr.\n", pCommandName));
        return -1;
    }

    auto pConsole_Command_Info = f->second;

    return Check_Split_User(pUser, pConsole_Command_Info->m_strUser.c_str());
}

int CConsolePromissions::Check_Split_User(const char* pUser, const char* pUserList) const
{
    string strTempUser;
    const char* pPromissPosBegin = pUserList;
    const char* pPromissPos = ACE_OS::strstr(pPromissPosBegin, ",");

    while (nullptr != pPromissPos)
    {
        auto u4NameLen = (uint32)(pPromissPos - pPromissPosBegin);

        if (u4NameLen > MAX_BUFF_50)
        {
            pPromissPosBegin = pPromissPos + 1;
            pPromissPos = ACE_OS::strstr(pUserList, ",");
            continue;
        }

        strTempUser.append(pPromissPosBegin, u4NameLen);

        if (strTempUser == pUser)
        {
            return 0;
        }

        pPromissPosBegin = pPromissPos + 1;
        pPromissPos = ACE_OS::strstr(pPromissPosBegin, ",");
        strTempUser = "";
    }

    //判断最后一个User
    if ((uint32)(pPromissPosBegin - pUserList) < (uint32)ACE_OS::strlen(pUserList))
    {
        uint32 u4NameLen = 0;

        if (pUserList == pPromissPosBegin)
        {
            //只有一个
            u4NameLen = (uint32)ACE_OS::strlen(pUserList);
            strTempUser.append(pPromissPosBegin, u4NameLen);
        }
        else
        {
            //最后一个
            u4NameLen = (uint32)strlen(pUserList) - (uint32)(pPromissPosBegin - pUserList - 1);
            strTempUser.append(pPromissPosBegin, u4NameLen);
        }

        if (strTempUser == pUser)
        {
            return 0;
        }
    }

    OUR_DEBUG((LM_INFO, "[CConsolePromissions::Check_Promission][%s]user is no promission.\n", pUser));
    return -2;
}

