#ifndef _CONSOLEMESSAGE_H
#define _CONSOLEMESSAGE_H

#include "MessageDispose.h"
#include "XmlConfig.h"
#include <string>
#include <sstream>

class CConsoleMessage
{
public:
    CConsoleMessage() = default;

    EM_CONSOLE_MESSAGE Dispose(const ACE_Message_Block* pmb, shared_ptr<IBuffPacket> pBuffPacket, uint8& u1OutputType);     //要处理的命令字解析, pBuffPacket为返回要发送给客户端的数据
    EM_CONSOLE_MESSAGE ParsePlugInCommand(const char* pCommand, shared_ptr<IBuffPacket> pBuffPacket) const;                 //执行命令

    void Close();                                                                           //关闭回收

    int Init();                                                                             //初始化命令参数

    //初始化部分
    bool SetConsoleKey(vector<xmlConsoleKeys::_ConsoleKey> vecConsoleKeyList);       //添加验证允许的key值

private:
    EM_CONSOLE_MESSAGE  ParseCommand_Plugin(const char* pCommand, shared_ptr<IBuffPacket> pBuffPacket, uint8& u1OutputType) const;                  //执行命令(插件内部调用)
    EM_CONSOLE_MESSAGE  ParseCommand(const char* pCommand, shared_ptr<IBuffPacket> pBuffPacket, uint8& u1OutputType);                               //执行命令
    EM_CONSOLE_MESSAGE  DoCommand(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pCurrBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket) const;  //处理命令数据
    bool GetCommandInfo(const string& strCommand, _CommandInfo& CommandInfo, bool blCheck = true) const;                      //把命令切割成应该有的数据格式
    bool CheckConsoleKey(const char* pKey) const;                                                                               //验证key

    vector<xmlConsoleKeys::_ConsoleKey> m_vecConsolekeyList;
    CConsolePromissions m_objConsolePromissions;

    //定义统一的函数指针模板
    using DoMessage_Logic = void(*)(const _CommandInfo& CommandInfo, shared_ptr<IBuffPacket> pBuffPacket, uint16& u2ReturnCommandID);
    using mapMessageList = ACE_Hash_Map<string, DoMessage_Logic>;
    mapMessageList      m_objHashMessageLogicList;             //处理函数Hash列表
};

using App_ConsoleManager = PSS_singleton<CConsoleMessage>;
#endif
