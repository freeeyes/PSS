#include "MessageDispose.h"

void Combo_Common_Return_Data(uint8 u1OutputType, uint8 u1State, const char* pMessage, IBuffPacket* pBuffPacket)
{
    if (u1OutputType == 0)
    {
        (*pBuffPacket) << (uint8)u1State;
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
        (*pBuffPacket) << (uint32)u4Count;
    }
    else
    {
        char szTemp[MAX_BUFF_200] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_200, pMessage);
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }
}

void Combo_Common_vecClientConnectInfo(uint8 u1OutputType, vecClientConnectInfo& VecClientConnectInfo, IBuffPacket* pBuffPacket)
{
    char szIP[MAX_BUFF_100] = { '\0' };

    for (int i = 0; i < (int)VecClientConnectInfo.size(); i++)
    {
        _ClientConnectInfo ClientConnectInfo = VecClientConnectInfo[i];

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

void Combo_Common_VecForbiddenIP(uint8 u1OutputType, VecForbiddenIP* pIPList, IBuffPacket* pBuffPacket)
{
    for (int i = 0; i < (int)pIPList->size(); i++)
    {
        if (u1OutputType == 0)
        {
            VCHARS_STR strSName;
            strSName.text = pIPList->at(i).m_szClientIP;
            strSName.u1Len = (uint8)ACE_OS::strlen(pIPList->at(i).m_szClientIP);

            (*pBuffPacket) << strSName;
            (*pBuffPacket) << pIPList->at(i).m_u1Type;
            (*pBuffPacket) << (uint32)pIPList->at(i).m_tvBegin.sec();
            (*pBuffPacket) << pIPList->at(i).m_u4Second;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden(%s)\n", pIPList->at(i).m_szClientIP);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden Type(%d)\n", pIPList->at(i).m_u1Type);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden BeginTime(%d)\n", pIPList->at(i).m_tvBegin.sec());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "IP Forbidden IntervalTime(%d)\n", pIPList->at(i).m_u4Second);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }
}

void DoMessage_LoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _FileInfo FileInfo;

    if (false == GetFileInfo(CommandInfo.m_szCommandExp, FileInfo) && CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint8)1;
    }
    else
    {
        char szTemp[MAX_BUFF_200] = { '\0' };
        sprintf_safe(szTemp, MAX_BUFF_200, "Command data is Error.\n");
        pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
    }

    //加载文件MessageManager
    //消息所有的工作线程同步更新一下各自的消息列表副本
    if (true == App_ModuleLoader::instance()->LoadModule(FileInfo.m_szFilePath, FileInfo.m_szFileName, FileInfo.m_szFileParam) &&
        true == App_MessageServiceGroup::instance()->PutUpdateCommandMessage(0))
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            char szTemp[MAX_BUFF_200] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_200, "LoadModule(%s) is fail.\n", FileInfo.m_szFileName);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
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
                char szTemp[MAX_BUFF_200] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_200, "LoadModule(%s) is OK.\n", FileInfo.m_szFileName);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_LOADMOUDLE;
}

void DoMessage_UnLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (true == App_MessageManager::instance()->UnloadModuleCommand(CommandInfo.m_szCommandExp, (uint8)1, App_MessageServiceGroup::instance()->GetWorkThreadCount()) &&
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

void DoMessage_ReLoadModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (true == App_MessageManager::instance()->UnloadModuleCommand(CommandInfo.m_szCommandExp, (uint8)2, App_MessageServiceGroup::instance()->GetWorkThreadCount()) &&
        true == App_MessageServiceGroup::instance()->PutUpdateCommandMessage(App_MessageManager::instance()->GetUpdateIndex()))
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            char szTemp[MAX_BUFF_200] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_200, "ReloadModule(%s) is ok.\n", CommandInfo.m_szCommandExp);
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
            sprintf_safe(szTemp, MAX_BUFF_200, "ReloadModule(%s) is false.\n", CommandInfo.m_szCommandExp);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_RELOADMOUDLE;
}

void DoMessage_ClientMessageCount(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-c") == 0)
    {
        //-c 只返回当前激活的链接数
#if WIN32
        int nActiveClient = App_ProConnectManager::instance()->GetCount();

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

#else
        int nActiveClient = App_ConnectManager::instance()->GetCount();

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

#endif
    }
    else if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-cp") == 0)
    {
        //-cp 返回当前激活连接数和池中剩余可分配数
#if WIN32
        int nActiveClient = App_ProConnectManager::instance()->GetCount();
        int nPoolClient = App_ProConnectHandlerPool::instance()->GetFreeCount();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nActiveClient;
            (*pBuffPacket) << (uint32)nPoolClient;
            (*pBuffPacket) << (uint16)GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "ActiveClient(%d).\nPoolClient(%d).\nMaxHandlerCount(%d).\n", nActiveClient, nPoolClient, App_MainConfig::instance()->GetMaxHandlerCount());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }

