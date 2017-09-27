#include "ProConnectAccept.h"

ProConnectAcceptor::ProConnectAcceptor()
{
    m_szListenIP[0]         = '\0';
    m_u4Port                = 0;
    m_u4AcceptCount         = 0;
    m_u4ClientProactorCount = 1;
    m_u4PacketParseInfoID   = 0;
}

void ProConnectAcceptor::InitClientProactor(uint32 u4ClientProactorCount)
{
    m_u4ClientProactorCount = u4ClientProactorCount;
}

void ProConnectAcceptor::SetPacketParseInfoID(uint32 u4PaccketParseInfoID)
{
    m_u4PacketParseInfoID = u4PaccketParseInfoID;
}

uint32 ProConnectAcceptor::GetPacketParseInfoID()
{
    return m_u4PacketParseInfoID;
}

CProConnectHandle* ProConnectAcceptor::make_handler(void)
{
    CProConnectHandle* pProConnectHandle = App_ProConnectHandlerPool::instance()->Create();

    if(NULL != pProConnectHandle)
    {
        pProConnectHandle->SetLocalIPInfo(m_szListenIP, m_u4Port);
        //这里会根据反应器线程配置，自动匹配一个空闲的反应器
        int nIndex = (int)(m_u4AcceptCount % m_u4ClientProactorCount);
        ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Client_Proactor(nIndex);
        pProConnectHandle->proactor(pProactor);
        pProConnectHandle->SetPacketParseInfoID(m_u4PacketParseInfoID);
        m_u4AcceptCount++;
    }

    return pProConnectHandle;
}

int ProConnectAcceptor::validate_connection (const ACE_Asynch_Accept::Result& result,
        const ACE_INET_Addr& remote,
        const ACE_INET_Addr& local)
{
    //如果正在处理的链接超过了服务器设定的数值，则不允许链接继续链接服务器
    if(App_ProConnectHandlerPool::instance()->GetUsedCount() > App_MainConfig::instance()->GetMaxHandlerCount())
    {
        OUR_DEBUG((LM_ERROR, "[ProConnectAcceptor::validate_connection]Connect is more MaxHandlerCount(%d > %d).\n", App_ProConnectHandlerPool::instance()->GetUsedCount(), App_MainConfig::instance()->GetMaxHandlerCount()));
        //不允许链接
        return -1;
    }
    else
    {
        //允许链接
        return 0;
    }
}

char* ProConnectAcceptor::GetListenIP()
{
    return m_szListenIP;
}

uint32 ProConnectAcceptor::GetListenPort()
{
    return m_u4Port;
}

void ProConnectAcceptor::SetListenInfo(const char* pIP, uint32 u4Port)
{
    sprintf_safe(m_szListenIP, MAX_BUFF_20, "%s", pIP);
    m_u4Port = u4Port;
}

CProConnectAcceptManager::CProConnectAcceptManager(void)
{
    m_nAcceptorCount = 0;
    m_szError[0]     = '\0';

    m_bFileTesting = false;
    m_bLoadCfgFile = false;
    m_n4TimerID = 0;
    m_n4ConnectCount = 0;
}

CProConnectAcceptManager::~CProConnectAcceptManager(void)
{
    Close();
}

