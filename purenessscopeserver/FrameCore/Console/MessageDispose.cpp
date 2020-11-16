#include "MessageDispose.h"

void Combo_Common_Return_Data(uint8 u1OutputType, uint8 u1State, const char* pMessage, IBuffPacket* pBuffPacket)
{
    if (u1OutputType == 0)
    {
        (*pBuffPacket) << u1State;
    }
    else
    {
        char szTemp[MAX_BUFF_200] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_200, pMessage);
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }
}

void Combo_Common_Head_Data(uint8 u1OutputType, uint32 u4Count, const char* pMessage, IBuffPacket* pBuffPacket)
{
    if (u1OutputType == 0)
    {
        (*pBuffPacket) << u4Count;
    }
    else
    {
        char szTemp[MAX_BUFF_200] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_200, pMessage);
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }
}

void Combo_Common_vecClientConnectInfo(uint8 u1OutputType, const vecClientConnectInfo VecClientConnectInfo, IBuffPacket* pBuffPacket)
{
    char szIP[MAX_BUFF_100] = { '\0' };

    for (_ClientConnectInfo ClientConnectInfo : VecClientConnectInfo)
    {
        if (true == ClientConnectInfo.m_blValid)
        {
            sprintf_safe(szIP, MAX_BUFF_100, "0.0.0.0:0");

            if (u1OutputType == 0)
            {
                VCHARS_STR strSName;
                strSName.text = szIP;
                strSName.u1Len = (uint8)ACE_OS::strlen(szIP);

                (*pBuffPacket) << strSName;
                (*pBuffPacket) << ClientConnectInfo.m_u4ConnectID;
                (*pBuffPacket) << ClientConnectInfo.m_u4RecvCount;
                (*pBuffPacket) << ClientConnectInfo.m_u4SendCount;
                (*pBuffPacket) << ClientConnectInfo.m_u4AllRecvSize;
                (*pBuffPacket) << ClientConnectInfo.m_u4AllSendSize;
                (*pBuffPacket) << ClientConnectInfo.m_u4BeginTime;
                (*pBuffPacket) << ClientConnectInfo.m_u4AliveTime;
                (*pBuffPacket) << ClientConnectInfo.m_u4RecvQueueCount;
                (*pBuffPacket) << ClientConnectInfo.m_u8RecvQueueTimeCost;
                (*pBuffPacket) << ClientConnectInfo.m_u8SendQueueTimeCost;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP IP(%s)\n", szIP);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP Command(%d)\n", ClientConnectInfo.m_u4ConnectID);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP RecvCount(%d)\n", ClientConnectInfo.m_u4RecvCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP SendCount(%d)\n", ClientConnectInfo.m_u4SendCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP AllRecvSize(%d)\n", ClientConnectInfo.m_u4AllRecvSize);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP AllSendSize(%d)\n", ClientConnectInfo.m_u4AllSendSize);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP BeginTime(%d)\n", ClientConnectInfo.m_u4BeginTime);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP AliveTime(%d)\n", ClientConnectInfo.m_u4AliveTime);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "UDP RecvQueueCount(%d)\n", ClientConnectInfo.m_u4RecvQueueCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }
}

void Combo_Common_VecForbiddenIP(uint8 u1OutputType, const VecForbiddenIP* pIPList, IBuffPacket* pBuffPacket)
{
    for(_ForbiddenIP forbidenip : *pIPList)
    {
        if (u1OutputType == 0)
        {
            VCHARS_STR strSName;
            strSName.text = (char* )forbidenip.m_strClientIP.c_str();
            strSName.u1Len = (uint8)forbidenip.m_strClientIP.length();

            (*pBuffPacket) << strSName;
            (*pBuffPacket) << forbidenip.m_u1Type;
            (*pBuffPacket) << (uint32)forbidenip.m_tvBegin.sec();
            (*pBuffPacket) << forbidenip.m_u4Second;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden(%s)\n", forbidenip.m_strClientIP.c_str());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden Type(%d)\n", forbidenip.m_u1Type);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden BeginTime(%d)\n", forbidenip.m_tvBegin.sec());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden IntervalTime(%d)\n", forbidenip.m_u4Second);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }
}

void DoMessage_LoadModule(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _FileInfo FileInfo;

    if (false == GetFileInfo(CommandInfo.m_strCommandExp.c_str(), FileInfo) && CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint8)1;
        return;
    }

    //加载文件MessageManager
    //消息所有的工作线程同步更新一下各自的消息列表副本
    if (true == App_ModuleLoader::instance()->LoadModule(FileInfo.m_strFilePath.c_str(), FileInfo.m_strFileName.c_str(), FileInfo.m_strFileParam.c_str()) &&
        true == App_MessageServiceGroup::instance()->PutUpdateCommandMessage(App_MessageManager::instance()->GetUpdateIndex()))
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "LoadModule(" << FileInfo.m_strFileName << ") is fail.\n";
            string strOutput = ss_format.str();
            pBuffPacket->WriteStream(strOutput.c_str(), (uint32)strOutput.length());
        }
    }
    else
    {
        if (NULL != pBuffPacket)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint8)1;
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "LoadModule(" << FileInfo.m_strFileName << ") is OK.\n";
                string strOutput = ss_format.str();
                pBuffPacket->WriteStream(strOutput.c_str(), (uint32)strOutput.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_LOADMOUDLE;
}

void DoMessage_UnLoadModule(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (true == App_MessageManager::instance()->UnloadModuleCommand(CommandInfo.m_strCommandExp.c_str(), (uint8)1, App_MessageServiceGroup::instance()->GetWorkThreadCount()) &&
        true == App_MessageServiceGroup::instance()->PutUpdateCommandMessage(App_MessageManager::instance()->GetUpdateIndex()))
    {
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "UnloadModule is ok.\n", pBuffPacket);
    }
    else
    {
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 1, "UnloadModule is fail.\n", pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_UNLOADMOUDLE;
}

void DoMessage_ReLoadModule(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (true == App_MessageManager::instance()->UnloadModuleCommand(CommandInfo.m_strCommandExp.c_str(), (uint8)2, App_MessageServiceGroup::instance()->GetWorkThreadCount()) &&
        true == App_MessageServiceGroup::instance()->PutUpdateCommandMessage(App_MessageManager::instance()->GetUpdateIndex()))
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            char szTemp[MAX_BUFF_200] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_200, "ReloadModule(%s) is ok.\n", CommandInfo.m_strCommandExp.c_str());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }
    else
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)1;
        }
        else
        {
            char szTemp[MAX_BUFF_200] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_200, "ReloadModule(%s) is false.\n", CommandInfo.m_strCommandExp.c_str());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_RELOADMOUDLE;
}

