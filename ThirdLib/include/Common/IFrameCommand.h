#ifndef _TFRAMECOMMAND_H
#define _TFRAMECOMMAND_H

//������������
//add by freeeyes
#include "define.h"
#include "IBuffPacket.h"

class IFrameCommand
{
public:
    virtual ~IFrameCommand() {}
    virtual int DoFrameCommand(const char* pCommand, IBuffPacket* pBuffPacket) = 0;   //ִ��Զ������ӿ�
};

#endif
