#include "MessageDispose.h"

void Combo_Common_Return_Data(uint8 u1OutputType, uint8 u1State, const char* pMessage, shared_ptr<IBuffPacket> pBuffPacket)
{
    if (u1OutputType == 0)
    {
        (*pBuffPacket) << u1State;
    }
    else
    {
        string strMessage = pMessage;
        pBuffPacket->WriteStream(strMessage.c_str(), (uint32)strMessage.length());
    }
}

void Combo_Common_Head_Data(uint8 u1OutputType, uint32 u4Count, const char* pMessage, shared_ptr<IBuffPacket> pBuffPacket)
{
    if (u1OutputType == 0)
    {
        (*pBuffPacket) << u4Count;
    }
    else
    {
        string strMessage = pMessage;
        pBuffPacket->WriteStream(pMessage, (uint32)strMessage.length());
    }
}

void Combo_Common_vecClientConnectInfo(uint8 u1OutputType, const vecClientConnectInfo VecClientConnectInfo, shared_ptr<IBuffPacket> pBuffPacket)
{
    string strIP;

    for (_ClientConnectInfo ClientConnectInfo : VecClientConnectInfo)
    {
        if (true == ClientConnectInfo.m_blValid)
        {
            strIP = "0.0.0.0:0";

            if (u1OutputType == 0)
            {
                (*pBuffPacket) << strIP;
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
                std::stringstream ss_format;
                ss_format << "UDP IP(" << strIP << ")\n"
                    << "UDP Command(" << ClientConnectInfo.m_u4ConnectID << ")\n"
                    << "UDP RecvCount(" << ClientConnectInfo.m_u4RecvCount << ")\n"
                    << "UDP SendCount(" << ClientConnectInfo.m_u4SendCount << ")\n"
                    << "UDP AllRecvSize(" << ClientConnectInfo.m_u4AllRecvSize << ")\n"
                    << "UDP AllSendSize(" << ClientConnectInfo.m_u4AllSendSize << ")\n"
                    << "UDP BeginTime(" << ClientConnectInfo.m_u4BeginTime << ")\n"
                    << "UDP RecvQueueCount(" << ClientConnectInfo.m_u4AliveTime << ")\n"
                    << "UDP AliveTime(" << ClientConnectInfo.m_u4RecvQueueCount << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }
}

void Combo_Common_VecForbiddenIP(uint8 u1OutputType, const VecForbiddenIP* pIPList, shared_ptr<IBuffPacket> pBuffPacket)
{
    for(_ForbiddenIP forbidenip : *pIPList)
    {
        if (u1OutputType == 0)
        {
            (*pBuffPacket) << forbidenip.m_strClientIP;
            (*pBuffPacket) << forbidenip.m_u1Type;
            (*pBuffPacket) << (uint32)CTimeStamp::Get_Time_use_second(forbidenip.m_tvBegin);
            (*pBuffPacket) << forbidenip.m_u4Second;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "IP Forbidden(" << forbidenip.m_strClientIP << ")\n"
                << "IP Forbidden Type(" << forbidenip.m_u1Type << ")\n"
                << "IP Forbidden BeginTime(" << CTimeStamp::Get_Time_use_second(forbidenip.m_tvBegin) << ")\n"
                << "IP Forbidden IntervalTime(" << forbidenip.m_u4Second << ")\n";
            string strLineText = ss_format.str();

            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }
}

void DoMessage_LoadModule(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    _FileInfo FileInfo;

    if (false == GetFileInfo(CommandInfo.m_strCommandExp, FileInfo) && CommandInfo.m_u1OutputType == 0)
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
        if (nullptr != pBuffPacket)
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

void DoMessage_UnLoadModule(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
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

void DoMessage_ReLoadModule(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
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
            std::stringstream ss_format;
            ss_format << "ReloadModule(" << CommandInfo.m_strCommandExp << ") is ok.\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
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
            std::stringstream ss_format;
            ss_format << "ReloadModule(" << CommandInfo.m_strCommandExp << ") is fail.\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_RELOADMOUDLE;
}

void DoMessage_ClientMessageCount(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-c")
    {
        //-c 只返回当前激活的链接数
        int nActiveClient = App_HandlerManager::instance()->GetCount();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nActiveClient;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "ActiveClient(" << CommandInfo.m_strCommandExp << ").\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }
    else if (CommandInfo.m_strCommandExp == "-cp")
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
            std::stringstream ss_format;
            ss_format << "ActiveClient(" << nActiveClient << ").\n"
                << "PoolClient(" << nPoolClient << ").\n"
                << "MaxHandlerCount(" << GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTCOUNT;
}

void DoMessage_ShowModule(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        vector<shared_ptr<_ModuleInfo>> vecModeInfo;
        App_ModuleLoader::instance()->GetAllModuleInfo(vecModeInfo);

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)App_ModuleLoader::instance()->GetCurrModuleCount();
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "ModuleCount(" << App_ModuleLoader::instance()->GetCurrModuleCount() << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (auto pModuleInfo : vecModeInfo)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                string strName = pModuleInfo->GetName();
                string strDesc = pModuleInfo->GetDesc();

                auto u1ModuleNameLen = (uint8)strName.length();
                (*pBuffPacket) << u1ModuleNameLen;
                pBuffPacket->WriteStream(pModuleInfo->GetName(), u1ModuleNameLen);
                auto u1SModileFileLen = (uint8)pModuleInfo->strModuleName.length();
                (*pBuffPacket) << u1SModileFileLen;
                pBuffPacket->WriteStream(pModuleInfo->strModuleName.c_str(), u1SModileFileLen);
                auto u1SModilePathLen = (uint8)pModuleInfo->strModulePath.length();
                (*pBuffPacket) << u1SModilePathLen;
                pBuffPacket->WriteStream(pModuleInfo->strModulePath.c_str(), u1SModilePathLen);
                auto u1SModileParamLen = (uint8)pModuleInfo->strModuleParam.length();
                (*pBuffPacket) << u1SModileParamLen;
                pBuffPacket->WriteStream(pModuleInfo->strModuleParam.c_str(), u1SModileParamLen);
                auto u1SModileDescLen = (uint8)strDesc.length();
                (*pBuffPacket) << u1SModileDescLen;
                pBuffPacket->WriteStream(pModuleInfo->GetDesc(), u1SModileDescLen);

                string strTime = CTimeStamp::Get_DateTime(pModuleInfo->tvCreateTime);
                (*pBuffPacket) << strTime;

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
                uint32 u4ErrorID = 0;
                pModuleInfo->GetModuleState(u4ErrorID);

                std::stringstream ss_format;
                ss_format << "ModuleName(" << pModuleInfo->GetName() << ")\n"
                    << "ModuleFileName(" << pModuleInfo->strModuleName << ")\n"
                    << "ModuleFilePath(" << pModuleInfo->strModulePath << ")\n"
                    << "ModuleParam(" << pModuleInfo->strModuleParam << ")\n"
                    << "ModuleDesc(" << pModuleInfo->GetDesc() << ")\n"
                    << "CreeateTime(" << CTimeStamp::Get_DateTime(pModuleInfo->tvCreateTime)
                    << ")\n"
                    << "ModuleState Run u4ErrorID(" << u4ErrorID << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }

        u2ReturnCommandID = CONSOLE_COMMAND_SHOWMOUDLE;
    }
    else
    {
        PSS_LOGGER_DEBUG("[DoMessage_ShowModule]Param is error.");
    }
}

void DoMessage_CommandInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    auto u2CommandID = (uint16)ACE_OS::strtol(CommandInfo.m_strCommandExp.c_str(), nullptr, 16);

    if (u2CommandID != 0)
    {
        _CommandData objCommandData;
        _CommandData objCommandDataIn;

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
                std::stringstream ss_format;
                ss_format << "CommandID is Find\n"
                    << "CommandID(" << u2CommandID << ")\n"
                    << "CommandCount(" << objCommandData.m_u4CommandCount << ")\n"
                    << "CommandCost(" << objCommandData.m_u8CommandCost << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
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
                std::stringstream ss_format;
                ss_format << "CommandID is no Find.\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }

        u2ReturnCommandID = CONSOLE_COMMAND_COMMANDINFO;
    }
}

void DoMessage_WorkThreadState(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp != "-s")
    {
        return;
    }

    //获得当前工作线程状态
    vector<_ThreadInfo> vecWorkThreadList;
    App_MessageServiceGroup::instance()->GetThreadInfo(vecWorkThreadList);

    (*pBuffPacket) << (uint8)vecWorkThreadList.size();

    for (const auto& pCurrThreadInfo : vecWorkThreadList)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)pCurrThreadInfo.m_u4ThreadID;
            (*pBuffPacket) << (uint32)CTimeStamp::Get_Time_use_second(pCurrThreadInfo.m_tvUpdateTime);
            (*pBuffPacket) << (uint32)CTimeStamp::Get_Time_use_second(pCurrThreadInfo.m_tvCreateTime);
            (*pBuffPacket) << (uint8)pCurrThreadInfo.m_u4State;
            (*pBuffPacket) << pCurrThreadInfo.m_u4RecvPacketCount;
            (*pBuffPacket) << pCurrThreadInfo.m_u2CommandID;
            (*pBuffPacket) << pCurrThreadInfo.m_u2PacketTime;
            (*pBuffPacket) << pCurrThreadInfo.m_u4CurrPacketCount;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "ThreadID(" << pCurrThreadInfo.m_u4ThreadID << ")\n"
                << "ThreadUpdateTime(" << CTimeStamp::Get_Time_use_second(pCurrThreadInfo.m_tvUpdateTime) << ")\n"
                << "ThreadCreateTime(" << CTimeStamp::Get_Time_use_second(pCurrThreadInfo.m_tvCreateTime) << ")\n"
                << "ThreadState(" << (int)pCurrThreadInfo.m_u4State << ")\n"
                << "ThreadRecvPacketCount(" << pCurrThreadInfo.m_u4RecvPacketCount << ")\n"
                << "ThreadCommandID(" << pCurrThreadInfo.m_u2CommandID << ")\n"
                << "ThreadPacketTime(" << pCurrThreadInfo.m_u2PacketTime << ")\n"
                << "ThreadCurrPacketCount(" << pCurrThreadInfo.m_u4CurrPacketCount << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_THREADINFO;
}

