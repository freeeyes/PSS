#include "FrameCommand.h"

int CFrameCommand::DoFrameCommand(const char* pCommand, shared_ptr<IBuffPacket> pBuffPacket)
{
    if (EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_SUCCESS != App_ConsoleManager::instance()->ParsePlugInCommand(pCommand, pBuffPacket))
    {
        return (int)EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_FAIL;
    }
    else
    {
        return (int)EM_CONSOLE_MESSAGE::CONSOLE_MESSAGE_SUCCESS;
    }
}


