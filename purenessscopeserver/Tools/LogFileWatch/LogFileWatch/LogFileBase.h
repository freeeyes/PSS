#ifndef _LOGFILEBASE_H
#define _LOGFILEBASE_H

//日志监控的基类，必须继承实现内容
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"

#define LOG_PATH_MAX_LENGTH      200

struct _LogFileInfo
{
	int  m_nID;                               //ID  
	char m_szLogPath[LOG_PATH_MAX_LENGTH];    //日志路径
	char m_szFileKey[LOG_PATH_MAX_LENGTH];    //日志关键字
	char m_szShellCmd[LOG_PATH_MAX_LENGTH];   //要执行的Shell命令（一般是一个shell命令脚本）
	int  m_nWatchTime;                        //日志监控时间周期，单位是秒

	_LogFileInfo()
	{
		m_nID           = 0;
		m_szLogPath[0]  = '\0';
		m_szFileKey[0]  = '\0';
		m_szShellCmd[0] = '\0';
		m_nWatchTime    = 0;
	}
};

class ILogFileBase : public ACE_Task<ACE_MT_SYNCH>
{
public:
	ILogFileBase() {};
	virtual ~ILogFileBase() {};

public:
	void Set_Log_Path(const char* pLogFile) { ACE_OS::snprintf(m_objLogFileInfo.m_szLogPath, LOG_PATH_MAX_LENGTH, "%s", pLogFile); };
	char* Get_Log_Path() { return m_objLogFileInfo.m_szLogPath; };
	void Set_File_Key(const char* pFileKey) {  ACE_OS::snprintf(m_objLogFileInfo.m_szFileKey, LOG_PATH_MAX_LENGTH, "%s", pFileKey); };
	char* Get_File_Key() { return m_objLogFileInfo.m_szFileKey; };
	void Set_Watch_Time(int nWatchTime) { m_objLogFileInfo.m_nWatchTime = nWatchTime; };
	int Get_Watch_Time() { return m_objLogFileInfo.m_nWatchTime; };
	void Set_Shell_Command(const char* pShellCmd) { ACE_OS::snprintf(m_objLogFileInfo.m_szShellCmd, LOG_PATH_MAX_LENGTH, "%s", pShellCmd); };
	char* Get_Shell_Command() { return m_objLogFileInfo.m_szShellCmd; };
	void Set_ID(int nID) { m_objLogFileInfo.m_nID = nID; };
	int Get_ID() { return m_objLogFileInfo.m_nID; };

	//以下需要继承类实现
	//需要继承类实现，获得文件名称
	virtual void GetFileName(char* pFileName, int nLen) = 0;
	//需要继承类实现，判定日志文件是否正常
	virtual bool Check_Log_File_State()                 = 0;

private:
	_LogFileInfo m_objLogFileInfo;
};

#endif
