#ifndef _CONSOLEMESSAGE_H
#define _CONSOLEMESSAGE_H

#include "MessageDispose.h"
#include "XmlConfig.h"

class CConsoleMessage
{
public:
    CConsoleMessage();

    int Dispose(const ACE_Message_Block* pmb, IBuffPacket* pBuffPacket, uint8& u1OutputType);     //要处理的命令字解析, pBuffPacket为返回要发送给客户端的数据
    int ParsePlugInCommand(const char* pCommand, IBuffPacket* pBuffPacket);                 //执行命令

    void Close();                                                                           //关闭回收

    int Init();                                                                             //初始化命令参数

    //初始化部分
    bool SetConsoleKey(vector<xmlConsoleKeys::_ConsoleKey> vecConsoleKeyList);       //添加验证允许的key值

private:
    int  ParseCommand_Plugin(const char* pCommand, IBuffPacket* pBuffPacket, uint8& u1OutputType) const;                  //执行命令(插件内部调用)
    int  ParseCommand(const char* pCommand, IBuffPacket* pBuffPacket, uint8& u1OutputType);                               //执行命令
    int  DoCommand(const _CommandInfo& CommandInfo, IBuffPacket* pCurrBuffPacket, IBuffPacket* pReturnBuffPacket) const;  //处理命令数据
    bool GetCommandInfo(const char* pCommand, _CommandInfo& CommandInfo, bool blCheck = true) const;                      //把命令切割成应该有的数据格式
    bool CheckConsoleKey(const char* pKey) const;                                                                               //验证key

    vector<xmlConsoleKeys::_ConsoleKey> m_vecConsolekeyList;
    CConsolePromissions m_objConsolePromissions;

    //定义统一的函数指针模板
    typedef void(*DoMessage_Logic)(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    typedef ACE_Hash_Map<string, DoMessage_Logic> mapMessageList;
    mapMessageList      m_objHashMessageLogicList;             //处理函数Hash列表
};

typedef ACE_Singleton<CConsoleMessage, ACE_Null_Mutex> App_ConsoleManager;
#endif
