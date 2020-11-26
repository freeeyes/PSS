#ifndef _ILOGOBJECT
#define _ILOGOBJECT

#include "define.h"
#include "ace/SString.h"
#include "ace/Message_Block.h"
#include <iostream>

//日志块数据结构
struct _LogBlockInfo
{
public:
    uint16 m_u2LogID  = 0;                //LogID标记
    uint16 m_u2MailID = 0;                //邮件对象的ID序号
    uint32 m_u4Length = 0;                //块长度
    string m_strBlock;                    //块指针
    string m_strMailTitle;                //邮件对象的标题

    _LogBlockInfo() = default;

    _LogBlockInfo(const _LogBlockInfo& ar)
    {
        this->m_strBlock    = ar.m_strBlock;
        this->m_u4Length    = ar.m_u4Length;
        this->m_u2LogID     = ar.m_u2LogID;
        this->m_u2MailID    = ar.m_u2MailID;
    }

    _LogBlockInfo& operator = (const _LogBlockInfo& ar)
    {
        this->m_strBlock    = ar.m_strBlock;
        this->m_u4Length    = ar.m_u4Length;
        this->m_u2LogID     = ar.m_u2LogID;
        this->m_u2MailID    = ar.m_u2MailID;
        return *this;
    }

    ~_LogBlockInfo() = default;


    void clear()
    {
        m_u4Length       = 0;
        m_u2LogID        = 0;
        m_u2MailID       = 0;
        m_strMailTitle   = "";
        m_strBlock       = "";
    }
};

//日志类对象(是一个虚类，由别的类继承实现内部)
class IServerLogger
{
public:
    virtual ~IServerLogger() = default;

    virtual void Close()                                                        = 0;
    virtual int DoLog(uint16 nLogType, shared_ptr<_LogBlockInfo> pLogBlockInfo) = 0;
    virtual int GetLogTypeCount()                                 = 0;

    virtual uint32 GetBlockSize()                                 = 0;
    virtual uint32 GetPoolCount()                                 = 0;

    virtual uint16 GetCurrLevel()                                 = 0;
    virtual uint16 GetLogID(uint16 u2Index)                       = 0;
    virtual const char*  GetLogInfoByServerName(uint16 u2LogID)   = 0;
    virtual const char*  GetLogInfoByLogName(uint16 u2LogID)      = 0;
    virtual int    GetLogInfoByLogDisplay(uint16 u2LogID)         = 0;
    virtual uint16 GetLogInfoByLogLevel(uint16 u2LogID)           = 0;
    virtual bool   ReSet(uint16 u2CurrLogLevel)                   = 0;
};
#endif
