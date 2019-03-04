#include "UserValidManager.h"
#include "UserInfoManager.h"

#include "ace/OS_main.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Auto_Ptr.h"
#include "ace/Singleton.h"
#include "ace/Thread.h"

//服务器启动的IP和端口
#define SERVER_IP     "127.0.0.1"
#define SERVER_PORT   10005

#define SERVER_COMMAND_USERVALID   0xc001    //到数据源查询UserValid请求
#define SERVER_COMMAND_USERINFO    0xc002    //到数据源查询UserInfo请求

#define SERVER_COMMAND_USERVALID_R 0xc101    //应答查询UserValid请求
#define SERVER_COMMAND_USERINFO_R  0xc102    //应答查询UserInfo请求

typedef ACE_Singleton<CUserValidManager, ACE_Null_Mutex> App_UserValidManager;
typedef ACE_Singleton<CUserInfoManager, ACE_Null_Mutex> App_UserInfoManager;

class CClientService : public ACE_Event_Handler
{
public:
    ACE_SOCK_Stream& peer (void)
    {
        return this->m_sckClient;
    }

    int open (void)
    {
        //注册读就绪回调函数
        return this->reactor ()->register_handler(this, ACE_Event_Handler::READ_MASK);
    }

    virtual ACE_HANDLE get_handle (void) const
    {
        return this->m_sckClient.get_handle();
    }

