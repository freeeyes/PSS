#include "ILogic.h"

ILogic::ILogic(uint32 nLogicThreadID, const char* descriptor, uint32 nTimeout)
	: uLogicThreadID(nLogicThreadID), 
		des(descriptor), 
		uTimeout(nTimeout)
{
}

ILogic::~ILogic()
{
}

uint32 ILogic::GetLogicThreadID()
{ 
	return uLogicThreadID;
}
void ILogic::SetTimeOut(uint32 nTimeout) 
{ 
	uTimeout = nTimeout; 
}
uint32 ILogic::GetTimeOut()
{ 
	return uTimeout; 
}

std::string ILogic::GetDescriptor()
{
	return des;
}