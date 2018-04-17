#ifndef _MONITORCOMMAND_H
#define _MONITORCOMMAND_H

#include "IBuffPacket.h"
#include "IMonitorCommand.h"
#include "PostMonitorData.h"
#include "cJSON.h"

#define FRAME_CONNECT_INFO    "ClientCount -cp"           //查看连接数
#define FRAME_CONNECT_TRAFFIC "ShowCurrProcessInfo -a"    //查看连接流量

//提交监控服务器参数
struct _MonitorPara
{
    uint16  m_u2MonitorPort;            //远程监控服务器端口
    uint32  m_u2TimeInterval;           //监听间隔
    char m_szMonitorIP[MAX_BUFF_50];    //远程监听服务器IP
    char m_szLocalIP[MAX_BUFF_50];      //本地服务器IP

    _MonitorPara()
    {
        m_u2MonitorPort  = 0;
        m_u2TimeInterval = 0;
        m_szMonitorIP[0] = '\0';
        m_szLocalIP[0]   = '\0';
    }
};

//实时上报监听进程
class CMonitorCommand : public IMonitorCommand, public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMonitorCommand();
    ~CMonitorCommand();

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //定时器检查

    int Init(const char* pJsonFileName, CServerObject* pServerObject);

    void Close();

    int Connect_Monitor();

    void SetState(EM_MONITOR_STATE em_monitor_state);

private:
    CServerObject*    m_pServerObject;
    _MonitorPara      m_objMonitorPara;        //监听服务器设置参数
    EM_MONITOR_STATE  m_emMonitorState;        //当前连接中间服务器状态
    int32             m_n4TimerID;             //当前定时器ID
    CPostServerData*  m_pPostServerData;       //连接服务器的返回数据接收对象
};

#endif