void DoMessage_ClientInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        vecClientConnectInfo VecClientConnectInfo;

        //待实现

        auto u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "Client IP Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTINFO;
}

void DoMessage_CloseClient(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    auto u4ConnectID = (uint32)ACE_OS::atoi(CommandInfo.m_strCommandExp.c_str());

    App_HandlerManager::instance()->CloseConnect(u4ConnectID);

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint8)0;
    }
    else
    {
        std::stringstream ss_format;
        ss_format << "Client Close is OK";
        string strLineText = ss_format.str();
        pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
    }

    u2ReturnCommandID = CONSOLE_COMMAND_COLSECLIENT;
}

void DoMessage_ForbiddenIP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    _ForbiddenIP ForbiddenIP;

    if (GetForbiddenIP(CommandInfo.m_strCommandExp, ForbiddenIP) == true)
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

void DoMessage_ShowForbiddenList(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        const VecForbiddenIP* pForeverForbiddenIP = App_ForbiddenIP::instance()->ShowForeverIP();
        const VecForbiddenIP* pTempForbiddenIP = App_ForbiddenIP::instance()->ShowTempIP();

        if (pForeverForbiddenIP == nullptr || pTempForbiddenIP == nullptr)
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

void DoMessage_LifedIP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    App_ForbiddenIP::instance()->DelForeverIP(CommandInfo.m_strCommandExp.c_str());
    App_ForbiddenIP::instance()->DelTempIP(CommandInfo.m_strCommandExp.c_str());

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint8)0;
    }
    else
    {
        std::stringstream ss_format;
        ss_format << "IP Forbidden cancel OK";
        string strLineText = ss_format.str();
        pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
    }

    u2ReturnCommandID = CONSOLE_COMMAND_LIFTED;
}

