#include "FileLogger.h"



//********************************************************

CFileLogger::CFileLogger()
{
	m_nCount         = 0;
	m_u4BlockSize    = 0;
	m_u4PoolCount    = 0;
	m_u4CurrLogLevel = 0;
	m_szLogRoot[0]   = '\0';
}

CFileLogger::~CFileLogger()
{
	OUR_DEBUG((LM_INFO, "[CFileLogger::~CFileLogger].\n"));
	for(mapLogFile::iterator b = m_mapLogFile.begin(); b!= m_mapLogFile.end(); b++)
	{
		CLogFile* pLogFile = (CLogFile* )b->second;
		SAFE_DELETE(pLogFile);
	}

	m_mapLogFile.clear();
	m_vecLogType.clear();
	OUR_DEBUG((LM_INFO, "[CFileLogger::~CFileLogger]End.\n"));
}

int CFileLogger::DoLog(int nLogType, _LogBlockInfo* pLogBlockInfo)
{
	mapLogFile::iterator f = m_mapLogFile.find(nLogType);
	if(f == m_mapLogFile.end())
	{
		return -1;
	}
	else
	{
		CLogFile* pLogFile = (CLogFile* )f->second;
		pLogFile->doLog(pLogBlockInfo);
	}		

	return 0;
}

int CFileLogger::GetLogTypeCount()
{
	return (int)m_vecLogType.size();
}

int CFileLogger::GetLogType(int nIndex)
{
	if(nIndex >= (int)m_vecLogType.size())
	{
		return 0;
	}

	return (int)m_vecLogType[nIndex];
}

bool CFileLogger::Init()
{
	CXmlOpeation objXmlOpeation;
	uint16 u2LogID                  = 0;
	uint8  u1FileClass              = 0;
	uint8  u1DisPlay                = 0;
	uint32 u4LogLevel               = 0; 
	char szFile[MAX_BUFF_1024]      = {'\0'};
	char szFileName[MAX_BUFF_100]   = {'\0'};
	char szServerName[MAX_BUFF_100] = {'\0'};
	char* pData = NULL;

	Close();

	sprintf_safe(szFile, MAX_BUFF_1024, "%s", FILELOG_CONFIG);
	if(false == objXmlOpeation.Init(szFile))
	{
		OUR_DEBUG((LM_ERROR,"[CFileLogger::Init] Read Configfile[%s] failed\n", szFile));
		return false; 
	}

	//得到服务器名称
	pData = objXmlOpeation.GetData("ServerLogHead", "Text");
	if(pData != NULL)
	{
		sprintf_safe(szServerName, MAX_BUFF_100, "%s", pData);
	}
	OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strServerName=%s\n", szServerName));	

	//得到绝对路径
	pData = objXmlOpeation.GetData("LogPath", "Path");
	if(pData != NULL)
	{
		sprintf_safe(m_szLogRoot, MAX_BUFF_100, "%s", pData);
	}
	OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]m_strRoot=%s\n", m_szLogRoot));

	//得到日志池配置信息，日志块的大小
	pData = objXmlOpeation.GetData("LogPool", "BlockSize");
	if(pData != NULL)
	{
		m_u4BlockSize = (uint32)ACE_OS::atoi(pData);
	}

	//得到日志池配置信息，缓冲池中日志块的个数
	pData = objXmlOpeation.GetData("LogPool", "PoolCount");
	if(pData != NULL)
	{
		m_u4PoolCount = (uint32)ACE_OS::atoi(pData);
	}

	//得到日志池中的当前日志级别
	//此功能感谢宇/ka程枫 的好想法，赞一个，积少成多就会汇聚洪流
	pData = objXmlOpeation.GetData("LogLevel", "CurrLevel");
	if(pData != NULL)
	{
		m_u4CurrLogLevel = (uint32)ACE_OS::atoi(pData);
	}

	//添加子类的个数
	TiXmlElement* pNextTiXmlElement        = NULL;
	TiXmlElement* pNextTiXmlElementPos     = NULL;
	TiXmlElement* pNextTiXmlElementIdx     = NULL;
	TiXmlElement* pNextTiXmlElementDisplay = NULL;
	TiXmlElement* pNextTiXmlElementLevel   = NULL;

	while(true)
	{
		//得到日志id
		pData = objXmlOpeation.GetData("LogInfo", "logid", pNextTiXmlElementIdx);  
		if(pData != NULL)
		{
			u2LogID = (uint16)atoi(pData);                                                      
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u2LogID=%d\n", u2LogID));
		}
		else
		{
			break;
		}

		//得到日志名称
		pData = objXmlOpeation.GetData("LogInfo", "logname", pNextTiXmlElement);
		if(pData != NULL)
		{
			sprintf_safe(szFileName, MAX_BUFF_100, "%s", pData);
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strFileValue=%s\n", szFileName));
		}
		else
		{
			break;
		}

		//得到日志类型
		pData = objXmlOpeation.GetData("LogInfo", "logtype", pNextTiXmlElementPos);  
		if(pData != NULL)
		{
			u1FileClass = (uint8)atoi(pData);                                                      
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1FileClass=%d\n", u1FileClass));
		}
		else
		{
			break;
		}

		//得到日志输出来源，0为输出到文件，1为输出到屏幕
		pData = objXmlOpeation.GetData("LogInfo", "Display", pNextTiXmlElementDisplay);  
		if(pData != NULL)
		{
			u1DisPlay = (uint8)atoi(pData);                                                      
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1DisPlay=%d\n", u1DisPlay));
		}
		else
		{
			break;
		}

		//得到日志当前级别
		pData = objXmlOpeation.GetData("LogInfo", "Level", pNextTiXmlElementLevel);  
		if(pData != NULL)
		{
			u4LogLevel = (uint32)atoi(pData);                                                      
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u4LogLevel=%d\n", u4LogLevel));
		}
		else
		{
			break;
		}

		//只有大于等于u4CurrLogLevel才会入库
		if(u4LogLevel >= m_u4CurrLogLevel)
		{
			//添加到管理日志文件对象map中
			mapLogFile::iterator f = m_mapLogFile.find(u2LogID);

			if(f != m_mapLogFile.end())
			{
				continue;
			}

			CLogFile* pLogFile = new CLogFile(m_szLogRoot, m_u4BlockSize);

			pLogFile->SetLoggerName(szFileName);
			pLogFile->SetLoggerType((int)u2LogID);
			pLogFile->SetLoggerClass((int)u1FileClass);
			pLogFile->SetServerName(szServerName);
			pLogFile->SetDisplay(u1DisPlay);
			pLogFile->Run();

			m_mapLogFile.insert(mapLogFile::value_type(pLogFile->GetLoggerType(), pLogFile));
			m_vecLogType.push_back(u2LogID);
		}
	}

	return true;
}


