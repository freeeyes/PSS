#ifndef _IMODULEINFO_H
#define _IMODULEINFO_H

#include "define.h"

class IModuleInfo
{
public:
    virtual ~IModuleInfo() {}
    virtual bool GetModuleExist(const char* pModuleName)                    = 0;   //ָ����ģ���Ƿ����
    virtual const char* GetModuleParam(const char* pModuleName)             = 0;   //�õ�ģ����ز���
    virtual const char* GetModuleFileName(const char* pModuleName)          = 0;   //�õ�ģ���ļ���
    virtual const char* GetModuleFilePath(const char* pModuleName)          = 0;   //�õ�ģ�����·��
    virtual const char* GetModuleFileDesc(const char* pModuleName)          = 0;   //�õ�ģ��������Ϣ
    virtual uint16 GetModuleCount()                                         = 0;   //�õ�����ģ�����
    virtual bool GetAllModuleName(uint32 u4Index, char* pName, uint16 nLen) = 0;   //��õ�ǰ�������е�ģ������
};

#endif