void DoMessage_UDPClientInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        vecClientConnectInfo VecClientConnectInfo;
        App_UDPConnectIDManager::instance()->GetClientConnectInfo(VecClientConnectInfo);

        auto u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "UDPClient Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_UDPCONNECTINFO;
}

void DoMessage_ServerConnectTCP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        vecClientConnectInfo VecClientConnectInfo;
#if PSS_PLATFORM == PLATFORM_WIN
        App_ClientProConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
#else
        App_ClientReConnectManager::instance()->GetConnectInfo(VecClientConnectInfo);
#endif

        auto u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "ServerConnect Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERCONNECT_TCP;
}

void DoMessage_ServerConnectUDP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        vecClientConnectInfo VecClientConnectInfo;
#if PSS_PLATFORM == PLATFORM_WIN
        App_ClientProConnectManager::instance()->GetUDPConnectInfo(VecClientConnectInfo);
#else
        App_ClientReConnectManager::instance()->GetUDPConnectInfo(VecClientConnectInfo);
#endif

        auto u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "ServerConnect Count(%d).\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERCONNECT_UDP;
}

void DoMessage_ShowProcessInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
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
            std::stringstream ss_format;
            ss_format << "CPUUsedRote(" << nCPU << "%%)\n"
                << "MemorySize(" << nMemorySize << ")\n"
                << "FLowSize(" << u1Flow << ")\n"
                << "FLowSize(" << u4FlowIn << ")\n"
                << "FlowOut(" << u4FlowOut << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_PROCESSINFO;
}