bool CFileLogger::ReSet(uint32 u4CurrLogLevel)
{
	CXmlOpeation objXmlOpeation;
	uint16 u2LogID                  = 0;
	uint8  u1FileClass              = 0;
	uint8  u1DisPlay                = 0;
	uint32 u4LogLevel               = 0; 
	char szFile[MAX_BUFF_1024]      = {'\0'};
	char szFileName[MAX_BUFF_100]   = {'\0'};
	char szServerName[MAX_BUFF_100] = {'\0'};
	char* pData = NULL;

	Close();

	sprintf_safe(szFile, MAX_BUFF_1024, "%s%s", App_MainConfig::instance()->GetModulePath(), FILELOG_CONFIG);
	if(false == objXmlOpeation.Init(szFile))
	{
		OUR_DEBUG((LM_ERROR,"[CFileLogger::Init] Read Configfile[%s] failed\n", szFile));
		return false; 
	}

	//得到服务器名称
	pData = objXmlOpeation.GetData("ServerLogHead", "Text");
	if(pData != NULL)
	{
		sprintf_safe(szServerName, MAX_BUFF_100, "%s", pData);
	}
	OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strServerName=%s\n", szServerName));	

	//得到绝对路径
	pData = objXmlOpeation.GetData("LogPath", "Path");
	if(pData != NULL)
	{
		sprintf_safe(m_szLogRoot, MAX_BUFF_100, "%s", pData);
	}
	OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]m_strRoot=%s\n", m_szLogRoot));

	m_u4CurrLogLevel = u4CurrLogLevel;

	//添加子类的个数
	TiXmlElement* pNextTiXmlElement        = NULL;
	TiXmlElement* pNextTiXmlElementPos     = NULL;
	TiXmlElement* pNextTiXmlElementIdx     = NULL;
	TiXmlElement* pNextTiXmlElementDisplay = NULL;
	TiXmlElement* pNextTiXmlElementLevel   = NULL;

	while(true)
	{
		//得到日志id
		pData = objXmlOpeation.GetData("LogInfo", "logid", pNextTiXmlElementIdx);  
		if(pData != NULL)
		{
			u2LogID = (uint16)atoi(pData);                                                      
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u2LogID=%d\n", u2LogID));
		}
		else
		{
			break;
		}

		//得到日志名称
		pData = objXmlOpeation.GetData("LogInfo", "logname", pNextTiXmlElement);
		if(pData != NULL)
		{
			sprintf_safe(szFileName, MAX_BUFF_100, "%s", pData);
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strFileValue=%s\n", szFileName));
		}
		else
		{
			break;
		}

		//得到日志类型
		pData = objXmlOpeation.GetData("LogInfo", "logtype", pNextTiXmlElementPos);  
		if(pData != NULL)
		{
			u1FileClass = (uint8)atoi(pData);                                                      
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1FileClass=%d\n", u1FileClass));
		}
		else
		{
			break;
		}

		//得到日志输出来源，0为输出到文件，1为输出到屏幕
		pData = objXmlOpeation.GetData("LogInfo", "Display", pNextTiXmlElementDisplay);  
		if(pData != NULL)
		{
			u1DisPlay = (uint8)atoi(pData);                                                      
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1DisPlay=%d\n", u1DisPlay));
		}
		else
		{
			break;
		}

		//得到日志当前级别
		pData = objXmlOpeation.GetData("LogInfo", "Level", pNextTiXmlElementLevel);  
		if(pData != NULL)
		{
			u4LogLevel = (uint32)atoi(pData);                                                      
			OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u4LogLevel=%d\n", u4LogLevel));
		}
		else
		{
			break;
		}

		//只有大于等于u4CurrLogLevel才会入库
		if(u4LogLevel >= m_u4CurrLogLevel)
		{
			//添加到管理日志文件对象map中
			mapLogFile::iterator f = m_mapLogFile.find(u2LogID);

			if(f != m_mapLogFile.end())
			{
				continue;
			}

			CLogFile* pLogFile = new CLogFile(m_szLogRoot, m_u4BlockSize);

			pLogFile->SetLoggerName(szFileName);
			pLogFile->SetLoggerType((int)u2LogID);
			pLogFile->SetLoggerClass((int)u1FileClass);
			pLogFile->SetServerName(szServerName);
			pLogFile->SetDisplay(u1DisPlay);
			pLogFile->Run();

			m_mapLogFile.insert(mapLogFile::value_type(pLogFile->GetLoggerType(), pLogFile));
			m_vecLogType.push_back(u2LogID);
		}
	}

	return true;
}