void DoMessage_ClientMessageCount(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-c") == 0)
    {
        //-c 只返回当前激活的链接数
        int nActiveClient = App_HandlerManager::instance()->GetCount();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nActiveClient;
        }
        else
        {
            char szTemp[MAX_BUFF_200] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_200, "ActiveClient(%d).\n", nActiveClient);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }
    else if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-cp") == 0)
    {
        //-cp 返回当前激活连接数和池中剩余可分配数

        int nActiveClient = App_HandlerManager::instance()->GetCount();
#if PSS_PLATFORM == PLATFORM_WIN
        int nPoolClient = App_ProConnectHandlerPool::instance()->GetFreeCount();
#else
        int nPoolClient = App_ConnectHandlerPool::instance()->GetFreeCount();
#endif
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nActiveClient;
            (*pBuffPacket) << (uint32)nPoolClient;
            (*pBuffPacket) << (uint16)GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "ActiveClient(%d).\nPoolClient(%d).\nMaxHandlerCount(%d).\n",
                         nActiveClient,
                         nPoolClient,
                         GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTCOUNT;
}

void DoMessage_ShowModule(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        vector<_ModuleInfo*> vecModeInfo;
        App_ModuleLoader::instance()->GetAllModuleInfo(vecModeInfo);

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)App_ModuleLoader::instance()->GetCurrModuleCount();
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleCount(%d)\n", App_ModuleLoader::instance()->GetCurrModuleCount());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (const _ModuleInfo* pModuleInfo : vecModeInfo)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                uint8 u1ModuleNameLen = (uint8)ACE_OS::strlen(pModuleInfo->GetName());
                (*pBuffPacket) << u1ModuleNameLen;
                pBuffPacket->WriteStream(pModuleInfo->GetName(), u1ModuleNameLen);
                uint8 u1SModileFileLen = (uint8)ACE_OS::strlen(pModuleInfo->strModuleName.c_str());
                (*pBuffPacket) << u1SModileFileLen;
                pBuffPacket->WriteStream(pModuleInfo->strModuleName.c_str(), u1SModileFileLen);
                uint8 u1SModilePathLen = (uint8)ACE_OS::strlen(pModuleInfo->strModulePath.c_str());
                (*pBuffPacket) << u1SModilePathLen;
                pBuffPacket->WriteStream(pModuleInfo->strModulePath.c_str(), u1SModilePathLen);
                uint8 u1SModileParamLen = (uint8)ACE_OS::strlen(pModuleInfo->strModuleParam.c_str());
                (*pBuffPacket) << u1SModileParamLen;
                pBuffPacket->WriteStream(pModuleInfo->strModuleParam.c_str(), u1SModileParamLen);
                uint8 u1SModileDescLen = (uint8)ACE_OS::strlen(pModuleInfo->GetDesc());
                (*pBuffPacket) << u1SModileDescLen;
                pBuffPacket->WriteStream(pModuleInfo->GetDesc(), u1SModileDescLen);

                VCHARS_STR strSName;
                char szTime[MAX_BUFF_100] = { '\0' };
                sprintf_safe(szTime, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", pModuleInfo->dtCreateTime.year(), pModuleInfo->dtCreateTime.month(), pModuleInfo->dtCreateTime.day(), pModuleInfo->dtCreateTime.hour(), pModuleInfo->dtCreateTime.minute(), pModuleInfo->dtCreateTime.second());
                strSName.text = szTime;
                strSName.u1Len = (uint8)ACE_OS::strlen(szTime);
                (*pBuffPacket) << strSName;

                //写入Module当前状态
                uint32 u4ErrorID = 0;
                uint8  u1MouduleState = 0;

                pModuleInfo->GetModuleState(u4ErrorID);
                u1MouduleState = (uint8)u4ErrorID;

                (*pBuffPacket) << u1MouduleState;
                (*pBuffPacket) << u4ErrorID;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleName(%s)\n", pModuleInfo->GetName());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleFileName(%s)\n", pModuleInfo->strModuleName.c_str());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleFilePath(%s)\n", pModuleInfo->strModulePath.c_str());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleParam(%s)\n", pModuleInfo->strModuleParam.c_str());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleDesc(%s)\n", pModuleInfo->GetDesc());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "CreeateTime(%04d-%02d-%02d %02d:%02d:%02d)\n", pModuleInfo->dtCreateTime.year(), pModuleInfo->dtCreateTime.month(), pModuleInfo->dtCreateTime.day(), pModuleInfo->dtCreateTime.hour(), pModuleInfo->dtCreateTime.minute(), pModuleInfo->dtCreateTime.second());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));

                uint32 u4ErrorID = 0;

                pModuleInfo->GetModuleState(u4ErrorID);
                sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleState Run u4ErrorID=%d.\n", u4ErrorID);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }

        u2ReturnCommandID = CONSOLE_COMMAND_SHOWMOUDLE;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[DoMessage_ShowModule]Param is error.\n"));
    }
}

void DoMessage_CommandInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint16 u2CommandID = (uint16)ACE_OS::strtol(CommandInfo.m_strCommandExp.c_str(), NULL, 16);

    if (u2CommandID != 0)
    {
        _CommandData objCommandData;
        _CommandData objCommandDataIn;
        _CommandData objCommandDataOut;

        if (objCommandDataIn.m_u2CommandID == u2CommandID)
        {
            objCommandData += objCommandDataIn;
        }

        if (objCommandData.m_u2CommandID == u2CommandID)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint32)1;
                (*pBuffPacket) << (uint16)1;
                (*pBuffPacket) << u2CommandID;
                (*pBuffPacket) << objCommandData.m_u4CommandCount;
                (*pBuffPacket) << (uint32)objCommandData.m_u8CommandCost;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "CommandID is Find.\n");
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "CommandID(0x%08x)\n", u2CommandID);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "CommandCount(%d)\n", objCommandData.m_u4CommandCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "CommandCost(%d)\n", objCommandData.m_u8CommandCost);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
        else
        {
            //没有找到
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint32)0;
                (*pBuffPacket) << (uint16)0;
                (*pBuffPacket) << 0;
                (*pBuffPacket) << 0;
                (*pBuffPacket) << (uint32)0;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "CommandID is no Find.\n");
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }

        u2ReturnCommandID = CONSOLE_COMMAND_COMMANDINFO;
    }
}

