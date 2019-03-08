#include "LuaFn.h"

CLuaFn::CLuaFn(void)
{
    m_pState = NULL;
    Init();
}

CLuaFn::~CLuaFn(void)
{
    Close();
}

void CLuaFn::Init()
{
    if(NULL == m_pState)
    {
        m_pState = lua_open();
        luaL_openlibs(m_pState);
    }
}

void CLuaFn::Close()
{
    if(NULL != m_pState)
    {
        lua_close(m_pState);
        m_pState = NULL;
    }
}

bool CLuaFn::LoadLuaFile(const char* pFileName)
{
    int nRet = 0;

    if(NULL == m_pState)
    {
        printf("[CLuaFn::LoadLuaFile]m_pState is NULL.\n");
        return false;
    }

    nRet = luaL_dofile(m_pState, pFileName);

    if (nRet != 0)
    {
        printf("[CLuaFn::LoadLuaFile]luaL_loadfile(%s) is file(%d)(%s).\n", pFileName, nRet, lua_tostring(m_pState, -1));
        return false;
    }

    lua_resume(m_pState, 0);

    return true;
}

bool CLuaFn::CallFileFn(const char* pFunctionName, CParamGroup& ParamIn, CParamGroup& ParamOut)
{
    int nRet = 0;
    int i    = 0;

    if(NULL == m_pState)
    {
        printf("[CLuaFn::CallFileFn]m_pState is NULL.\n");
        return false;
    }

    lua_getglobal(m_pState, pFunctionName);

    //加载输入参数
    for(i = 0; i < ParamIn.GetCount(); i++)
    {
        PushLuaData(m_pState, ParamIn.GetParam(i));
    }

    nRet = lua_pcall(m_pState, ParamIn.GetCount(), ParamOut.GetCount(), 0);

    if (nRet != 0)
    {
        printf("[CLuaFn::CallFileFn]call function(%s) error(%s).\n", pFunctionName, lua_tostring(m_pState, -1));
        return false;
    }

    //获得输出参数
    int nPos = 0;

    for(i = ParamOut.GetCount() - 1; i >= 0; i--)
    {
        nPos--;
        PopLuaData(m_pState, ParamOut.GetParam(i), nPos);
    }

    int nCount = lua_gettop(m_pState);
    lua_settop(m_pState, -1-ParamOut.GetCount());

    return true;
}

bool CLuaFn::InitClass()
{
    if(NULL == m_pState)
    {
        printf("[CLuaFn::InitClass]m_pState is NULL.\n");
        return false;
    }

    tolua_open(m_pState);
    tolua_module(m_pState, NULL, 0);
    tolua_beginmodule(m_pState, NULL);

    m_LuaFnRegister.AddFunc("LuaFn_Tcp_Print", LuaFn_Tcp_Print);
    m_LuaFnRegister.AddFunc("Lua_Tcp_Buffer_In_Int32", Lua_Tcp_Buffer_In_Int32);
    m_LuaFnRegister.AddFunc("Lua_Tcp_Buffer_In_Int16", Lua_Tcp_Buffer_In_Int16);
    m_LuaFnRegister.AddFunc("Lua_Tcp_Buffer_In_Int8", Lua_Tcp_Buffer_In_Int8);
    m_LuaFnRegister.AddFunc("Lua_Tcp_Buffer_In_Block", Lua_Tcp_Buffer_In_Block);
    m_LuaFnRegister.AddFunc("Lua_Tcp_Buffer_Out_Int32", Lua_Tcp_Buffer_Out_Int32);
    m_LuaFnRegister.AddFunc("Lua_Tcp_Buffer_Out_Int16", Lua_Tcp_Buffer_Out_Int16);
    m_LuaFnRegister.AddFunc("Lua_Tcp_Buffer_Out_Int8", Lua_Tcp_Buffer_Out_Int8);

    for(int i = 0; i < m_LuaFnRegister.GetSize(); i++)
    {
        _FuncInfo* pFuncInfo = m_LuaFnRegister.GetFuncInfo(i);

        if(NULL != pFuncInfo)
        {
            tolua_function(m_pState, pFuncInfo->m_strFuncName.c_str(), pFuncInfo->m_FuncExecute);
        }
    }

    tolua_endmodule(m_pState);


    return true;
}

