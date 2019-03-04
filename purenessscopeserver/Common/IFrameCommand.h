#ifndef _TFRAMECOMMAND_H
#define _TFRAMECOMMAND_H

//框架命令控制类
//add by freeeyes
#include "define.h"
#include "IBuffPacket.h"

class IFrameCommand
{
public:
    virtual ~IFrameCommand() {}
    virtual int DoFrameCommand(const char* pCommand, IBuffPacket* pBuffPacket) = 0;   //执行远程命令接口
};

#endif
