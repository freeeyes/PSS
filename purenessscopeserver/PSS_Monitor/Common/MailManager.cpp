#include "MailManager.h"

CMailManager::CMailManager()
{
	m_pMailAlert = NULL;
}

CMailManager::~CMailManager()
{
}

bool CMailManager::SendMail(const char* pTitle, const char* pText)
{
	//·¢ËÍÓÊ¼þ
	if(NULL == m_pMailAlert || 0 == m_pMailAlert->m_u4MailID)
	{
		OUR_DEBUG((LM_ERROR, "[CMailManager::SendMail]MailID not set.\n"));
		return false;
	}

	unsigned char * pMail = NULL;
	pMail = (unsigned char * )ACE_OS::calloc(1, 1);

	int nRet = 0;
	nRet = mailText(&pMail, 
		(const unsigned char *)m_pMailAlert->m_szFromMailAddr, 
		(const unsigned char *)m_pMailAlert->m_szToMailAddr, 
		(const unsigned char *)pTitle, 
		(const unsigned char *)pText);
	nRet = mailEnd(&pMail);

	ACE_HANDLE fd;

	nRet = connectSmtp(fd, (const unsigned char *)m_pMailAlert->m_szMailUrl, 
		m_pMailAlert->m_u4MailPort);
	if(nRet != 0)
	{
		OUR_DEBUG((LM_ERROR, "[CMailManager::SendMail]MailID connectSmtp error.\n"));
		free(pMail);
		return false;
	}

	nRet = authEmail(fd, 
		(const unsigned char *)m_pMailAlert->m_szFromMailAddr, 
		(const unsigned char *)m_pMailAlert->m_szMailPass);
	if(nRet != 0)
	{
		OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID authEmail error.\n"));
		free(pMail);
		return false;
	}

	nRet = sendEmail(fd, (const unsigned char *)m_pMailAlert->m_szFromMailAddr, 
		(const unsigned char *)m_pMailAlert->m_szToMailAddr,
		(const unsigned char *)pMail, 
		(const int)strlen((const char *)pMail));
	if(nRet != 0)
	{
		OUR_DEBUG((LM_ERROR, "[CLogFile::SendMail]MailID sendEmail error.\n"));
		free(pMail);
		return false;
	}

	free(pMail);
	return true;
}

void CMailManager::SetMailAlert(_MailAlert* pMailAlert)
{
	m_pMailAlert = pMailAlert;
}