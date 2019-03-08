#ifndef _TIME_VALUE_H
#define _TIME_VALUE_H

#include "stdio.h"
#include "time.h"
#include <string>

#define PLATFORM_WIN     0
#define PLATFORM_UNIX    1
#define PLATFORM_APPLE   2

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32) || defined(__WIN64__) || defined(WIN64) || defined(_WIN64)
#  define PSS_PLATFORM PLATFORM_WIN
#elif defined(__APPLE_CC__)
#  define PSS_PLATFORM PLATFORM_APPLE
#else
#  define PSS_PLATFORM PLATFORM_UNIX
#endif

//Time Value��������
//add by freeeyes

namespace ts_timer
{
    typedef long suseconds_t;                         //����ֵ
    suseconds_t const ONE_SECOND_IN_USECS = 1000000;  //һ�����ʱ��

    class CTime_Value
    {
    public:
        CTime_Value() : m_tv_sec(0), m_tv_usec(0) {};
        CTime_Value(time_t tv_sec, suseconds_t tv_usec);
        ~CTime_Value() {};

        //����ʱ��
        void Set_time(time_t tv_sec, suseconds_t tv_usec);

        time_t Get_sec() const;
        suseconds_t Get_usec() const;

        void normalize();            //��ʽ��ʱ���ַ���

        std::string Get_string();    //�õ�ʱ���ʽ���ַ���

        long Get_milliseconds();      //�õ���ǰ����ֵ

        bool IsZero();                //��ǰʱ���Ƿ��ǳ�ʼ��ʱ��

        //����+/-
        CTime_Value operator+(const CTime_Value& ar)
        {
            CTime_Value rs;
            time_t      tv_sec  = this->Get_sec() + ar.Get_sec();
            suseconds_t tv_usec = this->Get_usec() + ar.Get_usec();
            rs.Set_time(tv_sec, tv_usec);
            rs.normalize();
            return rs;
        }

        CTime_Value operator-(const CTime_Value& ar)
        {
            CTime_Value rs;
            time_t      tv_sec = this->Get_sec() - ar.Get_sec();
            suseconds_t tv_usec = this->Get_usec() - ar.Get_usec();
            rs.Set_time(tv_sec, tv_usec);
            rs.normalize();
            return rs;
        }

    private:
        time_t      m_tv_sec;     //��ֵ
        suseconds_t m_tv_usec;    //����ֵ
    };
}

#endif
