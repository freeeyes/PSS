#ifndef _WORKTHREADAI_H
#define _WORKTHREADAI_H

//实现对工作线程效率的监控
//此想法来源于Bobo的建议，如果当一个工作线程持续出现阻塞处理数据缓慢怎么办？
//杀死工作线程和增加工作线程都不是最好的办法
//最好的办法，就是找到罪魁祸首的那个数据包
//正常情况下，应该是某种数据处理引发的问题，而不能波及全部工作线程
//如果这个问题数据包在一定时间内拖累了工作线程太长时间，那么就果断采取措施
//停止此命令处理一段时间，并返回给客户端告知
//直接走短回路回应数据异常，保证其他数据包的正常数据处理
//并记录日志，或者提醒运维管理者
//这样做尽力减少因为某一个指令问题而导致的整体服务器相应效率底下的问题
//此功能，可以根据需求决定是否开启，在main.xml里面设置开关
//如果所有数据包处理都在缓慢不在此AI的管理范围之内
//PSS依旧还有最后的自我保护机制，重启工作线程
//不过既然全部数据包都超时了，应用也不可能让用户满意了
//此功能只是最大限度的保护用户体验，用户体验是服务器的衣食父母
//add by freeeyes

#include "Ring.h"
#include "define.h"
#include "TimeStamp.hpp"
#include "ConvertBuffer.h"

//相关参数设计
class _WorkThreadAIInfo
{
public:
    uint32     m_u4ThreadID       = 0;                    //工作线程ID
    uint8      m_u1WTAI           = 0;                    //工作线程AI开关，0为关闭，1为打开
    uint32     m_u4DisposeTime    = 0;                    //业务包处理超时时间
    uint32     m_u4WTCheckTime    = 0;                    //工作线程超时包的时间范围，单位是秒
    uint32     m_u4WTTimeoutCount = 0;                    //工作线程超时包的单位时间内的超时次数上限
    uint32     m_u4WTStopTime     = 0;                    //停止此命令服务的时间

    _WorkThreadAIInfo() = default;
};

//AI配置信息表
using vecWorkThreadAIInfo = vector<_WorkThreadAIInfo>;

//超时命令单元
class _CommandTimeout
{
public:
    uint32 m_u4ThreadID  = 0;       //工作线程ID
    uint16 m_u2CommandID = 0;       //超时的命令
    uint64 m_u8Second    = 0;       //超时当前时间，以1970年以来开始计算的秒数
    uint32 m_u4Timeout   = 0;       //命令执行时间，单位是毫秒

    _CommandTimeout() = default;
};

//这里用户回传相关查询信息
using vecCommandTimeout = vector<_CommandTimeout>;

class CWorkThreadAI
{
public:
    CWorkThreadAI() = default;

    void Close();

    void Init(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTTimeoutCount, uint32 u4WTStopTime, uint8 u1WTReturnDataType, const char* pReturnData);

    bool SaveTimeout(uint16 u2CommandID, uint32 u4TimeCost);

    uint16 GetReturnDataLength() const;

    void GetAIInfo(_WorkThreadAIInfo& objWorkThreadAIInfo) const;

    void ReSet(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime);

    bool CheckCurrTimeout(uint16 u2CommandID, uint64 u8Now);

    void GetAllTimeout(uint32 u4ThreadID, vecCommandTimeout& objTimeout) const;
    void GetAllForbiden(uint32 u4ThreadID, vecCommandTimeout& objForbiden) const;

private:
    int Do_Command_Account(uint16 u2CommandID, uint64 u8Now, uint32 u4TimeCost, bool& blRet);   //统计Command的AI数据

    uint8      m_u1WTAI                        = 0;           //工作线程AI开关，0为关闭，1为打开
    uint16     m_u2ReturnDataLen               = 0;           //返回数据体长度
    uint32     m_u4DisposeTime                 = 0;           //业务包处理超时时间
    uint32     m_u4WTCheckTime                 = 0;           //工作线程超时包的时间范围，单位是秒
    uint32     m_u4WTTimeoutCount              = 0;           //工作线程超时包的单位时间内的超时次数上限
    uint32     m_u4WTStopTime                  = 0;           //停止此命令服务的时间
    uint8      m_u1WTReturnDataType            = 0;           //返回错误数据的类型，1为二进制，2为文本
    string     m_strWTReturnData;                             //返回的数据体，最多1K

    //超时的命令集合
    class _CommandTime
    {
    public:
        uint16 m_u2CommandID = 0;
        CRingLink<_CommandTimeout> m_objTime;

        _CommandTime() = default;
    };

    vector<shared_ptr<_CommandTime>> m_vecCommandTime;
    vector<_CommandTimeout> m_vecCommandTimeout;
};
#endif
