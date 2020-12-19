#ifndef _PROTTYCLIENTMANAGER_H
#define _PROTTYCLIENTMANAGER_H

//TTy设备的Proactor实现
//add by freeeyes

#include "ITTyClientManager.h"
#include "ReTTyHandler.h"
#include "HashTable.h"
#include "XmlConfig.h"
#include "TimerManager.hpp"

class CReTTyClientManager : public ITTyClientManager
{
public:
    CReTTyClientManager();
    virtual ~CReTTyClientManager();

    bool StartConnectTask();                                                                             //启动定时器
    void CancelConnectTask();                                                                            //关闭定时器

    bool Init(ACE_Reactor* pReactor, uint16 u2MaxTTyCount, uint16 u2TimeCheck);                          //初始化管理器
    void Close();                                                                                        //关闭所有连接

    int Connect(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, ITTyMessage* pMessageRecv) final;  // 连接（打开）端口
    int ConnectFrame(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, uint32 u4PacketParseID) final;    // 连接（打开）端口

    bool GetClientDevInfo(uint16 u2ConnectID, _TTyDevParam& outParam) final;                           // 获取连接配置信息
    bool IsConnect(uint16 u2ConnectID) final;                                                          // 是否连接（打开）状态

    bool Close(uint16 u2ConnectID) final;                                                              // 关闭端口
    bool Pause(uint16 u2ConnectID) final;                                                              // 暂停端口
    bool Resume(uint16 u2ConnectID) final;                                                             // 恢复暂停端口

    bool SendMessage(uint16 u2ConnectID, char*& pMessage, uint32 u4Len) final;                         // 发送数据

    int timer_task(brynet::TimerMgr::Ptr timerMgr);
    void start_new_task(brynet::TimerMgr::Ptr timerMgr);

private:
    CHashTable<CReTTyHandler> m_objTTyClientHandlerList;                         //连接设备列表
    ACE_Recursive_Thread_Mutex m_ThreadWritrLock;                                //线程锁
    ACE_Reactor*               m_pReactor       = nullptr;                       //反应器句柄
    uint16                     m_u2MaxListCount = MAX_BUFF_100;                  //最大设备数量
    uint16                     m_u2TimeCheck    = 120;                           //定时器检测时间
    bool                       m_blTimerState   = true;                          //定时器检测
};

using App_ReTTyClientManager = PSS_singleton<CReTTyClientManager>;

#endif
