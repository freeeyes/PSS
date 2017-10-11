#include "FileTestManager.h"

CFileTestManager::CFileTestManager(void)
{
    m_bFileTesting     = false;
    m_n4TimerID        = 0;
    m_n4ConnectCount   = 0;
    m_u4ParseID        = 0;
    m_n4ResponseCount  = 0;
    m_n4ExpectTime     = 1000;
    m_n4ContentType    = 1;
    m_n4TimeInterval   = 0;
    m_mapResponseRecordSt.clear();
}

CFileTestManager::~CFileTestManager(void)
{
    if(m_n4TimerID > 0)
    {
        App_TimerManager::instance()->cancel(m_n4TimerID);
        m_n4TimerID = 0;
        m_bFileTesting = false;
    }

    return;
}

FileTestResultInfoSt CFileTestManager::FileTestStart(const char* szXmlFileTestName)
{
    FileTestResultInfoSt objFileTestResult;

    if(m_bFileTesting)
    {
        OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]m_bFileTesting:%d.\n",m_bFileTesting));
        objFileTestResult.n4Result = RESULT_ERR_TESTING;
        return objFileTestResult;
    }
    else
    {
        if(!LoadXmlCfg(szXmlFileTestName, objFileTestResult))
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]Loading config file error filename:%s.\n", szXmlFileTestName));
        }
        else
        {
            m_n4TimerID = App_TimerManager::instance()->schedule(this, (void*)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(m_n4TimeInterval), ACE_Time_Value(m_n4TimeInterval));

            if(-1 == m_n4TimerID)
            {
                OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Start timer error\n"));
                objFileTestResult.n4Result = RESULT_ERR_UNKOWN;
            }
            else
            {
                OUR_DEBUG((LM_ERROR, "[CMainConfig::LoadXmlCfg]Start timer OK.\n"));
                objFileTestResult.n4Result = RESULT_OK;
                m_bFileTesting = true;
            }
        }
    }

    return objFileTestResult;
}

int CFileTestManager::FileTestEnd()
{
    if(m_n4TimerID > 0)
    {
        App_TimerManager::instance()->cancel(m_n4TimerID);
        m_n4TimerID = 0;
        m_bFileTesting = false;
    }

    return 0;
}

void CFileTestManager::HandlerServerResponse(uint32 u4ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    mapResponseRecordSt::iterator iter= m_mapResponseRecordSt.find(u4ConnectID);

    if(iter != m_mapResponseRecordSt.end())
    {
        ResponseRecordSt& objResponseRecord = (ResponseRecordSt&)iter->second;

        if(objResponseRecord.m_u1ResponseCount + 1 == m_n4ResponseCount)
        {
            ACE_Time_Value atvResponse = ACE_OS::gettimeofday();

            if(m_n4ExpectTime <= (int)((uint64)atvResponse.get_msec() - objResponseRecord.m_u8StartTime))
            {
                //应答时间超过期望时间限制
                OUR_DEBUG((LM_INFO, "[CFileTestManager::HandlerServerResponse]Response time too long m_n4ExpectTime:%d.\n",m_n4ExpectTime));
            }

#ifndef WIN32
            App_ConnectManager::instance()->Close(u4ConnectID);
#else
            App_ProConnectManager::instance()->Close(u4ConnectID);
#endif
            m_mapResponseRecordSt.erase(iter->first);
        }
        else
        {
            objResponseRecord.m_u1ResponseCount = objResponseRecord.m_u1ResponseCount + 1;
        }
    }

    return;
}

