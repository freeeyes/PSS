#include "ConsolePromiss.h"

CConsolePromissions::CConsolePromissions()
{

}

CConsolePromissions::~CConsolePromissions()
{
    Close();
}

void CConsolePromissions::Init(const char* pFileName)
{
    CXmlOpeation m_ConcoleConfig;
    char* pData = NULL;

    //初始化队列
    Close();
    m_objHashCommandList.Init(MAX_COMSOLE_COMMAND_COUNT);

    if (false == m_ConcoleConfig.Init(pFileName))
    {
        OUR_DEBUG((LM_INFO, "[CConsolePromissions::Init]File Read Error = %s.\n", pFileName));
        return;
    }

    TiXmlElement* pNextTiXmlCommandName = NULL;
    TiXmlElement* pNextTiXmlUser        = NULL;

    while (true)
    {
        _Console_Command_Info* pConsole_Command_Info = new ;
        pData = m_ConcoleConfig.GetData("CommandInfo", "CommandName", pNextTiXmlCommandName);

        if (pData != NULL)
        {
            sprintf_safe(pConsole_Command_Info->m_szCommand, MAX_BUFF_50, "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_ConcoleConfig.GetData("CommandInfo", "User", pNextTiXmlUser);

        if (pData != NULL)
        {
            sprintf_safe(pConsole_Command_Info->m_szUser, MAX_BUFF_200, "%s", pData);
        }
        else
        {
            break;
        }

        if (-1 == m_objHashCommandList.Add_Hash_Data(pConsole_Command_Info->m_szCommand, pConsole_Command_Info))
        {
            OUR_DEBUG((LM_INFO, "[CConsolePromissions::Init](%s)Add error.\n", pConsole_Command_Info->m_szCommand));
        }
    }
}

void CConsolePromissions::Close()
{
    m_objHashCommandList.Close();
}

bool CConsolePromissions::Check_Promission(const char* pCommandName, const char* pUser)
{
    _Console_Command_Info* pConsole_Command_Info = m_objHashCommandList.Get_Hash_Box_Data(pCommandName);

    if (NULL == pConsole_Command_Info)
    {
        OUR_DEBUG((LM_INFO, "[CConsolePromissions::Check_Promission](%s)CommandName is null.\n", pCommandName));
        return false;
    }

    return Check_Split_User(pUser, pConsole_Command_Info->m_szUser);
}

bool CConsolePromissions::Check_Split_User(const char* pUser, const char* pUserList)
{
    return true;
}

