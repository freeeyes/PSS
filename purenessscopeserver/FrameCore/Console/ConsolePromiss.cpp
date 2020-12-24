#include "ConsolePromiss.h"

void CConsolePromissions::Init(const char* pFileName)
{
    CXmlOpeation m_ConcoleConfig;

    //初始化队列
    Close();

    if (false == m_ConcoleConfig.Init(pFileName))
    {
        PSS_LOGGER_DEBUG("[CConsolePromissions::Init]File Read Error = {0}.", pFileName);
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

int CConsolePromissions::Check_Promission(const string& pCommandName, const string& pUser)
{
    auto f = m_objHashCommandList.find(pCommandName);
    
    if (m_objHashCommandList.end() == f)
    {
        PSS_LOGGER_DEBUG("[CConsolePromissions::Check_Promission]({0})CommandName is nullptr.\n", pCommandName);
        return -1;
    }

    auto pConsole_Command_Info = f->second;

    return Check_Split_User(pUser, pConsole_Command_Info->m_strUser);
}

int CConsolePromissions::Check_Split_User(const string& pUser, const string& pUserList) const
{
    vector<string> strUserList;

    auto vecUserList = split_string(pUserList, ',');

    for (const auto& strFrameUser : vecUserList)
    {
        if (pUser == strFrameUser)
        {
            return 0;
        }
    }

    PSS_LOGGER_DEBUG("[CConsolePromissions::Check_Promission][{0}]user is no promission.", pUser);
    return -2;
}