void DoMessage_ShowClientHisTory(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        vecIPAccount VecIPAccount;
        App_IPAccount::instance()->GetInfo(VecIPAccount);

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)VecIPAccount.size();
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "ClientHisToryCount(" << VecIPAccount.size() << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (_IPAccount ipaccount : VecIPAccount)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {

                auto u1IPLen = (uint8)ipaccount.m_strIP.length();
                (*pBuffPacket) << u1IPLen;
                pBuffPacket->WriteStream(ipaccount.m_strIP.c_str(), u1IPLen);
                (*pBuffPacket) << (uint32)ipaccount.m_nCount;
                (*pBuffPacket) << (uint32)ipaccount.m_nAllCount;

                std::stringstream ss_format;
                ss_format << ipaccount.m_dtLastTime.year()
                    << "-" << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.month()
                    << "-" << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.day()
                    << " " << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.hour()
                    << ":" << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.minute()
                    << ":" << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.second();
                string strTime = ss_format.str();
                (*pBuffPacket) << strTime;
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "IP(" << ipaccount.m_strIP << ")\n"
                    << "Count(" << ipaccount.m_nCount << ")\n"
                    << "AllCount(" << ipaccount.m_nAllCount << ")\n"
                    << "Time(" << ipaccount.m_dtLastTime.year() 
                    << "-" << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.month()
                    << "-" << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.day()
                    << " " << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.hour()
                    << ":" << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.minute()
                    << ":" << std::setfill('0') << std::setw(2) << ipaccount.m_dtLastTime.second()
                    << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_CLIENTHISTORY;
}

void DoMessage_ShowAllCommandInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp != "-a")
    {
        return;
    }

    hashmapModuleClientList pHashModuleClient = App_MessageManager::instance()->GetModuleClient();

    //统计个数
    uint32 u4Count = 0;
    vector<shared_ptr<_ModuleClient>> vecModuleClient;

    for_each(pHashModuleClient.begin(), pHashModuleClient.end(), [&vecModuleClient](const std::pair<string, shared_ptr<_ModuleClient>>& iter) {
        vecModuleClient.emplace_back(iter.second);
        });

    u4Count = (uint32)vecModuleClient.size();

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << u4Count;
    }
    else
    {
        std::stringstream ss_format;
        ss_format << "CommandInfoCount(" << u4Count << ")\n";
        string strLineText = ss_format.str();
        pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
    }

    for (auto pModuleClient : vecModuleClient)
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)pModuleClient->m_vecClientCommandInfo.size();
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "ModuleClientCount(" << pModuleClient->m_vecClientCommandInfo.size() << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (shared_ptr<_ClientCommandInfo> pClientCommandInfo : pModuleClient->m_vecClientCommandInfo)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << pClientCommandInfo->m_strModuleName;
                (*pBuffPacket) << pClientCommandInfo->m_u2CommandID;
                (*pBuffPacket) << pClientCommandInfo->m_u4Count;
                (*pBuffPacket) << pClientCommandInfo->m_u4TimeCost;
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "ModuleName(" << pClientCommandInfo->m_strModuleName << ")\n"
                    << "CommandID(" << pClientCommandInfo->m_u2CommandID << ")\n"
                    << "Count(" << pClientCommandInfo->m_u4Count << ")\n"
                    << "TimeCost(" << pClientCommandInfo->m_u4TimeCost << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_ALLCOMMANDINFO;
}

