#ifndef LUAFNREGISTER_H
#define LUAFNREGISTER_H

//lua下的C++注册类，负责处理lua中用到的C++函数

#include "LuaDefine.h"
#include <vector>
#include <string>
using namespace std;

typedef int (*FuncExecute)(lua_State* plua_State);

struct _FuncInfo
{
    string      m_strFuncName;
    FuncExecute m_FuncExecute;
};

//这个类负责管理所有注册的lua对C++函数类
class CLuaFnRegister
{
public:
    CLuaFnRegister() {};
    ~CLuaFnRegister() {};

    void Close()
    {
        m_vecLuaFnRegCollection.clear();
    };

    bool AddFunc(const char* pFuncName, FuncExecute pFn)
    {
        _FuncInfo objFuncInfo;
        objFuncInfo.m_strFuncName = pFuncName;
        objFuncInfo.m_FuncExecute = pFn;

        m_vecLuaFnRegCollection.push_back(objFuncInfo);
        return true;
    };

    int GetSize()
    {
        return (int)m_vecLuaFnRegCollection.size();
    };

    _FuncInfo* GetFuncInfo(int nIndex)
    {
        if(nIndex >= (int)m_vecLuaFnRegCollection.size())
        {
            return NULL;
        }
        else
        {
            return (_FuncInfo* )&m_vecLuaFnRegCollection[nIndex];
        }
    };


private:
    typedef vector<_FuncInfo> vecLuaFnRegCollection;
    vecLuaFnRegCollection m_vecLuaFnRegCollection;
};

#endif
