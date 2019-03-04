#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_HTTP_DATA          0xea01
#define COMMAND_RETURN_HTTP_DATA   0xfa01

//设置握手相关信息
#define RETURN_HTTP_HTML "<HTML>\r\n<HEAD><title>PSS服务器信息</title></HEAD>\r\n<BODY><div align=\"center\">PSS Welcome!</div></BODY>\r\n</HTML>"

using namespace std;

//添加一个左右去空格的函数
//用于规范化数据串，因为文本串可能会有左右的空格
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

