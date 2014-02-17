/*
 * 消息处理
 * by w1w
 */
#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "CommandDefine.h"
#include "PostServerCommand.h"

#include <string>

using namespace std;

class CiConnectCommand :  public ACE_Task<ACE_MT_SYNCH>, public CClientCommand
{
public:
	CiConnectCommand(void);
	~CiConnectCommand(void);

	void InitConfig();                                                        //初始化配置
	void InitServer();                                                        //初始化服务
	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);

	void SetServerObject(CServerObject* pServerObject);
	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);    //定时器处理过程
private:
	CServerObject* m_pServerObject;
	CPostServerCommand m_PostServerCommand;
	ActiveTimer                   m_ActiveTimer;       //定时器，继承框架的定时器
	int                            m_nTimerID;
	int            m_nCount;
	void CancelCheckServerConnect();                                          //取消定时器
	bool StartCheckServerConnect(int nIntervalTime);                          //启动定时器

	bool CheckpMessage(IMessage* pMessage);                                   //初步检查空值
	uint32 getAppconnect( uint16 DestAppID );                                 //获取APP对应的连接
	void PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState);          //客户端发送数据
	void PostMessage(vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState); //客户端群发数据
	void SendData(int nServerID, const char* pData, int nSize, bool blIsDelete);                                                     //服务器间发送数据

	void handle_ClientDisConnect(uint32 ConnectID);                                           //连接断开
	bool handle_AppReg(uint32 ConnectID,uint16 AppID,VCHARB_STR strsModule);                  //注册App
	void BroadcastAppChange2Server(uint16 Appid, bool isdelete);                              //向所有服务器同步APP状态
	void BroadcastAppChange2LocalApp(uint16 Serverid, uint16 Appid, bool isdelete);           //向所有其他APP同步该APP状态
	void BroadCastServerState();                                                              //广播服务器状态

	IBuffPacket* MakeQueryServerRespPacket();                                                                     //服务器查询回应包
	IBuffPacket* MakeRegAcceptRespPacket();                                                                       //App注册成功回应包
	IBuffPacket* MakeRegRejectRespPacket();                                                                       //App注册失败回应包
	IBuffPacket* MakeRegRespPacket(uint8 RegResult);                                                              //App注册回应包
	int MakeQueryAppPacket(char*& pdata,uint16 Serverid);                                                         //App查询包
	IBuffPacket* MakeQueryAppPacket(uint16 Serverid);                                                             //App查询包
	int MakeQueryAppRespPacket(char*& pdata);                                                                     //App查询回应包
	IBuffPacket* MakeQueryAppRespPacket();                                                                        //App查询回应包
	IBuffPacket* MakeQueryAppRespPacket(uint16 Serverid);                                                         //App查询回应包
	int MakeSynAppPacket(char*& pdata,uint16 Appid, bool isdelete);                                               //App同步包
	IBuffPacket* MakeSynAppPacket(uint16 Serverid, uint16 Appid, bool isdelete);                                  //App同步包
	int MakeHeartPacket(char*& pdata);                                                                            //心跳包
	IBuffPacket* MakeHeartRespPacket();                                                                           //心跳回应包
	IBuffPacket* MakeServerDownPacket( uint16 DestServerID ,uint32 dataid);                                       //服务器不在线包
	IBuffPacket* MakeAppDownPacket( uint16 DestServerID,uint16 SourAppID, uint16 DestAppID, uint32 dataid );      //App不在线包
	int MakeAppDownPacket(char*& pdata, uint16 DestServerID,uint16 SourAppID, uint16 DestAppID, uint32 dataid );  //App不在线包
};