bool CLuaFn::GetLuaGlobal_Int(const char* pGlobalName, int& nGlobal)
{
    lua_getglobal(m_pState, pGlobalName);

    if(lua_isnumber(m_pState, -1) == 1)
    {
        nGlobal = (int)lua_tonumber(m_pState, -1);
        lua_pop(m_pState, -1);
        return true;
    }
    else
    {
        return false;
    }
}

bool CLuaFn::GetLuaGlobal_String(const char* pGlobalName, string& strGlobal)
{
    lua_getglobal(m_pState, pGlobalName);

    if(lua_isstring(m_pState, -1) == 1)
    {
        char* pGlobal = (char* )lua_tostring(m_pState, -1);

        if(NULL == pGlobal)
        {
            return false;
        }
        else
        {
            strGlobal = pGlobal;
            lua_pop(m_pState, -1);
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool CLuaFn::GetLuaGlobal_ArrayCount(const char* pGlobalName, int& nCount)
{
    char szCommand[MAX_LUA_BUFF_200] = {'\0'};
#if WIN32
    sprintf_s(szCommand, MAX_LUA_BUFF_200, "return #%s;", pGlobalName);
#else
    sprintf(szCommand, "return #%s;", pGlobalName);
#endif
    luaL_dostring(m_pState, szCommand);
    nCount = (int)lua_tonumber(m_pState, -1);
    lua_pop(m_pState, -1);
    return true;
}

bool CLuaFn::GetLuaGlobal_ArrayIndex(const char* pGlobalName, int nIndex, string& strGlobal)
{
    char szCommand[MAX_LUA_BUFF_200] = {'\0'};
#if WIN32
    sprintf_s(szCommand, MAX_LUA_BUFF_200, "return %s[%d];", pGlobalName, nIndex);
#else
    sprintf(szCommand, "return %s[%d];", pGlobalName, nIndex);
#endif
    luaL_dostring(m_pState, szCommand);
    strGlobal = (string)lua_tostring(m_pState, -1);
    lua_pop(m_pState, -1);
    return true;
}


bool CLuaFn::PushLuaData(lua_State* pState, _ParamData* pParam)
{
    if(pParam == NULL)
    {
        return false;
    }

    if(pParam->CompareType("string"))
    {
        lua_pushstring(m_pState, (char* )pParam->GetParam());
        return true;
    }
    else if(pParam->CompareType("int"))
    {
        int* nData = (int* )pParam->GetParam();
        lua_pushnumber(m_pState, *nData);
        return true;
    }
    else if(pParam->CompareType("void"))
    {
        void* pData = (void* )pParam->GetParam();
        lua_pushlightuserdata(m_pState, pData);
        return true;
    }
    else
    {
        void* pVoid = pParam->GetParam();
        tolua_pushusertype(m_pState, pVoid, pParam->GetType());
        return true;
    }
}

bool CLuaFn::PopLuaData(lua_State* pState, _ParamData* pParam, int nIndex)
{
    if(pParam == NULL)
    {
        return false;
    }

    if(pParam->CompareType("string"))
    {
        if (lua_isstring(m_pState, nIndex) == 1)
        {
            const char* pData = (const char*)lua_tostring(m_pState, nIndex);
            pParam->SetData((void* )pData, (int)strlen(pData));
        }

        return true;
    }
    else if(pParam->CompareType("int"))
    {
        if (lua_isnumber(m_pState, nIndex) == 1)
        {
            int nData = (int)lua_tonumber(m_pState, nIndex);
            pParam->SetData(&nData, sizeof(int));
        }

        return true;
    }
    else if(pParam->CompareType("void"))
    {
        if (lua_isuserdata(m_pState, nIndex) == 1)
        {
            void* pData = (void* )lua_touserdata(m_pState, nIndex);
            pParam->SetData(&pData, sizeof(void*));
        }

        return true;
    }
    else
    {
        pParam->SetData(tolua_tousertype(m_pState, nIndex, NULL), -1);
        return true;
    }
}
