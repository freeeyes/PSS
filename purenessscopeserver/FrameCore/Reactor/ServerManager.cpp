#include "ServerManager.h"

#ifdef _CPPUNIT_TEST
#include "CppUnitMain.h"
#endif

bool CServerManager::Init()
{
    int nServerPortCount = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();
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

    OUR_DEBUG((LM_INFO, "[CServerManager::Init]nReactorCount=%d.\n", nReactorCount));

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


    //��ʼ��TMSϵͳ
#ifdef _CPPUNIT_TEST
    m_TMService.Init(1);   //�򿪲��Խӿ�
#else
    m_TMService.Init();
#endif

    //��ʼ��������Ķ���ӿ�
    IConnectManager* pConnectManager           = dynamic_cast<IConnectManager*>(App_HandlerManager::instance());
    IClientManager*  pClientManager            = dynamic_cast<IClientManager*>(App_ClientReConnectManager::instance());
    IFrameCommand* pFrameCommand               = dynamic_cast<IFrameCommand*>(&m_objFrameCommand);
    ITMService* pTMService                     = dynamic_cast<ITMService*>(&m_TMService);
    IServerManager* pServerManager             = dynamic_cast<IServerManager*>(this);
    ITTyClientManager* pTTyClientManager       = dynamic_cast<ITTyClientManager*>(App_ReTTyClientManager::instance());
    IControlListen* pControlListen             = dynamic_cast<IControlListen*>(App_ControlListen::instance());

    Server_Manager_Common_IObject(pConnectManager,
                                  pClientManager,
                                  pFrameCommand,
                                  pServerManager,
                                  pTMService,
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
        OUR_DEBUG((LM_INFO, "[CServerManager::Init]%s.\n", App_ConnectAcceptorManager::instance()->GetError()));
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
            OUR_DEBUG((LM_INFO, "child Run failure.\n"));
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
        OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]... i=[%d].\n", i));

        if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_SELECT)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_Select, 1);
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_Select.\n"));
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_TPSELECT)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_TP, 1);
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_TP.\n"));
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_EPOLL)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_DEV_POLL, 1, GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL.\n"));
        }
        else if (u1NetMode == NETWORKMODE::NETWORKMODE_RE_EPOLL_ET)
        {
            blState = App_ReactorManager::instance()->AddNewReactor(i, EM_REACTOR_MODULE::Reactor_DEV_POLL_ET, 1, GetXmlConfigAttribute(xmlClientInfo)->MaxHandlerCount);
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor REACTOR_CLIENTDEFINE = Reactor_DEV_POLL_ET.\n"));
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewProactor NETWORKMODE Error.\n"));
            return false;
        }

        if (!blState)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]AddNewReactor [%d] Error.\n", i));
            OUR_DEBUG((LM_INFO, "[CServerManager::Init_Reactor]Error=%s.\n", App_ReactorManager::instance()->GetError()));
            return false;
        }
    }

    return blState;
}

