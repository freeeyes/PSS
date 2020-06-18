#ifndef _MESSAGEDEFINE_H
#define _MESSAGEDEFINE_H

//这里处理所有的接收数据命令函数
//这些命令处理以静态函数的方式暴露出来
//这样有助于代码的阅读和消息的映射
//这么做的原因是，避免大量的if else 命令处理语句
//优化代码结构，完善sonar代码检查结果
//add by freeeyes

#include <ace/OS_NS_sys_resource.h>

#include "define.h"
#include "ForbiddenIP.h"

//命令处理返回值类型定义
enum
{
    CONSOLE_MESSAGE_SUCCESS = 0,
    CONSOLE_MESSAGE_FAIL    = 1,
    CONSOLE_MESSAGE_CLOSE   = 2,
};

static const char COMMAND_SPLIT_STRING[] = " ";

//命令处理参数
class _CommandInfo
{
public:
    uint8 m_u1OutputType                = 0;        //输出类型，0为二进制，1为文本
    char m_szCommandTitle[MAX_BUFF_100] = {'\0'};   //处理命令头
    char m_szCommandExp[MAX_BUFF_100]   = {'\0'};   //处理命令扩展参数
    char m_szUser[MAX_BUFF_50]          = {'\0'};   //用户信息

    _CommandInfo()
    {
    }
};

//文件名结构
class _FileInfo
{
public:
    char m_szFilePath[MAX_BUFF_100]  = {'\0'};
    char m_szFileName[MAX_BUFF_100]  = {'\0'};
    char m_szFileParam[MAX_BUFF_200] = {'\0'};

    _FileInfo()
    {
    }
};

//监听端口信息
class _ListenInfo
{
public:
    uint32 m_u4Port                  = 0;
    uint32 m_u4PacketParseID         = 0;
    uint8  m_u1IPType                = TYPE_IPV4;
    char   m_szListenIP[MAX_BUFF_20] = {'\0'};

    _ListenInfo()
    {
    }
};

//染色IP信息
class _DyeIPInfo
{
public:
    char   m_szClientIP[MAX_BUFF_20] = {'\0'};   //染色客户端IP
    uint16 m_u2MaxCount              = 0;        //最大数量
};

//染色的CommandID
class _DyeCommandInfo
{
public:
    uint16 m_u2CommandID = 0;               //染色客户端命令
    uint16 m_u2MaxCount  = 0;                //最大数量
};

class _PoolName
{
public:
    char   m_szPoolName[MAX_BUFF_50] = {'\0'};   //内存池名字
    bool   m_blState                 = false;    //当前内存池创建信息状态
};

//协议解析, 公用数据部分
bool GetCommandParam(const char* pCommand, const char* pTag, char* pData, int nMaxSize);                  //解析出指定的Command参数
bool GetFileInfo(const char* pFile, _FileInfo& FileInfo);                                                 //将一个全路径切分成文件名
bool GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                                     //得到禁止的IP列表
bool GetConnectServerID(const char* pCommand, int& nServerID);                                            //得到一个指定的服务器ID
bool GetDebug(const char* pCommand, uint8& u1Debug);                                                      //得到当前设置的BUDEG
bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                                         //得到设置的追踪IP
bool GetLogLevel(const char* pCommand, int& nLogLevel);                                                   //得到日志等级
bool GetAIInfo(const char* pCommand, int& nAI, int& nDispose, int& nCheck, int& nStop);                   //得到AI设置
bool GetNickName(const char* pCommand, char* pName);                                                      //得到连接别名
bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag);                               //得到ConnectID
bool GetMaxConnectCount(const char* pCommand, uint16& u2MaxConnectCount);                                 //得到最大的连接总数
bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo);                                     //得到监听端口信息
bool GetTestFileName(const char* pCommand, char* pFileName);                                              //获得加载测试文件名
bool GetDyeingIP(const char* pCommand, _DyeIPInfo& objDyeIPInfo);                                         //获得染色IP的相关信息
bool GetDyeingCommand(const char* pCommand, _DyeCommandInfo& objDyeCommandInfo);                          //获得染色Command的相关信息
bool GetPoolSet(const char* pCommand, _PoolName& objPoolName);                                            //获得内存池的名字

#endif
