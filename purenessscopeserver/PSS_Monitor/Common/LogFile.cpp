#include "LogFile.h"

CLogFile::CLogFile()
{
}

CLogFile::~CLogFile()
{
}

void CLogFile::SetRoot(const char* pRoot)
{
	sprintf_safe(m_szRoot, MAX_BUFF_50, "%s", pRoot);

	//如果目录不存在则创建之
	char szPath[MAX_BUFF_100] = {'\0'};
	sprintf_safe(szPath, MAX_BUFF_100, "%s/MonitorLog/", m_szRoot);
	ACE_OS::mkdir(szPath);
}

void CLogFile::SetLogName(const char* pLogName)
{
	sprintf_safe(m_szFileName, MAX_BUFF_50, "%s", pLogName);

	//如果目录不存在则创建之
	char szPath[MAX_BUFF_100] = {'\0'};
	sprintf_safe(szPath, MAX_BUFF_100, "%s/MonitorLog/%s/", m_szRoot, pLogName);
	sprintf_safe(m_szLogPath, MAX_BUFF_100, "%s/MonitorLog/%s/", m_szRoot, pLogName);
	ACE_OS::mkdir(szPath);

	//设置当前日志时间
	CheckTime();
}

void CLogFile::CheckTime()
{
	ACE_Date_Time dt;
	dt.update(ACE_OS::gettimeofday());
	char szDate[MAX_BUFF_50] = {'\0'};
	sprintf_safe(szDate, MAX_BUFF_50, "%04d-%02d-%02d", dt.year(), dt.month(), dt.day());

	if(ACE_OS::strcmp(szDate, m_szLogTime) != 0)
	{
		sprintf_safe(m_szLogTime, MAX_BUFF_50, "%s", szDate);
		CreateFile(dt);
	}
}

void CLogFile::CreateFile(ACE_Date_Time& dt)
{
	char szFileName[MAX_BUFF_100] = {'\0'};
	bool blFileExist = false;

	m_File.close();

	sprintf_safe(szFileName, MAX_BUFF_100, "%s%s_%04d%02d%02d.xml", m_szLogPath, m_szFileName, dt.year(), dt.month(), dt.day());

	//判断当前文件是否存在
	int nRet = ACE_OS::access(szFileName, 0);
	if(-1 != nRet)
	{
		//文件已经存在
		blFileExist = true;
	}

	m_FileAddr.set((ACE_TCHAR *)szFileName);
	if(m_Connector.connect(m_File, m_FileAddr, 0, ACE_Addr::sap_any, 0, O_WRONLY|O_CREAT|O_APPEND) == -1)
	{
		OUR_DEBUG((LM_INFO,"[ServerLogger]Create file error[%s].\n", szFileName));
	}
	else
	{
		OUR_DEBUG((LM_INFO,"[ServerLogger]Create file ok[%s].\n",szFileName));
	}

	if(false == blFileExist)
	{
		//写XML头
		char szLog[MAX_BUFF_1000]     = {'\0'};
		sprintf_safe(szLog, MAX_BUFF_1000, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		m_File.send(szLog, strlen(szLog));
	}
}

void CLogFile::DoLog(const char* fmt, ...)
{
	char szDateBuff[MAX_BUFF_100] = {'\0'};
	char szLog[MAX_BUFF_1000]     = {'\0'};

	va_list ap;
	va_start(ap, fmt);
	ACE_OS::vsnprintf(szLog, MAX_BUFF_1000 - 1, fmt, ap);
	va_end(ap);

	//检测日期
	CheckTime();

	//计入日志
	int nLen = m_File.send(szLog, strlen(szLog));
	if(nLen != (int)strlen(szLog))
	{
		OUR_DEBUG((LM_INFO,"[%s]Write error[%s].\n", m_szFileName, szLog));
	}
}


