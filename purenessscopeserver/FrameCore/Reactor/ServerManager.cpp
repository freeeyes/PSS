#include "ServerManager.h"

#ifdef _CPPUNIT_TEST
#include "CppUnitMain.h"
#endif

bool CServerManager::Init()
{
    auto nServerPortCount = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();
    int nReactorCount = 3 + GetXmlConfigAttribute(xmlMessage)->Msg_Thread;

    //��ʼ��Console��ز���
    App_ConsoleManager::instance()->Init();

    //��ʼ��ģ��������ز���
    App_MessageManager::instance()->Init(GetXmlConfigAttribute(xmlModuleMangager)->MaxCount,
                                         GetXmlConfigAttribute(xmlCommandAccount)->MaxCommandCount);

    //��ʼ������ģ�����Ϣ
    App_ModuleLoader::instance()->Init(GetXmlConfigAttribute(xmlModuleMangager)->MaxCount);

    //��ʼ����ֹIP�б�
    App_ForbiddenIP::instance()->Init(FORBIDDENIP_FILE);

    PSS_LOGGER_DEBUG("[CServerManager::Init]nReactorCount={0}.", nReactorCount);

    //Ϊ�������׼�������epoll��epollet��ʼ������������ȥ��,��Ϊ�ڶ������epoll_create�������ӽ�����ȥ����
    if (NETWORKMODE::NETWORKMODE_RE_EPOLL != GetXmlConfigAttribute(xmlNetWorkMode)->Mode
        && NETWORKMODE::NETWORKMODE_RE_EPOLL_ET != GetXmlConfigAttribute(xmlNetWorkMode)->Mode)
    {
        //��ʼ����Ӧ������
        App_ReactorManager::instance()->Init((uint16)nReactorCount);

        Init_Reactor((uint8)nReactorCount, GetXmlConfigAttribute(xmlNetWorkMode)->Mode);
    }

    //��ʼ����־ϵͳ�߳�
    if (false == Server_Manager_Common_LogSystem())
    {
        return false;
    }

    //��ʼ�����ֶ����
    Server_Manager_Common_Pool();

    //��ʼ��ConnectHandler�����
    if (GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount <= 0)
    {
        //��ʼ��PacketParse�����
        App_ConnectHandlerPool::instance()->Init(MAX_HANDLE_POOL);
    }
    else
    {
        //��ʼ��PacketParse�����
        App_ConnectHandlerPool::instance()->Init(GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
    }

    //��ʼ��������Ķ���ӿ�
    auto pConnectManager   = dynamic_cast<IConnectManager*>(App_HandlerManager::instance());
    auto pClientManager    = dynamic_cast<IClientManager*>(App_ClientReConnectManager::instance());
    auto pFrameCommand     = dynamic_cast<IFrameCommand*>(&m_objFrameCommand);
    auto pServerManager    = dynamic_cast<IServerManager*>(this);
    auto pTTyClientManager = dynamic_cast<ITTyClientManager*>(App_ReTTyClientManager::instance());
    auto pControlListen    = dynamic_cast<IControlListen*>(App_ControlListen::instance());

    Server_Manager_Common_IObject(pConnectManager,
                                  pClientManager,
                                  pFrameCommand,
                                  pServerManager,
                                  pTTyClientManager,
                                  pControlListen);

    //��ʼ��ģ����أ���Ϊ������ܰ������м���������Ӽ���
    if (false == Server_Manager_Common_Module())
    {
        return false;
    }

    //��ʼ��ת���б�
    App_ForwardManager::instance()->Init();


    //�����е��߳̿�ͬ������
    App_MessageServiceGroup::instance()->CopyMessageManagerList();

    //��ʼ��������
    uint32 u4ClientReactorCount = (uint32)nReactorCount - 3;

    if (!App_ConnectAcceptorManager::instance()->InitConnectAcceptor(nServerPortCount, u4ClientReactorCount))
    {
        PSS_LOGGER_DEBUG("[CServerManager::Init]{0}.", App_ConnectAcceptorManager::instance()->GetError());
        return false;
    }

    return true;
}

bool CServerManager::Start()
{
    //����TCP������ʼ��
    bool blRet = false;
    blRet = Start_Tcp_Listen();

    if (false == blRet)
    {
        return blRet;
    }

    //������̨����˿ڼ���
    blRet = Start_Console_Tcp_Listen();

    if (false == blRet)
    {
        return blRet;
    }

    if (GetXmlConfigAttribute(xmlMessage)->Msg_Process > 1)
    {
        Multiple_Process_Start();
    }
    else
    {
        if (false == Run())
        {
            PSS_LOGGER_DEBUG("child Run failure.");
            return false;
        }
    }

    return true;
}

bool CServerManager::Init_Reactor(uint8 u1ReactorCount, NETWORKMODE u1NetMode) const
{
    bool blState = true;

    //��ʼ����Ӧ��
    for (uint8 i = 0; i < u1ReactorCount; i++)
    {
        PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]... i=[{0}].", i);

        if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_SELECT)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_Select, 1);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_Select.");
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_TPSELECT)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_TP, 1);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_TP.");
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_EPOLL)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_DEV_POLL, 1, GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL.");
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_EPOLL_ET)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_DEV_POLL_ET, 1, GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL_ET.");
        }
        else
        {
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewProactor NETWORKMODE Error.");
            return false;
        }

        if (!blState)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]AddNewReactor [{0}] Error.", i);
            PSS_LOGGER_DEBUG("[CServerManager::Init_Reactor]Error={0}.", App_ReactorManager::instance()->GetError());
            return false;
        }
    }

    return blState;
}