bool CFileTestManager::LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult)
{
    char* pData = NULL;
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]Filename = %s.\n", szXmlFileTestName));

    if(false == m_MainConfig.Init(szXmlFileTestName))
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]File Read Error = %s.\n", szXmlFileTestName));
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
        objFileTestResult.n4TimeInterval = m_n4TimeInterval;
        OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_n4TimeInterval = %d.\n", m_n4TimeInterval));
    }
    else
    {
        m_n4TimeInterval = 10;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ConnectCount");

    if(NULL != pData)
    {
        m_n4ConnectCount = (uint8)ACE_OS::atoi(pData);
        objFileTestResult.n4ConnectNum = m_n4ConnectCount;
        OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_n4ConnectCount = %d.\n", m_n4ConnectCount));
    }
    else
    {
        m_n4ConnectCount = 10;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ResponseCount");

    if(NULL != pData)
    {
        m_n4ResponseCount = (uint8)ACE_OS::atoi(pData);
        OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_n4ResponseCount = %d.\n", m_n4ResponseCount));
    }
    else
    {
        m_n4ResponseCount = 1;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ExpectTime");

    if(NULL != pData)
    {
        m_n4ExpectTime = (uint8)ACE_OS::atoi(pData);
        OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_n4ExpectTime = %d.\n", m_n4ExpectTime));
    }
    else
    {
        m_n4ExpectTime = 1000;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ParseID");

    if(NULL != pData)
    {
        m_u4ParseID = (uint8)ACE_OS::atoi(pData);
    }
    else
    {
        m_u4ParseID = 1;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ConnectCount");

    if(NULL != pData)
    {
        m_n4ContentType = (uint8)ACE_OS::atoi(pData);
    }
    else
    {
        m_n4ContentType = 1; //默认是二进制协议
    }

    //命令监控相关配置
    m_vecFileTestDataInfoSt.clear();
    TiXmlElement* pNextTiXmlElementFileName     = NULL;
    TiXmlElement* pNextTiXmlElementDesc         = NULL;

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
            break;
        }

        pData = m_MainConfig.GetData("FileInfo", "Desc", pNextTiXmlElementDesc);

        if(pData != NULL)
        {
            strFileDesc = pData;
            objFileTestResult.vecProFileDesc.push_back(strFileDesc);
        }
        else
        {
            break;
        }

        //读取数据包文件内容
        int nReadFileRet = ReadTestFile(strFileName.c_str(), m_n4ContentType, objFileTestDataInfo);

        if(RESULT_OK == nReadFileRet)
        {
            m_vecFileTestDataInfoSt.push_back(objFileTestDataInfo);
        }
        else
        {
            objFileTestResult.n4Result = nReadFileRet;
            return false;
        }
    }

    m_MainConfig.Close();

    objFileTestResult.n4ProNum = static_cast<int>(m_vecFileTestDataInfoSt.size());
    return true;
}

int CFileTestManager::ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo)
{
    ACE_FILE_Connector fConnector;
    ACE_FILE_IO ioFile;
    ACE_FILE_Addr fAddr(pFileName);

    if (fConnector.connect(ioFile, fAddr) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::ReadTestFile]Open filename:%s Error.\n", pFileName));
        return RESULT_ERR_PROFILE;
    }

    ACE_FILE_Info fInfo;

    if (ioFile.get_info(fInfo) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::ReadTestFile]Get file info filename:%s Error.\n", pFileName));
        return RESULT_ERR_PROFILE;
    }

    if (MAX_BUFF_10240 - 1 < fInfo.size_)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Protocol file too larger filename:%s.\n", pFileName));
        return RESULT_ERR_PROFILE;
    }
    else
    {
        char szFileContent[MAX_BUFF_10240] = { '\0' };
        ssize_t u4Size = ioFile.recv(szFileContent, fInfo.size_);

        if (u4Size != fInfo.size_)
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Read protocol file error filename:%s Error.\n", pFileName));
            return RESULT_ERR_PROFILE;
        }
        else
        {
            if (nType == 0)
            {
                //如果是文本协议
                memcpy_safe(szFileContent, static_cast<uint32>(u4Size), objFileTestDataInfo.m_szData, static_cast<uint32>(u4Size));
                objFileTestDataInfo.m_szData[u4Size] = '\0';
                objFileTestDataInfo.m_u4DataLength = static_cast<uint32>(u4Size);
                OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]u4Size:%d\n", u4Size));
                OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]m_szData:%s\n", objFileTestDataInfo.m_szData));
            }
            else
            {
                //如果是二进制协议
                CConvertBuffer objConvertBuffer;
                //将数据串转换成二进制串
                int nDataSize = MAX_BUFF_10240;
                objConvertBuffer.Convertstr2charArray(szFileContent, static_cast<int>(u4Size), (unsigned char*)objFileTestDataInfo.m_szData, nDataSize);
                objFileTestDataInfo.m_u4DataLength = static_cast<uint32>(nDataSize);
            }
        }
    }

    return RESULT_OK;
}

int CFileTestManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    if (NULL != arg)
    {
        OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]arg is not NULL.\n"));
    }

