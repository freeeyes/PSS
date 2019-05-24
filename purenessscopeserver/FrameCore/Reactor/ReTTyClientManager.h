#ifndef _PROTTYCLIENTMANAGER_H
#define _PROTTYCLIENTMANAGER_H

//TTy设备的Proactor实现
//add by freeeyes

#include "ITTyClientManager.h"
#include "ReTTyHandler.h"
#include "HashTable.h"
#include "XmlConfig.h"
#include "TimerManager.h"

class CReTTyClientManager : public ACE_Event_Handler, public ITTyClientManager
{
public:
    CReTTyClientManager();
    virtual ~CReTTyClientManager();

    bool StartConnectTask();                                                                             //启动定时器
    void CancelConnectTask();                                                                            //关闭定时器

    bool Init(ACE_Reactor* pReactor, uint16 u2MaxTTyCount, uint16 u2TimeCheck);                          //初始化管理器
    void Close();                                                                                        //关闭所有连接

    virtual int Connect(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, ITTyMessage* pMessageRecv);  // 连接（打开）端口
    virtual int ConnectFrame(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, uint32 u4PacketParseID);    // 连接（打开）端口

    virtual bool GetClientDevInfo(uint16 u2ConnectID, _TTyDevParam& outParam);                           // 获取连接配置信息
    virtual bool IsConnect(uint16 u2ConnectID);                                                          // 是否连接（打开）状态

    virtual bool Close(uint16 u2ConnectID);                                                              // 关闭端口
    virtual bool Pause(uint16 u2ConnectID);                                                              // 暂停端口
    virtual bool Resume(uint16 u2ConnectID);                                                             // 恢复暂停端口

    virtual bool SendMessage(uint16 u2ConnectID, char*& pMessage, uint32 u4Len);                         // 发送数据

    virtual int handle_timeout(const ACE_Time_Value& current_time, const void* act = 0);                 //定时器执行
private:
    CHashTable<CReTTyHandler> m_objTTyClientHandlerList;             //连接设备列表
    ACE_Recursive_Thread_Mutex m_ThreadWritrLock;                    //线程锁
    ACE_Reactor*               m_pReactor;                           //反应器句柄
    uint16                     m_u2MaxListCount;                     //最大设备数量
    uint16                     m_u2TimeCheck;                        //定时器检测时间
    int                        m_nTaskID;                            //定时器ID
};

typedef ACE_Singleton<CReTTyClientManager, ACE_Recursive_Thread_Mutex> App_ReTTyClientManager;


#endif
