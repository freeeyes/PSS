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
#include "TimeStamp.hpp"
#include "XmlOpeation.h"
#include "ace/Singleton.h"
#include <string>
#include <sstream>

const uint16 MAX_IP_SIZE = 50;

class _ForbiddenIP
{
public:
    uint32             m_u4Second                = 0;                                  //禁止的秒数
    EM_CONNECT_IO_TYPE m_u1ConnectType           = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP; //链接的类型，0为TCP，1为UDP
    uint8              m_u1Type                  = 0;                                  //禁止的类型，0为永久禁止，1为时段禁止。
    string             m_strClientIP;                                                  //被禁止的IP
    PSS_Time_Point     m_tvBegin                 = CTimeStamp::Get_Time_Stamp();       //时段禁止开始时间

    _ForbiddenIP() = default;
};

using VecForbiddenIP = vector<_ForbiddenIP>;

class CForbiddenIP
{
public:
    CForbiddenIP() = default;

    bool Init(const char* szConfigPath);                                                    //初始化加载永久封停IP文件
    bool CheckIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);                       //检测IP是否可以链接
    bool AddForeverIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);                  //添加永久封停的IP
    bool AddTempIP(const char* pIP, uint32 u4Second, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);    //添加临时封停的IP
    bool DelForeverIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);                  //删除永久封停IP
    bool DelTempIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP);                     //删除临时封停IP
    VecForbiddenIP* ShowForeverIP();                                                        //显示永久封停IP
    VecForbiddenIP* ShowTempIP();                                                           //显示临时封停IP

private:
    bool SaveConfig() const;                                                                //存储配置文件
    bool CompareIP(const char* pTargetIP, const char* pClientIP) const;                     //比较是否在当前IP段

    CXmlOpeation   m_ForbiddenData;
    VecForbiddenIP m_VecForeverForbiddenIP;           //永久封停的IP列表
    VecForbiddenIP m_VecTempForbiddenIP;              //临时封停的IP列表
};

using App_ForbiddenIP = PSS_singleton<CForbiddenIP>;

#endif
