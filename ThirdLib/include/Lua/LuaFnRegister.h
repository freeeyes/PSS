#ifndef LUAFNREGISTER_H
#define LUAFNREGISTER_H

//lua�µ�C++ע���࣬������lua���õ���C++����

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

//����ฺ���������ע���lua��C++������
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
