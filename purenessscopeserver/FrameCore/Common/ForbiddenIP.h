#ifndef _FORBIDDEN_H
#define _FORBIDDEN_H

//�ܾò������������д�����ˣ���ʵ��һ�����������һ�ּ�֡�����ֻʣ����һ���ˡ�
//�������ã���ʵ����������ң����������ڴ���Ҳ�����ڷ���������һ�ֱ�Ȼ��
//�ǵģ�����������������ô�򵥡�û��ʲô�������Ҽ�֣�Ҳ������ʲô�������ҷ�����
//���ǣ�������ǰ�У����汾Ȼ�ɡ�
//ȫ��xml��������һ�������⣬��л��͹man��
//add by freeeyes
//2013-03-09

#include "define.h"
#include "XmlOpeation.h"
#include "ace/Singleton.h"
#include <string>
#include <sstream>

const uint16 MAX_IP_SIZE = 50;

class _ForbiddenIP
{
public:
    uint32             m_u4Second                = 0;                                  //��ֹ������
    EM_CONNECT_IO_TYPE m_u1ConnectType           = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP; //���ӵ����ͣ�0ΪTCP��1ΪUDP
    uint8              m_u1Type                  = 0;                                  //��ֹ�����ͣ�0Ϊ���ý�ֹ��1Ϊʱ�ν�ֹ��
    string             m_strClientIP;                                                  //����ֹ��IP
    ACE_Time_Value     m_tvBegin                 = ACE_OS::gettimeofday();             //ʱ�ν�ֹ��ʼʱ��

    _ForbiddenIP()
    {
    }
};

typedef vector<_ForbiddenIP> VecForbiddenIP;

class CForbiddenIP
{
public:
    CForbiddenIP();

    bool Init(const char* szConfigPath);                                                    //��ʼ���������÷�ͣIP�ļ�
    bool CheckIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);                       //���IP�Ƿ��������
    bool AddForeverIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);                  //������÷�ͣ��IP
    bool AddTempIP(const char* pIP, uint32 u4Second, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);    //�����ʱ��ͣ��IP
    bool DelForeverIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);                  //ɾ�����÷�ͣIP
    bool DelTempIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);                     //ɾ����ʱ��ͣIP
    VecForbiddenIP* ShowForeverIP();                                                        //��ʾ���÷�ͣIP
    VecForbiddenIP* ShowTempIP();                                                           //��ʾ��ʱ��ͣIP

private:
    bool SaveConfig() const;                                                                //�洢�����ļ�
    bool CompareIP(const char* pTargetIP, const char* pClientIP) const;                     //�Ƚ��Ƿ��ڵ�ǰIP��

    CXmlOpeation   m_ForbiddenData;
    VecForbiddenIP m_VecForeverForbiddenIP;           //���÷�ͣ��IP�б�
    VecForbiddenIP m_VecTempForbiddenIP;              //��ʱ��ͣ��IP�б�
};

typedef ACE_Singleton<CForbiddenIP, ACE_Null_Mutex> App_ForbiddenIP;

#endif
