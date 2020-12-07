#include "FileTestManager.h"

FileTestResultInfoSt CFileTestManager::FileTestStart(const char* szXmlFileTestName)
{
    FileTestResultInfoSt objFileTestResult;

    if(m_bFileTesting)
    {
        OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]m_bFileTesting:%d.\n",m_bFileTesting));
        objFileTestResult.n4Result = FILE_TEST_RESULT::RESULT_ERR_TESTING;
        return objFileTestResult;
    }
    else
    {
        if(!LoadXmlCfg(szXmlFileTestName, objFileTestResult))
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]Loading config file error filename:%s.\n", szXmlFileTestName));
        }
    }

    return objFileTestResult;
}

int CFileTestManager::FileTestEnd() const 
{
    return 0;
}

void CFileTestManager::HandlerServerResponse(uint32 u4ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

    auto f = m_objResponseRecordList.find(u4ConnectID);

    if (m_objResponseRecordList.end() == f)
    {
        OUR_DEBUG((LM_INFO, "[CFileTestManager::HandlerServerResponse]Response time too long m_u4ExpectTimeNo find connectID=%d.\n", u4ConnectID));
        return;
    }

    auto pResponseRecord = f->second;
    if((uint32)(pResponseRecord->m_u1ResponseCount + 1) == m_u4ResponseCount)
    {
        ACE_Time_Value atvResponse = ACE_OS::gettimeofday();

        if(m_u4ExpectTime <= (uint32)(atvResponse.get_msec() - pResponseRecord->m_u8StartTime))
        {
            //应答时间超过期望时间限制
            OUR_DEBUG((LM_INFO, "[CFileTestManager::HandlerServerResponse]Response time too long m_u4ExpectTime:%d.\n",m_u4ExpectTime));
        }

        App_HandlerManager::instance()->CloseConnect(u4ConnectID);

        //回收对象类型
        m_objResponseRecordList.erase(f);
    }
    else
    {
        pResponseRecord->m_u1ResponseCount += 1;
    }

}

void CFileTestManager::Close()
{
    //清理m_objResponseRecordList
    m_objResponseRecordList.clear();
}

bool CFileTestManager::LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult)
{
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]Filename = %s.\n", szXmlFileTestName));

    if(false == m_MainConfig.Init(szXmlFileTestName))
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]File Read Error = %s.\n", szXmlFileTestName));
        objFileTestResult.n4Result = FILE_TEST_RESULT::RESULT_ERR_CFGFILE;
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
    TiXmlElement* pNextTiXmlElementFileName     = nullptr;
    TiXmlElement* pNextTiXmlElementDesc         = nullptr;

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
        FILE_TEST_RESULT nReadFileRet = ReadTestFile(strPathFile.c_str(), m_u4ContentType, objFileTestDataInfo);

        if(FILE_TEST_RESULT::RESULT_OK == nReadFileRet)
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

FILE_TEST_RESULT CFileTestManager::ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo) const
{
    ACE_FILE_Connector fConnector;
    ACE_FILE_IO ioFile;
    ACE_FILE_Addr fAddr(pFileName);

    if (fConnector.connect(ioFile, fAddr) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::ReadTestFile]Open filename:%s Error.\n", pFileName));
        return FILE_TEST_RESULT::RESULT_ERR_PROFILE;
    }

    ACE_FILE_Info fInfo;

    if (ioFile.get_info(fInfo) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::ReadTestFile]Get file info filename:%s Error.\n", pFileName));
        ioFile.close();
        return FILE_TEST_RESULT::RESULT_ERR_PROFILE;
    }

    if (MAX_BUFF_10240 - 1 < fInfo.size_)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Protocol file too larger filename:%s.\n", pFileName));
        ioFile.close();
        return FILE_TEST_RESULT::RESULT_ERR_PROFILE;
    }
    else
    {
        char szFileContent[MAX_BUFF_10240] = { '\0' };
        ssize_t u4Size = ioFile.recv(szFileContent, fInfo.size_);

        if (u4Size != fInfo.size_)
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Read protocol file error filename:%s Error.\n", pFileName));
            ioFile.close();
            return FILE_TEST_RESULT::RESULT_ERR_PROFILE;
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

    return FILE_TEST_RESULT::RESULT_OK;
}

int CFileTestManager::InitResponseRecordList()
{
    //初始化Hash表
    m_objResponseRecordList.clear();

    return 0;
}

bool CFileTestManager::AddResponseRecordList(uint32 u4ConnectID, const ACE_Time_Value& tv)
{

    if (0 != u4ConnectID)
    {
        auto pResponseRecord = std::make_shared<ResponseRecordSt>();
        pResponseRecord->m_u1ResponseCount = 0;
        pResponseRecord->m_u8StartTime = tv.get_msec();
        pResponseRecord->m_u4ConnectID = u4ConnectID;

        m_objResponseRecordList[u4ConnectID] = pResponseRecord;
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::AddResponseRecordList]AddResponseRecordList error\n"));
        return false;
    }
}



