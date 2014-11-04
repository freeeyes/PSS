#ifndef _LOGFILE_H
#define _LOGFILE_H

#include "define.h"
#include "ace/Date_Time.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"

class CLogFile
{
public:
	CLogFile();
	~CLogFile();

	void SetRoot(const char* pRoot);
	void SetLogName(const char* pLogName);

	void DoLog(const char* fmt, ...);

private:
	void CheckTime();
	void CreateFile(ACE_Date_Time& dt);

private:
	char m_szFileName[MAX_BUFF_50];
	char m_szRoot[MAX_BUFF_50];
	char m_szLogPath[MAX_BUFF_100];
	char m_szLogTime[MAX_BUFF_50];

	ACE_FILE_Connector  m_Connector;          //I/O²Ù×÷Á¬½ÓÆ÷
	ACE_FILE_IO         m_File;
	ACE_FILE_Addr       m_FileAddr; 
};

#endif
