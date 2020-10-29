#ifndef _BASESERVERMANAGER_H
#define _BASESERVERMANAGER_H

//ʵ��ServerManager�����й���������
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

//��ʼ����־ϵͳ
bool Server_Manager_Common_LogSystem();

//��ʼ�����������ϵͳ
bool Server_Manager_Common_Pool();

//��ʼ��������еĽӿڶ���
bool Server_Manager_Common_IObject(IConnectManager* pConnectManager,
                                   IClientManager* pClientManager,
                                   IUDPConnectManager* pUDPConnectManager,
                                   IFrameCommand* pFrameCommand,
                                   IServerManager* pIServerManager,
                                   ITMService* pTMService,
                                   ITTyClientManager* pTTyClientManager,
                                   IControlListen* pControlListen);

//��ʼ��ģ�����ļ���
bool Server_Manager_Common_Module();

//ת��Addr IP�ӿ�
bool Server_Manager_Common_Addr(uint8 u4IpType, const char* pIP, uint16 u2Port, ACE_INET_Addr& listenAddr);

#endif
