#include "FrameCommand.h"

CFrameCommand::CFrameCommand()
{
}

CFrameCommand::~CFrameCommand()
{
}

int CFrameCommand::DoFrameCommand(const char* pCommand, IBuffPacket* pBuffPacket)
{
    if (CONSOLE_MESSAGE_SUCCESS != App_ConsoleManager::instance()->ParsePlugInCommand(pCommand, pBuffPacket))
    {
        return CONSOLE_MESSAGE_FAIL;
    }
    else
    {
        return CONSOLE_MESSAGE_SUCCESS;
    }
}