bool CProConnectAcceptManager::InitConnectAcceptor(int nCount, uint32 u4ClientProactorCount)
{
    try
    {
        Close();

        for(int i = 0; i < nCount; i++)
        {
            ProConnectAcceptor* pConnectAcceptor = new ProConnectAcceptor();

            if(NULL == pConnectAcceptor)
            {
                throw "[CProConnectAcceptManager::InitConnectAcceptor]pConnectAcceptor new is fail.";
            }

            pConnectAcceptor->InitClientProactor(u4ClientProactorCount);
            m_vecConnectAcceptor.push_back(pConnectAcceptor);
        }

        if(m_n4TimerID > 0)
        {
            App_TimerManager::instance()->cancel(m_n4TimerID);
            m_n4TimerID = 0;
        }

        return true;
    }
    catch (const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

void CProConnectAcceptManager::Close()
{
    ACE_Time_Value tvSleep(0, 10000);

    for(int i = 0; i < (int)m_vecConnectAcceptor.size(); i++)
    {
        ProConnectAcceptor* pConnectAcceptor = m_vecConnectAcceptor[i];

        if(NULL != pConnectAcceptor)
        {
            pConnectAcceptor->cancel();
            ACE_OS::sleep(tvSleep);
            SAFE_DELETE(pConnectAcceptor);
        }
    }

    m_vecConnectAcceptor.clear();
    m_nAcceptorCount = 0;
}

bool CProConnectAcceptManager::Close( const char* pIP, uint32 n4Port )
{
    //找到符合条件指定的端口停止监听
    for(vecProConnectAcceptor::iterator b = m_vecConnectAcceptor.begin(); b != m_vecConnectAcceptor.end(); ++b)
    {
        ProConnectAcceptor* pConnectAcceptor = (ProConnectAcceptor*)(*b);

        if (NULL != pConnectAcceptor)
        {
            if(ACE_OS::strcmp(pConnectAcceptor->GetListenIP(), pIP) == 0
               && pConnectAcceptor->GetListenPort() == n4Port)
            {
                pConnectAcceptor->cancel();
                SAFE_DELETE(pConnectAcceptor);
                m_vecConnectAcceptor.erase(b);
                break;
            }
        }
    }

    return true;
}

int CProConnectAcceptManager::GetCount()
{
    return (int)m_vecConnectAcceptor.size();
}

ProConnectAcceptor* CProConnectAcceptManager::GetConnectAcceptor(int nIndex)
{
    if(nIndex < 0 || nIndex >= (int)m_vecConnectAcceptor.size())
    {
        return NULL;
    }

    return m_vecConnectAcceptor[nIndex];
}

const char* CProConnectAcceptManager::GetError()
{
    return m_szError;
}

bool CProConnectAcceptManager::CheckIPInfo(const char* pIP, uint32 n4Port)
{
    //找到符合条件指定的端口停止监听
    for(int i = 0; i < (int)m_vecConnectAcceptor.size(); i++)
    {
        if (NULL != m_vecConnectAcceptor[i])
        {
            if(ACE_OS::strcmp(m_vecConnectAcceptor[i]->GetListenIP(), pIP) == 0
               && m_vecConnectAcceptor[i]->GetListenPort() == n4Port)
            {
                return true;
            }
        }
    }

    return false;
}

ProConnectAcceptor* CProConnectAcceptManager::GetNewConnectAcceptor()
{
    ProConnectAcceptor* pConnectAcceptor = new ProConnectAcceptor();

    if(NULL == pConnectAcceptor)
    {
        return NULL;
    }

    m_vecConnectAcceptor.push_back(pConnectAcceptor);
    return pConnectAcceptor;
}

FileTestResultInfoSt CProConnectAcceptManager::FileTestStart(string strXmlCfg)
{
    FileTestResultInfoSt objFileTestResult;

    if(m_bFileTesting)
    {
        OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]当前状态已经处于文件测试状态.\n"));
        objFileTestResult.n4Result = RESULT_ERR_TESTING;
        return objFileTestResult;
    }
    else
    {
        if(m_bLoadCfgFile)
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]测试配置文件已经加载,发生未知错误.\n"));
        }
        else
        {
            if(!LoadXmlCfg(strXmlCfg,objFileTestResult))
            {
                OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]加载测试配置文件错误,文件:%s.\n",strXmlCfg.c_str()));
            }
            else
            {
                m_n4TimerID = App_TimerManager::instance()->schedule(this, (void*)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(m_n4TimeInterval), ACE_Time_Value(m_n4TimeInterval));

                if(-1 == m_n4TimerID)
                {
                    OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]启动定时器错误\n"));
                    objFileTestResult.n4Result = RESULT_ERR_UNKOWN;
                }
                else
                {
                    OUR_DEBUG((LM_ERROR, "[CMainConfig::LoadXmlCfg]Start thread time OK.\n"));
                    objFileTestResult.n4Result = RESULT_OK;
                }
            }
        }
    }

    return objFileTestResult;
}

int CProConnectAcceptManager::FileTestEnd()
{
    return 0;
}