void DoMessage_WorkThreadState(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-s") != 0)
    {
        return;
    }

    //获得当前工作线程状态
    CThreadInfoList* pThreadInfo = App_MessageServiceGroup::instance()->GetThreadInfo();

    if (NULL ==  pThreadInfo)
    {
        return;
    }

    int nThreadCount = pThreadInfo->GetThreadCount();
    (*pBuffPacket) << (uint8)nThreadCount;

    for (int i = 0; i < nThreadCount; i++)
    {
        const _ThreadInfo* pCurrThreadInfo = pThreadInfo->GetThreadInfo(i);

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)i;
            (*pBuffPacket) << (uint32)pCurrThreadInfo->m_tvUpdateTime.sec();
            (*pBuffPacket) << (uint32)pCurrThreadInfo->m_tvCreateTime.sec();
            (*pBuffPacket) << (uint8)pCurrThreadInfo->m_u4State;
            (*pBuffPacket) << pCurrThreadInfo->m_u4RecvPacketCount;
            (*pBuffPacket) << pCurrThreadInfo->m_u2CommandID;
            (*pBuffPacket) << pCurrThreadInfo->m_u2PacketTime;
            (*pBuffPacket) << pCurrThreadInfo->m_u4CurrPacketCount;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "ThreadID(%d)\n", i);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "ThreadUpdateTime(%d)\n", pCurrThreadInfo->m_tvUpdateTime.sec());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "ThreadCreateTime(%d)\n", pCurrThreadInfo->m_tvCreateTime.sec());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "ThreadState(%d)\n", pCurrThreadInfo->m_u4State);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "ThreadRecvPacketCount(%d)\n", pCurrThreadInfo->m_u4RecvPacketCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "ThreadCommandID(%d)\n", pCurrThreadInfo->m_u2CommandID);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "ThreadPacketTime(%d)\n", pCurrThreadInfo->m_u2PacketTime);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "ThreadCurrPacketCount(%d)\n", pCurrThreadInfo->m_u4CurrPacketCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_THREADINFO;
}

void DoMessage_ClientInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        vecClientConnectInfo VecClientConnectInfo;

        //待实现
        //App_HandlerManager::instance()->GetConnectInfo(VecClientConnectInfo);

        uint32 u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "Client IP Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTINFO;
}

void DoMessage_CloseClient(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint32 u4ConnectID = (uint32)ACE_OS::atoi(CommandInfo.m_strCommandExp.c_str());

    App_HandlerManager::instance()->CloseConnect(u4ConnectID);

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint8)0;
    }
    else
    {
        char szTemp[MAX_BUFF_1024] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_1024, "Client Close is OK\n");
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }

    u2ReturnCommandID = CONSOLE_COMMAND_COLSECLIENT;
}

void DoMessage_ForbiddenIP(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _ForbiddenIP ForbiddenIP;

    if (GetForbiddenIP(CommandInfo.m_strCommandExp.c_str(), ForbiddenIP) == true)
    {
        if (ForbiddenIP.m_u1Type == 0)
        {
            //永久封禁IP
            App_ForbiddenIP::instance()->AddForeverIP(ForbiddenIP.m_strClientIP.c_str());
        }
        else
        {
            //封禁时段IP
            App_ForbiddenIP::instance()->AddTempIP(ForbiddenIP.m_strClientIP.c_str(), ForbiddenIP.m_u4Second);
        }

        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "IP Forbidden is OK.\n", pBuffPacket);
    }
    else
    {
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 1, "IP Forbidden is fail.\n", pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_FORBIDDENIP;
}

void DoMessage_ShowForbiddenList(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        const VecForbiddenIP* pForeverForbiddenIP = App_ForbiddenIP::instance()->ShowForeverIP();
        const VecForbiddenIP* pTempForbiddenIP = App_ForbiddenIP::instance()->ShowTempIP();

        if (pForeverForbiddenIP == NULL || pTempForbiddenIP == NULL)
        {
            return;
        }

        uint32 u4Count = (uint32)pForeverForbiddenIP->size() + (uint32)pTempForbiddenIP->size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4Count, "IP Forbidden Count(%d).\n", pBuffPacket);

        Combo_Common_VecForbiddenIP(CommandInfo.m_u1OutputType, pForeverForbiddenIP, pBuffPacket);

        Combo_Common_VecForbiddenIP(CommandInfo.m_u1OutputType, pTempForbiddenIP, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_FORBIDDENIPSHOW;
}

void DoMessage_LifedIP(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    App_ForbiddenIP::instance()->DelForeverIP(CommandInfo.m_strCommandExp.c_str());
    App_ForbiddenIP::instance()->DelTempIP(CommandInfo.m_strCommandExp.c_str());

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint8)0;
    }
    else
    {
        char szTemp[MAX_BUFF_1024] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden cancel OK\n");
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }

    u2ReturnCommandID = CONSOLE_COMMAND_LIFTED;
}

void DoMessage_UDPClientInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        vecClientConnectInfo VecClientConnectInfo;
        App_UDPConnectIDManager::instance()->GetClientConnectInfo(VecClientConnectInfo);

        uint32 u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "UDPClient Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_UDPCONNECTINFO;
}

void DoMessage_ServerConnectTCP(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        vecClientConnectInfo VecClientConnectInfo;
#if PSS_PLATFORM == PLATFORM_WIN
        App_ClientProConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
#else
        App_ClientReConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
#endif

        uint32 u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "ServerConnect Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERCONNECT_TCP;
}

void DoMessage_ServerConnectUDP(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        vecClientConnectInfo VecClientConnectInfo;
#if PSS_PLATFORM == PLATFORM_WIN
        App_ClientProConnectManager::instance()->GetUDPConnectInfo(VecClientConnectInfo);
#else
        App_ClientReConnectManager::instance()->GetUDPConnectInfo(VecClientConnectInfo);
#endif

        uint32 u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "ServerConnect Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERCONNECT_UDP;
}

