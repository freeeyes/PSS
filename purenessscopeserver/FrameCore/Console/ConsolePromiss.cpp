#include "ConsolePromiss.h"

CConsolePromissions::CConsolePromissions()
{

}

void CConsolePromissions::Init(const char* pFileName)
{
    CXmlOpeation m_ConcoleConfig;

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
        const char* pCommandNameData = NULL;
        const char* pUserData        = NULL;

        auto pConsole_Command_Info = new _Console_Command_Info();
        pCommandNameData = m_ConcoleConfig.GetData("CommandInfo", "CommandName", pNextTiXmlCommandName);
        pUserData = m_ConcoleConfig.GetData("CommandInfo", "User", pNextTiXmlUser);

        if (NULL == pCommandNameData || NULL == pUserData)
        {
            SAFE_DELETE(pConsole_Command_Info);
            break;
        }

        pConsole_Command_Info->m_strCommand = pCommandNameData;
        pConsole_Command_Info->m_strUser    = pUserData;

        if (-1 == m_objHashCommandList.Add_Hash_Data(pConsole_Command_Info->m_strCommand.c_str(), pConsole_Command_Info))
        {
            SAFE_DELETE(pConsole_Command_Info);
            OUR_DEBUG((LM_INFO, "[CConsolePromissions::Init](%s)Add error.\n", pConsole_Command_Info->m_strCommand.c_str()));
        }
    }
}

void CConsolePromissions::Close()
{
    //清理配置列表
    vector<_Console_Command_Info*> vecConsoleCommandList;
    m_objHashCommandList.Get_All_Used(vecConsoleCommandList);

    auto u4Size = (uint32)vecConsoleCommandList.size();

    for (uint32 i = 0; i < u4Size; i++)
    {
        SAFE_DELETE(vecConsoleCommandList[i]);
    }

    m_objHashCommandList.Close();
}

int CConsolePromissions::Check_Promission(const char* pCommandName, const char* pUser)
{
    auto pConsole_Command_Info = m_objHashCommandList.Get_Hash_Box_Data(pCommandName);

    if (NULL == pConsole_Command_Info)
    {
        OUR_DEBUG((LM_INFO, "[CConsolePromissions::Check_Promission](%s)CommandName is null.\n", pCommandName));
        return -1;
    }

    return Check_Split_User(pUser, pConsole_Command_Info->m_strUser.c_str());
}

int CConsolePromissions::Check_Split_User(const char* pUser, const char* pUserList) const
{
    string strTempUser;
    const char* pPromissPosBegin = pUserList;
    const char* pPromissPos = ACE_OS::strstr(pPromissPosBegin, ",");

    while (NULL != pPromissPos)
    {
        uint32 u4NameLen = (uint32)(pPromissPos - pPromissPosBegin);

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
            u4NameLen = (uint32)strlen(pUserList) - (uint32)(pPromissPosBegin - pUserList - 1);;
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

