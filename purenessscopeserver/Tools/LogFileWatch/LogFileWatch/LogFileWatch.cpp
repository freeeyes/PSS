// LogFileWatch.cpp : 定义控制台应用程序的入口点。
//

#include "ace/pre.h"
#include "ace/Date_Time.h"
#include "ace/OS_NS_unistd.h"

#include "LogFile.h"
#include "XmlOpeation.h"
#include "TimeManager.h"

#include <vector>
using namespace std;

#define CONFIG_NAME "FileWatch.xml"

typedef vector<_LogFileInfo> vecLogFileInfo;

struct _ConsoleInfo
{
	char m_szConsoleIP[20];
	int  m_nPort;
	char m_szKey[50];

	_ConsoleInfo()
	{
		m_szConsoleIP[0] = '\0';
		m_nPort          = 0;
		m_szKey[0]       = '\0';
	}
};

//读取配置文件
bool Init_Read_Config(vecLogFileInfo& objvecLogFileInfo)
{
	CXmlOpeation objMainConfig;
	if(false == objMainConfig.Init(CONFIG_NAME))
	{
		ACE_DEBUG((LM_INFO, "[Init_Read_Config]open config file error.\n"));
		return false;
	}

	TiXmlElement* pNextTiXmlElementID          = NULL;
	TiXmlElement* pNextTiXmlElementPath        = NULL;
	TiXmlElement* pNextTiXmlElementKey         = NULL;
	TiXmlElement* pNextTiXmlElementWatchTime   = NULL;
	TiXmlElement* pNextTiXmlElementShellCmd    = NULL;

	char* pData = NULL;

	while(true)
	{
		_LogFileInfo objLogFileInfo;

		pData = objMainConfig.GetData("FileInfo", "ID", pNextTiXmlElementID);
		if(pData != NULL)
		{
			objLogFileInfo.m_nID = (int)ACE_OS::atoi(pData);
		}
		else
		{
			return true;
		}

		pData = objMainConfig.GetData("FileInfo", "Path", pNextTiXmlElementPath);
		if(pData != NULL)
		{
			ACE_OS::snprintf(objLogFileInfo.m_szLogPath, LOG_PATH_MAX_LENGTH, "%s", pData);
		}
		else
		{
			return true;
		}

		pData = objMainConfig.GetData("FileInfo", "Key", pNextTiXmlElementKey);
		if(pData != NULL)
		{
			ACE_OS::snprintf(objLogFileInfo.m_szFileKey, LOG_PATH_MAX_LENGTH, "%s", pData);
		}
		else
		{
			return true;
		}

		pData = objMainConfig.GetData("FileInfo", "WatchTime", pNextTiXmlElementWatchTime);
		if(pData != NULL)
		{
			objLogFileInfo.m_nWatchTime = (int)ACE_OS::atoi(pData);
		}
		else
		{
			return true;
		}

		pData = objMainConfig.GetData("FileInfo", "ShellCmd", pNextTiXmlElementShellCmd);
		if(pData != NULL)
		{
			ACE_OS::snprintf(objLogFileInfo.m_szShellCmd, LOG_PATH_MAX_LENGTH, "%s", pData);
		}
		else
		{
			return true;
		}

		objvecLogFileInfo.push_back(objLogFileInfo);
	}

	_ConsoleInfo objConsoleInfo;
	pData = objMainConfig.GetData("ConsoleInfo", "IP");
	if(pData != NULL)
	{
		ACE_OS::snprintf(objConsoleInfo.m_szConsoleIP, 20, "%s", pData);
	}
	else
	{
		return true;
	}

	pData = objMainConfig.GetData("ConsoleInfo", "Port");
	if(pData != NULL)
	{
		objConsoleInfo.m_nPort = ACE_OS::atoi(pData);
	}
	else
	{
		return true;
	}

	pData = objMainConfig.GetData("ConsoleInfo", "Keypwd");
	if(pData != NULL)
	{
		ACE_OS::snprintf(objConsoleInfo.m_szKey, 50, "%s", pData);
	}
	else
	{
		return true;
	}

	return true;
}


int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
	vecLogFileInfo objvecLogFileInfo;

	//读取配置文件
	if(false == Init_Read_Config(objvecLogFileInfo))
	{
		getchar();
		return 0;
	}

	CLogFile* pLogFile = NULL;

	//给相关配置添加相应的LOG算法
	for(int i = 0; i < (int)objvecLogFileInfo.size(); i++)
	{
		int nID = objvecLogFileInfo[i].m_nID;
		if(nID = 1)
		{
			//ClogFile处理第一个日志跟踪器
			pLogFile = new CLogFile();
			pLogFile->Set_ID(objvecLogFileInfo[i].m_nID);
			pLogFile->Set_Log_Path(objvecLogFileInfo[i].m_szLogPath);
			pLogFile->Set_File_Key(objvecLogFileInfo[i].m_szFileKey);
			pLogFile->Set_Shell_Command(objvecLogFileInfo[i].m_szShellCmd);
			pLogFile->Set_Watch_Time(objvecLogFileInfo[i].m_nWatchTime);
			
			ILogFileBase* pLogFileBase = (ILogFileBase* )pLogFile;
			//添加定时器
			App_TimerManager::instance()->schedule(pLogFileBase, NULL, 
				ACE_OS::gettimeofday() + ACE_Time_Value(pLogFileBase->Get_Watch_Time()), 
				ACE_Time_Value(pLogFileBase->Get_Watch_Time()));
		}
		else if(nID = 2)
		{
			//这里可以添加你自己的日志判定规则类
		}
	}

	//开启定时器
	App_TimerManager::instance()->activate();

	ACE_Thread_Manager::instance()->wait();

	//清空资源
	delete pLogFile;

	return 0;
}

