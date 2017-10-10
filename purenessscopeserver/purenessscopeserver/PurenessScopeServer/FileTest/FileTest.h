#ifndef _FILETEST_H
#define _FILETEST_H

#include "define.h"

class IFileTestManager
{
public:
    virtual ~IFileTestManager() {}
    virtual void HandlerServerResponse(uint32 u4ConnectID) = 0; //连接发送数据包回调测试文件接口信息
};

#endif
