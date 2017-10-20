#ifndef _CONSOLEPROMISS_H
#define _CONSOLEPROMISS_H

#include "define.h"
#include "HashTable.h"
#include "XmlOpeation.h"

//Console»®œﬁ±Ì
//add by freeeyes

#define MAX_COMSOLE_COMMAND_COUNT 100

struct _Console_Command_Info
{
    char m_szCommand[MAX_BUFF_50];
    char m_szUser[MAX_BUFF_200];

    _Console_Command_Info()
    {
        m_szCommand[0] = '\0';
        m_szUser[0] = '\0';
    }
};

class CConsolePromissions
{
public:
    CConsolePromissions();
    ~CConsolePromissions();

    void Init(const char* pFileName);
    void Close();

    int Check_Promission(const char* pCommandName, const char* pUser);

private:
    int Check_Split_User(const char* pUser, const char* pUserList);

private:
    CHashTable<_Console_Command_Info> m_objHashCommandList;
};

#endif
