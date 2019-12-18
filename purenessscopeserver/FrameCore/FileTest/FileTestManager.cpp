#include "FileTestManager.h"

CFileTestManager::CFileTestManager(void)
{
    m_bFileTesting     = false;
    m_n4TimerID        = 0;
    m_u4ConnectCount   = 0;
    m_u4ParseID        = 0;
    m_u4ResponseCount  = 0;
    m_u4ExpectTime     = 1000;
    m_u4ContentType    = 1;
    m_u4TimeInterval   = 0;
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
            m_n4TimerID = App_TimerManager::instance()->schedule(this, (void*)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(m_u4TimeInterval), ACE_Time_Value(m_u4TimeInterval));

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
    char szConnectID[10] = { '\0' };
    sprintf_safe(szConnectID, 10, "%d", u4ConnectID);

    ResponseRecordSt* pResponseRecord = m_objResponseRecordList.Get_Hash_Box_Data(szConnectID);

    if (NULL == pResponseRecord)
    {
        OUR_DEBUG((LM_INFO, "[CFileTestManager::HandlerServerResponse]Response time too long m_u4ExpectTimeNo find connectID=%d.\n", u4ConnectID));
        return;
    }

    if((uint32)(pResponseRecord->m_u1ResponseCount + 1) == m_u4ResponseCount)
    {
        ACE_Time_Value atvResponse = ACE_OS::gettimeofday();

        if(m_u4ExpectTime <= (uint32)((uint64)atvResponse.get_msec() - pResponseRecord->m_u8StartTime))
        {
            //应答时间超过期望时间限制
            OUR_DEBUG((LM_INFO, "[CFileTestManager::HandlerServerResponse]Response time too long m_u4ExpectTime:%d.\n",m_u4ExpectTime));
        }

#ifndef WIN32
        App_ConnectManager::instance()->Close(u4ConnectID);
#else
        App_ProConnectManager::instance()->Close(u4ConnectID);
#endif
        //回收对象类型
        m_objResponseRecordList.Del_Hash_Data(szConnectID);
        SAFE_DELETE(pResponseRecord);
    }
    else
    {
        pResponseRecord->m_u1ResponseCount += 1;
    }

}

void CFileTestManager::Close()
{
    //关闭定时器
    if (m_n4TimerID > 0)
    {
        App_TimerManager::instance()->cancel(m_n4TimerID);
        m_n4TimerID = 0;
        m_bFileTesting = false;
    }

    //清理m_objResponseRecordList
    vector<ResponseRecordSt*> vecList;
    m_objResponseRecordList.Get_All_Used(vecList);

    int nUsedSize = (int)vecList.size();

    if (nUsedSize > 0)
    {
        for (int i = 0; i < nUsedSize; i++)
        {
            char szConnectID[10] = { '\0' };
            sprintf_safe(szConnectID, 10, "%d", vecList[i]->m_u4ConnectID);
            m_objResponseRecordList.Del_Hash_Data(szConnectID);
            SAFE_DELETE(vecList[i]);
        }
    }

    m_objResponseRecordList.Close();
}

