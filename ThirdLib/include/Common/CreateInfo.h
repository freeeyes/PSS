#ifndef _CREATE_INFO_H
#define _CREATE_INFO_H

#include "define.h"

//��¼���󴴽�λ��
class CCreateInfo
{
public:
    CCreateInfo(void)
    {
        ACE_OS::memset(m_szUsedFileName, 0, MAX_BUFF_100);
        m_u4UsedLine = 0;
    }

    virtual ~CCreateInfo(void)
    {
    }

    void SetCreateInfo(const char* pFileName, uint32 u4Line)
    {
        sprintf_safe(m_szUsedFileName, MAX_BUFF_100, "%s", pFileName);
        m_u4UsedLine = u4Line;
    }

    char* GetCreateFileName()
    {
        return m_szUsedFileName;
    }

    uint32 GetCreateLine()
    {
        return m_u4UsedLine;
    }

    void ClearCreateInfo()
    {
        m_szUsedFileName[0] = '\0';
        m_u4UsedLine = 0;
    }

private:
    char    m_szUsedFileName[MAX_BUFF_100]; //�����˶�����ļ���
    uint32  m_u4UsedLine;                   //�����˶�����к�
};

#endif