bool CServerManager::Run()
{
    //��Ӧ����̣�epoll�������ӽ�������г�ʼ��
    if (NETWORKMODE::NETWORKMODE_RE_EPOLL == GetXmlConfigAttribute(xmlNetWorkMode)->Mode
        || NETWORKMODE::NETWORKMODE_RE_EPOLL_ET == GetXmlConfigAttribute(xmlNetWorkMode)->Mode)
    {
        //��ʼ����Ӧ������
        App_ReactorManager::instance()->Init((uint16)(3 + GetXmlConfigAttribute(xmlMessage)->Msg_Thread));

        if (false == Init_Reactor((uint8)(3 + GetXmlConfigAttribute(xmlMessage)->Msg_Thread), GetXmlConfigAttribute(xmlNetWorkMode)->Mode))
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Run]Init_Reactor Error.\n"));
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

    uint16 u2ServerPortCount = (uint16)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();

    //����������TCP��Ӧ��
    for (uint16 i = 0; i < u2ServerPortCount; i++)
    {
        //�õ�������
        ConnectAcceptor* pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

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
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]StartConnectTask error.\n"));
        return false;
    }

    //������ʱ��
    if (0 != App_TimerManager::instance()->activate())
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_TimerManager::instance()->Start() is error.\n"));
        return false;
    }

    //�������з�Ӧ��(���ǿͻ��˽��յķ�Ӧ���������ﲻ������)
    if (!App_ReactorManager::instance()->StartOtherReactor())
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_ReactorManager::instance()->StartOtherReactor is error.\n"));
        return false;
    }

	//�����м���������ӹ�����
	App_ClientReConnectManager::instance()->Init(App_ReactorManager::instance()->GetAce_Reactor(REACTOR_POSTDEFINE));

    //�������еĲ����ʼ������
    if (false == App_ModuleLoader::instance()->InitModule())
    {
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_ModuleLoader::instance()->InitModule() is error.\n"));
        return false;
    }

    //��ʼ��Ϣ�����߳�
    App_MessageServiceGroup::instance()->Start();

    if (GetXmlConfigAttribute(xmlConnectServer)->RunType == 1)
    {
        //�����첽�������������Ϣ���Ĺ���
        App_ServerMessageTask::instance()->Start();
    }

    //��ʼ����tty��ؼ���
    int nReTTyCount = (int)GetXmlConfigAttribute(xmlTTyDrives)->vec.size();

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
    int nS2SCount = (int)GetXmlConfigAttribute(xmlServer2Server)->vec.size();

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
        OUR_DEBUG((LM_INFO, "[CServerManager::Run]App_ReactorManager::instance()->StartClientReactor is error.\n"));
        return false;
    }

#ifdef _CPPUNIT_TEST
    //����CppUnit�Զ�������
    OUR_DEBUG((LM_INFO, "[CppUnit]********************************\n"));
    CCppUnitMain objCppUnitMain;
    objCppUnitMain.Run();
    OUR_DEBUG((LM_INFO, "[CppUnit]********************************\n"));
#endif

    ACE_Thread_Manager::instance()->wait();

    return true;
}

bool CServerManager::Start_Tcp_Listen() const
{
    int nServerPortCount = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();

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
        ConnectAcceptor* pConnectAcceptor = App_ConnectAcceptorManager::instance()->GetConnectAcceptor(i);

        if (nullptr == pConnectAcceptor)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start]pConnectAcceptor[%d] is nullptr.\n", i));
            return false;
        }

        pConnectAcceptor->SetPacketParseInfoID(GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].packetparseid);
        int nRet = pConnectAcceptor->Init_Open(listenAddr, 0, 1, 1, (int)GetXmlConfigAttribute(xmlNetWorkMode)->BackLog);

        if (-1 == nRet)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] pConnectAcceptor->open[%d] is error.\n", i));
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] error(%d).\n", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
            return false;
        }

        OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
    }

    return true;
}