bool CFileTestManager::LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult)
{
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]Filename = %s.\n", szXmlFileTestName));

    if(false == m_MainConfig.Init(szXmlFileTestName))
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]File Read Error = %s.\n", szXmlFileTestName));
        objFileTestResult.n4Result = RESULT_ERR_CFGFILE;
        return false;
    }

    //获得相对路径
    m_strProFilePath = "./";
    m_MainConfig.Read_XML_Data_Single_String("FileTestConfig", "Path", m_strProFilePath);

    //获得定时执行时间间隔
    m_u4TimeInterval = 10;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "TimeInterval", m_u4TimeInterval);
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_u4TimeInterval = %d.\n", m_u4TimeInterval));
    objFileTestResult.n4TimeInterval = (int32)m_u4TimeInterval;

    //获得连接总数
    m_u4ConnectCount = 10;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "ConnectCount", m_u4ConnectCount);
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_u4ConnectCount = %d.\n", m_u4ConnectCount));
    objFileTestResult.n4ConnectNum = (int32)m_u4ConnectCount;

    m_u4ResponseCount = 1;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "ResponseCount", m_u4ResponseCount);
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_u4ResponseCount = %d.\n", m_u4ResponseCount));

    m_u4ExpectTime = 1000;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "ExpectTime", m_u4ExpectTime);
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_u4ExpectTime = %d.\n", m_u4ExpectTime));

    //默认解析器类型
    m_u4ParseID = 1;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "ParseID", m_u4ParseID);

    //默认是二进制协议
    m_u4ContentType = 1;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "ContentType", m_u4ContentType);

    //命令监控相关配置
    TiXmlElement* pNextTiXmlElementFileName     = NULL;
    TiXmlElement* pNextTiXmlElementDesc         = NULL;

    while(true)
    {
        FileTestDataInfoSt objFileTestDataInfo;
        string strPathFile;
        string strFileName;
        string strFileDesc;

        bool blRet = m_MainConfig.Read_XML_Data_Multiple_String("FileInfo", "FileName", strFileName, pNextTiXmlElementFileName);

        if(false == blRet)
        {
            break;
        }

        strPathFile = m_strProFilePath + strFileName;
        blRet = m_MainConfig.Read_XML_Data_Multiple_String("FileInfo", "Desc", strFileDesc, pNextTiXmlElementDesc);

        if(false == blRet)
        {
            objFileTestResult.vecProFileDesc.push_back(strFileDesc);
        }

        //读取数据包文件内容
        int nReadFileRet = ReadTestFile(strPathFile.c_str(), m_u4ContentType, objFileTestDataInfo);

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

    //初始化连接返回数据数组
    InitResponseRecordList();

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
        ioFile.close();
        return RESULT_ERR_PROFILE;
    }

    if (MAX_BUFF_10240 - 1 < fInfo.size_)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Protocol file too larger filename:%s.\n", pFileName));
        ioFile.close();
        return RESULT_ERR_PROFILE;
    }
    else
    {
        char szFileContent[MAX_BUFF_10240] = { '\0' };
        ssize_t u4Size = ioFile.recv(szFileContent, fInfo.size_);

        if (u4Size != fInfo.size_)
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Read protocol file error filename:%s Error.\n", pFileName));
            ioFile.close();
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

        ioFile.close();
    }

    return RESULT_OK;
}

int CFileTestManager::InitResponseRecordList()
{
    //初始化Hash表
    m_objResponseRecordList.Close();
    m_objResponseRecordList.Init((int)m_u4ConnectCount);

    return 0;
}

bool CFileTestManager::AddResponseRecordList(uint32 u4ConnectID, const ACE_Time_Value& tv)
{

    if (0 != u4ConnectID)
    {
        ResponseRecordSt* pResponseRecord = new ResponseRecordSt();
        pResponseRecord->m_u1ResponseCount = 0;
        pResponseRecord->m_u8StartTime = tv.get_msec();
        pResponseRecord->m_u4ConnectID = u4ConnectID;

        char szConnectID[10] = { '\0' };
        sprintf_safe(szConnectID, 10, "%d", u4ConnectID);

        if (-1 == m_objResponseRecordList.Add_Hash_Data(szConnectID, pResponseRecord))
        {
            OUR_DEBUG((LM_INFO, "[CFileTestManager::AddResponseRecordList]Add m_objResponseRecordList error, ConnectID=%d.\n", u4ConnectID));
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::AddResponseRecordList]AddResponseRecordList error\n"));
        return false;
    }
}

int CFileTestManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    ACE_UNUSED_ARG(arg);
    ACE_UNUSED_ARG(tv);

    //首先遍历上一次定时器的执行数据对象是否已经全部释放
    vector<ResponseRecordSt*> vecList;
    m_objResponseRecordList.Get_All_Used(vecList);

    int nUsedSize = (int)vecList.size();

    if (nUsedSize > 0)
    {
        for (int i = 0; i < nUsedSize; i++)
        {
            //在上一个轮询周期，没有结束的对象
            if(m_u4ExpectTime <= (uint32)((uint64)tv.get_msec() - vecList[i]->m_u8StartTime))
            {
                //超过了执行范围时间
                OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]Response time too long m_u4ExpectTime:%d.\n", m_u4ExpectTime));
                AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_ERROR, "[CFileTestManager::handle_timeout]Response time too long connectID=%d, m_u4ExpectTime=%d.",
                                                      vecList[i]->m_u4ConnectID,
                                                      m_u4TimeInterval);
            }
            else
            {
                //超过了定时器时间
                OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]Response time too long m_u4TimeInterval:%d.\n", m_u4TimeInterval));
                //写入错误日志
                AppLogManager::instance()->WriteLog_i(LOG_SYSTEM_ERROR, "[CFileTestManager::handle_timeout]Response time too long connectID=%d, m_u4TimeInterval=%d.",
                                                      vecList[i]->m_u4ConnectID,
                                                      m_u4TimeInterval);
            }

            char szConnectID[10] = { '\0' };
            sprintf_safe(szConnectID, 10, "%d", vecList[i]->m_u4ConnectID);
            m_objResponseRecordList.Del_Hash_Data(szConnectID);
            SAFE_DELETE(vecList[i]);
        }
    }

#ifndef WIN32
    CConnectHandler* pConnectHandler = NULL;

    for (uint32 iLoop = 0; iLoop < m_u4ConnectCount; iLoop++)
    {
        pConnectHandler = App_ConnectHandlerPool::instance()->Create();

        if (NULL != pConnectHandler)
        {
            //文件数据包测试不需要用到pProactor对象，因为不需要实际发送数据，所以这里可以直接设置一个固定的pProactor
            ACE_Reactor* pReactor = App_ReactorManager::instance()->GetAce_Client_Reactor(0);
            pConnectHandler->reactor(pReactor);
            pConnectHandler->SetPacketParseInfoID(m_u4ParseID);

            uint32 u4ConnectID = pConnectHandler->file_open(dynamic_cast<IFileTestManager*>(this));

            AddResponseRecordList(u4ConnectID, tv);
        }
    }

#else
    CProConnectHandler* ptrProConnectHandle = NULL;

    for (uint32 iLoop = 0; iLoop < m_u4ConnectCount; iLoop++)
    {
        ptrProConnectHandle = App_ProConnectHandlerPool::instance()->Create();

        if (NULL != ptrProConnectHandle)
        {
            //文件数据包测试不需要用到pProactor对象，因为不需要实际发送数据，所以这里可以直接设置一个固定的pProactor
            ACE_Proactor* pPractor = App_ProactorManager::instance()->GetAce_Client_Proactor(0);
            ptrProConnectHandle->proactor(pPractor);
            ptrProConnectHandle->SetPacketParseInfoID(m_u4ParseID);

            uint32 u4ConnectID = ptrProConnectHandle->file_open(dynamic_cast<IFileTestManager*>(this));

            AddResponseRecordList(u4ConnectID, tv);
        }
    }

#endif

    //循环将数据包压入对象
    for (int iLoop = 0; iLoop < (int)m_vecFileTestDataInfoSt.size(); iLoop++)
    {
        FileTestDataInfoSt& objFileTestDataInfo = m_vecFileTestDataInfoSt[iLoop];

        vector<ResponseRecordSt*> vecExistList;
        m_objResponseRecordList.Get_All_Used(vecExistList);

        for (int jLoop = 0; jLoop < (int)vecExistList.size(); jLoop++)
        {
            uint32 u4ConnectID = vecExistList[jLoop]->m_u4ConnectID;
#if PSS_PLATFORM == PLATFORM_WIN
            App_ProConnectManager::instance()->handle_write_file_stream(u4ConnectID, objFileTestDataInfo.m_szData, objFileTestDataInfo.m_u4DataLength, m_u4ParseID);
#else
            App_ConnectManager::instance()->handle_write_file_stream(u4ConnectID, objFileTestDataInfo.m_szData, objFileTestDataInfo.m_u4DataLength, m_u4ParseID);
#endif
        }
    }


    return 0;
}



