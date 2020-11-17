/*
* 2013-9-10:build by ken <http://kenchowcn.com>
* һ��������࣬������ʹ��ace�滻���еĺ���ʵ�ֿ�ƽ̨
*
*/

#include "smtp.h"
#include "base64.h"

int stringCut(const unsigned char* pcSrc, const char* start, const char* end, char* pcDest)
{
    unsigned char* posStart = nullptr;
    unsigned char* posEnd = nullptr;
    int len;

    if (0 == *pcSrc || nullptr == pcDest)
    {
        return -1;
    }

    if (nullptr == start)
    {
        posStart = (unsigned char*)pcSrc;
    }
    else
    {
        posStart = (unsigned char* )ACE_OS::strstr((char* )pcSrc, (char* )start);

        if (nullptr == posStart)
        {
            return -1;
        }

        /* ignore header */
        posStart++;
    }

    if (nullptr == end)
    {
        posEnd = posStart + ACE_OS::strlen((char* )posStart);
    }
    else
    {
        posEnd = (unsigned char* )ACE_OS::strstr((char* )pcSrc, (char* )end);

        if (nullptr == posEnd)
        {
            return -1;
        }
    }

    len = (int)(posEnd - posStart);

    ACE_OS::strncpy((char* )pcDest, (char* )posStart, len);

    return len;
}

int connectSmtp(ACE_HANDLE& socketFd, const unsigned char* smtpUrl, const unsigned short smtpPort)
{
    struct sockaddr_in smtpAddr;
    struct hostent   host;
    ACE_HOSTENT_DATA buf  = {'\0'};
    int    hosterr        = 0;

    if(nullptr == ACE_OS::gethostbyname_r((char* )smtpUrl, &host, buf, &hosterr))
    {
        ACE_DEBUG((LM_ERROR, "[connectSmtp]mail gethostbyname error.\n"));
        return -1;
    }

    ACE_OS::memset(&smtpAddr, 0, sizeof(smtpAddr));
    smtpAddr.sin_family = AF_INET;
    smtpAddr.sin_port = htons(smtpPort);
    smtpAddr.sin_addr = *((struct in_addr*)host.h_addr);

    socketFd = ACE_OS::socket(PF_INET, SOCK_STREAM, 0);

    if (socketFd == ACE_INVALID_HANDLE)
    {
        return -1;
    }

    if(0 > ACE_OS::connect(socketFd, (struct sockaddr*)&smtpAddr, sizeof(struct sockaddr)))
    {
        ACE_OS::close(socketFd);
        ACE_DEBUG((LM_ERROR, "[connectSmtp]mail connect error.\n"));
        return -1;
    }

    return 0;
}

int safeRead(ACE_HANDLE socketFd, char* readData, int readLen)
{
    return (int)ACE_OS::recv(socketFd, (char* )readData, readLen, 0);
}

int safeWrite(ACE_HANDLE socketFd, char* writeData, int writeLen)
{
    return (int)ACE_OS::send(socketFd, (char* )writeData, writeLen, 0);
}

/*
* You can find more detail in here.
* http://www.ietf.org/rfc/rfc821.txt
*/
int recvStatus(const char* recvString)
{
    char statusStr[4] = {0};

    ACE_OS::memset(statusStr, 0, sizeof(statusStr));
    ACE_OS::strncpy(statusStr, (const char* )recvString, 3);

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
            ACE_DEBUG((LM_ERROR, "[recvStatus]You could be got exception status !\n"));
            return -1;
        }
    }

    return 0;
}

int Input_Mail_Command(char* pReadData, char* pWriteData, const ACE_HANDLE socketFd, const char* pName)
{
    if (0 >= safeWrite(socketFd, pWriteData, (int)strlen(pWriteData)))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]@ safeWrite error.\n"));
        return -1;
    }

    /* Recv: username */
    ACE_OS::memset(pReadData, 0, SMTP_MTU);

    if (0 >= safeRead(socketFd, pReadData, SMTP_MTU))
    {
        ACE_DEBUG((LM_INFO, "[%s]@ safeRead error.\n", pName));
        return -1;
    }

    if (0 > recvStatus(pReadData))
    {
        ACE_DEBUG((LM_INFO, "[%s]@ recvStatus error.\n", pName));
        return -1;
    }

    return 0;
}

