#ifndef _FRAMECOMMAND_H
#define _FRAMECOMMAND_H

#include "IFrameCommand.h"
#include "ConsoleMessage.h"

class CFrameCommand : public IFrameCommand
{
public:
    CFrameCommand() = default;
    ~CFrameCommand() final = default;

    int DoFrameCommand(const char* pCommand, IBuffPacket* pBuffPacket) final;
};

#endif