bool CServerManager::Run()
{
    //������ʱ��
    App_TimerManager::instance()->Start();

    //��Ӧ����̣�epoll�������ӽ�������г�ʼ��
    if (NETWORKMODE::NETWORKMODE_RE_EPOLL == GetXmlConfigAttribute(xmlNetWorkMode)->Mode
        || NETWORKMODE::NETWORKMODE_RE_EPOLL_ET == GetXmlConfigAttribute(xmlNetWorkMode)->Mode)
    {
        //��ʼ����Ӧ������
        App_ReactorManager::instance()->Init((uint16)(3 + GetXmlConfigAttribute(xmlMessage)->Msg_Thread));

        if (false == Init_Reactor((uint8)(3 + GetXmlConfigAttribute(xmlMessage)->Msg_Thread), GetXmlConfigAttribute(xmlNetWorkMode)->Mode))
        {
            PSS_LOGGER_DEBUG("[CServerManager::Run]Init_Reactor Error.");
            return false;
        }
    }

	//����UDP����
	bool blRet = Start_Udp_Listen();

	if (false == blRet)
	{
		return blRet;
	}

    //��ʼ��TTy���ӹ�����
    App_ReTTyClientManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE),
            GetXmlConfigAttribute(xmlTTyClientManagerInfo)->MaxTTyDevCount,
            GetXmlConfigAttribute(xmlTTyClientManagerInfo)->TimeCheck);

    auto u2ServerPortCount = (uint16)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();

    //����������TCP��Ӧ��
    for (uint16 i = 0; i < u2ServerPortCount; i++)
    {
        //�õ�������
        auto pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

        //�򿪼�����Ӧ�¼�
        pConnectAcceptor->Run_Open(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));
    }

    m_ConnectConsoleAcceptor.Run_Open(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));

    //������־�����߳�
    if (0 != AppLogManager::instance()->Start())
    {
        AppLogManager::instance()->WriteLog_r(LOG_SYSTEM, "[CServerManager::Init]AppLogManager is [error].");
    }
    else
    {
        AppLogManager::instance()->WriteLog_r(LOG_SYSTEM, "[CServerManager::Init]AppLogManager is [ok].");
    }

    //���������������߳�
    if (false == App_ClientReConnectManager::instance()->StartConnectTask(GetXmlConfigAttribute(xmlConnectServer)->TimeCheck))
    {
        PSS_LOGGER_DEBUG("[CServerManager::Run]StartConnectTask error.");
        return false;
    }

    //�������з�Ӧ��(���ǿͻ��˽��յķ�Ӧ���������ﲻ������)
    if (!App_ReactorManager::instance()->StartOtherReactor())
    {
        PSS_LOGGER_DEBUG("[CServerManager::Run]App_ReactorManager::instance()->StartOtherReactor is error.");
        return false;
    }

	//�����м���������ӹ�����
	App_ClientReConnectManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE));

    //�������еĲ����ʼ������
    if (false == App_ModuleLoader::instance()->InitModule())
    {
        PSS_LOGGER_DEBUG("[CServerManager::Run]App_ModuleLoader::instance()->InitModule() is error.");
        return false;
    }

    //��ʼ��Ϣ�����߳�
    App_MessageServiceGroup::instance()->Start();

    //��ʼ����tty��ؼ���
    auto nReTTyCount = (int)GetXmlConfigAttribute(xmlTTyDrives)->vec.size();

    for (int i = 0; i < nReTTyCount; i++)
    {
        _TTyDevParam objReCom;

        objReCom.m_nBaudRate = (int)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4Baud;
        objReCom.m_uDatabits = (unsigned char)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4DataBits;
        objReCom.m_uStopbits = (unsigned char)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4StopBits;
        objReCom.m_pParitymode = GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strParity.c_str();
        strcpy_safe(GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strPortName.c_str(), objReCom.m_szDevName, 32);

        //��Ӽ���
        App_ReTTyClientManager::instance()->ConnectFrame((uint16)GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4TTyID,
                GetXmlConfigAttribute(xmlTTyDrives)->vec[i].strPortName.c_str(),
                objReCom,
                GetXmlConfigAttribute(xmlTTyDrives)->vec[i].u4PacketParseID);

    }

    //������������ͨѶ���������Ϣ
    auto nS2SCount = (int)GetXmlConfigAttribute(xmlServer2Server)->vec.size();

    for (int i = 0; i < nS2SCount; i++)
    {
        uint8 u1IPType = TYPE_IPV4;
        Check_IPType(GetXmlConfigAttribute(xmlServer2Server)->vec[i].strServerIP.c_str(), u1IPType);

        //����������2����������
        App_ClientReConnectManager::instance()->ConnectFrame((uint16)GetXmlConfigAttribute(xmlServer2Server)->vec[i].u4ServerID,
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].strServerIP.c_str(),
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].u2ServerPort,
                u1IPType,
                GetXmlConfigAttribute(xmlServer2Server)->vec[i].u4PacketParseID);
    }

    //���բ�������ͻ��˷�Ӧ�����ÿͻ������ݽ���
    if (!App_ReactorManager::instance()->StartClientReactor())
    {
        PSS_LOGGER_DEBUG("[CServerManager::Run]App_ReactorManager::instance()->StartClientReactor is error.");
        return false;
    }

