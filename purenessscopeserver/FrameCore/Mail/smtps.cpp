#include "smtps.h"

#ifdef __THREE_LIBRARY_CURL__
static size_t read_callback(void* ptr, size_t size, size_t nmemb, void* userp)
{
    struct WriteThis* pooh = (struct WriteThis*)userp;
    const char* data;

    if (size*nmemb < 1)
    {
        return 0;
    }

    data = pooh->pText[pooh->counter];
    printf("[read_callback]data=%s.\n", data);

    if (data)
    {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        pooh->counter++; /* advance pointer */
        return len;
    }

    return 0;
}

static struct timeval tvnow(void)
{
    struct timeval now;
    now.tv_sec = (long)time(nullptr);
    now.tv_usec = 0;
    return now;

}

static long tvdiff(struct timeval newer, struct timeval older)
{
    return (newer.tv_sec - older.tv_sec) * 1000 +
           (newer.tv_usec - older.tv_usec) / 1000;
}
#endif

int Send_Mail_From_Ssl(const char* pUser, const char* pPass, const char* pFrom, const char* pTo, const char* pUrl, const char* pTitle, const char* pData)
{
#ifdef __THREE_LIBRARY_CURL__
    CURL* curl;
    CURLM* mcurl;
    int still_running = 1;
    struct timeval mp_start;
    struct WriteThis pooh;
    struct curl_slist* rcpt_list = nullptr;

    //组装发送内容
    char** textList = new char* [5];

    for (int i = 0; i < 5; i++)
    {
        textList[i] = new char[1024];
        memset(textList[i], 0, 1024);
    }

    sprintf(textList[0], "Subject: %s\n", pTitle);
    sprintf(textList[1], "\n");
    sprintf(textList[2], "%s\n", pData);
    sprintf(textList[3], "\n");
    textList[4] = nullptr;

    pooh.counter = 0;
    pooh.pText = textList;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if (!curl)
    {
        return 1;
    }

    mcurl = curl_multi_init();

    if (!mcurl)
    {
        return 2;
    }

    rcpt_list = curl_slist_append(rcpt_list, pTo);
    /* more addresses can be added here
    rcpt_list = curl_slist_append(rcpt_list, "<others@example.com>");
    */

    curl_easy_setopt(curl, CURLOPT_URL, pUrl); // 此处的要带上smtp端口
    curl_easy_setopt(curl, CURLOPT_USERNAME, pUser);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, pPass);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, pFrom);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, rcpt_list);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_SESSIONID_CACHE, 0L);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_multi_add_handle(mcurl, curl);

    mp_start = tvnow();

    /* we start some action by calling perform right away */
    curl_multi_perform(mcurl, &still_running);

    while (still_running)
    {
        struct timeval timeout;
        int rc; /* select() return code */

        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcep;
        int maxfd = -1;

        long curl_timeo = -1;

        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);

        /* set a suitable timeout to play around with */
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        curl_multi_timeout(mcurl, &curl_timeo);

        if (curl_timeo >= 0)
        {
            timeout.tv_sec = curl_timeo / 1000;

            if (timeout.tv_sec > 1)
            {
                timeout.tv_sec = 1;
            }
            else
            {
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
            }
        }

        /* get file descriptors from the transfers */
        curl_multi_fdset(mcurl, &fdread, &fdwrite, &fdexcep, &maxfd);

        /* In a real-world program you OF COURSE check the return code of the
        function calls.  On success, the value of maxfd is guaranteed to be
        greater or equal than -1.  We call select(maxfd + 1, ...), specially in
        case of (maxfd == -1), we call select(0, ...), which is basically equal
        to sleep. */

        rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);

        if (tvdiff(tvnow(), mp_start) > MULTI_PERFORM_HANG_TIMEOUT)
        {
            fprintf(stderr, "ABORTING TEST, since it seems "
                    "that it would have run forever.\n");
            break;
        }

        switch (rc)
        {
        case -1:
            /* select error */
            break;

        case 0: /* timeout */
        default: /* action */
            curl_multi_perform(mcurl, &still_running);
            break;
        }
    }


    for (int i = 0; i < 5; i++)
    {
        delete[] textList[i];
    }

    delete[] textList;

    curl_slist_free_all(rcpt_list);
    curl_multi_remove_handle(mcurl, curl);
    curl_multi_cleanup(mcurl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
#else
    return 0;
#endif
}

