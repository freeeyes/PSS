#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//����ͻ�������(TCP)
#define COMMAND_HTTP_DATA          0xea01
#define COMMAND_RETURN_HTTP_DATA   0xfa01

//�������������Ϣ
#define RETURN_HTTP_HTML "<HTML>\r\n<HEAD><title>PSS��������Ϣ</title></HEAD>\r\n<BODY><div align=\"center\">PSS Welcome!</div></BODY>\r\n</HTML>"

using namespace std;

//���һ������ȥ�ո�ĺ���
//���ڹ淶�����ݴ�����Ϊ�ı������ܻ������ҵĿո�
inline void TrimString(char* pOld, char* pNew)
{
    int len = ACE_OS::strlen(pOld);
    int i = 0;
    int j = 1;

    while (*(pOld + i) == ' ')
    {
        i++;
    }

    while (*(pOld + len - j) == ' ')
    {
        j++;
    }

    len = len - i - j + 1;

    for (int p = 0; p < len; p++)
    {
        *(pNew + p) = *(pOld + i + p);
    }

    pNew[len] = '\0';
}

class CBaseCommand : public CClientCommand
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);

    void SetServerObject(CServerObject* pServerObject);

private:
    int DoMessage_HttpData(IMessage* pMessage, bool& bDeleteFlag);

private:
    CServerObject* m_pServerObject;
    int            m_nCount;
};