#else
        int nActiveClient = App_ConnectManager::instance()->GetCount();
        int nPoolClient = App_ConnectHandlerPool::instance()->GetFreeCount();

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

#endif
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTCOUNT;
}

void DoMessage_ShowModule(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
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

        for (int i = 0; i < (int)vecModeInfo.size(); i++)
        {
            _ModuleInfo* pModuleInfo = vecModeInfo[i];

            if (CommandInfo.m_u1OutputType == 0)
            {
                VCHARS_STR strSName;
                strSName.u1Len = (uint8)ACE_OS::strlen(pModuleInfo->GetName());
                strSName.text = (char*)pModuleInfo->GetName();
                (*pBuffPacket) << strSName;
                VCHARS_STR strSModileFile;
                strSModileFile.u1Len = (uint8)ACE_OS::strlen(pModuleInfo->strModuleName.c_str());
                strSModileFile.text = (char*)pModuleInfo->strModuleName.c_str();
                (*pBuffPacket) << strSModileFile;
                VCHARS_STR strSModilePath;
                strSModilePath.u1Len = (uint8)ACE_OS::strlen(pModuleInfo->strModulePath.c_str());
                strSModilePath.text = (char*)pModuleInfo->strModulePath.c_str();
                (*pBuffPacket) << strSModilePath;
                VCHARS_STR strSModileParam;
                strSModileParam.u1Len = (uint8)ACE_OS::strlen(pModuleInfo->strModuleParam.c_str());
                strSModileParam.text = (char*)pModuleInfo->strModuleParam.c_str();
                (*pBuffPacket) << strSModileParam;
                VCHARS_STR strSModileDesc;
                strSModileDesc.u1Len = (uint8)ACE_OS::strlen(pModuleInfo->GetDesc());
                strSModileDesc.text = (char*)pModuleInfo->GetDesc();
                (*pBuffPacket) << strSModileDesc;

                char szTime[MAX_BUFF_100] = { '\0' };
                sprintf_safe(szTime, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", pModuleInfo->dtCreateTime.year(), pModuleInfo->dtCreateTime.month(), pModuleInfo->dtCreateTime.day(), pModuleInfo->dtCreateTime.hour(), pModuleInfo->dtCreateTime.minute(), pModuleInfo->dtCreateTime.second());
                strSName.text = (char*)szTime;
                strSName.u1Len = (uint8)ACE_OS::strlen(szTime);
                (*pBuffPacket) << strSName;

                //写入Module当前状态
                uint32 u4ErrorID = 0;
                uint8  u1MouduleState = 0;

                pModuleInfo->GetModuleState(u4ErrorID);
                u1MouduleState = u4ErrorID;

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

void DoMessage_CommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint16 u2CommandID = (uint16)ACE_OS::strtol(CommandInfo.m_szCommandExp, NULL, 16);

    if (u2CommandID != 0)
    {
        _CommandData objCommandData;
        _CommandData objCommandDataIn;
        _CommandData objCommandDataOut;

        //先查询接收命令
        App_MessageServiceGroup::instance()->GetCommandData(u2CommandID, objCommandDataIn);

        if (objCommandDataIn.m_u2CommandID == u2CommandID)
        {
            objCommandData += objCommandDataIn;
        }

#if WIN32
        //查询发送命令
        App_ProConnectManager::instance()->GetCommandData(u2CommandID, objCommandDataOut);

        if (objCommandDataOut.m_u2CommandID == u2CommandID)
        {

            objCommandData += objCommandDataOut;
        }

#else
        //查询发送命令
        App_ConnectManager::instance()->GetCommandData(u2CommandID, objCommandDataOut);

        if (objCommandDataOut.m_u2CommandID == u2CommandID)
        {

            objCommandData += objCommandDataOut;
        }

#endif

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

            u2ReturnCommandID = CONSOLE_COMMAND_COMMANDINFO;
        }
    }
}

void DoMessage_WorkThreadState(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-s") == 0)
    {
        //获得当前工作线程状态
        CThreadInfo* pThreadInfo = App_MessageServiceGroup::instance()->GetThreadInfo();

        if (NULL != pThreadInfo)
        {
            int nThreadCount = pThreadInfo->GetThreadCount();
            (*pBuffPacket) << (uint8)nThreadCount;

            for (int i = 0; i < nThreadCount; i++)
            {
                _ThreadInfo* pCurrThreadInfo = pThreadInfo->GetThreadInfo(i);

                if (CommandInfo.m_u1OutputType == 0)
                {
                    (*pBuffPacket) << (uint8)i;
                    (*pBuffPacket) << (uint32)pCurrThreadInfo->m_tvUpdateTime.sec();
                    (*pBuffPacket) << (uint32)pCurrThreadInfo->m_tvCreateTime.sec();
                    (*pBuffPacket) << (uint8)pCurrThreadInfo->m_u4State;
                    (*pBuffPacket) << (uint32)pCurrThreadInfo->m_u4RecvPacketCount;
                    (*pBuffPacket) << (uint16)pCurrThreadInfo->m_u2CommandID;
                    (*pBuffPacket) << (uint16)pCurrThreadInfo->m_u2PacketTime;
                    (*pBuffPacket) << (uint32)pCurrThreadInfo->m_u4CurrPacketCount;
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
    }
}

void DoMessage_ClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        vecClientConnectInfo VecClientConnectInfo;
#ifdef WIN32
        App_ProConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
#else
        App_ConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
#endif

        uint32 u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "Client IP Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTINFO;
}

void DoMessage_CloseClient(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint32 u4ConnectID = (uint32)ACE_OS::atoi(CommandInfo.m_szCommandExp);
#ifdef WIN32
    App_ProConnectManager::instance()->CloseConnect(u4ConnectID);

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

#else
    App_ConnectManager::instance()->CloseConnect(u4ConnectID);

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

#endif

    u2ReturnCommandID = CONSOLE_COMMAND_COLSECLIENT;
}

void DoMessage_ForbiddenIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _ForbiddenIP ForbiddenIP;

    if (GetForbiddenIP(CommandInfo.m_szCommandExp, ForbiddenIP) == true)
    {
        if (ForbiddenIP.m_u1Type == 0)
        {
            //永久封禁IP
            App_ForbiddenIP::instance()->AddForeverIP(ForbiddenIP.m_szClientIP);
        }
        else
        {
            //封禁时段IP
            App_ForbiddenIP::instance()->AddTempIP(ForbiddenIP.m_szClientIP, ForbiddenIP.m_u4Second);
        }

        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "IP Forbidden is OK.\n", pBuffPacket);
    }
    else
    {
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 1, "IP Forbidden is fail.\n", pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_FORBIDDENIP;
}

void DoMessage_ShowForbiddenList(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        VecForbiddenIP* pForeverForbiddenIP = App_ForbiddenIP::instance()->ShowForeverIP();
        VecForbiddenIP* pTempForbiddenIP = App_ForbiddenIP::instance()->ShowTemoIP();

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

void DoMessage_LifedIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    App_ForbiddenIP::instance()->DelForeverIP(CommandInfo.m_szCommandExp);
    App_ForbiddenIP::instance()->DelTempIP(CommandInfo.m_szCommandExp);

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

void DoMessage_UDPClientInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        vecClientConnectInfo VecClientConnectInfo;
#ifdef WIN32
        App_ProUDPManager::instance()->GetClientConnectInfo(VecClientConnectInfo);
#else
        App_ReUDPManager::instance()->GetClientConnectInfo(VecClientConnectInfo);
#endif

        uint32 u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "UDPClient Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_UDPCONNECTINFO;
}

void DoMessage_ServerConnectTCP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        vecClientConnectInfo VecClientConnectInfo;
#ifdef WIN32
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

void DoMessage_ServerConnectUDP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        vecClientConnectInfo VecClientConnectInfo;
#ifdef WIN32
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

void DoMessage_ShowProcessInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        int nCPU = 0;
        int nMemorySize = 0;
        uint8 u1Flow = GetXmlConfigAttribute(xmlCommandAccount)->FlowAccount;      //流量统计标记位
        uint32 u4FlowIn = 0;      //总流量流入字节
        uint32 u4FlowOut = 0;      //总流量流出字节


        //得到入口的所有流量统计
        uint32 u4MessageFlowIn = 0;
        uint32 u4MessageFlowOut = 0;
        App_MessageServiceGroup::instance()->GetFlowInfo(u4MessageFlowIn, u4MessageFlowOut);
        u4FlowIn += u4MessageFlowIn;
        u4FlowOut += u4MessageFlowOut;

#ifdef WIN32  //如果是windows
        //得到所有TCP出口流量统计
        uint32 u4ConnectFlowIn = 0;
        uint32 u4ConnectFlowOut = 0;
        App_ProConnectManager::instance()->GetFlowInfo(u4ConnectFlowIn, u4ConnectFlowOut);
        u4FlowIn += u4ConnectFlowIn;
        u4FlowOut += u4ConnectFlowOut;

        //得到多有UDP出口流量统计
        uint32 u4UdpFlowIn = 0;
        uint32 u4UdpFlowOut = 0;
        App_ProUDPManager::instance()->GetFlowInfo(u4UdpFlowIn, u4UdpFlowOut);
        u4FlowIn += u4UdpFlowIn;
        u4FlowOut += u4UdpFlowOut;

        nCPU = GetProcessCPU_Idel();
        nMemorySize = GetProcessMemorySize();
#else   //如果是linux
        //得到所有TCP出口流量统计
        uint32 u4ConnectFlowIn = 0;
        uint32 u4ConnectFlowOut = 0;
        App_ConnectManager::instance()->GetFlowInfo(u4ConnectFlowIn, u4ConnectFlowOut);
        u4FlowIn += u4MessageFlowIn;
        u4FlowOut += u4MessageFlowOut;

        //得到多有UDP出口流量统计
        uint32 u4UdpFlowIn = 0;
        uint32 u4UdpFlowOut = 0;
        App_ReUDPManager::instance()->GetFlowInfo(u4UdpFlowIn, u4UdpFlowOut);
        u4FlowIn += u4UdpFlowIn;
        u4FlowOut += u4UdpFlowOut;

        nCPU = GetProcessCPU_Idel_Linux();
        nMemorySize = GetProcessMemorySize_Linux();
#endif

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

void DoMessage_ShowClientHisTory(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
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

        for (int i = 0; i < (int)VecIPAccount.size(); i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                VCHARS_STR strSName;
                strSName.text = (char*)VecIPAccount[i].m_strIP.c_str();
                strSName.u1Len = (uint8)VecIPAccount[i].m_strIP.length();

                (*pBuffPacket) << strSName;
                (*pBuffPacket) << (uint32)VecIPAccount[i].m_nCount;
                (*pBuffPacket) << (uint32)VecIPAccount[i].m_nAllCount;

                sprintf_safe(szTime, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", VecIPAccount[i].m_dtLastTime.year(), VecIPAccount[i].m_dtLastTime.month(), VecIPAccount[i].m_dtLastTime.day(), VecIPAccount[i].m_dtLastTime.hour(), VecIPAccount[i].m_dtLastTime.minute(), VecIPAccount[i].m_dtLastTime.second());
                strSName.text = (char*)szTime;
                strSName.u1Len = (uint8)ACE_OS::strlen(szTime);

                (*pBuffPacket) << strSName;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "IP(%s)\n", VecIPAccount[i].m_strIP.c_str());
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "Count(%d)\n", VecIPAccount[i].m_nCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "AllCount(%d)\n", VecIPAccount[i].m_nAllCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "AllCount(%d)\n", VecIPAccount[i].m_nAllCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTime, MAX_BUFF_100, "%04d-%02d-%02d %02d:%02d:%02d", VecIPAccount[i].m_dtLastTime.year(), VecIPAccount[i].m_dtLastTime.month(), VecIPAccount[i].m_dtLastTime.day(), VecIPAccount[i].m_dtLastTime.hour(), VecIPAccount[i].m_dtLastTime.minute(), VecIPAccount[i].m_dtLastTime.second());
                sprintf_safe(szTemp, MAX_BUFF_1024, "DateTime(%s)\n", VecIPAccount[i].m_nAllCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTHISTORY;
}

void DoMessage_ShowAllCommandInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
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

        for (int i = 0; i < (int)vecModuleClient.size(); i++)
        {
            _ModuleClient* pModuleClient = vecModuleClient[i];

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

            for (int j = 0; j < (int)pModuleClient->m_vecClientCommandInfo.size(); j++)
            {
                _ClientCommandInfo* pClientCommandInfo = pModuleClient->m_vecClientCommandInfo[j];

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
    }

    u2ReturnCommandID = CONSOLE_COMMAND_ALLCOMMANDINFO;
}

void DoMessage_ShowServerInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            VCHARS_STR strSTemp;

            //返回服务器ID
            uint16 u2SerevrID = GetXmlConfigAttribute(xmlServerID)->id;
            (*pBuffPacket) << u2SerevrID;

            //返回服务器名称
            strSTemp.text = (char*)GetXmlConfigAttribute(xmlServerName)->name.c_str();
            strSTemp.u1Len = (uint8)GetXmlConfigAttribute(xmlServerName)->name.length();
            (*pBuffPacket) << strSTemp;

            //返回服务器版本
            strSTemp.text = (char*)GetXmlConfigAttribute(xmlServerVersion)->Version.c_str();
            strSTemp.u1Len = (uint8)GetXmlConfigAttribute(xmlServerVersion)->Version.length();
            (*pBuffPacket) << strSTemp;

            //返回加载模块个数
            (*pBuffPacket) << (uint16)App_ModuleLoader::instance()->GetCurrModuleCount();

            //返回工作线程个数
            (*pBuffPacket) << (uint16)App_MessageServiceGroup::instance()->GetThreadInfo()->GetThreadCount();

            //返回当前协议包的版本号
            strSTemp.text = (char*)GetXmlConfigAttribute(xmlServerVersion)->Version.c_str();
            strSTemp.u1Len = (uint8)GetXmlConfigAttribute(xmlServerVersion)->Version.length();
            (*pBuffPacket) << strSTemp;

            //返回当前服务器是大端还是小端
            if (App_MainConfig::instance()->GetCharOrder() == SYSTEM_LITTLE_ORDER)
            {
                (*pBuffPacket) << (uint8)0;     //小端
            }
            else
            {
                (*pBuffPacket) << (uint8)1;     //大端
            }

            //返回当前网络包字序规则
            if (App_MainConfig::instance()->GetByteOrder() == false)
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

            if (App_MainConfig::instance()->GetByteOrder() == false)
            {
                sprintf_safe(szCharOrder, MAX_BUFF_20, "HostOrder");   //主机字序
            }
            else
            {
                sprintf_safe(szCharOrder, MAX_BUFF_20, "NetOrder");    //网络字序
            }


            char szConsoleOutput[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szConsoleOutput, MAX_BUFF_1024, "ServerID:%d\nServerName:%s\nServerVersion:%s\nServerModuleCount=%d\nWorkthreadCount:%d\nServerProtocol:%s\nCharOrder:%s\n",
                         App_MainConfig::instance()->GetServerID(),
                         App_MainConfig::instance()->GetServerName(),
                         App_MainConfig::instance()->GetServerVersion(),
                         App_ModuleLoader::instance()->GetCurrModuleCount(),
                         App_MessageServiceGroup::instance()->GetThreadInfo()->GetThreadCount(),
                         App_MainConfig::instance()->GetServerVersion(),
                         szCharOrder);

            pBuffPacket->WriteStream(szConsoleOutput, (uint32)ACE_OS::strlen(szConsoleOutput));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERINFO;
}

void DoMessage_ReConnectServer(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    int nSerevrID = 0;

    if (GetConnectServerID(CommandInfo.m_szCommandExp, nSerevrID) == true)
    {
        //获得当前连接状态
        vecClientConnectInfo VecClientConnectInfo;
#ifdef WIN32  //如果是windows
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

void DoMessage_CommandTimeout(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
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

void DoMessage_CommandTimeoutclr(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
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

void DoMessage_CommandDataLog(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        //存储所有接收统计日志
        App_MessageServiceGroup::instance()->SaveCommandDataLog();

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

void DoMessage_SetDebug(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint8 u1Debug = 0;

    if (GetDebug(CommandInfo.m_szCommandExp, u1Debug) == true)
    {
        if (u1Debug == DEBUG_OFF)
        {
            App_MainConfig::instance()->SetDebug(DEBUG_OFF);
        }
        else
        {
            App_MainConfig::instance()->SetDebug(DEBUG_ON);
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

void DoMessage_ShowDebug(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)App_MainConfig::instance()->GetDebug();
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "DebugState(%d)\n", App_MainConfig::instance()->GetDebug());
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            sprintf_safe(szTemp, MAX_BUFF_1024, "State(OK)\n");
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SHOWDEBUG;
}

void DoMessage_SetTrackIP(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _DyeIPInfo objDyeIPInfo;

    if (true == GetDyeingIP(CommandInfo.m_szCommandExp, objDyeIPInfo))
    {
        App_MessageServiceGroup::instance()->AddDyringIP(objDyeIPInfo.m_szClientIP, objDyeIPInfo.m_u2MaxCount);
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 0, "State(OK).\n", pBuffPacket);
    }
    else
    {
        Combo_Common_Return_Data(CommandInfo.m_u1OutputType, 1, "State(FAIL).\n", pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETTRACKIP;
}

void DoMessage_SetTraceCommand(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _DyeCommandInfo objDyeCommandInfo;

    if (true == GetDyeingCommand(CommandInfo.m_szCommandExp, objDyeCommandInfo))
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

void DoMessage_GetTrackCommand(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    vec_Dyeing_Command_list objList;

    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
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

        for (int i = 0; i < (int)objList.size(); i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint16)objList[i].m_u2CommandID;
                (*pBuffPacket) << (uint16)objList[i].m_u2CurrCount;
                (*pBuffPacket) << (uint16)objList[i].m_u2MaxCount;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "CommandID ID(%d)\n", objList[i].m_u2CommandID);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "CurrCount ID(%d)\n", objList[i].m_u2CurrCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
                sprintf_safe(szTemp, MAX_BUFF_1024, "MaxCount ID(%d)\n", objList[i].m_u2MaxCount);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETTRACKCOMMAND;
}

void DoMessage_GetConnectIPInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    int nConnectID = 0;

    if (GetConnectServerID(CommandInfo.m_szCommandExp, nConnectID) == true)
    {
#ifdef WIN32  //如果是windows
        _ClientIPInfo objClientIPInfo = App_ProConnectManager::instance()->GetClientIPInfo((uint32)nConnectID);
#else
        _ClientIPInfo objClientIPInfo = App_ConnectManager::instance()->GetClientIPInfo((uint32)nConnectID);
#endif


        if (ACE_OS::strlen(objClientIPInfo.m_szClientIP) == 0)
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
                strSName.text = objClientIPInfo.m_szClientIP;
                strSName.u1Len = (uint8)ACE_OS::strlen(objClientIPInfo.m_szClientIP);

                (*pBuffPacket) << strSName;                         //IP
                (*pBuffPacket) << (uint32)objClientIPInfo.m_nPort;  //端口
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "ClientIP=%s,Port=%d.\n", objClientIPInfo.m_szClientIP, objClientIPInfo.m_nPort);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETCONNECTIPINFO;
}

void DoMessage_GetLogLevelInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
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
            VCHARS_STR strSServerName;
            VCHARS_STR strSLogName;

            char* pServerName = AppLogManager::instance()->GetLogInfoByServerName(u2LogID);

            if (NULL == pServerName)
            {
                //如果服务器名称为空则直接返回
                continue;
            }

            strSServerName.text = pServerName;
            strSServerName.u1Len = (uint8)ACE_OS::strlen(pServerName);

            char* pLogName = AppLogManager::instance()->GetLogInfoByLogName(u2LogID);

            if (NULL == pLogName)
            {
                //如果服务器名称为空则直接返回
                return;
            }

            strSLogName.text = pLogName;
            strSLogName.u1Len = (uint8)ACE_OS::strlen(pLogName);

            uint8 u1LogType = (uint8)AppLogManager::instance()->GetLogInfoByLogDisplay(u2LogID);

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << u2LogID;
                (*pBuffPacket) << strSServerName;
                (*pBuffPacket) << strSLogName;
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

void DoMessage_SetLogLevelInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    int nLogLevel = 1;

    if (GetLogLevel(CommandInfo.m_szCommandExp, nLogLevel) == true)
    {
        AppLogManager::instance()->ResetLogData(nLogLevel);

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

void DoMessage_GetThreadAI(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        vecWorkThreadAIInfo objvecWorkThreadAIInfo;
        App_MessageServiceGroup::instance()->GetWorkThreadAIInfo(objvecWorkThreadAIInfo);

        uint16 u2ThreadCount = (uint16)objvecWorkThreadAIInfo.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint16)u2ThreadCount;
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

void DoMessage_GetWorkThreadTO(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        vecCommandTimeout objTimeout;
        vecCommandTimeout objTimeoutF;
        App_MessageServiceGroup::instance()->GetAITO(objTimeout);

        uint16 u2ThreadCount = (uint16)objTimeout.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint16)u2ThreadCount;
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
                (*pBuffPacket) << objTimeout[i].m_u4Second;
                (*pBuffPacket) << objTimeout[i].m_u4Timeout;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_u4ThreadID=%d,m_u2CommandID=%d,m_u4Second=%d,m_u4Timeout=%d.\n",
                             objTimeout[i].m_u4ThreadID,
                             objTimeout[i].m_u2CommandID,
                             objTimeout[i].m_u4Second,
                             objTimeout[i].m_u4Timeout);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }

        App_MessageServiceGroup::instance()->GetAITF(objTimeoutF);

        u2ThreadCount = (uint16)objTimeoutF.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint16)u2ThreadCount;
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
                (*pBuffPacket) << objTimeoutF[i].m_u4Second;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_u4ThreadID=%d,m_u2CommandID=%d,m_u4Second=%d.\n",
                             objTimeoutF[i].m_u4ThreadID,
                             objTimeoutF[i].m_u2CommandID,
                             objTimeoutF[i].m_u4Second);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETWTTIMEOUT;
}

void DoMessage_SetWorkThreadAI(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    int nAI = 0;
    int nDispose = 0;
    int nCheck = 0;
    int nStop = 0;

    if (GetAIInfo(CommandInfo.m_szCommandExp, nAI, nDispose, nCheck, nStop) == true
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

void DoMessage_GetNickNameInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    char szNickName[MAX_BUFF_100] = { '\0' };
    vecClientNameInfo objClientNameInfo;

    if (GetNickName(CommandInfo.m_szCommandExp, szNickName) == true)
    {
#ifdef WIN32
        App_ProConnectManager::instance()->GetClientNameInfo(szNickName, objClientNameInfo);
#else
        App_ConnectManager::instance()->GetClientNameInfo(szNickName, objClientNameInfo);
#endif

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

        for (uint32 i = 0; i < objClientNameInfo.size(); i++)
        {
            VCHARS_STR strIP;
            strIP.text = objClientNameInfo[i].m_szClientIP;
            strIP.u1Len = (uint8)ACE_OS::strlen(objClientNameInfo[i].m_szClientIP);

            VCHARS_STR strName;
            strName.text = objClientNameInfo[i].m_szName;
            strName.u1Len = (uint8)ACE_OS::strlen(objClientNameInfo[i].m_szName);

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint32)objClientNameInfo[i].m_nConnectID;
                (*pBuffPacket) << strIP;
                (*pBuffPacket) << (uint32)objClientNameInfo[i].m_nPort;
                (*pBuffPacket) << strName;
                (*pBuffPacket) << (uint8)objClientNameInfo[i].m_nLog;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_nConnectID=%d,strIP=%s,m_nPort=%d,strName=%s,m_nLog=%d.\n",
                             objClientNameInfo[i].m_nConnectID,
                             objClientNameInfo[i].m_szClientIP,
                             objClientNameInfo[i].m_nPort,
                             objClientNameInfo[i].m_szName,
                             objClientNameInfo[i].m_nLog);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETNICKNAMEINFO;
}

void DoMessage_SetConnectLog(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint32 u4ConnectID = 0;
    bool blIsLog = false;

    if (GetConnectID(CommandInfo.m_szCommandExp, u4ConnectID, blIsLog) == true)
    {
#ifdef WIN32
        App_ProConnectManager::instance()->SetIsLog(u4ConnectID, blIsLog);
#else
        App_ConnectManager::instance()->SetIsLog(u4ConnectID, blIsLog);
#endif
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

void DoMessage_SetMaxConnectCount(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint16 u2MaxConnectHandler = 0;

    if (GetMaxConnectCount(CommandInfo.m_szCommandExp, u2MaxConnectHandler) == true && u2MaxConnectHandler > 0)
    {
        App_MainConfig::instance()->SetMaxHandlerCount(u2MaxConnectHandler);
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

void DoMessage_AddListen(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    bool blState = false;
    _ListenInfo objListenInfo;

    if (GetListenInfo(CommandInfo.m_szCommandExp, objListenInfo) == true)
    {
#ifdef WIN32
        blState = App_ProControlListen::instance()->AddListen(objListenInfo.m_szListenIP,
                  objListenInfo.m_u4Port,
                  objListenInfo.m_u1IPType,
                  objListenInfo.m_u4PacketParseID);
#else
        blState = App_ControlListen::instance()->AddListen(objListenInfo.m_szListenIP,
                  objListenInfo.m_u4Port,
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

void DoMessage_DelListen(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    bool blState = false;
    _ListenInfo objListenInfo;

    if (GetListenInfo(CommandInfo.m_szCommandExp, objListenInfo) == true)
    {
#ifdef WIN32
        blState = App_ProControlListen::instance()->DelListen(objListenInfo.m_szListenIP,
                  objListenInfo.m_u4Port);
#else
        blState = App_ControlListen::instance()->DelListen(objListenInfo.m_szListenIP,
                  objListenInfo.m_u4Port);
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

void DoMessage_ShowListen(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    uint32 u4ListenCount = 0;

    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
#ifdef WIN32
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
#ifdef WIN32
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

void DoMessage_MonitorInfo(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        uint32 u4FlowIn = 0;
        uint32 u4FlowOut = 0;

        //得到入口的所有流量统计
        App_MessageServiceGroup::instance()->GetFlowInfo(u4FlowIn, u4FlowOut);

        //得到所有Connect流量统计
        uint32 u4ConnectFlowIn = 0;
        uint32 u4ConnectFlowOut = 0;
#if WIN32
        App_ProConnectManager::instance()->GetFlowInfo(u4ConnectFlowIn, u4ConnectFlowOut);
#else
        App_ConnectManager::instance()->GetFlowInfo(u4ConnectFlowIn, u4ConnectFlowOut);
#endif
        u4FlowIn += u4ConnectFlowIn;
        u4FlowOut += u4ConnectFlowOut;

        //得到所有UDP的流量统计
        uint32 u4UdpFlowIn  = 0;
        uint32 u4UdpFlowOut = 0;
        int nActiveClient   = 0;
        int nPoolClient     = 0;
#ifdef WIN32
        App_ProUDPManager::instance()->GetFlowInfo(u4UdpFlowIn, u4UdpFlowOut);
        nActiveClient = App_ProConnectManager::instance()->GetCount();
        nPoolClient = App_ProConnectHandlerPool::instance()->GetFreeCount();
#else
        App_ReUDPManager::instance()->GetFlowInfo(u4UdpFlowIn, u4UdpFlowOut);
        nActiveClient = App_ConnectManager::instance()->GetCount();
        nPoolClient = App_ConnectHandlerPool::instance()->GetFreeCount();
#endif
        u4FlowIn += u4UdpFlowIn;
        u4FlowOut += u4UdpFlowOut;

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nActiveClient;
            (*pBuffPacket) << (uint32)nPoolClient;
            (*pBuffPacket) << (uint16)App_MainConfig::instance()->GetMaxHandlerCount();
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
                         App_MainConfig::instance()->GetMaxHandlerCount(),
                         App_ConnectAccount::instance()->GetCurrConnect(),
                         u4FlowIn,
                         u4FlowOut);
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_MONITOR_INFO;
}

void DoMessage_ServerClose(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (NULL == pBuffPacket)
    {
        return;
    }

    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0 && App_MainConfig::instance()->GetServerClose() == 0)
    {
        u2ReturnCommandID = CONSOLE_COMMAND_CLOSE_SERVER;
        App_ServerObject::instance()->GetServerManager()->Close();
    }
}

void DoMessage_TestFileStart(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    char szFileName[MAX_BUFF_200] = { '\0' };

    if (GetTestFileName(CommandInfo.m_szCommandExp, szFileName) == true)
    {
        OUR_DEBUG((LM_INFO, "[Do_Message_TestFileStart]file=%s.\n", szFileName));
        u2ReturnCommandID = CONSOLE_COMMAND_FILE_TEST_START;

        FileTestResultInfoSt objFileResult;

        objFileResult = App_FileTestManager::instance()->FileTestStart(szFileName);

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << objFileResult.n4Result;
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
            (*pBuffPacket) << (int)-1;
        }
        else
        {
            char szTemp[MAX_BUFF_1024] = { '\0' };
            sprintf_safe(szTemp, MAX_BUFF_1024, "Fail.\n");
            pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
        }
    }
}

void DoMessage_TestFileStop(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
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

void DoMessage_PortList(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
    {
        u2ReturnCommandID = CONSOLE_COMMAND_PORT_FLOW;

        vector<_Port_Data_Account> vec_Port_Data_Account;
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
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u1Type;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u4Port;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u4FlowIn;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u4FlowOut;
            }
            else
            {
                char szTemp[MAX_BUFF_1024] = { '\0' };
                sprintf_safe(szTemp, MAX_BUFF_1024, "m_u1Type=%d,m_u4Port=%d,m_u4FlowIn=%d,m_u4FlowOut=%d.\n",
                             vec_Port_Data_Account[i].m_u1Type,
                             vec_Port_Data_Account[i].m_u4Port,
                             vec_Port_Data_Account[i].m_u4FlowIn,
                             vec_Port_Data_Account[i].m_u4FlowOut);
                pBuffPacket->WriteStream(szTemp, (uint32)ACE_OS::strlen(szTemp));
            }
        }
    }
}

void Do_Message_BuffPacket(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    if (ACE_OS::strcmp(CommandInfo.m_szCommandExp, "-a") == 0)
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

void Do_Message_PoolSet(_CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID)
{
    _PoolName objPoolName;

    if (GetPoolSet(CommandInfo.m_szCommandExp, objPoolName) == true)
    {
        u2ReturnCommandID = CONSOLE_COMMAND_POOL_SET;

        if (ACE_OS::strcmp(objPoolName.m_szPoolName, "BuffPacket") == 0)
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
        else if (ACE_OS::strcmp(objPoolName.m_szPoolName, "PacketParse") == 0)
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