int authEmail(const ACE_HANDLE socketFd, const unsigned char* mailAddr, const unsigned char* mailPasswd)
{
    int outSize   = 0;
    char readData[SMTP_MTU] = {0};
    char writeData[SMTP_MTU] = {0};
    char userName[MAX_EMAIL_LEN] = {0};
    char userPasswd[MAX_EMAIL_LEN] = {0};

    ACE_OS::memset(&readData, 0, SMTP_MTU);

    if (0 >= safeRead(socketFd, readData, SMTP_MTU))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]safeRead error.\n"));
        return -1;
    }

    /* Send: EHLO */
    char szRELO[50] = {'\0'};
    ACE_OS::sprintf(szRELO, "EHLO Here\r\n");

    if (0 >= safeWrite(socketFd, szRELO, (int)ACE_OS::strlen("EHLO Here\r\n")))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]EHLO safeWrite error.\n"));
        return -1;
    }

    /* Recv: EHLO */
    ACE_OS::memset(&readData, 0, SMTP_MTU);

    if (0 >= safeRead(socketFd, readData, SMTP_MTU))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]EHLO safeRead error.\n"));
        return -1;
    }

    if (0 > recvStatus(readData))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]EHLO recvStatus error.\n"));
        return -1;
    }

    /* Send: AUTH LOGIN */
    char szLOGIN[50] = {'\0'};
    ACE_OS::sprintf(szLOGIN, "AUTH LOGIN\r\n");

    if (0 >= safeWrite(socketFd, szLOGIN, (int)ACE_OS::strlen("AUTH LOGIN\r\n")))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]AUTH safeWrite error.\n"));
        return -1;
    }

    /* Recv: AUTH LOGIN */
    ACE_OS::memset(&readData, 0, SMTP_MTU);

    if (0 >= safeRead(socketFd, readData, SMTP_MTU))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]AUTH safeRead error.\n"));
        return -1;
    }

    if (0 > recvStatus(readData))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]AUTH recvStatus error.\n"));
        return -1;
    }

    /* Send: username */
    ACE_OS::memset(&userName, 0, MAX_EMAIL_LEN);
    ACE_OS::memset(&writeData, 0, SMTP_MTU);

    if (0 > stringCut((unsigned char*)mailAddr, nullptr, (char*)"@", userName))
    {
        ACE_DEBUG((LM_INFO, "[authEmail]stringCut error.\n"));
        return -1;
    }

    outSize = (int)BASE64_SIZE(strlen(userName));
    base64_encode(writeData, (int)outSize, (const unsigned char*)userName, (int)strlen(userName));
    ACE_OS::strcat(writeData, "\r\n");

    if (-1 == Input_Mail_Command(readData, writeData, socketFd, "authEmail"))
    {
        return -1;
    }

    /* Send: passwd */
    ACE_OS::memset(&userPasswd, 0, MAX_EMAIL_LEN);
    ACE_OS::strcpy((char* )userPasswd, (char* )mailPasswd);
    ACE_OS::memset(&writeData, 0, SMTP_MTU);
    outSize = (int)BASE64_SIZE(strlen(userPasswd));
    base64_encode(writeData, (int)outSize, (const unsigned char*)userPasswd, (int)strlen(userPasswd));
    ACE_OS::strcat(writeData, "\r\n");

    if (-1 == Input_Mail_Command(readData, writeData, socketFd, "authEmail"))
    {
        return -1;
    }

    return 0;
}

int sendEmail(const ACE_HANDLE socketFd, const unsigned char* fromMail, const unsigned char* toMail,
              const unsigned char* textMail, const int textLen)
{
    char readData[SMTP_MTU] = {0};
    char writeData[SMTP_MTU] = {0};

    /* Send: MAIL FROM */
    ACE_OS::memset(&writeData, 0, SMTP_MTU);
    ACE_OS::sprintf(writeData, "MAIL FROM: <%s>\r\n", fromMail);

    if (-1 == Input_Mail_Command(readData, writeData, socketFd, "sendEmail"))
    {
        return -1;
    }

    /* Send: RCPT TO */
    ACE_OS::memset(&writeData, 0, SMTP_MTU);
    ACE_OS::sprintf(writeData, "RCPT TO: <%s>\r\n", toMail);

    if (-1 == Input_Mail_Command(readData, writeData, socketFd, "sendEmail"))
    {
        return -1;
    }

    /* Send: DATA */
    ACE_OS::memset(&writeData, 0, SMTP_MTU);
    char szDATA[50] = {'\0'};
    ACE_OS::sprintf(szDATA, "DATA\r\n");

    if (0 >= safeWrite(socketFd, szDATA, (int)ACE_OS::strlen("DATA\r\n")))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]DATA safeWrite error.\n"));
        return -1;
    }

    /* Recv: DATA */
    ACE_OS::memset(&readData, 0, SMTP_MTU);

    if (0 >= safeRead(socketFd, readData, SMTP_MTU))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]SMTP_MTU safeRead error.\n"));
        return -1;
    }

    if (0 > recvStatus(readData))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]SMTP_MTU recvStatus error.\n"));
        return -1;
    }

    /* Send: MAIL TEXT */
    if (0 >= safeWrite(socketFd, (char*)textMail, textLen))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]SMTP_MTU safeWrite error.\n"));
        return -1;
    }

    /* Recv: MAIL TEXT */
    ACE_OS::memset(&readData, 0, SMTP_MTU);

    if (0 >= safeRead(socketFd, readData, SMTP_MTU))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]SMTP_MTU safeRead error.\n"));
        return -1;
    }

    if (0 > recvStatus(readData))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]SMTP_MTU recvStatus error.\n"));
        return -1;
    }

    /* Send: QUIT */
    ACE_OS::memset(&writeData, 0, SMTP_MTU);
    char szQUIT[50] = {'\0'};
    ACE_OS::sprintf(szQUIT, "QUIT\r\n");

    if (0 >= safeWrite(socketFd, szQUIT, (int)ACE_OS::strlen("QUIT\r\n")))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]SMTP_MTU safeWrite error.\n"));
        return -1;
    }

    /* Recv: QUIT */
    ACE_OS::memset(&readData, 0, SMTP_MTU);

    if (0 >= safeRead(socketFd, readData, SMTP_MTU))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]SMTP_MTU safeRead error.\n"));
        return -1;
    }

    if (0 > recvStatus(readData))
    {
        ACE_DEBUG((LM_INFO, "[sendEmail]SMTP_MTU recvStatus error.\n"));
        return -1;
    }

    return 0;
}