#ifdef _CPPUNIT_TEST
    //����CppUnit�Զ�������
    PSS_LOGGER_DEBUG("[CppUnit]********************************");
    CCppUnitMain objCppUnitMain;
    objCppUnitMain.Run();
    PSS_LOGGER_DEBUG("[CppUnit]********************************");
#endif

    ACE_Thread_Manager::instance()->wait();

    return true;
}

bool CServerManager::Start_Tcp_Listen() const
{
    auto nServerPortCount = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();

    for (int i = 0; i < nServerPortCount; i++)
    {
        ACE_INET_Addr listenAddr;

        if (false == Server_Manager_Common_Addr(GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].ipType,
                                                GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].ip.c_str(),
                                                GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].port,
                                                listenAddr))
        {
            return false;
        }

        //�õ�������
        auto pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

        if (nullptr == pConnectAcceptor)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start]pConnectAcceptor[{0}] is nullptr.", i);
            return false;
        }

        pConnectAcceptor->SetPacketParseInfoID(GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].packetparseid);
        int nRet = pConnectAcceptor->Init_Open(listenAddr, 0, 1, 1, (int)GetXmlConfigAttribute(xmlNetWorkMode)->BackLog);

        if (-1 == nRet)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start] pConnectAcceptor->open[{0}] is error.", i);
            PSS_LOGGER_DEBUG("[CServerManager::Start] Listen from [{0}:{1}] error({1}).", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno);
            return false;
        }

        PSS_LOGGER_DEBUG("[CServerManager::Start] Listen from [{0}:{1}] OK.", listenAddr.get_host_addr(), listenAddr.get_port_number());
    }

    return true;
}