void DoMessage_ShowServerInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            //返回服务器ID
            auto u2SerevrID = (uint16)GetXmlConfigAttribute(xmlServerID)->id;
            (*pBuffPacket) << u2SerevrID;

            //返回服务器名称
            auto u1ServerNameLen = (uint8)GetXmlConfigAttribute(xmlServerName)->name.length();
            (*pBuffPacket) << u1ServerNameLen;
            pBuffPacket->WriteStream(GetXmlConfigAttribute(xmlServerName)->name.c_str(), u1ServerNameLen);

            //返回服务器版本
            auto u1ServerVersionLen = (uint8)GetXmlConfigAttribute(xmlServerVersion)->Version.length();
            (*pBuffPacket) << u1ServerVersionLen;
            pBuffPacket->WriteStream(GetXmlConfigAttribute(xmlServerVersion)->Version.c_str(), u1ServerVersionLen);


            //返回加载模块个数
            (*pBuffPacket) << (uint16)App_ModuleLoader::instance()->GetCurrModuleCount();


            //返回工作线程个数
            (*pBuffPacket) << (uint16)App_MessageServiceGroup::instance()->GetWorkThreadCount();

            //返回当前协议包的版本号
            auto u1PacketVersionLen = (uint8)GetXmlConfigAttribute(xmlServerVersion)->Version.length();
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
            string strCharOrder;

            if (GetXmlConfigAttribute(xmlNetWorkMode)->NetByteOrder == false)
            {
                strCharOrder = "HostOrder";   //主机字序
            }
            else
            {
                strCharOrder = "NetOrder";    //网络字序
            }

            std::stringstream ss_format;
            ss_format << "ServerID(" << GetXmlConfigAttribute(xmlServerID)->id << ")\n"
                << "ServerName(" << GetXmlConfigAttribute(xmlServerName)->name << ")\n"
                << "ServerVersion(" << GetXmlConfigAttribute(xmlServerVersion)->Version << ")\n"
                << "ServerModuleCount(" << App_ModuleLoader::instance()->GetCurrModuleCount() << ")\n"
                << "WorkthreadCount(" << App_MessageServiceGroup::instance()->GetWorkThreadCount() << ")\n"
                << "CharOrder(" << strCharOrder << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERINFO;
}

void DoMessage_ReConnectServer(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    int nSerevrID = 0;

    if (GetConnectServerID(CommandInfo.m_strCommandExp, nSerevrID) == true)
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

        auto u4ConnectCount = (uint32)VecClientConnectInfo.size();
        Combo_Common_Head_Data(CommandInfo.m_u1OutputType, u4ConnectCount, "ConnectServerCount(%d)\n", pBuffPacket);

        Combo_Common_vecClientConnectInfo(CommandInfo.m_u1OutputType, VecClientConnectInfo, pBuffPacket);
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SERVERRECONNECT;
}

void DoMessage_CommandTimeout(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        uint32 u4Count = 0;

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u4Count;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "CommandTimeoutCount(" << u4Count << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_COMMANDTIMEOUT;
}

void DoMessage_CommandTimeoutclr(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            string strLineText = "State(OK)\n";
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_COMMANDTIMEOUTCLR;
}

void DoMessage_CommandDataLog(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        //不实现

        if (CommandInfo.m_u1OutputType == 0)
        {
            //存储所有发送统计日志
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            string strLineText = "State(OK)\n";
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_COMMANDDATALOG;
}

void DoMessage_SetDebug(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    uint8 u1Debug = 0;

    if (GetDebug(CommandInfo.m_strCommandExp, u1Debug) == true)
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
            string strLineText = "State(OK)\n";
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETDEBUG;
}

void DoMessage_ShowDebug(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << GetXmlConfigAttribute(xmlAceDebug)->TrunOn;
            (*pBuffPacket) << (uint8)0;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "DebugState("<< GetXmlConfigAttribute(xmlAceDebug)->TrunOn << ")\n"
                << "State(OK)\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SHOWDEBUG;
}

void DoMessage_SetTrackIP(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    _DyeIPInfo objDyeIPInfo;

    if (true == GetDyeingIP(CommandInfo.m_strCommandExp, objDyeIPInfo))
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

void DoMessage_SetTraceCommand(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    _DyeCommandInfo objDyeCommandInfo;

    if (true == GetDyeingCommand(CommandInfo.m_strCommandExp, objDyeCommandInfo))
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

void DoMessage_GetTrackCommand(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp != "-a")
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
        std::stringstream ss_format;
        ss_format << "CommandID Count(" << objList.size() << ")\n";
        string strLineText = ss_format.str();
        pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
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
            std::stringstream ss_format;
            ss_format << "CommandID ID(" << dyeingcommand.m_u2CommandID << ")\n"
                << "CurrCount ID(" << dyeingcommand.m_u2CurrCount << ")\n"
                << "MaxCount ID(" << dyeingcommand.m_u2MaxCount << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETTRACKCOMMAND;
}

void DoMessage_GetConnectIPInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    int nConnectID = 0;

    if (GetConnectServerID(CommandInfo.m_strCommandExp, nConnectID) == true)
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
                string strLineText = "IP not find\n";
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
        else
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                //找到了对应的IP信息
                (*pBuffPacket) << (uint16)0;

                (*pBuffPacket) << objClientIPInfo.m_strClientIP;                          //IP
                (*pBuffPacket) << (uint32)objClientIPInfo.m_u2Port;  //端口
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "ClientIP(" << objClientIPInfo.m_strClientIP << ")\n"
                    << "Port(" << objClientIPInfo.m_u2Port << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETCONNECTIPINFO;
}

void DoMessage_GetLogLevelInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << AppLogManager::instance()->GetLogCount();
            (*pBuffPacket) << AppLogManager::instance()->GetCurrLevel();
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "LogCount(" << AppLogManager::instance()->GetLogCount() << ")\n"
                << "LogLevel(" << AppLogManager::instance()->GetCurrLevel() << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (uint16 i = 0; i < (uint16)AppLogManager::instance()->GetLogCount(); i++)
        {
            uint16 u2LogID = AppLogManager::instance()->GetLogID(i);

            auto pServerName = AppLogManager::instance()->GetLogInfoByServerName(u2LogID);

            if (nullptr == pServerName)
            {
                //如果服务器名称为空则直接返回
                continue;
            }

            string strServerName = pServerName;
            auto u1ServerNameLen = (uint8)strServerName.length();

            auto pLogName = AppLogManager::instance()->GetLogInfoByLogName(u2LogID);

            if (nullptr == pLogName)
            {
                //如果服务器名称为空则直接返回
                return;
            }

            string strLogName = pLogName;
            auto u1LogNameLen = (uint8)strLogName.length();

            auto u1LogType = (uint8)AppLogManager::instance()->GetLogInfoByLogDisplay(u2LogID);

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
                std::stringstream ss_format;
                ss_format << "m_u1OutputType(" << u2LogID << ")\n"
                    << "pServerName(" << pServerName << ")\n"
                    << "pLogName(" << pLogName << ")\n"
                    << "u1LogType(" << u1LogType << ")\n"
                    << "GetLogInfoByLogLevel(" << AppLogManager::instance()->GetLogInfoByLogLevel(i) << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETLOGINFO;
}

void DoMessage_SetLogLevelInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    int nLogLevel = 1;

    if (GetLogLevel(CommandInfo.m_strCommandExp, nLogLevel) == true)
    {
        AppLogManager::instance()->ResetLogData((uint16)nLogLevel);

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)0;
        }
        else
        {
            string strLineText = "OK.\n";
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETLOGLEVEL;
}

