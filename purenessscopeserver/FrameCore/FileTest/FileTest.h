#ifndef _FILETEST_H
#define _FILETEST_H

#include "define.h"

class IFileTestManager
{
public:
    virtual ~IFileTestManager() = default;
    virtual void HandlerServerResponse(uint32 u4ConnectID) = 0; //���ӷ������ݰ��ص������ļ��ӿ���Ϣ
};

#endif
