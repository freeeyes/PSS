#ifndef _MAILMANAGER_H
#define _MAILMANAGER_H

#include "define.h"
#include "smtp.h"

class CMailManager
{
public:
	CMailManager();
	~CMailManager();

	void SetMailAlert(_MailAlert* pMailAlert);

	bool SendMail(const char* pTitle, const char* pText);

private:
	_MailAlert* m_pMailAlert;
};

#endif
