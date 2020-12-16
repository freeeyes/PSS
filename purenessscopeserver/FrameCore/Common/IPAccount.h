#ifndef _IPACCOUNT_H
#define _IPACCOUNT_H

//��ӶԿͻ������ӵ�ͳ����Ϣ
//add by freeeyes
//2016-11-25
//�������Hash����ķ�ʽ���
//Ĭ���ǵ�ǰ������ӵ�2��

#include <string>
#include "ace/Date_Time.h"
#include "define.h"
#include "ace/Recursive_Thread_Mutex.h"
#include <unordered_map>

//IP����ͳ��ģ��
class _IPAccount
{
public:
    int32          m_nCount    = 0;          //��ǰ���Ӵ���
    int32          m_nAllCount = 0;          //ָ��IP���Ӵ����ܺ�
    int32          m_nMinute   = 0;          //��ǰ������
    string         m_strIP;                  //��ǰ���ӵ�ַ
    ACE_Date_Time  m_dtLastTime;             //�������ʱ��

    _IPAccount();

    void Add(ACE_Date_Time const& dtNowTime);

    //falseΪ�����ѹ���
    bool Check(ACE_Date_Time const& dtNowTime) const;
};

using vecIPAccount = vector<_IPAccount>;

class CIPAccount
{
public:
    CIPAccount();

    void Close();

    void Init(uint32 u4IPCount);

    bool AddIP(const string& strIP);

    int32 GetCount() const;

    uint32 GetLastConnectCount();

    void GetInfo(vecIPAccount& VecIPAccount);

private:
    //��ʱ����Hash����
    void Clear_Hash_Data(uint16 u2NowTime, const ACE_Date_Time& dtNowTime);

    using hashmapIPAccount = unordered_map<string, shared_ptr<_IPAccount>>;

    uint32                           m_u4MaxConnectCount  = 0;                 //ÿ������������������ǰ����m_nNeedCheck = 0;�Ż���Ч
    uint32                           m_u4CurrConnectCount = 0;                 //��ǰ��������
    uint32                           m_u4LastConnectCount = 0;                 //֮ǰһ���ӵ�����������¼
    uint16                           m_u2CurrTime         = 0;                 //��ǰʱ��
    uint8                            m_u1Minute           = 0;                 //��ǰ������
    hashmapIPAccount                 m_objIPList;                              //IPͳ����Ϣ
    ACE_Recursive_Thread_Mutex       m_ThreadLock;                             //���߳���
};

using App_IPAccount = PSS_singleton<CIPAccount>;

//��λʱ��������ͳ��
class CConnectAccount
{
public:
    CConnectAccount() = default;

    uint32 Get4ConnectMin() const;

    uint32 GetConnectMax() const;

    uint32 GetDisConnectMin() const;

    uint32 GetDisConnectMax() const;

    uint32 GetCurrConnect() const;

    uint32 GetCurrDisConnect() const;

    void Init(uint32 u4ConnectMin, uint32 u4ConnectMax, uint32 u4DisConnectMin, uint32 u4DisConnectMax);

    bool AddConnect();

    bool AddDisConnect();

    int32 CheckConnectCount() const;

    int32 CheckDisConnectCount() const;

private:
    uint32 m_u4CurrConnect    = 0;
    uint32 m_u4CurrDisConnect = 0;
    uint32 m_u4ConnectMin     = 0;
    uint32 m_u4ConnectMax     = 0;
    uint32 m_u4DisConnectMin  = 0;
    uint32 m_u4DisConnectMax  = 0;
    uint8  m_u1Minute         = 0;   //��ǰ������
};

using App_ConnectAccount = PSS_singleton<CConnectAccount>;
#endif
