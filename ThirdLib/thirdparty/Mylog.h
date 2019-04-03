#ifndef __LOG2FILE_H_
#define __LOG2FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdarg.h>
#include <errno.h>

#include <fstream>
#include <sstream>
#include <ctime>
#include <time.h>
#include <iostream>
#include <chrono>
#include <mutex>


using  namespace std;

inline std::string getCurrentSystemTime()
{
    auto time_now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(time_now);
    auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
    auto duration_in_s = std::chrono::duration_cast<std::chrono::seconds>(time_now.time_since_epoch());
    int theMs = duration_in_ms.count() - duration_in_s.count() * 1000;
    struct tm* ptm = localtime(&tt);
    char date[60] = { 0 };
    sprintf(date, "%d-%02d-%02d-%02d.%02d.%02d.%03d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec, theMs);
    return std::string(date);
}


//整数类型文件行号 ->转换为string类型
inline std::string int2string(int line) 
{
	std::ostringstream oss;
	oss << line;
	return oss.str();
}


//定义一个在日志后添加 文件名 函数名 行号 的宏定义
#define suffix_debug(msg)  std::string(getCurrentSystemTime()).append(" [")\
        .append("DEBUG]     ").append(__FILE__).append(":").append(__func__)\
        .append(":").append(int2string(__LINE__))\
        .append("   ").append(msg).append(" ")

#define suffix_warn(msg)  std::string(getCurrentSystemTime()).append(" [")\
        .append("WARNING]   ").append(__FILE__).append(":").append(__func__)\
        .append(":").append(int2string(__LINE__))\
        .append("   ").append(msg).append(" ")

#define suffix_info(msg)  std::string(getCurrentSystemTime()).append(" [")\
        .append("INFO]      ").append(__FILE__).append(":").append(__func__)\
        .append(":").append(int2string(__LINE__))\
        .append("   ").append(msg).append(" ")

#define suffix_error(msg)  std::string(getCurrentSystemTime()).append(" [")\
        .append("ERROR]     ").append(__FILE__).append(":").append(__func__)\
        .append(":").append(int2string(__LINE__))\
        .append("   ").append(msg).append(" ")



/*
 * 功能：log日志类，输入信息到文件
*/
class CLog2File
{
public:
    static CLog2File& Initialize( );
	~CLog2File(void);

private:
	CLog2File( );
    static CLog2File* pInstance ;

public:
	//初始化log，设置文件名（文件路径），打开文件
	bool InitLogFile(const std::string &szFileName);

	//停止log，关闭文件
	void CloseLogFile();

    void WriteLog(const std::string &x);

private:
	std::string m_FileName;		//文件名称
    std::ofstream m_LogFile;


};


#ifdef _LOG2FILE_
CLog2File &m_log = CLog2File::Initialize();
#else
extern CLog2File &m_log;
#endif


//缩短并简化函数调用形式
#define logError(msg) m_log.WriteLog( suffix_error(msg))
#define logWarn(msg) m_log.WriteLog( suffix_warn(msg))
#define logInfo(msg) m_log.WriteLog( suffix_info(msg))
#define logDebug(msg) m_log.WriteLog( suffix_debug(msg))





#endif  //__LOG2FILE_H_

