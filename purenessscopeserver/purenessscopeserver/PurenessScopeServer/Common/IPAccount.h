#ifndef _IPACCOUNT_H
#define _IPACCOUNT_H

//添加对客户端链接的统计信息
//add by freeeyes
//2016-11-25
//这里采用Hash数组的方式替代
//默认是当前最大连接的2倍


#include <string>
#include "ace/Date_Time.h"
#include "define.h"
#include "HashTable.h"
#include "ace/Recursive_Thread_Mutex.h"

//IP访问统计模块
struct _IPAccount
{
    int32          m_nCount;             //当前链接次数
    int32          m_nAllCount;          //指定IP链接次数总和
    int32          m_nMinute;            //当前分钟数
    string         m_strIP;              //当前链接地址
    ACE_Date_Time  m_dtLastTime;         //最后链接时间

    _IPAccount()
    {
        m_nCount     = 0;
        m_nAllCount  = 0;
        m_dtLastTime.update();
        m_nMinute    = (int32)m_dtLastTime.minute();
    }

    void Add(ACE_Date_Time dtNowTime)
    {
        m_dtLastTime = dtNowTime;

        if(m_dtLastTime.minute() != m_nMinute)
        {
            m_nMinute  = (int32)m_dtLastTime.minute();
            m_nCount   = 1;
            m_nAllCount++;
        }
        else
        {
            m_nCount++;
            m_nAllCount++;
        }
    }

    //false为数据已过期
    bool Check(ACE_Date_Time dtNowTime)
    {
        //如果3分钟内没有更新，则清理之
        uint16 u2NowTime = (uint32)dtNowTime.minute();

        if(u2NowTime - m_nMinute  < 0)
        {
            u2NowTime += 60;
        }

        if(u2NowTime  - m_nMinute >= 3)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};

typedef vector<_IPAccount> vecIPAccount;

class CIPAccount
{
public:
    CIPAccount()
    {
        m_u4MaxConnectCount = 100;  //默认每秒最高100次
        m_u2CurrTime        = 0;
    }

    ~CIPAccount()
    {
        OUR_DEBUG((LM_INFO, "[CIPAccount::~CIPAccount].\n"));
        Close();
        OUR_DEBUG((LM_INFO, "[CIPAccount::~CIPAccount]End.\n"));
    }

    void Close()
    {
        for(int32 i = 0; i < m_objIPList.Get_Count(); i++)
        {
            _IPAccount* pIPAccount = m_objIPList.Pop();
            SAFE_DELETE(pIPAccount);
        }

        m_objIPList.Close();
    }

    void Init(uint32 u4IPCount)
    {
        m_u4MaxConnectCount = u4IPCount;

        //初始化HashTable
        m_objIPList.Init((int32)u4IPCount);

        ACE_Date_Time  dtNowTime;
        m_u2CurrTime = (uint16)dtNowTime.minute();
    }

    bool AddIP(string strIP)
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

        //检查当前时间，10分钟清理一轮当前Hash数组
        ACE_Date_Time  dtNowTime;
        uint16 u2NowTime = (uint16)dtNowTime.minute();

        if((int32)(u2NowTime - m_u2CurrTime)  < 0)
        {
            u2NowTime += 60;
        }

        if(u2NowTime - m_u2CurrTime >= 10)
        {
            //清理Hash数组
            vector<_IPAccount* > vecIPAccount;
            m_objIPList.Get_All_Used(vecIPAccount);

            for(int32 i = 0; i < (int32)vecIPAccount.size(); i++)
            {
                _IPAccount* pIPAccount =vecIPAccount[i];

                if(NULL != pIPAccount)
                {
                    if(false == pIPAccount->Check(dtNowTime))
                    {
                        if (-1 == m_objIPList.Del_Hash_Data(pIPAccount->m_strIP.c_str()))
                        {
                            OUR_DEBUG((LM_INFO, "[CIPAccount::AddIP]Del_Hash_Data(%s) is error.\n", pIPAccount->m_strIP.c_str()));
                        }

                        SAFE_DELETE(pIPAccount);
                    }
                }
            }
        }

        bool blRet = false;

        //看看需要不需要判定，如果需要，则进行IP统计
        if(m_u4MaxConnectCount > 0)
        {
            _IPAccount* pIPAccount = m_objIPList.Get_Hash_Box_Data(strIP.c_str());

            if(NULL == pIPAccount)
            {
                //没有找到，添加
                pIPAccount = new _IPAccount();

                if(NULL == pIPAccount)
                {
                    blRet = true;
                }
                else
                {
                    pIPAccount->m_strIP     = strIP;
                    pIPAccount->Add(dtNowTime);

                    //查看缓冲是否已满
                    if(m_objIPList.Get_Count() == m_objIPList.Get_Used_Count())
                    {
                        //暂时不处理
                        SAFE_DELETE(pIPAccount);
                        return true;
                    }
                    else
                    {
                        if (-1 == m_objIPList.Add_Hash_Data(strIP.c_str(), pIPAccount))
                        {
                            OUR_DEBUG((LM_INFO, "[CIPAccount::AddIP]Add_Hash_Data(%s) is error.\n", strIP.c_str()));
                        }
                    }
                }
            }
            else
            {
                pIPAccount->Add(dtNowTime);

                if((uint32)pIPAccount->m_nCount >= m_u4MaxConnectCount)
                {
                    blRet = false;
                }
            }

            blRet = true;
        }
        else
        {
            blRet = true;
        }