bool CServerManager::Start_Udp_Listen() const
{
    int nUDPServerPortCount = (int)GetXmlConfigAttribute(xmlUDPServerIPs)->vec.size();

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
        CReactorUDPHander* pReactorUDPHandler = new CReactorUDPHander();

        if (nullptr == pReactorUDPHandler)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start]UDP pReactorUDPHandler[%d] is nullptr.\n", i));
            return false;
        }

        pReactorUDPHandler->SetPacketParseInfoID(GetXmlConfigAttribute(xmlUDPServerIPs)->vec[i].uPacketParseID);
        int nRet = pReactorUDPHandler->OpenAddress(listenAddr, App_ReactorManager::instance()->GetAce_Reactor(REACTOR_CLIENTDEFINE));

        if (-1 == nRet)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] UDP Listen from [%s:%d] error(%d).\n", listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
            return false;
        }

        OUR_DEBUG((LM_INFO, "[CServerManager::Start] UDP Listen from [%s:%d] OK.\n", listenAddr.get_host_addr(), listenAddr.get_port_number()));
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
            OUR_DEBUG((LM_INFO, "[CServerManager::Start]listenConsoleAddr set_address error[%d].\n", errno));
            return false;
        }

        int nRet = m_ConnectConsoleAcceptor.Init_Open(listenConsoleAddr);

        if (-1 == nRet)
        {
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] pConnectAcceptor->open is error.\n"));
            OUR_DEBUG((LM_INFO, "[CServerManager::Start] Listen from [%s:%d] error(%d).\n", listenConsoleAddr.get_host_addr(), listenConsoleAddr.get_port_number(), errno));
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
    char szFileName[MAX_BUFF_200] = { '\0' };
    ACE_OS::snprintf(szFileName, MAX_BUFF_200, "%s/lockwatch.lk", szWorkDir);
    //�ļ���Ȩ��Ϊ  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
    fd_lock = open(szFileName, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);

    if (fd_lock < 0)
    {
        OUR_DEBUG((LM_ERROR, "open the flock and exit, errno = %d.\n", errno));
        exit(1);
    }

    //�鿴��ǰ�ļ����Ƿ�����
    nRet = SeeLock(fd_lock, 0, sizeof(int));

    if (nRet == -1 || nRet == 2)
    {
        OUR_DEBUG((LM_ERROR, "file is already exist!\n"));
        exit(1);
    }

    //����ļ���û��������ס��ǰ�ļ���
    if (AcquireWriteLock(fd_lock, 0, sizeof(int)) != 0)
    {
        OUR_DEBUG((LM_ERROR, "lock the file failure and exit, idx = 0.\n"));
        exit(1);
    }

    //д���ӽ�������Ϣ
    lseek(fd_lock, 0, SEEK_SET);

    for (int nIndex = 0; nIndex <= nNumChlid; nIndex++)
    {
        ssize_t stSize = write(fd_lock, &nIndex, sizeof(nIndex));

        if (0 == stSize)
        {
            OUR_DEBUG((LM_ERROR, "write idx = %d, error.\n", nIndex));
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

void CServerManager::Run_Child_Process_Start(int nNumChlid, int& fd_lock)
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
				OUR_DEBUG((LM_ERROR, "child %d Run failure.\n", nChlidIndex));
				exit(1);
			}

			//�ӽ�����ִ�������������˳�ѭ�����ͷ���
			ReleaseLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));
		}
	}
}

bool CServerManager::Close()
{
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close begin....\n"));
    m_TMService.Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close m_TSThread OK.\n"));
    App_ConnectAcceptorManager::instance()->Close();
    m_ConnectConsoleAcceptor.close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]AppLogManager OK\n"));
    App_TimerManager::instance()->deactivate();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ModuleLoader OK.\n"));
    App_ClientReConnectManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ClientReConnectManager OK.\n"));
    App_ReTTyClientManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ReTTyClientManager OK.\n"));
    App_MessageManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_MessageManager OK.\n"));
    App_MessageServiceGroup::instance()->Close();
    OUR_DEBUG((LM_INFO, "[App_MessageServiceGroup::Close]Close App_MessageServiceGroup OK.\n"));
    App_ModuleLoader::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_MessageManager OK.\n"));
    App_ServerMessageTask::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ServerMessageTask OK.\n"));
    App_ConnectHandlerPool::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ConnectHandlerPool OK.\n"));
    AppLogManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]AppLogManager OK\n"));
    App_BuffPacketManager::instance()->Close_Object(CBuffPacketManager::Close_Callback);
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]BuffPacketManager OK\n"));
    App_ForwardManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ForwardManager OK.\n"));
    App_IPAccount::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_IPAccount OK.\n"));
    App_MessageBlockManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_MessageBlockManager OK.\n"));
    App_PacketParsePool::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_PacketParsePool OK.\n"));
    App_FileTestManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_FileTestManager OK.\n"));
    App_ReactorManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ReactorManager OK.\n"));
    App_ConsoleManager::instance()->Close();
    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close App_ConsoleManager OK.\n"));

    OUR_DEBUG((LM_INFO, "[CServerManager::Close]Close end....\n"));

    return true;
}


