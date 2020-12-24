#include "FileTestManager.h"

FileTestResultInfoSt CFileTestManager::FileTestStart(const char* szXmlFileTestName)
{
    FileTestResultInfoSt objFileTestResult;

    if(m_bFileTesting)
    {
        PSS_LOGGER_DEBUG("[CProConnectAcceptManager::FileTestStart]m_bFileTesting:{0}.",m_bFileTesting);
        objFileTestResult.n4Result = FILE_TEST_RESULT::RESULT_ERR_TESTING;
        return objFileTestResult;
    }
    else
    {
        if(!LoadXmlCfg(szXmlFileTestName, objFileTestResult))
        {
            PSS_LOGGER_DEBUG("[CProConnectAcceptManager::FileTestStart]Loading config file error filename:{0}.", szXmlFileTestName);
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
        PSS_LOGGER_DEBUG("[CFileTestManager::HandlerServerResponse]Response time too long m_u4ExpectTimeNo find connectID={0}.\n", u4ConnectID);
        return;
    }

    auto pResponseRecord = f->second;
    if((uint32)(pResponseRecord->m_u1ResponseCount + 1) == m_u4ResponseCount)
    {
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
    PSS_LOGGER_DEBUG("[CProConnectAcceptManager::LoadXmlCfg]Filename = {0}.", szXmlFileTestName);

    if(false == m_MainConfig.Init(szXmlFileTestName))
    {
        PSS_LOGGER_DEBUG("[CMainConfig::LoadXmlCfg]File Read Error = %{0}.", szXmlFileTestName);
        objFileTestResult.n4Result = FILE_TEST_RESULT::RESULT_ERR_CFGFILE;
        return false;
    }

    //获得相对路径
    m_strProFilePath = "./";
    m_MainConfig.Read_XML_Data_Single_String("FileTestConfig", "Path", m_strProFilePath);

    //获得定时执行时间间隔
    m_u4TimeInterval = 10;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "TimeInterval", m_u4TimeInterval);
    PSS_LOGGER_DEBUG("[CProConnectAcceptManager::LoadXmlCfg]m_u4TimeInterval = {0}.", m_u4TimeInterval);
    objFileTestResult.n4TimeInterval = (int32)m_u4TimeInterval;

    //获得连接总数
    m_u4ConnectCount = 10;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "ConnectCount", m_u4ConnectCount);
    PSS_LOGGER_DEBUG("[CProConnectAcceptManager::LoadXmlCfg]m_u4ConnectCount = {0}.", m_u4ConnectCount);
    objFileTestResult.n4ConnectNum = (int32)m_u4ConnectCount;

    m_u4ResponseCount = 1;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "ResponseCount", m_u4ResponseCount);
    PSS_LOGGER_DEBUG("[CProConnectAcceptManager::LoadXmlCfg]m_u4ResponseCount = {0}.", m_u4ResponseCount);

    m_u4ExpectTime = 1000;
    m_MainConfig.Read_XML_Data_Single_Uint32("FileTestConfig", "ExpectTime", m_u4ExpectTime);
    PSS_LOGGER_DEBUG("[CProConnectAcceptManager::LoadXmlCfg]m_u4ExpectTime = {0}.", m_u4ExpectTime);

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
        PSS_LOGGER_DEBUG("[CMainConfig::ReadTestFile]Open filename:{0} Error.", pFileName);
        return FILE_TEST_RESULT::RESULT_ERR_PROFILE;
    }

    ACE_FILE_Info fInfo;

    if (ioFile.get_info(fInfo) == -1)
    {
        PSS_LOGGER_DEBUG("[CMainConfig::ReadTestFile]Get file info filename:{0} Error.", pFileName);
        ioFile.close();
        return FILE_TEST_RESULT::RESULT_ERR_PROFILE;
    }

    if (MAX_BUFF_10240 - 1 < fInfo.size_)
    {
        PSS_LOGGER_DEBUG("[CMainConfig::LoadXmlCfg]Protocol file too larger filename:{0}.", pFileName);
        ioFile.close();
        return FILE_TEST_RESULT::RESULT_ERR_PROFILE;
    }
    else
    {
        char szFileContent[MAX_BUFF_10240] = { '\0' };
        ssize_t u4Size = ioFile.recv(szFileContent, fInfo.size_);

        if (u4Size != fInfo.size_)
        {
            PSS_LOGGER_DEBUG("[CMainConfig::LoadXmlCfg]Read protocol file error filename:{0} Error.", pFileName);
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
                PSS_LOGGER_DEBUG("[CMainConfig::LoadXmlCfg]u4Size:{0}.", u4Size);
                PSS_LOGGER_DEBUG("[CMainConfig::LoadXmlCfg]m_szData:{0}.", objFileTestDataInfo.m_szData);
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
        PSS_LOGGER_DEBUG("[CMainConfig::AddResponseRecordList]AddResponseRecordList error.");
        return false;
    }
}