/* subject and body could be nullptr. */
int mailText(unsigned char** mail, const unsigned char* fromMail, const unsigned char* toMail,
             const unsigned char* mailSubject, const unsigned char* mailBody)
{
    char* szMailText = nullptr;
    int mailTextLen = 0;
    char fromName[MAX_EMAIL_LEN] = {0};
    char toName[MAX_EMAIL_LEN] = {0};

    if ((nullptr == *mail) || (nullptr == fromMail) || (nullptr == toMail))
    {
        ACE_DEBUG((LM_ERROR, "[mailText][%s][%d] \r\n", __FILE__, __LINE__));
        return -1;
    }

    mailTextLen = (int)ACE_OS::strlen((char* )fromMail) +
                  (int)ACE_OS::strlen((char* )toMail) +
                  (int)ACE_OS::strlen((char* )mailSubject) +
                  (int)ACE_OS::strlen((char* )mailBody) + 500;

    szMailText = (char* )ACE_OS::calloc(mailTextLen, 1);

    if (nullptr == szMailText)
    {
        ACE_DEBUG((LM_ERROR, "[mailText]szMailText malloc fail!\n"));
        free(szMailText);
        return -1;
    }

    ACE_OS::memset(&fromName, 0, MAX_EMAIL_LEN);

    if (-1 == stringCut(fromMail, nullptr, "@", fromName))
    {
        ACE_DEBUG((LM_ERROR, "[mailText]stringCut fromMail fail!\n"));
        free(szMailText);
        return -1;
    }

    ACE_OS::memset(&toName, 0, MAX_EMAIL_LEN);

    if (-1 == stringCut(toMail, nullptr, "@", toName))
    {
        ACE_DEBUG((LM_ERROR, "[mailText]stringCut toMail fail!\n"));
        free(szMailText);
        return -1;
    }

    ACE_OS::snprintf(szMailText, mailTextLen, "From: \"%s\"<%s>\r\nTo: \"%s\"<%s>\r\nSubject: %s\r\nMIME-Version:1.0\r\nContent-Type:multipart/mixed;boundary=\"%s\"\r\n\r\n--%s\r\nContent-Type: text/plain; charset=\"gb2312\"\r\n\r\n%s\r\n\r\n--%s\r\n", fromName, fromMail, toName, toMail, mailSubject, TEXT_BOUNDARY, TEXT_BOUNDARY, mailBody, TEXT_BOUNDARY);

    *mail = (unsigned char* )realloc(*mail, strlen((char* )*mail)+strlen(szMailText)+1);

    if (nullptr == *mail)
    {
        ACE_DEBUG((LM_ERROR, "[mailText]realloc fail.\n"));
        free(szMailText);
        return -1;
    }

    ACE_OS::strcat((char* )*mail, szMailText);

    free(szMailText);

    /* If I am in danger, let me konw, please */
    return (int)(mailTextLen - strlen((char* )*mail));
}

