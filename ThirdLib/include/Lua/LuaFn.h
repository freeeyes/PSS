#pragma once

#include "LuaFnRegister.h"
#include "ParamData.h"
#include <string>

#include "LuaFn_Tcp.h"

#define MAX_LUA_BUFF_200 200

using namespace std;

class CLuaFn
{
public:
    CLuaFn(void);
    ~CLuaFn(void);

    void Init();
    void Close();

    bool InitClass();

    bool LoadLuaFile(const char* pFileName);
    bool CallFileFn(const char* pFunctionName, CParamGroup& ParamIn, CParamGroup& ParamOut);

    bool GetLuaGlobal_Int(const char* pGlobalName, int& nGlobal);
    bool GetLuaGlobal_String(const char* pGlobalName, string& strGlobal);
    bool GetLuaGlobal_ArrayCount(const char* pGlobalName, int& nCount);
    bool GetLuaGlobal_ArrayIndex(const char* pGlobalName, int nIndex, string& strGlobal);

private:
    bool PushLuaData(lua_State* pState, _ParamData* pParam);
    bool PopLuaData(lua_State* pState, _ParamData* pParam, int nIndex);

private:
    lua_State*     m_pState;
    CLuaFnRegister m_LuaFnRegister;
};
