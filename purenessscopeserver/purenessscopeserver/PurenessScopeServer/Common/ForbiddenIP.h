#ifndef _FORBIDDEN_H
#define _FORBIDDEN_H

//很久不往这个程序里写故事了，其实做一个事情绝对是一种坚持。哪怕只剩下你一个人。
//后来觉得，其实这个东西对我，并不存在期待，也不存在放弃，而是一种本然。
//是的，我想做出来，就这么简单。没有什么理由让我坚持，也不必有什么理由让我放弃。
//于是，继续的前行，跟随本然吧。
//全部xml化，这是一个好主意，感谢凹凸man。
//add by freeeyes
//2013-03-09

#include "define.h"
#include "XmlOpeation.h"
#include "ace/Singleton.h"

#define MAX_IP_SIZE 50

struct _ForbiddenIP
{
    uint32         m_u4Second;                  //禁止的秒数
    uint8          m_u1ConnectType;             //链接的类型，0为TCP，1为UDP
    uint8          m_u1Type;                    //禁止的类型，0为永久禁止，1为时段禁止。
    char           m_szClientIP[MAX_IP_SIZE];   //被禁止的IP
    ACE_Time_Value m_tvBegin;                   //时段禁止开始时间

    _ForbiddenIP()
    {
        m_szClientIP[0] = '\0';
        m_u1Type        = 0;
        m_u4Second      = 0;
        m_u1ConnectType = CONNECT_TCP;            //默认为TCP包
        m_tvBegin       = ACE_OS::gettimeofday(); //默认开始时间为当前时间
    }
};

typedef vector<_ForbiddenIP> VecForbiddenIP;

class CForbiddenIP
{
public:
    CForbiddenIP();
    ~CForbiddenIP();

    bool Init(const char* szConfigPath);                                                    //初始化加载永久封停IP文件
    bool CheckIP(const char* pIP, uint8 u1ConnectType = CONNECT_TCP);                       //检测IP是否可以链接
    bool AddForeverIP(const char* pIP, uint8 u1ConnectType = CONNECT_TCP);                  //添加永久封停的IP
    bool AddTempIP(const char* pIP, uint32 u4Second, uint8 u1ConnectType = CONNECT_TCP);    //添加临时封停的IP
    bool DelForeverIP(const char* pIP, uint8 u1ConnectType = CONNECT_TCP);                  //删除永久封停IP
    bool DelTempIP(const char* pIP, uint8 u1ConnectType = CONNECT_TCP);                     //删除临时封停IP
    VecForbiddenIP* ShowForeverIP() const;                                                  //显示永久封停IP
    VecForbiddenIP* ShowTemoIP() const;                                                     //显示临时封停IP

private:
    bool SaveConfig();                                                                      //存储配置文件
    bool CompareIP(char* pTargetIP, char* pClientIP);                                       //比较是否在当前IP段

private:
    CXmlOpeation   m_ForbiddenData;
    VecForbiddenIP m_VecForeverForbiddenIP;           //永久封停的IP列表
    VecForbiddenIP m_VecTempForbiddenIP;              //临时封停的IP列表
};

typedef ACE_Singleton<CForbiddenIP, ACE_Null_Mutex> App_ForbiddenIP;

#endif
