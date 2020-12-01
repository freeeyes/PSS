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
#include <string>

//命令处理返回值类型定义
enum class EM_CONSOLE_MESSAGE
{
    CONSOLE_MESSAGE_SUCCESS = 0,
    CONSOLE_MESSAGE_FAIL    = 1,
    CONSOLE_MESSAGE_CLOSE   = 2,
};

static const string COMMAND_SPLIT_STRING = " ";

//命令处理参数
class _CommandInfo
{
public:
    uint8 m_u1OutputType                = 0;        //输出类型，0为二进制，1为文本
    string m_strCommandTitle;                       //处理命令头
    string m_strCommandExp;                         //处理命令扩展参数
    string m_strUser  = {'\0'};                     //用户信息

    _CommandInfo() = default;
};

//文件名结构
class _FileInfo
{
public:
    string m_strFilePath;
    string m_strFileName;
    string m_strFileParam;

    _FileInfo() = default;
};

//监听端口信息
class _ListenInfo
{
public:
    uint16 m_u2Port                  = 0;
    uint32 m_u4PacketParseID         = 0;
    uint8  m_u1IPType                = TYPE_IPV4;
    string m_strListenIP;

    _ListenInfo() = default;
};

//染色IP信息
class _DyeIPInfo
{
public:
    string m_strClientIP             = {'\0'};   //染色客户端IP
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
    string m_strPoolName             = {'\0'};   //内存池名字
    bool   m_blState                 = false;    //当前内存池创建信息状态
};

//协议解析, 公用数据部分
bool GetCommandParam(const string& strCommand, const char* pTag, string& strValue);                             //解析出指定的Command参数
bool GetFileInfo(const string& strFile, _FileInfo& FileInfo);                                                 //将一个全路径切分成文件名
bool GetForbiddenIP(const string& Command, _ForbiddenIP& ForbiddenIP);                                     //得到禁止的IP列表
bool GetConnectServerID(const string& strCommand, int& nServerID);                                            //得到一个指定的服务器ID
bool GetDebug(const string& strCommand, uint8& u1Debug);                                                      //得到当前设置的BUDEG
bool GetTrackIP(const string& strCommand, _ForbiddenIP& ForbiddenIP);                                         //得到设置的追踪IP
bool GetLogLevel(const string& strCommand, int& nLogLevel);                                                   //得到日志等级
bool GetAIInfo(const string& strCommand, int& nAI, int& nDispose, int& nCheck, int& nStop);                   //得到AI设置
bool GetNickName(const string& strCommand, string& strName);                                                  //得到连接别名
bool GetConnectID(const string& strCommand, uint32& u4ConnectID, bool& blFlag);                               //得到ConnectID
bool GetMaxConnectCount(const string& strCommand, uint16& u2MaxConnectCount);                                 //得到最大的连接总数
bool GetListenInfo(const string& strCommand, _ListenInfo& objListenInfo);                                     //得到监听端口信息
bool GetTestFileName(const string& strCommand, string& strFileName);                                          //获得加载测试文件名
bool GetDyeingIP(const string& strCommand, _DyeIPInfo& objDyeIPInfo);                                         //获得染色IP的相关信息
bool GetDyeingCommand(const string& strCommand, _DyeCommandInfo& objDyeCommandInfo);                          //获得染色Command的相关信息
bool GetPoolSet(const string& strCommand, _PoolName& objPoolName);                                            //获得内存池的名字

#endif
