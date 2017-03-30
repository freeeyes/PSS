#ifndef _WINDOWSDUMP_H
#define _WINDOWSDUMP_H

#ifdef WIN32

#include <Windows.h>
#include <dbghelp.h>

#define DUMP_FILE ".\\WindowsP.dmp"

void CreateDumpFile(LPCSTR lpstrDumpFilePathName, EXCEPTION_POINTERS* pException)
{
    // 创建Dump文件
    //
    HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Dump信息
    //
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    // 写入Dump文件内容
    //
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
    // 这里弹出一个错误对话框并退出程序
    CreateDumpFile(DUMP_FILE, pException);
    FatalAppExit(-1,  "*** Unhandled Exception! ***");

    return EXCEPTION_EXECUTE_HANDLER;
}

#endif

#endif