    virtual int handle_input (ACE_HANDLE fd )
    {
        if(fd == ACE_INVALID_HANDLE)
        {
            OUR_DEBUG((LM_ERROR, "[handle_input]fd is ACE_INVALID_HANDLE.\n"));
            return -1;
        }

        ACE_Time_Value nowait(0, MAX_QUEUE_TIMEOUT);

        //处理接收逻辑
        //接收字节，先接收4字节包长度，然后接收用户名字符串
        char szPacketSize[4] = {'\0'};
        int nDataLen = this->peer().recv(szPacketSize, 4, MSG_NOSIGNAL, &nowait);

        if(nDataLen != 4 && nDataLen <= 0)
        {
            return -1;
        }

        int nPacketSize = 0;
        ACE_OS::memcpy(&nPacketSize, szPacketSize, 4);

        char* pBuff = new char[nPacketSize];
        ACE_OS::memset(pBuff, 0, nPacketSize);

        //因为是内网程序，目前不考虑分包和粘包，这里在大流量下可以优化的，先以完成功能为主。
        //组包规则，2字节用户名长度+用户名+4字节ConnectID
        nDataLen = this->peer().recv(pBuff, nPacketSize, MSG_NOSIGNAL, &nowait);

        if(nDataLen != nPacketSize && nDataLen <= 0)
        {
            SAFE_DELETE_ARRAY(pBuff);
            return -1;
        }

        int nRecvPos      = 0;

        //解析命令ID
        uint16 u2WatchCommand = 0;
        ACE_OS::memcpy((char* )&u2WatchCommand, (char* )&pBuff[nRecvPos], sizeof(uint16));
        nRecvPos += sizeof(uint16);

        if(u2WatchCommand == SERVER_COMMAND_USERVALID)
        {
            //解析数据
            int nUserNameSize = 0;
            int nUserPassSize = 0;
            int nConnectID    = 0;

            ACE_OS::memcpy((char* )&nUserNameSize, (char* )&pBuff[nRecvPos], 2);
            nRecvPos += 2;
            char* pUserName = new char[nUserNameSize + 1];
            ACE_OS::memset(pUserName, 0, nUserNameSize + 1);
            ACE_OS::memcpy((char* )pUserName, (char* )&pBuff[nRecvPos], nUserNameSize);
            nRecvPos += nUserNameSize;

            ACE_OS::memcpy((char* )&nUserPassSize, (char* )&pBuff[nRecvPos], 2);
            nRecvPos += 2;
            char* pUserPass = new char[nUserPassSize + 1];
            ACE_OS::memset(pUserPass, 0, nUserPassSize + 1);
            ACE_OS::memcpy((char* )pUserPass, (char* )&pBuff[nRecvPos], nUserPassSize);
            nRecvPos += nUserPassSize;

            ACE_OS::memcpy((char* )&nConnectID, (char* )&pBuff[nRecvPos], 4);
            nRecvPos += 4;

            int nSendSize = 4 + 2 + 2 + nUserNameSize + 2 + nUserPassSize + 1 + 4 + 4;
            char* pSend = new char[nSendSize];
            int nSendPos = 0;

            uint16 u2CommandReturn = SERVER_COMMAND_USERVALID_R;

            //处理接收数据
            uint32 u4CacheIndex = 0;
            bool blState = App_UserValidManager::instance()->Load_From_DataResouce(pUserName, u4CacheIndex);

            if(blState == false)
            {
                //没有找到这个用户数据，组成返回包
                int nSendPacketSize = 2 + 2 + nUserNameSize + 2 + nUserPassSize + 1 + 4 + 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nSendPacketSize, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u2CommandReturn, 2);
                nSendPos += 2;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nUserNameSize, 2);
                nSendPos += 2;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )pUserName, nUserNameSize);
                nSendPos += nUserNameSize;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nUserPassSize, 2);
                nSendPos += 2;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )pUserPass, nUserPassSize);
                nSendPos += nUserPassSize;
                int nRet = 1;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nRet, 1);
                nSendPos += 1;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u4CacheIndex, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nConnectID, 4);
                nSendPos += 4;
            }
            else
            {
                //找到了这个用户数据，组成返回包
                int nSendPacketSize = 2 + 2 + nUserNameSize + 2 + nUserPassSize + 1 + 4 + 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nSendPacketSize, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u2CommandReturn, 2);
                nSendPos += 2;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nUserNameSize, 2);
                nSendPos += 2;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )pUserName, nUserNameSize);
                nSendPos += nUserNameSize;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nUserPassSize, 2);
                nSendPos += 2;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )pUserPass, nUserPassSize);
                nSendPos += nUserPassSize;
                int nRet = 0;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nRet, 1);
                nSendPos += 1;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u4CacheIndex, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nConnectID, 4);
                nSendPos += 4;
            }

            //发送返回数据
            this->peer().send(pSend, nSendSize, &nowait);

            SAFE_DELETE_ARRAY(pUserPass);
            SAFE_DELETE_ARRAY(pUserName);
            SAFE_DELETE_ARRAY(pSend);
        }
        else if(u2WatchCommand == SERVER_COMMAND_USERINFO)
        {
            //解析数据
            uint32 u4UserID    = 0;
            uint32 u4ConnectID = 0;

            ACE_OS::memcpy((char* )&u4UserID, (char* )&pBuff[nRecvPos], sizeof(uint32));
            nRecvPos += sizeof(uint32);
            ACE_OS::memcpy((char* )&u4ConnectID, (char* )&pBuff[nRecvPos], sizeof(uint32));
            nRecvPos += sizeof(uint32);

            int nSendSize = 4 + 2 + 1 + 4 + 4 + 4;
            char* pSend = new char[nSendSize];
            int nSendPos = 0;

            uint16 u2CommandReturn = SERVER_COMMAND_USERINFO_R;

            uint32 u4CacheIndex = 0;
            bool blState = App_UserInfoManager::instance()->Load_From_DataResouce(u4UserID, u4CacheIndex);

            if(blState == false)
            {
                //没有找到这个用户数据，组成返回包
                int nSendPacketSize = 2 + 1 + 4 + 4 + 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nSendPacketSize, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u2CommandReturn, 2);
                nSendPos += 2;
                int nRet = 1;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nRet, 1);
                nSendPos += 1;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u4UserID, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u4CacheIndex, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u4ConnectID, 4);
                nSendPos += 4;
            }
            else
            {
                //找到了数据并加载了内存，组成返回包
                int nSendPacketSize = 2 + 1 + 4 + 4 + 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nSendPacketSize, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u2CommandReturn, 2);
                nSendPos += 2;
                int nRet = 0;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&nRet, 1);
                nSendPos += 1;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u4UserID, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u4CacheIndex, 4);
                nSendPos += 4;
                ACE_OS::memcpy((char* )&pSend[nSendPos], (char* )&u4ConnectID, 4);
                nSendPos += 4;
            }

            //发送返回数据
            this->peer().send(pSend, nSendSize, &nowait);

            //发送数据
            SAFE_DELETE_ARRAY(pSend);
        }


        SAFE_DELETE_ARRAY(pBuff);
        return 0;
    }

    // 释放相应资源
    virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask mask)
    {
        if (mask == ACE_Event_Handler::WRITE_MASK)
        {
            return 0;
        }

        mask = ACE_Event_Handler::ALL_EVENTS_MASK |
               ACE_Event_Handler::DONT_CALL;
        this->reactor ()->remove_handler (this, mask);
        this->m_sckClient.close ();
        delete this;    //socket出错时，将自动删除该客户端，释放相应资源
        return 0;
    }

protected:
    ACE_SOCK_Stream m_sckClient;
};

class CServerAcceptor : public ACE_Event_Handler
{
public:
    virtual ~CServerAcceptor ()
    {
        this->handle_close (ACE_INVALID_HANDLE, 0);
    }