#ifndef WIN32
    vector<uint32> vecu4ConnectID;
    CConnectHandler* pConnectHandler = NULL;

    for (int iLoop = 0; iLoop < m_n4ConnectCount; iLoop++)
    {
        pConnectHandler = App_ConnectHandlerPool::instance()->Create();

        if (NULL != pConnectHandler)
        {
            //文件数据包测试不需要用到pProactor对象，因为不需要实际发送数据，所以这里可以直接设置一个固定的pProactor
            ACE_Reactor* pReactor = App_ReactorManager::instance()->GetAce_Client_Reactor(0);
            pConnectHandler->reactor(pReactor);
            pConnectHandler->SetPacketParseInfoID(m_u4ParseID);

            uint32 u4ConnectID = pConnectHandler->file_open(dynamic_cast<IFileTestManager*>(this));

            if (0 != u4ConnectID)
            {
                vecu4ConnectID.push_back(u4ConnectID);
                ResponseRecordSt objResponseRecord;
                objResponseRecord.m_u1ResponseCount = 0;
                objResponseRecord.m_u8StartTime = tv.get_msec();
                m_mapResponseRecordSt.insert(mapResponseRecordSt::value_type(u4ConnectID, objResponseRecord));
            }
            else
            {
                OUR_DEBUG((LM_INFO, "[CMainConfig::handle_timeout]file_open error\n"));
            }
        }
    }

    for (int iLoop = 0; iLoop < (int)m_vecFileTestDataInfoSt.size(); iLoop++)
    {
        FileTestDataInfoSt& objFileTestDataInfo = m_vecFileTestDataInfoSt[iLoop];

        for (int jLoop = 0; jLoop < (int)vecu4ConnectID.size(); jLoop++)
        {
            uint32 u4ConnectID = vecu4ConnectID[jLoop];
            App_ConnectManager::instance()->handle_write_file_stream(u4ConnectID, objFileTestDataInfo.m_szData, objFileTestDataInfo.m_u4DataLength, m_u4ParseID);
        }
    }

#else
    vector<uint32> vecu4ConnectID;
    CProConnectHandle* ptrProConnectHandle = NULL;

    for (int iLoop = 0; iLoop < m_n4ConnectCount; iLoop++)
    {
        ptrProConnectHandle = App_ProConnectHandlerPool::instance()->Create();

        if (NULL != ptrProConnectHandle)
        {
            //文件数据包测试不需要用到pProactor对象，因为不需要实际发送数据，所以这里可以直接设置一个固定的pProactor
            ACE_Proactor* pPractor = App_ProactorManager::instance()->GetAce_Client_Proactor(0);
            ptrProConnectHandle->proactor(pPractor);
            ptrProConnectHandle->SetPacketParseInfoID(m_u4ParseID);

            uint32 u4ConnectID = ptrProConnectHandle->file_open(dynamic_cast<IFileTestManager*>(this));

            if (0 != u4ConnectID)
            {
                vecu4ConnectID.push_back(u4ConnectID);
                ResponseRecordSt objResponseRecord;
                objResponseRecord.m_u1ResponseCount = 0;
                objResponseRecord.m_u8StartTime = tv.get_msec();
                m_mapResponseRecordSt.insert(mapResponseRecordSt::value_type(u4ConnectID, objResponseRecord));
            }
            else
            {
                OUR_DEBUG((LM_INFO, "[CMainConfig::handle_timeout]file_open error\n"));
            }
        }
    }

    for (int iLoop = 0; iLoop < (int)m_vecFileTestDataInfoSt.size(); iLoop++)
    {
        FileTestDataInfoSt& objFileTestDataInfo = m_vecFileTestDataInfoSt[iLoop];

        for (int jLoop = 0; jLoop < (int)vecu4ConnectID.size(); jLoop++)
        {
            uint32 u4ConnectID = vecu4ConnectID[jLoop];
            App_ProConnectManager::instance()->handle_write_file_stream(u4ConnectID, objFileTestDataInfo.m_szData, objFileTestDataInfo.m_u4DataLength, m_u4ParseID);
        }
    }

#endif

    //超过5倍的定时器间隔时间检查一下
    if(5*m_n4TimeInterval <= tv.sec() - m_atvLastCheck.sec())
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
        m_atvLastCheck = tv;

        for(mapResponseRecordSt::iterator iter= m_mapResponseRecordSt.begin(); iter!=m_mapResponseRecordSt.end(); ++iter)
        {
            ResponseRecordSt& objResponseRecord = (ResponseRecordSt&)iter->second;

            if(m_n4ExpectTime <= (int)((uint64)tv.get_msec() - objResponseRecord.m_u8StartTime))
            {
                //应答时间超过期望时间限制
                OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]Response time too long m_n4ExpectTime:%d.\n",m_n4ExpectTime));
            }
        }
    }

    return 0;
}



