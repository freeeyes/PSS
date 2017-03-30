#ifndef _IMODULEINFO_H
#define _IMODULEINFO_H

#include "define.h"

class IModuleInfo
{
public:
    virtual ~IModuleInfo() {}
    virtual bool GetModuleExist(const char* pModuleName)             = 0;   //指定的模块是否存在
    virtual const char* GetModuleParam(const char* pModuleName)      = 0;   //得到模块加载参数
    virtual const char* GetModuleFileName(const char* pModuleName)   = 0;   //得到模块文件名
    virtual const char* GetModuleFilePath(const char* pModuleName)   = 0;   //得到模块加载路径
    virtual const char* GetModuleFileDesc(const char* pModuleName)   = 0;   //得到模块描述信息
    virtual uint16 GetModuleCount()                                  = 0;   //得到加载模块个数
    virtual void GetAllModuleName(vector<string> vecModuleName)      = 0;   //获得当前加载所有的模块名称
};

#endif