void DoMessage_ShowProcessInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        int nCPU = 0;
        int nMemorySize = 0;
        uint8 u1Flow = GetXmlConfigAttribute(xmlMessage)->Packet_Counter;      //流量统计标记位
        uint32 u4FlowIn = 0;      //总流量流入字节
        uint32 u4FlowOut = 0;      //总流量流出字节

        vector<CWorkThread_Packet_Info> vec_Port_Data_Account;
        App_MessageServiceGroup::instance()->GetFlowPortList(vec_Port_Data_Account);

        for (const CWorkThread_Packet_Info& objCWorkThread_Packet_Info : vec_Port_Data_Account)
        {
            u4FlowIn += objCWorkThread_Packet_Info.m_u4RecvSize;
            u4FlowOut += objCWorkThread_Packet_Info.m_u4SendSize;
        }

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nCPU;
            (*pBuffPacket) << (uint32)nMemorySize;
            (*pBuffPacket) << u1Flow;
            (*pBuffPacket) << u4FlowIn;
            (*pBuffPacket) << u4FlowOut;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "CPUUsedRote(%d%%)\n", nCPU);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "MemorySize(%d Byte)\n", nMemorySize);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "FLowSize(%d)\n", u1Flow);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "FlowIn(%d)\n", u4FlowIn);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "FlowOut(%d)\n", u4FlowOut);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_PROCESSINFO;
}

void DoMessage_ShowClientHisTory(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        char szTime[MAX_BUFF_100] = { '\0' };

        vecIPAccount VecIPAccount;
        App_IPAccount::instance()->GetInfo(VecIPAccount);

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)VecIPAccount.size();
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "ClientHisToryCount(%d)\n", VecIPAccount.size());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (_IPAccount ipaccount : VecIPAccount)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {

                uint8 u1IPLen = (uint8)ipaccount.m_strIP.length();
                (*pBuffPacket) << u1IPLen;
                pBuffPacket->WriteStream(ipaccount.m_strIP.c_str(), u1IPLen);
                (*pBuffPacket) << (uint32)ipaccount.m_nCount;
                (*pBuffPacket) << (uint32)ipaccount.m_nAllCount;

                VCHARS_STR strSName;
                sprintf_safe(szTime, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", 
                    ipaccount.m_dtLastTime.year(), 
                    ipaccount.m_dtLastTime.month(), 
                    ipaccount.m_dtLastTime.day(), 
                    ipaccount.m_dtLastTime.hour(), 
                    ipaccount.m_dtLastTime.minute(), 
                    ipaccount.m_dtLastTime.second());
                strSName.text = szTime;
                strSName.u1Len = (uint8)ACE_OS::strlen(szTime);

                (*pBuffPacket) << strSName;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "IP(%s)\n", ipaccount.m_strIP.c_str());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "Count(%d)\n", ipaccount.m_nCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "AllCount(%d)\n", ipaccount.m_nAllCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "AllCount(%d)\n", ipaccount.m_nAllCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTime, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", 
                    ipaccount.m_dtLastTime.year(), 
                    ipaccount.m_dtLastTime.month(), 
                    ipaccount.m_dtLastTime.day(), 
                    ipaccount.m_dtLastTime.hour(), 
                    ipaccount.m_dtLastTime.minute(), 
                    ipaccount.m_dtLastTime.second());
                sprintf_safe(szTemp, MAX_BUFF_1024, "DateTime(%s)\n", ipaccount.m_nAllCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTHISTORY;
}

void DoMessage_ShowAllCommandInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") != 0)
    {
        return;
    }

    CHashTable<_ModuleClient>* pHashModuleClient = App_MessageManager::instance()->GetModuleClient();

    //统计个数
    uint32 u4Count = 0;
    vector<_ModuleClient*> vecModuleClient;
    pHashModuleClient->Get_All_Used(vecModuleClient);
    u4Count = (uint32)vecModuleClient.size();

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << u4Count;
    }
    else
    {
        char szTemp[MAX_BUFF_1024] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_1024, "CommandInfoCount(%d)\n", u4Count);
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }

    for (const _ModuleClient* pModuleClient : vecModuleClient)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)pModuleClient->m_vecClientCommandInfo.size();
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleClientCount(%d)\n", pModuleClient->m_vecClientCommandInfo.size());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (_ClientCommandInfo* pClientCommandInfo : pModuleClient->m_vecClientCommandInfo)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                VCHARS_STR strSName;
                strSName.text = pClientCommandInfo->m_szModuleName;
                strSName.u1Len = (uint8)ACE_OS::strlen(pClientCommandInfo->m_szModuleName);
                (*pBuffPacket) << strSName;
                (*pBuffPacket) << pClientCommandInfo->m_u2CommandID;
                (*pBuffPacket) << pClientCommandInfo->m_u4Count;
                (*pBuffPacket) << pClientCommandInfo->m_u4TimeCost;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "ModuleName(%s)\n", pClientCommandInfo->m_szModuleName);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "CommandID(%d)\n", pClientCommandInfo->m_u2CommandID);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "Count(%d)\n", pClientCommandInfo->m_u4Count);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "TimeCost(%d)\n", pClientCommandInfo->m_u4TimeCost);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_ALLCOMMANDINFO;
}

