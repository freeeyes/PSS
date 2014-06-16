/*
* 2013-9-10:build by ken <http://kenchowcn.com>
* 一个不错的类，我这里使用ace替换其中的函数实现跨平台
*
*/

#include <ace/OS_NS_sys_socket.h>
#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/OS_NS_string.h>
#include <ace/Asynch_IO.h>
#include <ace/OS.h> 
#include <ace/Log_Msg.h>
#include "smtp.h"
#include "base64.h"

int stringCut(const unsigned char *pcSrc, const char *start, const char *end, char *pcDest)
{
	unsigned char *posStart = NULL, *posEnd = NULL;
	int len;

	if (0 == *pcSrc || NULL == pcDest)
	{
		return -1;
	}

	if (NULL == start)
	{
		posStart = (unsigned char*)pcSrc;
	}
	else
	{
		posStart = (unsigned char* )ACE_OS::strstr((char* )pcSrc, (char* )start);
		if (NULL == posStart)
		{
			return -1;
		}
		/* ignore header */
		posStart++;
	}

	if (NULL == end)
	{
		posEnd = posStart + ACE_OS::strlen((char* )posStart);
	}
	else
	{
		posEnd = (unsigned char* )ACE_OS::strstr((char* )pcSrc, (char* )end);
		if (NULL == posEnd)
		{
			return -1;
		}
	}

	len = posEnd - posStart;

	ACE_OS::strncpy((char* )pcDest, (char* )posStart, len);

	return len;
}

int connectSmtp(ACE_HANDLE& socketFd, const unsigned char* smtpUrl, const unsigned short smtpPort)
{
	struct sockaddr_in smtpAddr;
	struct hostent *host = NULL;

	if(NULL == (host = ACE_OS::gethostbyname((char* )smtpUrl)))
	{
		//ACE_DEBUG((LM_ERROR, "[connectSmtp]mail gethostbyname error.\n"));
		return -1;
	}

	ACE_OS::memset(&smtpAddr, 0, sizeof(smtpAddr));
	smtpAddr.sin_family = AF_INET;
	smtpAddr.sin_port = htons(smtpPort);
	smtpAddr.sin_addr = *((struct in_addr *)host->h_addr);

	socketFd = ACE_OS::socket(PF_INET, SOCK_STREAM, 0);

	if(0 > ACE_OS::connect(socketFd, (struct sockaddr *)&smtpAddr, sizeof(struct sockaddr)))
	{
		ACE_OS::close(socketFd);
		//ACE_DEBUG((LM_ERROR, "[connectSmtp]mail connect error.\n"));
		return -1;
	}

	return 0;
}

int safeRead(ACE_HANDLE socketFd, char *readData, int readLen)
{
	return ACE_OS::recv(socketFd, (char* )readData, readLen, 0);
}

int safeWrite(ACE_HANDLE socketFd, char *writeData, int writeLen)
{
	return ACE_OS::send(socketFd, (char* )writeData, writeLen, 0);
}

/*
* You can find more detail in here.
* http://www.ietf.org/rfc/rfc821.txt
*/
int recvStatus(const char *recvString)
{
	char statusStr[4] = {0};

	ACE_OS::memset(statusStr, 0, sizeof(statusStr));
	ACE_OS::strncpy(statusStr, (const char* )recvString, 3);

	SMTP_Print6("[%s][%d] status = %d\r\n", __FILE__, __LINE__, atoi(statusStr));

	switch (ACE_OS::atoi(statusStr))
	{
	case 250:
		{
			break;
		}
	case 235:
		{
			break;
		}
	case 354:
		{
			break;
		}
	case 334:
		{
			break;
		}
	case 221:
		{
			break;
		}
	default:
		{
			//ACE_DEBUG((LM_ERROR, "[recvStatus]You could be got exception status !\n"));
			return -1;
		}
	}

	return 0;
}