/* static attachmemt size */
int mailAttachment(unsigned char** mail, const unsigned char* filePath)
{
    int fileSize, base64Size, headerSize;
    char* attach = nullptr, *base64Attach = nullptr, *attachHeader = nullptr;
    char fileName[MAX_EMAIL_LEN] = {0};
    const char* contentType = "Content-Type: application/octet-stream";
    const char* contentEncode = "Content-Transfer-Encoding: base64";
    const char* contentDes = "Content-Disposition: attachment";

    FILE* fp = ACE_OS::fopen((char* )filePath, "rb");

    if (nullptr == fp)
    {
        ACE_DEBUG((LM_ERROR, "[mailAttachment]fp fopen fail.\n"));
        return -1;
    }

    ACE_OS::fseek(fp, 0, SEEK_END);
    fileSize = ACE_OS::ftell(fp);

    if (0 > fileSize)
    {
        ACE_DEBUG((LM_ERROR, "[mailAttachment]fp ftell fail.\n"));
        ACE_OS::fclose(fp);
        return -1;
    }

    ACE_OS::rewind(fp);

    attach = (char* )calloc(fileSize, 1);

    if (nullptr == attach)
    {
        ACE_DEBUG((LM_ERROR, "[mailAttachment]fp calloc fileSize fail.\n"));
        ACE_OS::fclose(fp);
        return -1;
    }

    headerSize = (int)ACE_OS::strlen(contentType)+(int)ACE_OS::strlen(contentEncode)+(int)ACE_OS::strlen(contentDes)+200;
    attachHeader = (char* )calloc(headerSize, 1);

    if (nullptr == attachHeader)
    {
        ACE_DEBUG((LM_ERROR, "[mailAttachment]fp calloc headerSize fail.\n"));
        ACE_OS::fclose(fp);
        free(attach);
        return -1;
    }

    /* attachment header */
    if (-1 == stringCut(filePath, "/", nullptr, fileName))
    {
        ACE_DEBUG((LM_INFO, "[mailAttachment]stringCut filePath fail.\n"));
        ACE_OS::fclose(fp);
        free(attachHeader);
        free(attach);
        return -1;
    }

    ACE_OS::sprintf(attachHeader, "%s;name=\"%s\"\r\n%s\r\n%s;filename=\"%s\"\r\n\r\n", contentType, fileName, contentEncode, contentDes, fileName);

    base64Size = BASE64_SIZE(fileSize);
    base64Attach = (char* )calloc(base64Size, 1);

    if (nullptr == base64Attach)
    {
        ACE_DEBUG((LM_ERROR, "[mailAttachment]fp calloc base64Size fail.\n"));
        ACE_OS::fclose(fp);
        free(attach);
        free(attachHeader);
        return -1;
    }

    size_t u4Read = ACE_OS::fread(attach, sizeof(char), fileSize, fp);

    if (u4Read <= 0)
    {
        ACE_DEBUG((LM_ERROR, "[mailAttachment]fp realloc base64Size fail.\n"));
        ACE_OS::fclose(fp);
        free(attach);
        free(attachHeader);
        free(base64Attach);
        return -1;
    }

    /* attachment transform to base64 */
    base64_encode(base64Attach, base64Size, (const unsigned char*)attach, fileSize);

    free(attach);

    *mail = (unsigned char* )realloc(*mail, strlen((char* )*mail)+headerSize+base64Size+1);

    if (nullptr == *mail)
    {
        ACE_DEBUG((LM_ERROR, "[mailAttachment]fp realloc base64Size fail.\n"));
        ACE_OS::fclose(fp);
        free(attachHeader);
        free(base64Attach);
        return -1;
    }

    ACE_OS::strcat((char* )*mail, attachHeader);
    ACE_OS::strcat((char* )*mail, base64Attach);

    free(attachHeader);
    free(base64Attach);

    ACE_OS::fclose(fp);

    return fileSize;
}

int mailEnd(unsigned char** mail)
{
    char bodyEnd[200] = {0};
    int len;

    ACE_OS::memset(bodyEnd, 0, sizeof(bodyEnd));
    ACE_OS::sprintf(bodyEnd, "\r\n--%s--\r\n\r\n.\r\n", TEXT_BOUNDARY);

    len = (int)ACE_OS::strlen(bodyEnd);

    *mail = (unsigned char* )ACE_OS::realloc((char* )*mail, strlen((char* )*mail)+len+1);

    if (nullptr == *mail)
    {
        ACE_DEBUG((LM_ERROR, "[mailEnd]realloc mail fail.\n"));
        return -1;
    }

    ACE_OS::strcat((char* )*mail, bodyEnd);

    return 0;
}


//���Դ���
/*
int main()
{
    int ret = 0;
    unsigned char *mail = nullptr;
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

    //SMTP_Print6("\r\n%s \r\n", mail);

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



