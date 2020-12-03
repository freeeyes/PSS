#ifndef _CREATE_INFO_H
#define _CREATE_INFO_H

#include "define.h"

//��¼���󴴽�λ��
class CCreateInfo
{
public:
    CCreateInfo(void) = default;

    virtual ~CCreateInfo(void) = default;

    void SetCreateInfo(const char* pFileName, uint32 u4Line)
    {
        m_strUsedFileName = pFileName;
        m_u4UsedLine = u4Line;
    }

    string GetCreateFileName()
    {
        return m_strUsedFileName;
    }

    uint32 GetCreateLine()
    {
        return m_u4UsedLine;
    }

    void ClearCreateInfo()
    {
        m_u4UsedLine = 0;
    }

private:
    string  m_strUsedFileName;                       //�����˶�����ļ���
    uint32  m_u4UsedLine                   = 0;      //�����˶�����к�
};

#endif
