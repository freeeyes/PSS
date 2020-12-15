#pragma once

#include "define.h"

#if PSS_PLATFORM == PLATFORM_WIN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

//实现不同OS的动态库加载
//add by freeeyes

#if PSS_PLATFORM == PLATFORM_WIN
using Pss_Library_Handler = HINSTANCE;
#else
using Pss_Library_Handler = void*;
#endif

class CLoadLibrary
{
public:
    static Pss_Library_Handler PSS_dlopen(const char* pFilePath, const int nMode)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        PSS_UNUSED_ARG(nMode);
        return ::LoadLibrary(pFilePath);
#else
        return dlopen(pFilePath, nMode);
#endif
    };

    static void* PSS_dlsym(Pss_Library_Handler h, const char* pFuncName)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        return ::GetProcAddress(h, pFuncName);
#else
        return dlsym(h, pFuncName);
#endif
    };

    static char* PSS_dlerror()
    {
#if PSS_PLATFORM == PLATFORM_WIN
        static char buf[128] = { '\0' };
        ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
            0,
            ::GetLastError(),
            0,
            buf,
            sizeof buf / sizeof buf[0],
            0);

        return buf;
#else
        return dlerror();
#endif
    };

    static void PSS_dlClose(Pss_Library_Handler h)
    {
#if PSS_PLATFORM == PLATFORM_WIN
        ::FreeLibrary(h);
#else
        dlclose(h);
#endif
    };
};
