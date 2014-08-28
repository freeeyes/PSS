#include "ConsoleAccept.h"

int ConnectConsoleAcceptor::make_svc_handler(CConsoleHandler*& sh)
{
	//如果正在处理的链接超过了服务器设定的数值，则不允许链接继续链接服务器
	CConsoleHandler* pConsoleHandler = new CConsoleHandler();

	if (NULL != pConsoleHandler)
	{
		pConsoleHandler->reactor(this->reactor());
		sh = pConsoleHandler;
		return 0;
	}
	else
	{
		return -1;
	}
}

