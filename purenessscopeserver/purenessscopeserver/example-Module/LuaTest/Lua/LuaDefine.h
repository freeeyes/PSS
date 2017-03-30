#ifndef _LUADEFINE_H
#define _LUADEFINE_H

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tolua++.h"
};

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/timeb.h>

//计算时间成本的宏
#define _ENTRYTIME { timeb tBegin; ftime(&tBegin);

#define _LEAVETIME timeb tEnd; ftime(&tEnd); printf("[TimeCost]Cost = %d.\n", (tEnd.time * 1000 + tEnd.millitm - tBegin.time * 1000 - tBegin.millitm)); }

#ifdef WIN32
#define MEMCOPY_SAFE(x, y, len) memcpy_s(x, len, y, len);
#else
#define MEMCOPY_SAFE(x, y, len) memcpy(x, y, len);
#endif

#endif
