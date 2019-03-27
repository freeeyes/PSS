#ifdef _LINUX_PLATFORM_


#include <stdio.h>
#include <stdarg.h>


static int sprintf_s(
    char* buffer,
    size_t sizeOfBuffer,
    const char* format,...)
{
    va_list ap;

    va_start(ap, format);
    int res = vsprintf(buffer, format, ap);
    va_end(ap);
    return res;
}

static int printf_s(
    const char* format,...)
{
    va_list ap;

    va_start(ap, format);
    int res = vprintf(format, ap);
    va_end(ap);
    return res;
}

#define memcpy_s(dest,num,src,count) memcpy(dest,src,count)
#define fprintf_s fprintf
#define _strdate_s(buf,num) _strdate(buf)
#define strcat_s(dest,num,src) strcat(dest,src)
#define fopen_s(pf,name,mode) *pf=fopen(name,mode)
#define strncpy_s(dest,num,src,count) strncpy(dest,src,count)
#define localtime_s(tm,time) *tm=*localtime(time)
#define _strdup strdup


#endif



