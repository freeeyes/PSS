/*
 * 2013-9-10:build by ken <http://kenchowcn.com>
 *
 */

#include <ace/OS_NS_sys_socket.h>
#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/OS_NS_string.h>
#include <ace/Asynch_IO.h>
#include <ace/OS.h> 
#include <ace/Log_Msg.h>

#define SMTP_PORT         25
#define SMTP_MTU          800
#define MAX_EMAIL_LEN     64
#define TEXT_BOUNDARY     "KkK170891tpbkKk__FV_KKKkkkjjwq"


int authEmail(const ACE_HANDLE socketFd, const unsigned char *mailAddr, const unsigned char *mailPasswd);
int sendEmail(const ACE_HANDLE socketFd, const unsigned char *fromMail, const unsigned char *toMail,
                 const unsigned char *textMail, const int textLen);
int mailText(unsigned char **mail, const unsigned char *fromMail, const unsigned char *toMail, 
                 const unsigned char *mailSubject, const unsigned char *mailBody);
int mailAttachment(unsigned char **mail, const unsigned char *filePath);
int mailEnd(unsigned char **mail);
int connectSmtp(ACE_HANDLE& socketFd, const unsigned char* smtpUrl, const unsigned short smtpPort);


