#ifndef IMONITORCOMMAND_H
#define IMONITORCOMMAND_H

#include "define.h"
#include "IObject.h"

//���࣬�û���������ͨѶ�ص�ʹ��
//add by freeeyes
#define MONITER_SERVER_ID          1

#define COMMAND_MONITOR_LOGIN      0x2001    //����ע������
#define COMMAND_MONITOR_DATA       0x2002    //����������ǰ������������
#define COMMAND_MONITOR_LOGIN_ACK  0x8001    //ע�������Ӧ
#define COMMAND_MONITOR_DATA_ACK   0x8002    //��ǰ�������������Ӧ

enum EM_MONITOR_STATE
{
    MONITOR_STATE_DISCONNECT = 0,   //δ���ӵ�Զ�̼�ط�����
    MONITOR_STATE_CONNECT,          //�����ӵ�Զ�̼�ط�����
    MONITOR_STATE_LOGINED,          //Զ�̼�ط���������֤
};

class IMonitorCommand
{
public:
    virtual ~IMonitorCommand() {};

    virtual void SetState(EM_MONITOR_STATE em_monitor_state) = 0;

};

#endif
