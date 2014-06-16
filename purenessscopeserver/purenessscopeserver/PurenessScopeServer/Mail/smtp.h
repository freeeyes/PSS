/*
 * 2013-9-10:build by ken <http://kenchowcn.com>
 *
 */

int debug = 1;

#define SMTP_Print6       if(debug) printf


#define SMTP_PORT         25
#define SMTP_MTU          800
#define MAX_EMAIL_LEN     64
#define TEXT_BOUNDARY     "KkK170891tpbkKk__FV_KKKkkkjjwq"


int authEmail(const int socketFd, const unsigned char *mailAddr, const unsigned char *mailPasswd);
int sendEmail(const int socketFd, const unsigned char *fromMail, const unsigned char *toMail,
                 const unsigned char *textMail, const int textLen);
int mailText(unsigned char **mail, const unsigned char *fromMail, const unsigned char *toMail, 
                 const unsigned char *mailSubject, const unsigned char *mailBody);
int mailAttachment(unsigned char **mail, const unsigned char *filePath);
int mailEnd(unsigned char **mail);

