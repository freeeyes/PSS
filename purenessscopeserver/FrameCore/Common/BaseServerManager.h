#ifndef _BASESERVERMANAGER_H
#define _BASESERVERMANAGER_H

//实现ServerManager的所有公共函数。
//add by freeeyes

#include "define.h"
#include "ForbiddenIP.h"
#include "MessageService.h"
#include "LoadModule.h"
#include "LogManager.h"
#include "FileLogger.h"
#include "IObject.h"
#include "ModuleMessageManager.h"
#include "FrameCommand.h"
#include "BuffPacketManager.h"
#include "Frame_Logging_Strategy.h"
#include "IControlListen.h"
#include "XmlConfig.h"

//初始化ACE输出性质
bool Server_Manager_Common_FrameLogging(Frame_Logging_Strategy*& pFrameLoggingStrategy);

//初始化日志系统
bool Server_Manager_Common_LogSystem();

//初始化其它相关子系统
bool Server_Manager_Common_Pool();

//初始化插件所有的接口对象
bool Server_Manager_Common_IObject(IConnectManager* pConnectManager,
                                   IClientManager* pClientManager,
                                   IUDPConnectManager* pUDPConnectManager,
                                   IFrameCommand* pFrameCommand,
                                   IServerManager* pIServerManager,
                                   ITMService* pTMService,
                                   ITTyClientManager* pTTyClientManager,
                                   IMessageQueueManager* pLogicThreadManager,
                                   IControlListen* pControlListen);

//初始化模块插件的加载
bool Server_Manager_Common_Module();

//转换Addr IP接口
bool Server_Manager_Common_Addr(uint8 u4IpType, const char* pIP, uint32 u4Port, ACE_INET_Addr& listenAddr);

#endif
