#include "KafkaClient.h"

//日志输出线程
static void logger_watch(const rd_kafka_t* rk, int level,
                         const char* fac, const char* buf)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    fprintf(stderr, "%u.%03u RDKAFKA-%i-%s: %s: %s\n",
            (int)tv.tv_sec, (int)(tv.tv_usec / 1000),
            level, fac, rk ? rd_kafka_name(rk) : NULL, buf);
}


CKafkaClient::CKafkaClient()
{
    m_kafka_conf = NULL;
    m_topic_conf = NULL;
    m_rkt        = NULL;
    m_rk         = NULL;
    memset(m_szError, 0, 500);
    m_start_offset = 0;
    m_seek_offset  = 0;
}

CKafkaClient::~CKafkaClient()
{
    Close();
}

void CKafkaClient::Set_Log_Path(const char* pPath)
{
    sprintf(m_szPath, "%s", pPath);
}

void CKafkaClient::Set_Start_Offset(int nBegin)
{
    m_start_offset = nBegin;
}

void CKafkaClient::Set_Log_File(int nIndex)
{
    FILE* pFile = fopen(m_szPath, "w");

    if(NULL == pFile)
    {
        return;
    }

    char szBuffer[50] = {'\0'};
    sprintf(szBuffer, "%d", nIndex);
    fwrite(szBuffer, strlen(szBuffer), sizeof(char), pFile);
    fclose(pFile);
}

bool CKafkaClient::Connect_Kafka_Server(const char* pAddress, ENUM_KAFKA_QUEUE_CLASS em_Class)
{
    m_kafka_conf = rd_kafka_conf_new();

    rd_kafka_conf_set_log_cb(m_kafka_conf, logger_watch);

    rd_kafka_conf_set(m_kafka_conf, "broker.version.fallback", "0.8.1", NULL, 0);

    if(em_Class == KA_PRODUCER)
    {
        //创建连接(消息创造者)
        m_rk = rd_kafka_new(RD_KAFKA_PRODUCER, m_kafka_conf, m_szError, sizeof(m_szError));
    }
    else
    {
        //创建连接(消息消费者)
        m_rk = rd_kafka_new(RD_KAFKA_CONSUMER, m_kafka_conf, m_szError, sizeof(m_szError));
    }

    if(NULL == m_rk)
    {
        printf("[CKafkaClient::Connect_Kafka_Server]err=%s",
               rd_kafka_err2str(rd_kafka_last_error()));
        return false;
    }

    rd_kafka_set_log_level(m_rk, LOG_DEBUG);
    rd_kafka_brokers_add(m_rk, pAddress);

    return true;
}

bool CKafkaClient::Create_topic(const char* pTopic)
{
    m_topic_conf = rd_kafka_topic_conf_new();

    m_rkt = rd_kafka_topic_new(m_rk, pTopic, m_topic_conf);

    if(NULL == m_rkt)
    {
        printf("[CKafkaClient::Create_topic]err=%s",
               rd_kafka_err2str(rd_kafka_last_error()));
        return false;
    }

    return true;
}

bool CKafkaClient::Send_Topic_Message(const char* pData, int nLen)
{
    //向标题发送数据
    //RD_KAFKA_MSG_F_COPY和RD_KAFKA_MSG_F_FREE
    int nPartition = 0;
    int nRet = rd_kafka_produce(m_rkt,
                                nPartition,
                                RD_KAFKA_MSG_F_COPY,
                                (void* )pData,
                                nLen,
                                NULL, 0, NULL);

    if(-1 == nRet)
    {
        printf("[CKafkaClient::Send_Topic](%s)err=%s",
               rd_kafka_topic_name(m_rkt),
               rd_kafka_err2str(rd_kafka_last_error()));
        rd_kafka_poll(m_rk, 0);
        return false;
    }

    rd_kafka_poll(m_rk, 0);
    return true;
}

bool CKafkaClient::Recv_Topic_Message()
{
    int nPartition = 0;
    m_seek_offset  = 0;

    if (rd_kafka_consume_start(m_rkt, nPartition, m_start_offset) == -1)
    {
        rd_kafka_resp_err_t err = rd_kafka_last_error();
        printf("[CKafkaClient::Recv_Topic_Message]Failed to start consuming: %s\n",
               rd_kafka_err2str(err));
    }

    //开始接收数据
    while (true)
    {
        rd_kafka_message_t* rkmessage = NULL;

        rkmessage = rd_kafka_consume(m_rkt, nPartition, 1000);

        if (!rkmessage)
        {
            sleep(1);
            printf("[CKafkaClient::Recv_Topic_Message]rkmessageis NULL.\n");
            continue;
        }

        if(rkmessage->err != 0)
        {
            rd_kafka_resp_err_t err = rd_kafka_last_error();
            printf("[CKafkaClient::Recv_Topic_Message]Failed to rkmessage: (%d)%s\n",
                   err, rd_kafka_err2str(err));

            if(err == 0)
            {
                sleep(1);
                printf("[CKafkaClient::Recv_Topic_Message]rkmessageis NULL.\n");
                continue;
            }
            else
            {
                break;
            }
        }
        else if(rkmessage->err == 0)
        {
            rd_kafka_timestamp_type_t tstype;
            int64_t timestamp = 0;
            timestamp = rd_kafka_message_timestamp(rkmessage, &tstype);


            char szTimeName[50] = {'\0'};

            if (tstype != RD_KAFKA_TIMESTAMP_NOT_AVAILABLE)
            {
                if (tstype == RD_KAFKA_TIMESTAMP_CREATE_TIME)
                {
                    sprintf(szTimeName, "create time");
                }
                else if (tstype == RD_KAFKA_TIMESTAMP_LOG_APPEND_TIME)
                {
                    sprintf(szTimeName, "log append time");
                }
            }

            char szMessage[200] = {'\0'};
            memcpy(szMessage, rkmessage->payload, rkmessage->len);

            printf("[CKafkaClient::Recv_Topic_Message](partition=%d,offset=%d) szMessage=%s.\n", (int)rkmessage->partition, (int)rkmessage->offset, szMessage);

            Set_Log_File((int)m_start_offset++);
        }

        //接收完成数据销毁指针
        rd_kafka_message_destroy(rkmessage);
    }

    rd_kafka_consume_stop(m_rkt, nPartition);

    while (rd_kafka_outq_len(m_rk) > 0)
    {
        printf("[CKafkaClient::Recv_Topic_Message]len=%d", rd_kafka_outq_len(m_rk));
        rd_kafka_poll(m_rk, 10);
    }

    rd_kafka_consumer_close(m_rk);

    return true;
}

void CKafkaClient::Close()
{
    if(NULL != m_rkt)
    {
        rd_kafka_topic_destroy(m_rkt);
    }

    if(NULL != m_rk)
    {
        rd_kafka_destroy(m_rk);
    }

    //等待销毁全部完成
    int run = 5;

    while (run-- > 0 && rd_kafka_wait_destroyed(1000) == -1)
    {
        printf("[CKafkaClient::Close]Waiting for librdkafka to decommission\n");
    }
}
