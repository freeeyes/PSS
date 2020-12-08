#ifndef _CONSOLEPROMISS_H
#define _CONSOLEPROMISS_H

#include "define.h"
#include "HashTable.h"
#include "XmlOpeation.h"
#include <unordered_map>

//Console»®œﬁ±Ì
//add by freeeyes

const uint32 MAX_COMSOLE_COMMAND_COUNT = 100;

class _Console_Command_Info
{
public:
    string m_strCommand;
    string m_strUser;

    _Console_Command_Info() = default;
};

class CConsolePromissions
{
public:
    CConsolePromissions() = default;

    void Init(const char* pFileName);
    void Close();

    int Check_Promission(const string& pCommandName, const string& pUser);

private:
    int Check_Split_User(const string& pUser, const string& pUserList) const;

    using hashmapCommandInfo = unordered_map <string, shared_ptr<_Console_Command_Info>>;
    hashmapCommandInfo m_objHashCommandList;
};

#endif
