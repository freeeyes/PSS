#include "LogFile.h"

#ifndef WIN32
#include <stdio.h>
#include <stdlib.h>
#endif

CLogFile::CLogFile(void)
{
}

CLogFile::~CLogFile(void)
{
}

void CLogFile::GetFileName(char* pFileName, int nLen)
{
	ACE_OS::snprintf(pFileName, nLen, "%s.log", Get_File_Key());
}

bool CLogFile::Check_Log_File_State()
{
	//在这里判断文件状态
	ACE_DEBUG((LM_INFO, "[CLogFile::Check_Log_File_State]Begin Check.\n"));
	Check_File_Update_time();
	ACE_DEBUG((LM_INFO, "[CLogFile::Check_Log_File_State]End.\n"));
	return 0;
}

int CLogFile::handle_timeout( const ACE_Time_Value &tv, const void *arg )
{
	//定时器到时，调用算法判定日志是否符合规范

	//这里测试几个接口
	Check_Log_File_State();
	Check_File_Size();
	Check_File_Last_Line();
	return 0;
}

//检查文件最后更新时间
bool CLogFile::Check_File_Update_time()
{
	ACE_stat objStat;
	char szFileName[50] = {'\0'};

	int nErrID = 0;
	GetFileName(szFileName, 50);
	int nerr = ACE_OS::stat(szFileName, &objStat);
	if(nerr != 0)
	{
		ACE_DEBUG((LM_INFO, "[Check_File_Update_time]error=%d.\n", errno));
		return false;
	}

	time_t ttUpdateTime = objStat.st_mtime;

	time_t ttNow = ACE_OS::time(NULL);

	//判断更新时间是否和当前时间差距超过30秒。
	if(ttNow - ttUpdateTime > 30)
	{
		struct tm *pUpdateTime = NULL;

		pUpdateTime = ACE_OS::localtime(&ttUpdateTime);

		ACE_DEBUG((LM_INFO, "[Check_File_Update_time]File UpDate Time is[%02d-%02d-%02d %02d:%02d:%02d].\n",
			pUpdateTime->tm_year + 1900, 
			pUpdateTime->tm_mon + 1,
			pUpdateTime->tm_mday,
			pUpdateTime->tm_hour,
			pUpdateTime->tm_min,
			pUpdateTime->tm_sec));

		return false;
	}
	else
	{
		return true;
	}
}

//检查文件最后大小
bool CLogFile::Check_File_Size()
{
	ACE_stat objStat;
	char szFileName[50] = {'\0'};

	int nErrID = 0;
	GetFileName(szFileName, 50);
	int nerr = ACE_OS::stat(szFileName, &objStat);
	if(nerr != 0)
	{
		ACE_DEBUG((LM_INFO, "[CLogFile::Check_File_Size]error=%d.\n", errno));
		return false;
	}

	ACE_DEBUG((LM_INFO, "[CLogFile::Check_File_Size]File size is[%d].\n", objStat.st_size));

	return true;
}

//得到文件的最后一行数据
bool CLogFile::Check_File_Last_Line()
{
	char szFileName[50] = {'\0'};

	int nErrID = 0;
	GetFileName(szFileName, 50);

#ifdef WIN32
	ACE_DEBUG((LM_INFO, "[CLogFile::Check_File_Last_Line]windows not support.\n"));
#else
	char szCmd[300] = {'\0'};
	ACE_OS::snprintf(szCmd, 300, "tail -1 %s", szFileName);
	FILE * out = popen(szCmd, "r");
	if(out == NULL)
	{
		ACE_DEBUG((LM_INFO, "[CLogFile::Check_File_Last_Line]No Find File.\n"));
	}
	else
	{
		char szBuff[100] = {'\0'};
		fread(szBuff, sizeof(char), 100 - 1, out);
		ACE_DEBUG((LM_INFO, "Buff=%s", szBuff));
		pclose(out);
	}
#endif

	return true;
}

bool CLogFile::Exec_Shell_Command()
{
	int nErr = ACE_OS::system((ACE_TCHAR* )Get_Shell_Command());
	if(nErr != 0)
	{
		ACE_DEBUG((LM_INFO, "[CLogFile::Exec_Shell_Command]error=%d.\n", errno));
		return false;
	}
	else
	{
		ACE_DEBUG((LM_INFO, "[CLogFile::Exec_Shell_Command]exec success.\n", errno));
		return true;
	}
}