void DoMessage_ShowServerInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            //返回服务器ID
            uint16 u2SerevrID = (uint16)GetXmlConfigAttribute(xmlServerID)->id;
            (*pBuffPacket) << u2SerevrID;

            //返回服务器名称
            uint8 u1ServerNameLen = (uint8)GetXmlConfigAttribute(xmlServerName)->name.length();
            (*pBuffPacket) << u1ServerNameLen;
            pBuffPacket->WriteStream(GetXmlConfigAttribute(xmlServerName)->name.c_str(), u1ServerNameLen);

            //返回服务器版本
            uint8 u1ServerVersionLen = (uint8)GetXmlConfigAttribute(xmlServerVersion)->Version.length();
            (*pBuffPacket) << u1ServerVersionLen;
            pBuffPacket->WriteStream(GetXmlConfigAttribute(xmlServerVersion)->Version.c_str(), u1ServerVersionLen);


            //返回加载模块个数
            (*pBuffPacket) << (uint16)App_ModuleLoader::instance()->GetCurrModuleCount();


            //返回工作线程个数
            (*pBuffPacket) << (uint16)App_MessageServiceGroup::instance()->GetThreadInfo()->GetThreadCount();

            //返回当前协议包的版本号
            uint8 u1PacketVersionLen = (uint8)GetXmlConfigAttribute(xmlServerVersion)->Version.length();
            (*pBuffPacket) << u1PacketVersionLen;
            pBuffPacket->WriteStream(GetXmlConfigAttribute(xmlServerVersion)->Version.c_str(), u1PacketVersionLen);

            //返回当前服务器是大端还是小端
            if (GetXmlConfigAttribute(xmlNetWorkMode)->LocalByteOrder == ENUM_CHAR_ORDER::SYSTEM_LITTLE_ORDER)
            {
                (*pBuffPacket) << (uint8)0;     //小端
            }
            else
            {
                (*pBuffPacket) << (uint8)1;     //大端
            }

            //返回当前网络包字序规则
            if (GetXmlConfigAttribute(xmlNetWorkMode)->NetByteOrder == false)
            {
                (*pBuffPacket) << (uint8)0;   //主机字序
            }
            else
            {
                (*pBuffPacket) << (uint8)1;   //网络字序
            }
        }
        else
        {
            //文本输出
            char szCharOrder[MAX_BUFF_20] = { '\0' };

            if (GetXmlConfigAttribute(xmlNetWorkMode)->NetByteOrder == false)
            {
                sprintf_safe(szCharOrder, MAX_BUFF_20, "HostOrder");   //主机字序
            }
            else
            {
                sprintf_safe(szCharOrder, MAX_BUFF_20, "NetOrder");    //网络字序
            }


            char szConsoleOutput[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szConsoleOutput, MAX_BUFF_1024, "ServerID:%d\nServerName:%s\nServerVersion:%s\nServerModuleCount=%d\nWorkthreadCount:%d\nServerProtocol:%s\nCharOrder:%s\n",
                         GetXmlConfigAttribute(xmlServerID)->id,
                         GetXmlConfigAttribute(xmlServerName)->name.c_str(),
                         GetXmlConfigAttribute(xmlServerVersion)->Version.c_str(),
                         App_ModuleLoader::instance()->GetCurrModuleCount(),
                         App_MessageServiceGroup::instance()->GetThreadInfo()->GetThreadCount(),
                         GetXmlConfigAttribute(xmlServerVersion)->Version.c_str(),
                         szCharOrder);

            pBuffPacket->WriteStream(szConsoleOutput, (uint32)ACE_OS::strlen(szConsoleOutput));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERINFO;
}

void DoMessage_ReConnectServer(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    int nSerevrID = 0;

    if (GetConnectServerID(CommandInfo.m_strCommandExp.c_str(), nSerevrID) == true)
    {
        //获得当前连接状态
        vecClientConnectInfo VecClientConnectInfo;
#if PSS_PLATFORM == PLATFORM_WIN  //如果是windows
        App_ClientProConnectManager::instance()->ReConnect(nSerevrID);
        App_ClientProConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
#else
        App_ClientReConnectManager::instance()->ReConnect(nSerevrID);
        App_ClientReConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
#endif

        uint32 u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "ConnectServerCount(%d)\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERRECONNECT;
}

void DoMessage_CommandTimeout(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        uint32 u4Count = 0;

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u4Count;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "CommandTimeoutCount(%d)\n", u4Count);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_COMMANDTIMEOUT;
}

void DoMessage_CommandTimeoutclr(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "State(OK)\n");
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_COMMANDTIMEOUTCLR;
}

void DoMessage_CommandDataLog(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        //不实现

        if (CommandInfo.m_u1OutputType == 0)
        {
            //存储所有发送统计日志
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "State(OK)\n");
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_COMMANDDATALOG;
}

void DoMessage_SetDebug(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint8 u1Debug = 0;

    if (GetDebug(CommandInfo.m_strCommandExp.c_str(), u1Debug) == true)
    {
        if (u1Debug == DEBUG_OFF)
        {
            GetXmlConfigAttribute(xmlAceDebug)->TrunOn = DEBUG_OFF;
        }
        else
        {
            GetXmlConfigAttribute(xmlAceDebug)->TrunOn = DEBUG_ON;
        }

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "State(OK)\n");
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETDEBUG;
}

void DoMessage_ShowDebug(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << GetXmlConfigAttribute(xmlAceDebug)->TrunOn;
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "DebugState(%d)\n", GetXmlConfigAttribute(xmlAceDebug)->TrunOn);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "State(OK)\n");
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SHOWDEBUG;
}

void DoMessage_SetTrackIP(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _DyeIPInfo objDyeIPInfo;

    if (true == GetDyeingIP(CommandInfo.m_strCommandExp.c_str(), objDyeIPInfo))
    {
        App_MessageServiceGroup::instance()->AddDyringIP(objDyeIPInfo.m_strClientIP.c_str(), 
            objDyeIPInfo.m_u2MaxCount);
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "State(OK).\n", pBuffPacket);
    }
    else
    {
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 1, "State(FAIL).\n", pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETTRACKIP;
}

void DoMessage_SetTraceCommand(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _DyeCommandInfo objDyeCommandInfo;

    if (true == GetDyeingCommand(CommandInfo.m_strCommandExp.c_str(), objDyeCommandInfo))
    {
        //清除追踪(此功能咱不提供)
        if (true == App_MessageServiceGroup::instance()->AddDyeingCommand(objDyeCommandInfo.m_u2CommandID, objDyeCommandInfo.m_u2MaxCount))
        {
            Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "State(OK).\n", pBuffPacket);
        }
        else
        {
            Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 1, "State(FAIL).\n", pBuffPacket);
        }

    }
    else
    {
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 1, "State(FAIL).\n", pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETTRACECOMMAND;
}

void DoMessage_GetTrackCommand(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") != 0)
    {
        return;
    }

    vec_Dyeing_Command_list objList;

    //记录总个数(此功能咱不提供)
    App_MessageServiceGroup::instance()->GetDyeingCommand(objList);

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint8)objList.size();
    }
    else
    {
        char szTemp[MAX_BUFF_1024] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_1024, "CommandID Count(%d)\n", objList.size());
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }

    for (_Dyeing_Command dyeingcommand : objList)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << dyeingcommand.m_u2CommandID;
            (*pBuffPacket) << dyeingcommand.m_u2CurrCount;
            (*pBuffPacket) << dyeingcommand.m_u2MaxCount;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "CommandID ID(%d)\n", dyeingcommand.m_u2CommandID);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "CurrCount ID(%d)\n", dyeingcommand.m_u2CurrCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "MaxCount ID(%d)\n", dyeingcommand.m_u2MaxCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETTRACKCOMMAND;
}

