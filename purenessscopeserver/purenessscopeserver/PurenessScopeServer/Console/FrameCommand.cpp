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
        SAFE_DELETE_ARRAY(pCommand);
        return CONSOLE_MESSAGE_FAIL;
    }
    else
    {
        SAFE_DELETE_ARRAY(pCommand);
        return CONSOLE_MESSAGE_SUCCESS;
    }
}