void DoMessage_GetThreadAI(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        vecWorkThreadAIInfo objvecWorkThreadAIInfo;
        App_MessageServiceGroup::instance()->GetWorkThreadAIInfo(objvecWorkThreadAIInfo);

        auto u2ThreadCount = (uint16)objvecWorkThreadAIInfo.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u2ThreadCount;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "u2ThreadCount(" << u2ThreadCount << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
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
                std::stringstream ss_format;
                ss_format << "m_u4ThreadID(" << objvecWorkThreadAIInfo[i].m_u4ThreadID << ")\n"
                    << "m_u1WTAI(" << objvecWorkThreadAIInfo[i].m_u1WTAI << ")\n"
                    << "m_u4WTTimeoutCount(" << objvecWorkThreadAIInfo[i].m_u4WTTimeoutCount << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETWTAI;
}

void DoMessage_GetWorkThreadTO(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp ==  "-a")
    {
        vecCommandTimeout objTimeout;
        vecCommandTimeout objTimeoutF;
        App_MessageServiceGroup::instance()->GetAITO(objTimeout);

        auto u2ThreadCount = (uint16)objTimeout.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u2ThreadCount;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "u2ThreadCount(" << u2ThreadCount << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
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
                std::stringstream ss_format;
                ss_format << "m_u4ThreadID" << objTimeout[i].m_u4ThreadID << ")\n"
                    << "m_u2CommandID(" << objTimeout[i].m_u2CommandID << ")\n"
                    << "m_u4Second(" << (uint32)objTimeout[i].m_u8Second << ")\n"
                    << "m_u4Timeout(" << objTimeout[i].m_u4Timeout << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
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
            std::stringstream ss_format;
            ss_format << "u2ThreadCount(" << u2ThreadCount << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
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
                std::stringstream ss_format;
                ss_format << "m_u4ThreadID(" << objTimeoutF[i].m_u4ThreadID << ")\n"
                    << "m_u2CommandID=" << objTimeoutF[i].m_u2CommandID << ")\n"
                    << "m_u4Second=" << (uint32)objTimeoutF[i].m_u8Second << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETWTTIMEOUT;
}