    int open (const ACE_INET_Addr& listen_addr)
    {
        if (this->m_objAcceptor.open (listen_addr, 1) == -1)
        {
            OUR_DEBUG((LM_INFO, "open port fail.\n"));
            return -1;
        }

        OUR_DEBUG((LM_INFO, "Begin Listen....\n"));
        //注册接受连接回调事件
        return this->reactor ()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
    }

    virtual ACE_HANDLE get_handle (void) const
    {
        return this->m_objAcceptor.get_handle ();
    }

    virtual int handle_input (ACE_HANDLE fd )
    {
        if(fd == ACE_INVALID_HANDLE)
        {
            return -1;
        }

        CClientService* client = new CClientService();
        auto_ptr<CClientService> p(client);

        if (this->m_objAcceptor.accept (client->peer ()) == -1)
        {
            OUR_DEBUG((LM_INFO, "accept client fail.\n"));
            return -1;
        }

        p.release ();
        client->reactor (this->reactor ());

        if (client->open () == -1)
        {
            client->handle_close (ACE_INVALID_HANDLE, 0);
        }

        return 0;
    }

    virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
    {
        OUR_DEBUG((LM_ERROR, "[handle_close]close_mask=%d.\n", (int)close_mask));

        if (handle != ACE_INVALID_HANDLE)
        {
            ACE_Reactor_Mask m = ACE_Event_Handler::ACCEPT_MASK |
                                 ACE_Event_Handler::DONT_CALL;
            this->reactor ()->remove_handler (this, m);
            this->m_objAcceptor.close ();
        }

        return 0;
    }

protected:
    ACE_SOCK_Acceptor m_objAcceptor;
};

void* worker_UserValid(void* arg)
{
    if(NULL != arg)
    {
        OUR_DEBUG((LM_INFO, "[worker]have param.\n"));
    }

    while(true)
    {
        OUR_DEBUG((LM_INFO, "[Watch]Valid Begin.\n"));
        App_UserValidManager::instance()->Sync_DataReaource_To_Memory();
        OUR_DEBUG((LM_INFO, "[Watch]Valid End.\n"));
        App_UserValidManager::instance()->Display();
        ACE_Time_Value tvSleep(60, 0);
        ACE_OS::sleep(tvSleep);
    }

    return NULL;
}

void* worker_UserInfo(void* arg)
{
    if(NULL != arg)
    {
        OUR_DEBUG((LM_INFO, "[worker]have param.\n"));
    }

    while(true)
    {
        OUR_DEBUG((LM_INFO, "[Watch]Valid Begin.\n"));
        App_UserInfoManager::instance()->Sync_Memory_To_DataReaource();
        OUR_DEBUG((LM_INFO, "[Watch]Valid End.\n"));
        App_UserInfoManager::instance()->Display();
        ACE_Time_Value tvSleep(60, 0);
        ACE_OS::sleep(tvSleep);
    }

    return NULL;
}


int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
    if(argc > 0)
    {
        OUR_DEBUG((LM_INFO, "[main]argc = %d.\n", argc));

        for(int i = 0; i < argc; i++)
        {
            OUR_DEBUG((LM_INFO, "[main]argc(%d) = %s.\n", argc, argv[i]));
        }
    }

    ACE_thread_t  threadId;
    ACE_hthread_t threadHandle;

    //初始化共享内存
    App_UserValidManager::instance()->Init((uint32)MAX_LOGIN_VALID_COUNT, SHM_USERVALID_KEY, (uint32)sizeof(_UserValid));
    App_UserInfoManager::instance()->Init((uint32)MAX_LOGIN_INFO_COUNT, SHM_USERINFO_KEY, (uint32)sizeof(_UserInfo));

    //首先创建工作线程(Valid的工作线程)
    ACE_Thread::spawn(
        (ACE_THR_FUNC)worker_UserValid,  //线程执行函数
        NULL,                            //执行函数参数
        THR_JOINABLE | THR_NEW_LWP,
        &threadId,
        &threadHandle
    );

    //再创建工作线程
    //首先创建工作线程(Valid的工作线程)
    ACE_Thread::spawn(
        (ACE_THR_FUNC)worker_UserInfo,   //线程执行函数
        NULL,                            //执行函数参数
        THR_JOINABLE | THR_NEW_LWP,
        &threadId,
        &threadHandle
    );

    //然后打开监听
    ACE_INET_Addr addr(SERVER_PORT, SERVER_IP);
    CServerAcceptor server;
    server.reactor(ACE_Reactor::instance());
    server.open(addr);

    while(true)
    {
        ACE_Reactor::instance()->handle_events();
    }

    return 0;
}
