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

    EM_CONSOLE_MESSAGE Dispose(const ACE_Message_Block* pmb, IBuffPacket* pBuffPacket, uint8& u1OutputType);     //Ҫ����������ֽ���, pBuffPacketΪ����Ҫ���͸��ͻ��˵�����
    EM_CONSOLE_MESSAGE ParsePlugInCommand(const char* pCommand, IBuffPacket* pBuffPacket) const;                 //ִ������

    void Close();                                                                           //�رջ���

    int Init();                                                                             //��ʼ���������

    //��ʼ������
    bool SetConsoleKey(vector<xmlConsoleKeys::_ConsoleKey> vecConsoleKeyList);       //�����֤�����keyֵ

private:
    EM_CONSOLE_MESSAGE  ParseCommand_Plugin(const char* pCommand, IBuffPacket* pBuffPacket, uint8& u1OutputType) const;                  //ִ������(����ڲ�����)
    EM_CONSOLE_MESSAGE  ParseCommand(const char* pCommand, IBuffPacket* pBuffPacket, uint8& u1OutputType);                               //ִ������
    EM_CONSOLE_MESSAGE  DoCommand(const _CommandInfo& CommandInfo, IBuffPacket* pCurrBuffPacket, IBuffPacket* pReturnBuffPacket) const;  //������������
    bool GetCommandInfo(const string& strCommand, _CommandInfo& CommandInfo, bool blCheck = true) const;                      //�������и��Ӧ���е����ݸ�ʽ
    bool CheckConsoleKey(const char* pKey) const;                                                                               //��֤key

    vector<xmlConsoleKeys::_ConsoleKey> m_vecConsolekeyList;
    CConsolePromissions m_objConsolePromissions;

    //����ͳһ�ĺ���ָ��ģ��
    using DoMessage_Logic = void(*)(const _CommandInfo& CommandInfo, IBuffPacket* pBuffPacket, uint16& u2ReturnCommandID);
    using mapMessageList = ACE_Hash_Map<string, DoMessage_Logic>;
    mapMessageList      m_objHashMessageLogicList;             //������Hash�б�
};

using App_ConsoleManager = ACE_Singleton<CConsoleMessage, ACE_Null_Mutex>;
#endif