        return blRet;
    }

    int32 GetCount()
    {
        return m_objIPList.Get_Used_Count();
    }

    void GetInfo(vecIPAccount& VecIPAccount)
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
        vector<_IPAccount* > vecIPAccount;
        m_objIPList.Get_All_Used(vecIPAccount);

        for(int32 i = 0; i < (int32)vecIPAccount.size(); i++)
        {
            _IPAccount* pIPAccount = vecIPAccount[i];

            if(NULL != pIPAccount)
            {
                VecIPAccount.push_back(*pIPAccount);
            }
        }
    }

private:
    uint32                           m_u4MaxConnectCount;                  //每秒允许的最大连接数，前提是m_nNeedCheck = 0;才会生效
    uint16                           m_u2CurrTime;                         //当前时间
    CHashTable<_IPAccount>           m_objIPList;                          //IP统计信息
    ACE_Recursive_Thread_Mutex       m_ThreadLock;                         //多线程锁
};

typedef ACE_Singleton<CIPAccount, ACE_Recursive_Thread_Mutex> App_IPAccount;

//单位时间连接数统计
class CConnectAccount
{
public:
    CConnectAccount()
    {
        m_u4ConnectMin     = 0;
        m_u4ConnectMax     = 0;
        m_u4DisConnectMin  = 0;
        m_u4DisConnectMax  = 0;
        m_u4CurrConnect    = 0;
        m_u4CurrDisConnect = 0;
        m_u1Minute         = 0;
    }

    ~CConnectAccount()
    {

    }

    uint32 Get4ConnectMin()
    {
        return m_u4ConnectMin;
    }

    uint32 GetConnectMax()
    {
        return m_u4ConnectMax;
    }

    uint32 GetDisConnectMin()
    {
        return m_u4DisConnectMin;
    }

    uint32 GetDisConnectMax()
    {
        return m_u4DisConnectMax;
    }

    uint32 GetCurrConnect()
    {
        return m_u4CurrConnect;
    }

    uint32 GetCurrDisConnect()
    {
        return m_u4CurrDisConnect;
    }

    void Init(uint32 u4ConnectMin, uint32 u4ConnectMax, uint32 u4DisConnectMin, uint32 u4DisConnectMax)
    {
        m_u4ConnectMin     = u4ConnectMin;
        m_u4ConnectMax     = u4ConnectMax;
        m_u4DisConnectMin  = u4DisConnectMin;
        m_u4DisConnectMax  = u4DisConnectMax;
        m_u4CurrConnect    = 0;
        m_u4CurrDisConnect = 0;

        ACE_Date_Time  dtLastTime;
        m_u1Minute = (uint8)dtLastTime.minute();
    }

    bool AddConnect()
    {
        if(m_u4ConnectMax > 0)
        {
            ACE_Date_Time  dtLastTime;

            if(m_u1Minute != (uint8)dtLastTime.minute())
            {
                //新的一分钟
                m_u4CurrConnect = 1;
            }
            else
            {
                m_u4CurrConnect++;
            }

            return true;
        }
        else
        {
            return true;
        }
    }

    bool AddDisConnect()
    {
        if(m_u4ConnectMax > 0)
        {
            ACE_Date_Time  dtLastTime;

            if(m_u1Minute != (uint8)dtLastTime.minute())
            {
                //新的一分钟
                m_u4CurrDisConnect = 1;
            }
            else
            {
                m_u4CurrDisConnect++;
            }

            return true;
        }
        else
        {
            return true;
        }
    }

    int32 CheckConnectCount()
    {
        if(m_u4ConnectMax > 0)
        {
            if(m_u4CurrConnect > m_u4ConnectMax)
            {
                return 1;   //1为超越max上限
            }
        }

        if(m_u4ConnectMin > 0)
        {
            if(m_u4CurrConnect < m_u4ConnectMin)
            {
                return 2;    //2为低于min下限
            }
        }
        else
        {
            return 0;
        }

        return 0;
    }

    int32 CheckDisConnectCount()
    {
        if(m_u4DisConnectMax > 0)
        {
            if(m_u4CurrDisConnect > m_u4DisConnectMax)
            {
                return 1;    //1为超越max上限
            }
        }

        if(m_u4ConnectMin > 0)
        {
            if(m_u4CurrDisConnect < m_u4DisConnectMin)
            {
                return 2;    //2为低于min下限
            }
        }
        else
        {
            return 0;
        }

        return 0;
    }

private:
    uint32 m_u4CurrConnect;
    uint32 m_u4CurrDisConnect;

    uint32 m_u4ConnectMin;
    uint32 m_u4ConnectMax;
    uint32 m_u4DisConnectMin;
    uint32 m_u4DisConnectMax;
    uint8  m_u1Minute;            //当前分钟数
};

typedef ACE_Singleton<CConnectAccount, ACE_Recursive_Thread_Mutex> App_ConnectAccount;
#endif