bool CProConnectAcceptManager::LoadXmlCfg(string strXmlCfg,FileTestResultInfoSt& objFileTestResult)
{
    char* pData = NULL;
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]Filename = %s.\n", strXmlCfg.c_str()));

    if(false == m_MainConfig.Init(strXmlCfg.c_str()))
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]File Read Error = %s.\n", strXmlCfg.c_str()));
        objFileTestResult.n4Result = RESULT_ERR_CFGFILE;
        return false;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "Path");

    if(NULL != pData)
    {
        m_strProFilePath = pData;
    }
    else
    {
        m_strProFilePath = "./";
    }

    pData = m_MainConfig.GetData("FileTestConfig", "TimeInterval");

    if(NULL != pData)
    {
        m_n4TimeInterval = (uint8)ACE_OS::atoi(pData);
    }
    else
    {
        m_n4TimeInterval = 1;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ConnectCount");

    if(NULL != pData)
    {
        m_n4ConnectCount = (uint8)ACE_OS::atoi(pData);
    }
    else
    {
        m_n4ConnectCount = 100;
    }

    //命令监控相关配置
    m_vecFileTestDataInfoSt.clear();
    TiXmlElement* pNextTiXmlElementFileName     = NULL;
    TiXmlElement* pNextTiXmlElementParseID       = NULL;
    TiXmlElement* pNextTiXmlElementDesc      = NULL;

    while(true)
    {
        FileTestDataInfoSt objFileTestDataInfo;
        string strFileName;
        string strFileDesc;

        pData = m_MainConfig.GetData("FileInfo", "FileName", pNextTiXmlElementFileName);

        if(pData != NULL)
        {
            strFileName = m_strProFilePath + pData;
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]File Read FileName Error = %s.\n", strXmlCfg.c_str()));
            objFileTestResult.n4Result = RESULT_ERR_PROFILE;
            return false;
        }

        pData = m_MainConfig.GetData("FileInfo", "ParseID", pNextTiXmlElementParseID);

        if(pData != NULL)
        {
            objFileTestDataInfo.m_u4ParseID = (uint32)ACE_OS::atoi(pData);
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]File Read ParseID Error = %s.\n", strXmlCfg.c_str()));
            objFileTestResult.n4Result = RESULT_ERR_PROFILE;
            return false;
        }

        pData = m_MainConfig.GetData("FileInfo", "Desc", pNextTiXmlElementDesc);

        if(pData != NULL)
        {
            strFileDesc = pData;
            objFileTestResult.vecProFileDesc.push_back(strFileDesc);
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]File Read Desc Error = %s.\n", strXmlCfg.c_str()));
            objFileTestResult.n4Result = RESULT_ERR_PROFILE;
            return false;
        }

        ACE_FILE_Connector connector;
        ACE_FILE_IO file;
        ACE_FILE_Addr file_addr(strFileName.c_str());

        if (connector.connect(file, file_addr) == -1)
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Open FileName:%s Error.\n", strFileName.c_str()));
            objFileTestResult.n4Result = RESULT_ERR_PROFILE;
            return false;
        }

        ACE_FILE_Info info;

        if (file.get_info (info) == -1)
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Get File Info FileName:%s Error.\n", strFileName.c_str()));
            objFileTestResult.n4Result = RESULT_ERR_PROFILE;
            return false;
        }

        if(MAX_BUFF_10240 - 1 < info.size_)
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]协议文件内容过大 FileName:%s Error.\n", strFileName.c_str()));
            objFileTestResult.n4Result = RESULT_ERR_PROFILE;
            return false;
        }
        else
        {
            // Read the file into the buffer.
            ssize_t size = file.recv (objFileTestDataInfo.m_szData,info.size_);

            if (size != info.size_)
            {
                OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]读取协议文件错误 FileName:%s Error.\n", strFileName.c_str()));
                objFileTestResult.n4Result = RESULT_ERR_PROFILE;
                return false;
            }
            else
            {
                // Make sure to NUL-terminate this turkey!
                objFileTestDataInfo.m_szData[size] = '\0';
                objFileTestDataInfo.m_u4DataLength = size;
            }
        }

        m_vecFileTestDataInfoSt.push_back(objFileTestDataInfo);
    }

    m_MainConfig.Close();
    return true;
}


int CProConnectAcceptManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    

    return 0;
}



