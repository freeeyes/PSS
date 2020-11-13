#ifndef _CONSOLEPROMISS_H
#define _CONSOLEPROMISS_H

#include "define.h"
#include "HashTable.h"
#include "XmlOpeation.h"

//Console»®œﬁ±Ì
//add by freeeyes

const uint32 MAX_COMSOLE_COMMAND_COUNT = 100;

class _Console_Command_Info
{
public:
    string m_strCommand;
    string m_strUser;

    _Console_Command_Info()
    {
    }
};

class CConsolePromissions
{
public:
    CConsolePromissions();

    void Init(const char* pFileName);
    void Close();

    int Check_Promission(const char* pCommandName, const char* pUser);

private:
    int Check_Split_User(const char* pUser, const char* pUserList) const;

    CHashTable<_Console_Command_Info> m_objHashCommandList;
};

#endif
