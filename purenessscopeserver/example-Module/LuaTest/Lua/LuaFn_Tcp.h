#ifndef _LUAFN_MESSION_H
#define _LUAFN_MESSION_H

#include "LuaDefine.h"

//打印调用参数
static int LuaFn_Tcp_Print(lua_State* pState)
{
    int nLen = (int)lua_tonumber(pState, 2);
    char* pData = (char* )lua_tostring(pState, 1);

#if WIN32
    printf_s("[LuaFn_Tcp_Print]nLen=%d.\n", nLen);
#else
    printf("[LuaFn_Tcp_Print]nLen=%d.\n", nLen);
#endif

    return 0;
}

//数据流，流入一个int
static int Lua_Tcp_Buffer_In_Int32(lua_State* pState)
{
    int nCount = lua_gettop(pState);

    int nNextIndex  = -1;
    int nMaxLen     = (int)lua_tonumber(pState, 4);
    int nCurrIndex  = (int)lua_tonumber(pState, 3);
    int nData       = (int)lua_tonumber(pState, 2);
    char* pData     = (char* )lua_touserdata(pState, 1);

    //格式化流入的数据
    if(nCurrIndex + (int)sizeof(int) >= nMaxLen || nCurrIndex < 0 || NULL == pData)
    {
        lua_pushnumber(pState, (int)nNextIndex);
    }
    else
    {
        MEMCOPY_SAFE(&pData[nCurrIndex], &nData, sizeof(int));
        nNextIndex = nCurrIndex + sizeof(int);
        lua_pushnumber(pState, (int)nNextIndex);
    }

    return 1;
}

//数据流，流入一个short
static int Lua_Tcp_Buffer_In_Int16(lua_State* pState)
{
    int nNextIndex  = -1;
    int nMaxLen     = (int)lua_tonumber(pState, 4);
    int nCurrIndex  = (int)lua_tonumber(pState, 3);
    int nData       = (int)lua_tonumber(pState, 2);
    char* pData     = (char* )lua_touserdata(pState, 1);

    //格式化流入的数据
    if(nCurrIndex + (int)sizeof(short) >= nMaxLen || nCurrIndex < 0 || NULL == pData)
    {
        lua_pushnumber(pState, (int)nNextIndex);
    }
    else
    {
        MEMCOPY_SAFE(&pData[nCurrIndex], &nData, sizeof(short));
        nNextIndex = nCurrIndex + sizeof(short);
        lua_pushnumber(pState, (int)nNextIndex);
    }

    return 1;
}

//数据流，流入一个uint8
static int Lua_Tcp_Buffer_In_Int8(lua_State* pState)
{
    int nNextIndex  = -1;
    int nMaxLen     = (int)lua_tonumber(pState, 4);
    int nCurrIndex  = (int)lua_tonumber(pState, 3);
    int nData       = (int)lua_tonumber(pState, 2);
    char* pData     = (char* )lua_touserdata(pState, 1);

    //格式化流入的数据
    if(nCurrIndex + (int)sizeof(char) >= nMaxLen || nCurrIndex < 0 || NULL == pData)
    {
        lua_pushnumber(pState, (int)nNextIndex);
    }
    else
    {
        MEMCOPY_SAFE(&pData[nCurrIndex], &nData, sizeof(char));
        nNextIndex = nCurrIndex + sizeof(char);
        lua_pushnumber(pState, (int)nNextIndex);
    }

    return 1;
}

//数据流，流入一个short
static int Lua_Tcp_Buffer_In_Block(lua_State* pState)
{
    int nNextIndex   = -1;
    int nMaxLen      = (int)lua_tonumber(pState, 5);
    int nCurrIndex   = (int)lua_tonumber(pState, 4);
    int nBlockLength = (int)lua_tonumber(pState, 3);
    char* pBlock     = (char* )lua_tostring(pState, 2);
    char* pData      = (char* )lua_touserdata(pState, 1);

    //格式化流入的数据
    if(nCurrIndex + nBlockLength >= nMaxLen || nCurrIndex < 0 || NULL == pData)
    {
        lua_pushnumber(pState, (int)nNextIndex);
    }
    else
    {
        MEMCOPY_SAFE(&pData[nCurrIndex], pBlock, nBlockLength);
        nNextIndex = nCurrIndex + nBlockLength;
        lua_pushnumber(pState, (int)nNextIndex);
    }

    return 1;
}

//数据流，流出一个int
static int Lua_Tcp_Buffer_Out_Int32(lua_State* pState)
{
    int nCount = lua_gettop(pState);

    int nMaxLength  = (int)lua_tonumber(pState, 3);
    int nCurrIndex  = (int)lua_tonumber(pState, 2);
    char* pData     = (char* )lua_touserdata(pState, 1);

    //格式化流入的数据
    if(nCurrIndex + (int)sizeof(int) >= nMaxLength || nCurrIndex < 0 || NULL == pData)
    {
        lua_pushnumber(pState, (int)0);
    }
    else
    {
        int nData = 0;
        MEMCOPY_SAFE(&nData, &pData[nCurrIndex], sizeof(int));
        lua_pushnumber(pState, (int)nData);
    }

    return 1;
}

//数据流，流出一个short
static int Lua_Tcp_Buffer_Out_Int16(lua_State* pState)
{
    int nCount = lua_gettop(pState);

    int nMaxLength  = (int)lua_tonumber(pState, 3);
    int nCurrIndex  = (int)lua_tonumber(pState, 2);
    char* pData     = (char* )lua_touserdata(pState, 1);

    //格式化流入的数据
    if(nCurrIndex + (int)sizeof(short) >= nMaxLength || nCurrIndex < 0 || NULL == pData)
    {
        lua_pushnumber(pState, (int)0);
    }
    else
    {
        int nData = 0;
        MEMCOPY_SAFE(&nData, &pData[nCurrIndex], sizeof(short));
        lua_pushnumber(pState, (int)nData);
    }

    return 1;
}

//数据流，流出一个字节
static int Lua_Tcp_Buffer_Out_Int8(lua_State* pState)
{
    int nCount = lua_gettop(pState);

    int nMaxLength  = (int)lua_tonumber(pState, 3);
    int nCurrIndex  = (int)lua_tonumber(pState, 2);
    char* pData     = (char* )lua_touserdata(pState, 1);

    //格式化流入的数据
    if(nCurrIndex + (int)sizeof(char) >= nMaxLength || nCurrIndex < 0 || NULL == pData)
    {
        lua_pushnumber(pState, (int)0);
    }
    else
    {
        int nData = 0;
        MEMCOPY_SAFE(&nData, &pData[nCurrIndex], sizeof(char));
        lua_pushnumber(pState, (int)nData);
    }

    return 1;
}


#endif
