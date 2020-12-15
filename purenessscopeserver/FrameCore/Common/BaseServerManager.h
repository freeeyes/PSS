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
#include "IControlListen.h"
#include "XmlConfig.h"
#include "Echartlog.h"

//初始化日志系统
bool Server_Manager_Common_LogSystem();

//初始化其它相关子系统
bool Server_Manager_Common_Pool();

//初始化插件所有的接口对象
bool Server_Manager_Common_IObject(IConnectManager* pConnectManager,
                                   IClientManager* pClientManager,
                                   IFrameCommand* pFrameCommand,
                                   IServerManager* pIServerManager,
                                   ITTyClientManager* pTTyClientManager,
                                   IControlListen* pControlListen);

//初始化模块插件的加载
bool Server_Manager_Common_Module();

//转换Addr IP接口
bool Server_Manager_Common_Addr(uint8 u4IpType, const char* pIP, uint16 u2Port, ACE_INET_Addr& listenAddr);

#endif
