#ifndef MESSAGE_DYEING_MANAGER_H
#define MESSAGE_DYEING_MANAGER_H

#include "IMessage.h"
#include "Trace.h"
#include <array>

//�������ݰ�Ⱦɫ��ع���
//���ǵ������Ĵ��ۣ�Ŀǰ���Ǿ���������С�������Ӧ���ǿ��Խ��ܵġ�
//add by freeeyes

const uint16 MAX_DYEING_COMMAND_COUNT = 10;  //���Ⱦɫ������

//IP��أ�Ŀǰ����ͬһʱ��ֻ�ܼ��һ��
class _Dyeing_IP
{
public:
    uint16  m_u2MaxCount     = 0;        //��ǰ��Ҫ��ص�������
    uint16  m_u2CurrCount    = 0;        //��ǰ���Ѵ���ĸ���
    bool m_blIsValid         = false;    //����Ƿ���Ч
    string m_strIP;                      //�û�IP

    _Dyeing_IP() = default;

    void Clear()
    {
		m_u2MaxCount = 0;
		m_u2CurrCount = 0;
		m_blIsValid = false;
        m_strIP = "";
    }
};

//����������
class _Dyeing_Command
{
public:
    uint16  m_u2CommandID = 0;       //��ǰCommandID
    uint16  m_u2MaxCount  = 0;       //��ǰ��Ҫ��ص�������
    uint16  m_u2CurrCount = 0;       //��ǰ���Ѵ���ĸ���
    bool    m_blIsValid   = false;   //����Ƿ���Ч

    _Dyeing_Command()
    {
        Clear();
    }

    void Clear()
    {
        m_u2CommandID = 0;
        m_u2MaxCount = 0;
        m_u2CurrCount = 0;
        m_blIsValid = false;
    }
};

using vec_Dyeing_Command_list = vector<_Dyeing_Command>;  //CommandIDȾɫ�б�

class CMessageDyeingManager
{
public:
    CMessageDyeingManager() = default;

    void AddDyringIP(const char* pClientIP, uint16 u2MaxCount);                      //���һ��IPȾɫ����
    bool AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount);                    //���һ������Ⱦɫ����

    void GetDyeingCommand(vec_Dyeing_Command_list& objList) const;                   //��õ�ǰ����Ⱦɫ״̬

    string GetTraceID(const char* pClientIP, short sClintPort, uint16 u2CommandID);  //�����Ƿ�����µ�traceID

private:
    std::array<_Dyeing_Command, MAX_DYEING_COMMAND_COUNT> m_objCommandList;   //��ǰ��������Ⱦɫ��CommandID�б�
    _Dyeing_IP      m_objDyeingIP;                                //��ǰ����Ⱦɫ��IP
    uint16          m_u2CurrCommandCount = 0;                     //��ǰ����ִ�е�ȾɫCommandID����
};

#endif
