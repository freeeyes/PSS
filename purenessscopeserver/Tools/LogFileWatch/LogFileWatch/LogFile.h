#ifndef _LOGFILE_H
#define _LOGFILE_H

#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_time.h"
#include "ace/Date_Time.h"

#include "LogFileBase.h"

//实现一个简单的日志监控


class CLogFile : public ILogFileBase
{
public:
	CLogFile(void);
	virtual ~CLogFile(void);

	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

	void GetFileName(char* pFileName, int nLen);
	bool Check_Log_File_State();


private:
	bool Check_File_Update_time();
	bool Check_File_Size();
	bool Check_File_Last_Line();
	bool Exec_Shell_Command();
};
#endif