void DoMessage_SetWorkThreadAI(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    int nAI = 0;
    int nDispose = 0;
    int nCheck = 0;
    int nStop = 0;

    if (GetAIInfo(CommandInfo.m_strCommandExp, nAI, nDispose, nCheck, nStop) == true
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
        string strLineText = "OK.\n";
        pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETWTAI;
}

void DoMessage_GetNickNameInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    string strNickName;
    vecClientNameInfo objClientNameInfo;

    if (GetNickName(CommandInfo.m_strCommandExp, strNickName) == true)
    {
        //暂时不实现

        //返回信息列表
        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)objClientNameInfo.size();
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "objClientNameInfo size(" << objClientNameInfo.size() << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (_ClientNameInfo clinetnameinfo : objClientNameInfo)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                
                (*pBuffPacket) << clinetnameinfo.m_strName;
                (*pBuffPacket) << (uint32)clinetnameinfo.m_nConnectID;
                (*pBuffPacket) << clinetnameinfo.m_strClientIP;
                (*pBuffPacket) << (uint32)clinetnameinfo.m_u2Port;
                (*pBuffPacket) << clinetnameinfo.m_strName;
                (*pBuffPacket) << (uint8)clinetnameinfo.m_nLog;
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "m_nConnectID(" << clinetnameinfo.m_nConnectID << ")\n"
                    << "strIP(" << clinetnameinfo.m_strClientIP << ")\n"
                    << "m_nPort(" << clinetnameinfo.m_u2Port << ")\n"
                    << "strName(" << clinetnameinfo.m_strName << ")\n"
                    << "m_nLog=%(" << clinetnameinfo.m_nLog << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_GETNICKNAMEINFO;
}

void DoMessage_SetConnectLog(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    uint32 u4ConnectID = 0;
    bool blIsLog = false;

    if (GetConnectID(CommandInfo.m_strCommandExp, u4ConnectID, blIsLog) == true)
    {
        App_HandlerManager::instance()->SetIsLog(u4ConnectID, blIsLog);
    }

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint32)0;
    }
    else
    {
        string strLineText = "OK.\n";
        pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETCONNECTLOG;
}

