#ifndef _ILOGOBJECT
#define _ILOGOBJECT

#include "ace/SString.h"
#include "ace/Message_Block.h"

//日志块数据结构
struct _LogBlockInfo
{
public:
    uint32 m_u4LogID;                    //LogID标记
    uint32 m_u4MailID;                   //邮件对象的ID序号
    uint32 m_u4Length;                   //块长度
    char*  m_pBlock;                     //块指针
    bool   m_blIsUsed;                   //是否正在使用
    char   m_szMailTitle[MAX_BUFF_200];  //邮件对象的标题

    ACE_Message_Block*  m_pmbQueuePtr;   //消息队列指针块

    _LogBlockInfo()
    {
        m_pBlock         = NULL;
        m_u4Length       = 0;
        m_u4LogID        = 0;
        m_blIsUsed       = false;
        m_u4MailID       = 0;
        m_szMailTitle[0] = '\0';

        //这里设置消息队列模块指针内容，这样就不必反复的new和delete，提升性能
        //指针关系也可以在这里直接指定，不必使用的使用再指定
        m_pmbQueuePtr  = new ACE_Message_Block(sizeof(_LogBlockInfo*));

        _LogBlockInfo** ppMessage = (_LogBlockInfo**)m_pmbQueuePtr->base();
        *ppMessage = this;
    }

    _LogBlockInfo(const _LogBlockInfo& ar)
    {
        this->m_pBlock      = ar.m_pBlock;
        this->m_u4Length    = ar.m_u4Length;
        this->m_blIsUsed    = ar.m_blIsUsed;
        this->m_u4LogID     = ar.m_u4LogID;
        this->m_u4MailID    = ar.m_u4MailID;
        this->m_pmbQueuePtr = new ACE_Message_Block(sizeof(_LogBlockInfo*));
        sprintf_safe(m_szMailTitle, MAX_BUFF_200, "%s", ar.m_szMailTitle);

        _LogBlockInfo** ppMessage = (_LogBlockInfo**)m_pmbQueuePtr->base();
        *ppMessage = this;
    }

    _LogBlockInfo& operator = (const _LogBlockInfo& ar)
    {
        if(NULL != this->m_pBlock)
        {
            SAFE_DELETE(this->m_pBlock);
        }

        this->m_pBlock      = ar.m_pBlock;
        this->m_u4Length    = ar.m_u4Length;
        this->m_blIsUsed    = ar.m_blIsUsed;
        this->m_u4LogID     = ar.m_u4LogID;
        this->m_u4MailID    = ar.m_u4MailID;
        memcpy_safe((char* )ar.m_pmbQueuePtr->base(), (uint32)ar.m_pmbQueuePtr->length(), m_pmbQueuePtr->base(), (uint32)m_pmbQueuePtr->length());
        sprintf_safe(m_szMailTitle, MAX_BUFF_200, "%s", ar.m_szMailTitle);

        _LogBlockInfo** ppMessage = (_LogBlockInfo**)m_pmbQueuePtr->base();
        *ppMessage = this;
        return *this;
    }

    ~_LogBlockInfo()
    {
        if(NULL != m_pmbQueuePtr)
        {
            m_pmbQueuePtr->release();
            m_pmbQueuePtr = NULL;
        }
    }

    ACE_Message_Block* GetQueueMessage()
    {
        return m_pmbQueuePtr;
    }

    void clear()
    {
        m_u4Length       = 0;
        m_u4LogID        = 0;
        m_blIsUsed       = false;
        m_u4MailID       = 0;
        m_szMailTitle[0] = '\0';
    }
};

//日志类对象(是一个虚类，由别的类继承实现内部)
class CServerLogger
{
public:
    virtual ~CServerLogger() {}

    virtual int DoLog(int nLogType, _LogBlockInfo* pLogBlockInfo) = 0;
    virtual int GetLogTypeCount()                                 = 0;

    virtual uint32 GetBlockSize()                                 = 0;
    virtual uint32 GetPoolCount()                                 = 0;

    virtual uint32 GetCurrLevel()                                 = 0;
    virtual uint16 GetLogID(uint16 u2Index)                       = 0;
    virtual char*  GetLogInfoByServerName(uint16 u2LogID)         = 0;
    virtual char*  GetLogInfoByLogName(uint16 u2LogID)            = 0;
    virtual int    GetLogInfoByLogDisplay(uint16 u2LogID)         = 0;
    virtual uint16 GetLogInfoByLogLevel(uint16 u2LogID)           = 0;
    virtual bool   ReSet(uint32 u4CurrLogLevel)                   = 0;
};
#endif