void DoMessage_GetConnectIPInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    int nConnectID = 0;

    if (GetConnectServerID(CommandInfo.m_strCommandExp.c_str(), nConnectID) == true)
    {
        _ClientIPInfo objClientIPInfo = App_HandlerManager::instance()->GetClientIPInfo((uint32)nConnectID);

        if (objClientIPInfo.m_strClientIP == "")
        {
            //没有找到对应的IP信息
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint16)1;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "IP not find\n");
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
        else
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                //找到了对应的IP信息
                (*pBuffPacket) << (uint16)0;

                VCHARS_STR strSName;
                strSName.text = (char* )objClientIPInfo.m_strClientIP.c_str();
                strSName.u1Len = (uint8)objClientIPInfo.m_strClientIP.length();

                (*pBuffPacket) << strSName;                          //IP
                (*pBuffPacket) << (uint32)objClientIPInfo.m_u2Port;  //端口
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "ClientIP=%s,Port=%d.\n", 
                    objClientIPInfo.m_strClientIP.c_str(), 
                    objClientIPInfo.m_u2Port);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETCONNECTIPINFO;
}

void DoMessage_GetLogLevelInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << AppLogManager::instance()->GetLogCount();
            (*pBuffPacket) << AppLogManager::instance()->GetCurrLevel();
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "LogCount=%d,LogLevel=%d.\n", AppLogManager::instance()->GetLogCount(), AppLogManager::instance()->GetCurrLevel());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (uint16 i = 0; i < (uint16)AppLogManager::instance()->GetLogCount(); i++)
        {
            uint16 u2LogID = AppLogManager::instance()->GetLogID(i);

            const char* pServerName = AppLogManager::instance()->GetLogInfoByServerName(u2LogID);

            if (NULL == pServerName)
            {
                //如果服务器名称为空则直接返回
                continue;
            }

            uint8 u1ServerNameLen = (uint8)ACE_OS::strlen(pServerName);

            const char* pLogName = AppLogManager::instance()->GetLogInfoByLogName(u2LogID);

            if (NULL == pLogName)
            {
                //如果服务器名称为空则直接返回
                return;
            }

            uint8 u1LogNameLen = (uint8)ACE_OS::strlen(pLogName);

            uint8 u1LogType = (uint8)AppLogManager::instance()->GetLogInfoByLogDisplay(u2LogID);

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << u2LogID;
                (*pBuffPacket) << u1ServerNameLen;
                pBuffPacket->WriteStream(pServerName, u1ServerNameLen);
                (*pBuffPacket) << u1LogNameLen;
                pBuffPacket->WriteStream(pLogName, u1LogNameLen);
                (*pBuffPacket) << u1LogType;
                (*pBuffPacket) << AppLogManager::instance()->GetLogInfoByLogLevel(i);
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_u1OutputType=%d.\n", u2LogID);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "pServerName=%d.\n", pServerName);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "pLogName=%d.\n", pLogName);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "u1LogType=%d,GetLogInfoByLogLevel=%d.\n", u1LogType, AppLogManager::instance()->GetLogInfoByLogLevel(i));
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETLOGINFO;
}

void DoMessage_SetLogLevelInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    int nLogLevel = 1;

    if (GetLogLevel(CommandInfo.m_strCommandExp.c_str(), nLogLevel) == true)
    {
        AppLogManager::instance()->ResetLogData((uint16)nLogLevel);

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)0;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "OK.\n");
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETLOGLEVEL;
}

void DoMessage_GetThreadAI(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        vecWorkThreadAIInfo objvecWorkThreadAIInfo;
        App_MessageServiceGroup::instance()->GetWorkThreadAIInfo(objvecWorkThreadAIInfo);

        uint16 u2ThreadCount = (uint16)objvecWorkThreadAIInfo.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u2ThreadCount;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "u2ThreadCount=%d.\n", u2ThreadCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (uint16 i = 0; i < u2ThreadCount; i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4ThreadID;
                (*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u1WTAI;
                (*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4DisposeTime;
                (*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4WTCheckTime;
                (*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4WTTimeoutCount;
                (*pBuffPacket) << objvecWorkThreadAIInfo[i].m_u4WTStopTime;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_u4ThreadID=%d,m_u1WTAI=%d,m_u4WTTimeoutCount=%d.\n",
                             objvecWorkThreadAIInfo[i].m_u4ThreadID,
                             objvecWorkThreadAIInfo[i].m_u1WTAI,
                             objvecWorkThreadAIInfo[i].m_u4WTTimeoutCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETWTAI;
}

void DoMessage_GetWorkThreadTO(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        vecCommandTimeout objTimeout;
        vecCommandTimeout objTimeoutF;
        App_MessageServiceGroup::instance()->GetAITO(objTimeout);

        uint16 u2ThreadCount = (uint16)objTimeout.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u2ThreadCount;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "u2ThreadCount=%d.\n", u2ThreadCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (uint16 i = 0; i < u2ThreadCount; i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << objTimeout[i].m_u4ThreadID;
                (*pBuffPacket) << objTimeout[i].m_u2CommandID;
                (*pBuffPacket) << (uint32)objTimeout[i].m_u8Second;
                (*pBuffPacket) << objTimeout[i].m_u4Timeout;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_u4ThreadID=%d,m_u2CommandID=%d,m_u4Second=%d,m_u4Timeout=%d.\n",
                             objTimeout[i].m_u4ThreadID,
                             objTimeout[i].m_u2CommandID,
                             (uint32)objTimeout[i].m_u8Second,
                             objTimeout[i].m_u4Timeout);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }

        App_MessageServiceGroup::instance()->GetAITF(objTimeoutF);

        u2ThreadCount = (uint16)objTimeoutF.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u2ThreadCount;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "u2ThreadCount=%d.\n", u2ThreadCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (uint16 i = 0; i < u2ThreadCount; i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << objTimeoutF[i].m_u4ThreadID;
                (*pBuffPacket) << objTimeoutF[i].m_u2CommandID;
                (*pBuffPacket) << (uint32)objTimeoutF[i].m_u8Second;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_u4ThreadID=%d,m_u2CommandID=%d,m_u4Second=%d.\n",
                             objTimeoutF[i].m_u4ThreadID,
                             objTimeoutF[i].m_u2CommandID,
                             (uint32)objTimeoutF[i].m_u8Second);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETWTTIMEOUT;
}

void DoMessage_SetWorkThreadAI(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    int nAI = 0;
    int nDispose = 0;
    int nCheck = 0;
    int nStop = 0;

    if (GetAIInfo(CommandInfo.m_strCommandExp.c_str(), nAI, nDispose, nCheck, nStop) == true
        && nDispose > 0
        && nCheck > 0
        && nStop > 0)
    {
        //规范化数据
        if (nAI > 0)
        {
            nAI = 1;
        }
        else
        {
            nAI = 0;
        }

        App_MessageServiceGroup::instance()->SetAI((uint8)nAI, (uint32)nDispose, (uint32)nCheck, (uint32)nStop);
    }

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint32)0;
    }
    else
    {
        char szTemp[MAX_BUFF_1024] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_1024, "OK.\n");
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETWTAI;
}

void DoMessage_GetNickNameInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    string strNickName;
    vecClientNameInfo objClientNameInfo;

    if (GetNickName(CommandInfo.m_strCommandExp.c_str(), strNickName) == true)
    {
        //暂时不实现
        //App_HandlerManager::instance()->GetClientNameInfo(szNickName, objClientNameInfo);

        //返回信息列表
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)objClientNameInfo.size();
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "objClientNameInfo size=%d.\n", objClientNameInfo.size());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (_ClientNameInfo clinetnameinfo : objClientNameInfo)
        {
            VCHARS_STR strIP;
            strIP.text = clinetnameinfo.m_szClientIP;
            strIP.u1Len = (uint8)ACE_OS::strlen(clinetnameinfo.m_szClientIP);

            VCHARS_STR strName;
            strName.text = clinetnameinfo.m_szName;
            strName.u1Len = (uint8)ACE_OS::strlen(clinetnameinfo.m_szName);

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint32)clinetnameinfo.m_nConnectID;
                (*pBuffPacket) << strIP;
                (*pBuffPacket) << (uint32)clinetnameinfo.m_u2Port;
                (*pBuffPacket) << strName;
                (*pBuffPacket) << (uint8)clinetnameinfo.m_nLog;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_nConnectID=%d,strIP=%s,m_nPort=%d,strName=%s,m_nLog=%d.\n",
                    clinetnameinfo.m_nConnectID,
                    clinetnameinfo.m_szClientIP,
                    clinetnameinfo.m_u2Port,
                    clinetnameinfo.m_szName,
                    clinetnameinfo.m_nLog);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETNICKNAMEINFO;
}