bool CServerManager::Start_Udp_Listen()
{
    auto nUDPServerPortCount = (int)GetXmlConfigAttribute(xmlUDPServerIPs)->vec.size();

    for (int i = 0; i < nUDPServerPortCount; i++)
    {
        ACE_INET_Addr listenAddr;

        if (false == Server_Manager_Common_Addr(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uipType,
                                                GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uip.c_str(),
                                                GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uport,
                                                listenAddr))
        {
            return false;
        }

        //�õ�������
        auto pReactorUDPHandler = std::make_shared<CReactorUDPHander>();

        pReactorUDPHandler->SetPacketParseInfoID(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uPacketParseID);
        int nRet = pReactorUDPHandler->OpenAddress(listenAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));

        if (-1 == nRet)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start] UDP Listen from [{0}:{1}] error({2}).", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno);
            return false;
        }

        m_vecUDPList.emplace_back(pReactorUDPHandler);

        PSS_LOGGER_DEBUG("[CServerManager::Start] UDP Listen from [{0}:{1}] OK.", listenAddr.get_host_addr(), listenAddr.get_port_number());
    }

    return true;
}

bool CServerManager::Start_Console_Tcp_Listen()
{
    if (GetXmlConfigAttribute(xmlConsole)->support == CONSOLE_ENABLE)
    {
        ACE_INET_Addr listenConsoleAddr;
        int nErr = 0;

        if (GetXmlConfigAttribute(xmlConsole)->ipType == TYPE_IPV4)
        {
            if (ACE_OS::strcmp(GetXmlConfigAttribute(xmlConsole)->sip.c_str(), "INADDR_ANY") == 0)
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport, PSS_INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             GetXmlConfigAttribute(xmlConsole)->sip.c_str());
            }
        }
        else
        {
            if (ACE_OS::strcmp(GetXmlConfigAttribute(xmlConsole)->sip.c_str(), "INADDR_ANY") == 0)
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport, PSS_INADDR_ANY);
            }
            else
            {
                nErr = listenConsoleAddr.set(GetXmlConfigAttribute(xmlConsole)->sport,
                                             GetXmlConfigAttribute(xmlConsole)->sip.c_str(), 1, PF_INET6);
            }
        }

        if (nErr != 0)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start]listenConsoleAddr set_address error[{0}].", errno);
            return false;
        }

        int nRet = m_ConnectConsoleAcceptor.Init_Open(listenConsoleAddr);

        if (-1 == nRet)
        {
            PSS_LOGGER_DEBUG("[CServerManager::Start] pConnectAcceptor->open is error.");
            PSS_LOGGER_DEBUG("[CServerManager::Start] Listen from [{0}:{1}] error({2}).", listenConsoleAddr.get_host_addr(), listenConsoleAddr.get_port_number(), errno);
            return false;
        }
    }

    return true;
}

