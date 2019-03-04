#ifndef _FRAMECOMMAND_H
#define _FRAMECOMMAND_H

#include "IFrameCommand.h"
#include "ConsoleMessage.h"

class CFrameCommand : public IFrameCommand
{
public:
    CFrameCommand();
    virtual ~CFrameCommand();

    int DoFrameCommand(const char* pCommand, IBuffPacket* pBuffPacket);
};

#endif