void DoMessage_SetConnectLog(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint32 u4ConnectID = 0;
    bool blIsLog = false;

    if (GetConnectID(CommandInfo.m_strCommandExp.c_str(), u4ConnectID, blIsLog) == true)
    {
        App_HandlerManager::instance()->SetIsLog(u4ConnectID, blIsLog);
    }

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint32)0;
    }
    else
    {
        char szTemp[MAX_BUFF_1024] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_1024, "OK.\n");
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETCONNECTLOG;
}

void DoMessage_SetMaxConnectCount(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint16 u2MaxConnectHandler = 0;

    if (GetMaxConnectCount(CommandInfo.m_strCommandExp.c_str(), u2MaxConnectHandler) == true && u2MaxConnectHandler > 0)
    {
        GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount = u2MaxConnectHandler;
    }

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint32)0;
    }
    else
    {
        char szTemp[MAX_BUFF_1024] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_1024, "OK.\n");
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETMAXCONNECTCOUNT;
}

void DoMessage_AddListen(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    bool blState = false;
    _ListenInfo objListenInfo;

    if (GetListenInfo(CommandInfo.m_strCommandExp.c_str(), objListenInfo) == true)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        blState = App_ProControlListen::instance()->AddListen(objListenInfo.m_strListenIP.c_str(),
                  objListenInfo.m_u2Port,
                  objListenInfo.m_u1IPType,
                  objListenInfo.m_u4PacketParseID);
#else
        blState = App_ControlListen::instance()->AddListen(objListenInfo.m_strListenIP.c_str(),
                  objListenInfo.m_u2Port,
                  objListenInfo.m_u1IPType,
                  objListenInfo.m_u4PacketParseID);
#endif

        if (true == blState)
        {
            Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "AddListen ok.\n", pBuffPacket);
        }
        else
        {
            Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "AddListen fail.\n", pBuffPacket);
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_ADD_LISTEN;
}

void DoMessage_DelListen(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    bool blState = false;
    _ListenInfo objListenInfo;

    if (GetListenInfo(CommandInfo.m_strCommandExp.c_str(), objListenInfo) == true)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        blState = App_ProControlListen::instance()->DelListen(objListenInfo.m_strListenIP.c_str(),
                  objListenInfo.m_u2Port);
#else
        blState = App_ControlListen::instance()->DelListen(objListenInfo.m_strListenIP.c_str(),
                  objListenInfo.m_u2Port);
#endif

        if (true == blState)
        {
            Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "DelListen ok.\n", pBuffPacket);
        }
        else
        {
            Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "DelListen fail.\n", pBuffPacket);
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_DEL_LISTEN;
}

void DoMessage_ShowListen(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint32 u4ListenCount = 0;

    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        u4ListenCount = App_ProControlListen::instance()->GetListenCount();
#else
        u4ListenCount = App_ControlListen::instance()->GetListenCount();
#endif

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u4ListenCount;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "objControlInfo Count=%d.\n", u4ListenCount);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (uint32 i = 0; i < u4ListenCount; i++)
        {
            _ControlInfo obj_ControlInfo;
#if PSS_PLATFORM == PLATFORM_WIN
            App_ProControlListen::instance()->ShowListen(i, obj_ControlInfo);
#else
            App_ControlListen::instance()->ShowListen(i, obj_ControlInfo);
#endif
            VCHARS_STR strIP;
            strIP.text = obj_ControlInfo.m_szListenIP;
            strIP.u1Len = (uint8)ACE_OS::strlen(obj_ControlInfo.m_szListenIP);

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << strIP;
                (*pBuffPacket) << obj_ControlInfo.m_u4Port;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "strIP=%s,m_u4Port=%d.\n",
                             obj_ControlInfo.m_szListenIP,
                             obj_ControlInfo.m_u4Port);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SHOW_LISTEN;
}