int authEmail(const ACE_HANDLE socketFd, const unsigned char *mailAddr, const unsigned char *mailPasswd)
{
	int outSize   = 0; 
	char readData[SMTP_MTU] = {0};
	char writeData[SMTP_MTU] = {0};
	char userName[MAX_EMAIL_LEN] = {0};
	char userPasswd[MAX_EMAIL_LEN] = {0};

	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);

	/* Send: EHLO */
	char szRELO[50] = {'\0'};
	ACE_OS::sprintf(szRELO, "EHLO Here\r\n");
	safeWrite(socketFd, szRELO, ACE_OS::strlen("EHLO Here\r\n"));

	/* Recv: EHLO */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	/* Send: AUTH LOGIN */
		char szLOGIN[50] = {'\0'};
	ACE_OS::sprintf(szLOGIN, "EHLO Here\r\n");
	safeWrite(socketFd, szLOGIN, ACE_OS::strlen("AUTH LOGIN\r\n"));

	/* Recv: AUTH LOGIN */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	/* Send: username */	
	ACE_OS::memset(&userName, 0, MAX_EMAIL_LEN);
	ACE_OS::memset(&writeData, 0, SMTP_MTU);
	stringCut((unsigned char*)mailAddr, NULL, (char* )"@", userName);

	outSize = BASE64_SIZE(strlen(userName));
	base64_encode(writeData, outSize, (const unsigned char *)userName, strlen(userName));
	ACE_OS::strcat(writeData, "\r\n");
	safeWrite(socketFd, writeData, strlen(writeData));

	/* Recv: username */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	/* Send: passwd */	
	ACE_OS::memset(&userPasswd, 0, MAX_EMAIL_LEN);
	ACE_OS::strcpy((char* )userPasswd, (char* )mailPasswd);
	ACE_OS::memset(&writeData, 0, SMTP_MTU);
	outSize = BASE64_SIZE(strlen(userPasswd));
	base64_encode(writeData, outSize, (const unsigned char *)userPasswd, strlen(userPasswd));
	ACE_OS::strcat(writeData, "\r\n");
	safeWrite(socketFd, writeData, strlen(writeData));

	/* Recv: passwd */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	return 0;
}

int sendEmail(const ACE_HANDLE socketFd, const unsigned char *fromMail, const unsigned char *toMail,
			  const unsigned char *textMail, const int textLen)
{
	char readData[SMTP_MTU] = {0};
	char writeData[SMTP_MTU] = {0};

	/* Send: MAIL FROM */
	ACE_OS::memset(&writeData, 0, SMTP_MTU);
	ACE_OS::sprintf(writeData, "MAIL FROM: <%s>\r\n", fromMail);
	safeWrite(socketFd, writeData, strlen(writeData));

	/* Recv: MAIL FROM */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	/* Send: RCPT TO */
	ACE_OS::memset(&writeData, 0, SMTP_MTU);
	ACE_OS::sprintf(writeData, "RCPT TO: <%s>\r\n", toMail);
	safeWrite(socketFd, writeData, strlen(writeData));

	/* Recv: RCPT TO */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	/* Send: DATA */
	ACE_OS::memset(&writeData, 0, SMTP_MTU);
	char szDATA[50] = {'\0'};
	ACE_OS::sprintf(szDATA, "DATA\r\n");
	safeWrite(socketFd, szDATA, ACE_OS::strlen("DATA\r\n"));

	/* Recv: DATA */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	/* Send: MAIL TEXT */
	safeWrite(socketFd, (char* )textMail, textLen);

	/* Recv: MAIL TEXT */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	/* Send: QUIT */
	ACE_OS::memset(&writeData, 0, SMTP_MTU);
		char szQUIT[50] = {'\0'};
		ACE_OS::sprintf(szQUIT, "QUIT\r\n");
	safeWrite(socketFd, szQUIT, ACE_OS::strlen("QUIT\r\n"));

	/* Recv: QUIT */
	ACE_OS::memset(&readData, 0, SMTP_MTU);
	safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
	recvStatus(readData);

	return 0;
}