void DoMessage_SetMaxConnectCount(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    uint16 u2MaxConnectHandler = 0;

    if (GetMaxConnectCount(CommandInfo.m_strCommandExp, u2MaxConnectHandler) == true && u2MaxConnectHandler > 0)
    {
        GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount = u2MaxConnectHandler;
    }

    if (CommandInfo.m_u1OutputType == 0)
    {
        (*pBuffPacket) << (uint32)0;
    }
    else
    {
        string strLineText = "OK.\n";
        pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SETMAXCONNECTCOUNT;
}

void DoMessage_AddListen(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    bool blState = false;
    _ListenInfo objListenInfo;

    if (GetListenInfo(CommandInfo.m_strCommandExp, objListenInfo) == true)
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

void DoMessage_DelListen(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    bool blState = false;
    _ListenInfo objListenInfo;

    if (GetListenInfo(CommandInfo.m_strCommandExp, objListenInfo) == true)
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

void DoMessage_ShowListen(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    uint32 u4ListenCount = 0;

    if (CommandInfo.m_strCommandExp == "-a")
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
            std::stringstream ss_format;
            ss_format << "objControlInfo(" << u4ListenCount << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (uint32 i = 0; i < u4ListenCount; i++)
        {
            _ControlInfo obj_ControlInfo;
#if PSS_PLATFORM == PLATFORM_WIN
            App_ProControlListen::instance()->ShowListen(i, obj_ControlInfo);
#else
            App_ControlListen::instance()->ShowListen(i, obj_ControlInfo);
#endif
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << obj_ControlInfo.m_strListenIP;
                (*pBuffPacket) << obj_ControlInfo.m_u4Port;
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "strIP(" << obj_ControlInfo.m_strListenIP << ")\n"
                    << "m_u4Port(" << obj_ControlInfo.m_u4Port << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_SHOW_LISTEN;
}

void DoMessage_MonitorInfo(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
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
            std::stringstream ss_format;
            ss_format << "nActiveClient(" << nActiveClient << ")\n"
                << "nPoolClient(" << nPoolClient << ")\n"
                << "MaxHandlerCoun(" << GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount << ")\n"
                << "CurrConnect(" << App_ConnectAccount::instance()->GetCurrConnect() << ")\n"
                << "u4FlowIn(" << u4FlowIn << ")\n"
                << "u4FlowOut(" << u4FlowIn << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }

    u2ReturnCommandID = CONSOLE_COMMAND_MONITOR_INFO;
}

void DoMessage_ServerClose(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (nullptr == pBuffPacket)
    {
        return;
    }

    //这里暂不实现，没必要
    ACE_UNUSED_ARG(CommandInfo);
    (*pBuffPacket) << u2ReturnCommandID;
    u2ReturnCommandID = CONSOLE_COMMAND_COLSECLIENT;
}

void DoMessage_TestFileStart(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    string strFileName;

    if (GetTestFileName(CommandInfo.m_strCommandExp, strFileName) == true)
    {
        PSS_LOGGER_DEBUG("[Do_Message_TestFileStart]file={0}.", strFileName);
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
            std::stringstream ss_format;
            ss_format << "n4Result(" << (int)objFileResult.n4Result << ")\n"
                << "n4TimeInterval(" << objFileResult.n4TimeInterval << ")\n"
                << "n4ProNum(" << objFileResult.n4ProNum << ")\n"
                << "vecProFileDesc(" << objFileResult.vecProFileDesc.size() << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (uint16 i = 0; i < (uint16)objFileResult.vecProFileDesc.size(); i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << objFileResult.vecProFileDesc[i];
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "vecProFileDesc(" << objFileResult.vecProFileDesc[i].c_str() << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
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
            string strLineText = "Fail.\n";
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }
}

void DoMessage_TestFileStop(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        u2ReturnCommandID = CONSOLE_COMMAND_FILE_TEST_STOP;
        int nRet = App_FileTestManager::instance()->FileTestEnd();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << (uint32)nRet;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "Ret(" << nRet << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }
    }
}

void DoMessage_PortList(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    PSS_LOGGER_DEBUG("[DoMessage_PortList]In.");

    if (CommandInfo.m_strCommandExp == "-a")
    {
        PSS_LOGGER_DEBUG("[DoMessage_PortList]In 1.");
        u2ReturnCommandID = CONSOLE_COMMAND_PORT_FLOW;

        vector<CWorkThread_Packet_Info> vec_Port_Data_Account;
        App_MessageServiceGroup::instance()->GetFlowPortList(vec_Port_Data_Account);

        auto u4Count = (uint32)vec_Port_Data_Account.size();

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u4Count;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "u4Count(" << u4Count << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (uint32 i = 0; i < u4Count; i++)
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u1ThreadID;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u1Minute;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u4PacketIn;
                (*pBuffPacket) << vec_Port_Data_Account[i].m_u4PacketOut;
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "m_u1Type(" << vec_Port_Data_Account[i].m_u1ThreadID << ")\n"
                    << "m_u4Port(" << vec_Port_Data_Account[i].m_u1Minute << ")\n"
                    << "m_u4FlowIn(" << vec_Port_Data_Account[i].m_u4PacketIn << ")\n"
                    << "m_u4FlowOut(" << vec_Port_Data_Account[i].m_u4PacketOut << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }
}

void Do_Message_BuffPacket(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    if (CommandInfo.m_strCommandExp == "-a")
    {
        u2ReturnCommandID = CONSOLE_COMMAND_PACKET_STATE;

        uint32 u4Count = 0;

        if (CommandInfo.m_u1OutputType == 0)
        {
            (*pBuffPacket) << u4Count;
        }
        else
        {
            std::stringstream ss_format;
            ss_format << "u4Count(" << u4Count << ")\n";
            string strLineText = ss_format.str();
            pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
        }

        for (uint32 i = 0; i < u4Count; i++)
        {
            _Object_Create_Info objCreateInfo;

            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << objCreateInfo.m_strCreateFileName;
                (*pBuffPacket) << objCreateInfo.m_u4Line;
                (*pBuffPacket) << objCreateInfo.m_u4Count;
            }
            else
            {
                std::stringstream ss_format;
                ss_format << "strFileName(" << objCreateInfo.m_strCreateFileName << ")\n"
                    << "m_u4Line(" << objCreateInfo.m_u4Line << ")\n"
                    << "m_u4Count(" << objCreateInfo.m_u4Count << ")\n";
                string strLineText = ss_format.str();
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }
}

void Do_Message_PoolSet(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID)
{
    _PoolName objPoolName;

    if (GetPoolSet(CommandInfo.m_strCommandExp, objPoolName) == true)
    {
        u2ReturnCommandID = CONSOLE_COMMAND_POOL_SET;

        if (objPoolName.m_strPoolName == "BuffPacket")
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint8)0;
            }
            else
            {
                string strLineText = "Set ok.\n";
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
        else if (objPoolName.m_strPoolName == "PacketParse")
        {
            if (CommandInfo.m_u1OutputType == 0)
            {
                (*pBuffPacket) << (uint8)0;
            }
            else
            {
                string strLineText = "Set ok.\n";
                pBuffPacket->WriteStream(strLineText.c_str(), (uint32)strLineText.length());
            }
        }
    }
}