void DoMessage_MonitorInfo(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        uint32 u4FlowIn = 0;
        uint32 u4FlowOut = 0;

        //得到入口的所有流量统计
        vector<CWorkThread_Packet_Info> vec_Port_Data_Account;
        App_MessageServiceGroup::instance()->GetFlowPortList(vec_Port_Data_Account);

        for (const CWorkThread_Packet_Info& objCWorkThread_Packet_Info : vec_Port_Data_Account)
        {
            u4FlowIn += objCWorkThread_Packet_Info.m_u4RecvSize;
            u4FlowOut += objCWorkThread_Packet_Info.m_u4SendSize;
        }

        int nActiveClient   = 0;
        int nPoolClient     = 0;

        nActiveClient = App_HandlerManager::instance()->GetCount();
#if PSS_PLATFORM == PLATFORM_WIN
        nPoolClient = App_ProConnectHandlerPool::instance()->GetFreeCount();
#else
        nPoolClient = App_ConnectHandlerPool::instance()->GetFreeCount();
#endif

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nActiveClient;
            (*pBuffPacket) << (uint32)nPoolClient;
            (*pBuffPacket) << (uint16)GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount;
            (*pBuffPacket) << (uint16)App_ConnectAccount::instance()->GetCurrConnect();
            (*pBuffPacket) << u4FlowIn;
            (*pBuffPacket) << u4FlowOut;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "nActiveClient=%d,nPoolClient=%d,MaxHandlerCoun=%d,CurrConnect=%d,u4FlowIn=%d,u4FlowOut=%d.\n",
                         nActiveClient,
                         nPoolClient,
                         GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount,
                         App_ConnectAccount::instance()->GetCurrConnect(),
                         u4FlowIn,
                         u4FlowOut);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_MONITOR_INFO;
}

void DoMessage_ServerClose(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (NULL == pBuffPacket)
    {
        return;
    }

    //这里暂不实现，没必要
    ACE_UNUSED_ARG(CommandInfo);
    (*pBuffPacket) << u2ReturnCommandID;
    u2ReturnCommandID = CONSOLE_COMMAND_COLSECLIENT;
}

void DoMessage_TestFileStart(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    string strFileName;

    if (GetTestFileName(CommandInfo.m_strCommandExp.c_str(), strFileName) == true)
    {
        OUR_DEBUG((LM_INFO, "[Do_Message_TestFileStart]file=%s.\n", strFileName.c_str()));
        u2ReturnCommandID = CONSOLE_COMMAND_FILE_TEST_START;

        FileTestResultInfoSt objFileResult;

        objFileResult = App_FileTestManager::instance()->FileTestStart(strFileName.c_str());

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << static_cast<uint32>(objFileResult.n4Result);
            (*pBuffPacket) << objFileResult.n4TimeInterval;
            (*pBuffPacket) << objFileResult.n4ProNum;
            (*pBuffPacket) << (uint16)objFileResult.vecProFileDesc.size();
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "n4Result=%d,n4TimeInterval=%d,n4ProNum=%d,vecProFileDesc=%d.\n",
                         objFileResult.n4Result,
                         objFileResult.n4TimeInterval,
                         objFileResult.n4ProNum,
                         objFileResult.vecProFileDesc.size());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (uint16 i = 0; i < (uint16)objFileResult.vecProFileDesc.size(); i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << objFileResult.vecProFileDesc[i];
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "vecProFileDesc=%s.\n",
                             objFileResult.vecProFileDesc[i].c_str());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }
    else
    {
        u2ReturnCommandID = CONSOLE_COMMAND_FILE_TEST_START;

        if (CommandInfo.m_u1OutputType == 0)
        {
            int nErrorRet = -1;
            (*pBuffPacket) << nErrorRet;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "Fail.\n");
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }
}

void DoMessage_TestFileStop(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        u2ReturnCommandID = CONSOLE_COMMAND_FILE_TEST_STOP;
        int nRet = App_FileTestManager::instance()->FileTestEnd();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nRet;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "Ret=%d.\n", nRet);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }
}

void DoMessage_PortList(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    OUR_DEBUG((LM_INFO, "[DoMessage_PortList]In.\n"));

    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        OUR_DEBUG((LM_INFO, "[DoMessage_PortList]In 1.\n"));
        u2ReturnCommandID = CONSOLE_COMMAND_PORT_FLOW;

        vector<CWorkThread_Packet_Info> vec_Port_Data_Account;
        App_MessageServiceGroup::instance()->GetFlowPortList(vec_Port_Data_Account);

        uint32 u4Count = (uint32)vec_Port_Data_Account.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u4Count;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "u4Count=%d.\n", u4Count);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (uint32 i = 0; i < u4Count; i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u1ThreadID;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u4Minute;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u4PacketIn;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u4PacketOut;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_u1Type=%d,m_u4Port=%d,m_u4FlowIn=%d,m_u4FlowOut=%d.\n",
                             vec_Port_Data_Account[i].m_u1ThreadID,
                             vec_Port_Data_Account[i].m_u4Minute,
                             vec_Port_Data_Account[i].m_u4PacketIn,
                             vec_Port_Data_Account[i].m_u4PacketOut);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }
}

void Do_Message_BuffPacket(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_strCommandExp.c_str(), "-a") == 0)
    {
        u2ReturnCommandID = CONSOLE_COMMAND_PACKET_STATE;

        uint32 u4Count = App_BuffPacketManager::instance()->GetCreateInfoCount();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u4Count;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "u4Count=%d.\n", u4Count);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

        for (uint32 i = 0; i < u4Count; i++)
        {
            _Object_Create_Info objCreateInfo;
            App_BuffPacketManager::instance()->GetCreateInfoList(i, objCreateInfo);

            VCHARS_STR strFileName;
            strFileName.text = objCreateInfo.m_szCreateFileName;
            strFileName.u1Len = (uint8)ACE_OS::strlen(objCreateInfo.m_szCreateFileName);

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << strFileName;
                (*pBuffPacket) << objCreateInfo.m_u4Line;
                (*pBuffPacket) << objCreateInfo.m_u4Count;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "strFileName=%s,m_u4Line=%d,m_u4Count=%d.\n",
                             objCreateInfo.m_szCreateFileName,
                             objCreateInfo.m_u4Line,
                             objCreateInfo.m_u4Count);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }
}

void Do_Message_PoolSet(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _PoolName objPoolName;

    if (GetPoolSet(CommandInfo.m_strCommandExp.c_str(), objPoolName) == true)
    {
        u2ReturnCommandID = CONSOLE_COMMAND_POOL_SET;

        if (objPoolName.m_strPoolName == "BuffPacket")
        {
            App_BuffPacketManager::instance()->SetCreateFlag(objPoolName.m_blState);

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint8)0;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "Set ok.\n");
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
        else if (objPoolName.m_strPoolName == "PacketParse")
        {
            App_PacketParsePool::instance()->SetCreateFlag(objPoolName.m_blState);

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint8)0;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "Set ok.\n");
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }
}