bool CFileLogger::Close()
{
	for(mapLogFile::iterator b = m_mapLogFile.begin(); b != m_mapLogFile.end(); b++)
	{
		CLogFile* pLogFile = (CLogFile* )b->second;
		delete pLogFile;
	}

	m_mapLogFile.clear();
	m_vecLogType.clear();
	m_nCount = 0;

	return true;
}

uint32 CFileLogger::GetBlockSize()
{
	return m_u4BlockSize;
}

uint32 CFileLogger::GetPoolCount()
{
	return m_u4PoolCount;
}

uint32 CFileLogger::GetCurrLevel()
{
	return m_u4CurrLogLevel;
}

uint16 CFileLogger::GetLogID(uint16 u2Index)
{
	if(u2Index >= m_vecLogType.size())
	{
		return 0;
	}
	else
	{
		return m_vecLogType[u2Index];
	}
}

char* CFileLogger::GetLogInfoByServerName(uint16 u2LogID)
{
	mapLogFile::iterator f = m_mapLogFile.find(u2LogID);

	if(f != m_mapLogFile.end())
	{
		CLogFile* pLogFile = (CLogFile* )f->second;
		return (char* )pLogFile->GetServerName().c_str();
	}
	else
	{
		return NULL;
	}
}

char* CFileLogger::GetLogInfoByLogName(uint16 u2LogID)
{
	mapLogFile::iterator f = m_mapLogFile.find(u2LogID);

	if(f != m_mapLogFile.end())
	{
		CLogFile* pLogFile = (CLogFile* )f->second;
		return (char* )pLogFile->GetLoggerName().c_str();
	}
	else
	{
		return NULL;
	}
}

int CFileLogger::GetLogInfoByLogDisplay(uint16 u2LogID)
{
	mapLogFile::iterator f = m_mapLogFile.find(u2LogID);

	if(f != m_mapLogFile.end())
	{
		CLogFile* pLogFile = (CLogFile* )f->second;
		return pLogFile->GetDisPlay();
	}
	else
	{
		return NULL;
	}
}
