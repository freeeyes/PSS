#ifndef _KAFKACLINT_H
#define _KAFKACLINT_H

#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <sys/time.h>
#include "rdkafka.h"

//add by freeeyes
//≤‚ ‘kafka∂”¡–

enum ENUM_KAFKA_QUEUE_CLASS
{
    KA_PRODUCER = 0,
    KA_CONSUMER,
};

class CKafkaClient
{
public:
    CKafkaClient();
    ~CKafkaClient();

    void Set_Start_Offset(int nBegin);

    void Set_Log_Path(const char* pPath);

    bool Connect_Kafka_Server(const char* pAddress, ENUM_KAFKA_QUEUE_CLASS em_Class);

    bool Create_topic(const char* pTopic);

    bool Send_Topic_Message(const char* pData, int nLen);

    bool Recv_Topic_Message();

    void Close();

private:
    void Set_Log_File(int nIndex);

private:
    rd_kafka_conf_t*       m_kafka_conf;
    rd_kafka_topic_conf_t* m_topic_conf;
    rd_kafka_topic_t*      m_rkt;
    rd_kafka_t*            m_rk;
    int64_t                m_start_offset;
    int64_t                m_seek_offset;
    char                   m_szError[500];
    char                   m_szPath[256];
};

#endif
