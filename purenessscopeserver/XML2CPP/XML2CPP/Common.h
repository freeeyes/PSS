#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <ctype.h>

//最长代码行字数
#define MAX_CODE_LINE_SIZE 1024

inline void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
    if(szText == NULL)
    {
        return;
    }

    va_list ap;
    va_start(ap, fmt);

    vsnprintf(szText, nLen, fmt, ap);
    szText[nLen - 1] = '\0';

    va_end(ap);
};

static void To_Upper_String(const char* pSrc, char* pRet)
{
    int nLen = (int)strlen(pSrc);

    for(int i = 0; i < nLen; i++)
    {
        pRet[i] = toupper(pSrc[i]);
    }

    pRet[nLen] = '\0';
}

#endif