/* subject and body could be null. */
int mailText(unsigned char **mail, const unsigned char *fromMail, const unsigned char *toMail, 
			 const unsigned char *mailSubject, const unsigned char *mailBody)
{
	char *szMailText = NULL;
	int mailTextLen = 0;
	char fromName[MAX_EMAIL_LEN] = {0};
	char toName[MAX_EMAIL_LEN] = {0};

	if ((NULL == *mail) || (NULL == fromMail) || (NULL == toMail))
	{
		//ACE_DEBUG((LM_ERROR, "[%s][%d] \r\n", __FILE__, __LINE__));
		return -1;
	}

	mailTextLen = ACE_OS::strlen((char* )fromMail) + 
		ACE_OS::strlen((char* )toMail) + 
		ACE_OS::strlen((char* )mailSubject) + 
		ACE_OS::strlen((char* )mailBody) + 500;

	szMailText = (char* )ACE_OS::calloc(mailTextLen, 1);
	if (NULL == szMailText)
	{
		//ACE_DEBUG((LM_ERROR, "[mailText]szMailText malloc fail!\n"));
		return -1;
	}

	ACE_OS::memset(&fromName, 0, MAX_EMAIL_LEN);
	stringCut(fromMail, NULL, "@", fromName);

	ACE_OS::memset(&toName, 0, MAX_EMAIL_LEN);
	stringCut(toMail, NULL, "@", toName);

	ACE_OS::snprintf(szMailText, mailTextLen, "From: \"%s\"<%s>\r\nTo: \"%s\"<%s>\r\nSubject: %s\r\nMIME-Version:1.0\r\nContent-Type:multipart/mixed;boundary=\"%s\"\r\n\r\n--%s\r\nContent-Type: text/plain; charset=\"gb2312\"\r\n\r\n%s\r\n\r\n--%s\r\n", fromName, fromMail, toName, toMail, mailSubject, TEXT_BOUNDARY, TEXT_BOUNDARY, mailBody, TEXT_BOUNDARY);

	*mail = (unsigned char* )realloc(*mail, strlen((char* )*mail)+strlen(szMailText)+1);
	if (NULL == *mail)
	{
		//ACE_DEBUG((LM_ERROR, "[mailText]realloc fail.\n"));
		return -1;
	}

	ACE_OS::strcat((char* )*mail, szMailText);

	free(szMailText);

	/* If I am in danger, let me konw, please */
	return (mailTextLen - strlen((char* )*mail));
}

