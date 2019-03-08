#ifndef _MONITORCOMMAND_H
#define _MONITORCOMMAND_H

#include "IBuffPacket.h"
#include "IMonitorCommand.h"
#include "PostMonitorData.h"
#include "cJSON.h"

#define FRAME_CONNECT_INFO    "ClientCount -cp"           //�鿴������
#define FRAME_CONNECT_TRAFFIC "ShowCurrProcessInfo -a"    //�鿴��������

//�ύ��ط���������
struct _MonitorPara
{
    uint16  m_u2MonitorPort;            //Զ�̼�ط������˿�
    uint32  m_u2TimeInterval;           //�������
    char m_szMonitorIP[MAX_BUFF_50];    //Զ�̼���������IP
    char m_szLocalIP[MAX_BUFF_50];      //���ط�����IP

    _MonitorPara()
    {
        m_u2MonitorPort  = 0;
        m_u2TimeInterval = 0;
        m_szMonitorIP[0] = '\0';
        m_szLocalIP[0]   = '\0';
    }
};

//ʵʱ�ϱ���������
class CMonitorCommand : public IMonitorCommand, public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMonitorCommand();
    ~CMonitorCommand();

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //��ʱ�����

    int Init(const char* pJsonFileName, CServerObject* pServerObject);

    void Close();

    int Connect_Monitor();

    void SetState(EM_MONITOR_STATE em_monitor_state);

private:
    CServerObject*    m_pServerObject;
    _MonitorPara      m_objMonitorPara;        //�������������ò���
    EM_MONITOR_STATE  m_emMonitorState;        //��ǰ�����м������״̬
    int32             m_n4TimerID;             //��ǰ��ʱ��ID
    CPostServerData*  m_pPostServerData;       //���ӷ������ķ������ݽ��ն���
};

#endif