void CServerManager::Multiple_Process_Start()
{
#ifndef WIN32
    //��ǰ������̸߳���
    int nNumChlid = GetXmlConfigAttribute(xmlMessage)->Msg_Process;

    //���ʱ��������
    //�����̼��ʱ����������ÿ��5��һ�Σ�
    ACE_Time_Value tvMonitorSleep(5, 0);

    //�ļ���
    int fd_lock = 0;

    int nRet = 0;

    //��õ�ǰ·��
    char szWorkDir[MAX_BUFF_500] = { 0 };

    if (!ACE_OS::getcwd(szWorkDir, sizeof(szWorkDir)))
    {
        exit(1);
    }

    //��Linux�²��ö���̵ķ�ʽ����
    // �򿪣����������ļ�
    string strFileName = fmt::format("{0}/lockwatch.lk", szWorkDir);
    //�ļ���Ȩ��Ϊ  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
    fd_lock = open(strFileName.c_str(), O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);

    if (fd_lock < 0)
    {
        PSS_LOGGER_DEBUG("open the flock and exit, errno = {0}.", errno);
        exit(1);
    }

    //�鿴��ǰ�ļ����Ƿ�����
    nRet = SeeLock(fd_lock, 0, sizeof(int));

    if (nRet == -1 || nRet == 2)
    {
        PSS_LOGGER_DEBUG("file is already exist!");
        exit(1);
    }

    //����ļ���û��������ס��ǰ�ļ���
    if (AcquireWriteLock(fd_lock, 0, sizeof(int)) != 0)
    {
        PSS_LOGGER_DEBUG("lock the file failure and exit, idx = 0.");
        exit(1);
    }

    //д���ӽ�������Ϣ
    lseek(fd_lock, 0, SEEK_SET);

    for (int nIndex = 0; nIndex <= nNumChlid; nIndex++)
    {
        ssize_t stSize = write(fd_lock, &nIndex, sizeof(nIndex));

        if (0 == stSize)
        {
            PSS_LOGGER_DEBUG("write idx = {0}, error.", nIndex);
            exit(1);
        }
    }

    //����������ӽ���
    while (true)
    {
        Run_Child_Process_Start(nNumChlid, fd_lock);

        //�����
        ACE_OS::sleep(tvMonitorSleep);
    }

#endif
}

void CServerManager::Run_Child_Process_Start(int nNumChlid, const int& fd_lock)
{
    int nRet = 0;
	for (int nChlidIndex = 1; nChlidIndex <= nNumChlid; nChlidIndex++)
	{
		//����100ms
		ACE_Time_Value tvSleep(0, 100000);
		ACE_OS::sleep(tvSleep);

		//����ÿ���ӽ��̵����Ƿ񻹴���
		nRet = SeeLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));

		if (nRet == -1 || nRet == 2)
		{
			continue;
		}

		//����ļ���û�б������������ļ������������ӽ���
		int npid = ACE_OS::fork();

		if (npid == 0)
		{
			//���ļ���
			AcquireWriteLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));

			//�����ӽ���
			if (false == Run())
			{
                PSS_LOGGER_DEBUG("child {0} Run failure.", nChlidIndex);
				exit(1);
			}

			//�ӽ�����ִ�������������˳�ѭ�����ͷ���
			ReleaseLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));
		}
	}
}

bool CServerManager::Close()
{
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close begin....");
    App_ConnectAcceptorManager::instance()->Close();
    m_ConnectConsoleAcceptor.close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close Acceptor OK");
    App_ClientReConnectManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ClientReConnectManager OK.");
    App_ReTTyClientManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ReTTyClientManager OK.");
    App_MessageManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_MessageManager OK.");
    App_MessageServiceGroup::instance()->Close();
    PSS_LOGGER_DEBUG("[App_MessageServiceGroup::Close]Close App_MessageServiceGroup OK.");
    App_ModuleLoader::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_MessageManager OK.");
    App_ConnectHandlerPool::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ConnectHandlerPool OK.");
    AppLogManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]AppLogManager OK.");
    App_ForwardManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ForwardManager OK.");
    App_IPAccount::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_IPAccount OK.");
    App_MessageBlockManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_MessageBlockManager OK.");
    App_FileTestManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_FileTestManager OK.");
    for (auto UdpServer : m_vecUDPList)
    {
        UdpServer->CloseFinaly();
    }
    m_vecUDPList.clear();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close UDP Listen OK.");
    App_ReactorManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ReactorManager OK.");
    App_ConsoleManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_ConsoleManager OK.");
    App_TimerManager::instance()->Close();
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close App_TimerManager OK.");
    PSS_LOGGER_DEBUG("[CServerManager::Close]Close end....");

    return true;
}