/* static attachmemt size */
int mailAttachment(unsigned char **mail, const unsigned char *filePath)
{
	FILE *fp = NULL;
	int fileSize, base64Size, headerSize, len;
	char *attach = NULL, *base64Attach = NULL, *attachHeader = NULL;
	char fileName[MAX_EMAIL_LEN] = {0};
	const char *contentType = "Content-Type: application/octet-stream";
	const char *contentEncode = "Content-Transfer-Encoding: base64";
	const char *contentDes = "Content-Disposition: attachment";

	fp = ACE_OS::fopen((char* )filePath, "rb");
	if (NULL == fp)
	{
		//ACE_DEBUG((LM_ERROR, "[mailAttachment]fp fopen fail.\n"));
		return -1;
	}

	ACE_OS::fseek(fp, 0, SEEK_END);
	fileSize = ACE_OS::ftell(fp);
	if (0 > fileSize)
	{
		//ACE_DEBUG((LM_ERROR, "[mailAttachment]fp ftell fail.\n"));
		ACE_OS::fclose(fp);
		return -1;
	}

	ACE_OS::rewind(fp);

	attach = (char* )calloc(fileSize, 1);
	if (NULL == attach)
	{
		//ACE_DEBUG((LM_ERROR, "[mailAttachment]fp calloc fileSize fail.\n"));
		ACE_OS::fclose(fp);
		return -1;
	}

	headerSize = ACE_OS::strlen(contentType)+ACE_OS::strlen(contentEncode)+ACE_OS::strlen(contentDes)+200;
	attachHeader = (char* )calloc(headerSize, 1);
	if (NULL == attach)
	{
		//ACE_DEBUG((LM_ERROR, "[mailAttachment]fp calloc headerSize fail.\n"));
		ACE_OS::fclose(fp);
		return -1;
	}

	/* attachment header */
	stringCut(filePath, "/", NULL, fileName);

	ACE_OS::sprintf(attachHeader, "%s;name=\"%s\"\r\n%s\r\n%s;filename=\"%s\"\r\n\r\n", contentType, fileName, contentEncode, contentDes, fileName);

	base64Size = BASE64_SIZE(fileSize);
	base64Attach = (char* )calloc(base64Size, 1);
	if (NULL == base64Attach)
	{
		//ACE_DEBUG((LM_ERROR, "[mailAttachment]fp calloc base64Size fail.\n"));
		ACE_OS::fclose(fp);
		return -1;
	}

	len = ACE_OS::fread(attach, sizeof(char), fileSize, fp);

	SMTP_Print6("[%s][%d] %s size = %d, base64Size = %d \r\n",__FILE__, __LINE__, filePath, fileSize, base64Size);

	/* attachment transform to base64 */
	base64_encode(base64Attach, base64Size, (const unsigned char *)attach, fileSize);

	free(attach);

	*mail = (unsigned char* )realloc(*mail, strlen((char* )*mail)+headerSize+base64Size+1);
	if (NULL == *mail)
	{
		//ACE_DEBUG((LM_ERROR, "[mailAttachment]fp realloc base64Size fail.\n"));
		ACE_OS::fclose(fp);
		return -1;
	}

	ACE_OS::strcat((char* )*mail, attachHeader);
	ACE_OS::strcat((char* )*mail, base64Attach);

	free(attachHeader);
	free(base64Attach);

	ACE_OS::fclose(fp);

	return fileSize;
}

int mailEnd(unsigned char **mail)
{
	char bodyEnd[200] = {0};
	int len;

	ACE_OS::memset(bodyEnd, 0, sizeof(bodyEnd));
	ACE_OS::sprintf(bodyEnd, "\r\n--%s--\r\n\r\n.\r\n", TEXT_BOUNDARY);

	len = ACE_OS::strlen(bodyEnd);

	*mail = (unsigned char* )ACE_OS::realloc((char* )*mail, strlen((char* )*mail)+len+1);
	if (NULL == *mail)
	{
		//ACE_DEBUG((LM_ERROR, "[mailEnd]realloc mail fail.\n"));
		return -1;
	}

	ACE_OS::strcat((char* )*mail, bodyEnd);

	return 0;
}


//测试代码
/*
int main()
{
	int ret = 0;
	unsigned char *mail = NULL;
	const unsigned char *filePath = (const unsigned char*)"./test.jpg";
	const unsigned char *mailSubject = (const unsigned char*)"test";
	const unsigned char *mailBody = (const unsigned char*)"hello smtp!!";

	const unsigned char *fromMailAddr= (const unsigned char*)"centnet_voip@163.com";
	const unsigned char *mailPasswd= (const unsigned char*)"centnet";
	const unsigned char *toMailAddr= (const unsigned char*)"liuchao412@foxmail.com";
	const unsigned char *smtpUrl = (const unsigned char*)"smtp.163.com";

	mail = (unsigned char*) calloc(1, 1);

	ret = mailText(&mail, fromMailAddr, toMailAddr, mailSubject, mailBody);
	ret = mailAttachment(&mail, filePath);
	ret = mailEnd(&mail);

	SMTP_Print6("\r\n%s \r\n", mail);

	printf("\r\nprepare email OK ...\r\n");

	ACE_HANDLE fd;

	ret = connectSmtp(fd, smtpUrl, SMTP_PORT);
	if(ret == 0)
	{
		printf("connect OK ...\r\n");
	}

	ret = authEmail(fd, fromMailAddr, mailPasswd);

	printf("auth OK ...\r\n");

	ret = sendEmail(fd, fromMailAddr, toMailAddr, (const unsigned char *)mail, (const int)strlen((const char *)mail));

	printf("send OK ...\r\n");

	return 0;
}
*/



