#ifndef _PSS_TIMERSTAMP_H
#define _PSS_TIMERSTAMP_H

//��úͼ���ʱ����������
//add by freeeyes

#include "define.h"

using PSS_Time_Point = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;

class CTimeStamp
{
public:
    CTimeStamp() = default;

    //�õ���ǰʱ���
    static PSS_Time_Point Get_Time_Stamp()
    {
        return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    };   

    //����ʱ����õ���ǰ������ʱ��
    static string Get_DateTime(const PSS_Time_Point& time_point)
    {
        auto tt = std::chrono::system_clock::to_time_t(time_point);
#if PSS_PLATFORM == PLATFORM_WIN
        struct tm tmNow_s;
        localtime_s(&tmNow_s, &tt);
        struct tm* now = &tmNow_s;
#else
        std::tm* now = std::localtime(&tt);
#endif

        string strDate = fmt::format("{0:04d}-{1:02d}-{2:02d} {3:02d}:{4:02d}:{5:02d}",
            now->tm_year + 1900,
            now->tm_mon + 1,
            now->tm_mday,
            now->tm_hour,
            now->tm_min,
            now->tm_sec);

        return strDate;
    };

    //����ʱ����õ���ǰ����
    static uint16 Get_Date_Year(const PSS_Time_Point& time_point)
    {
        auto tt = std::chrono::system_clock::to_time_t(time_point);
#if PSS_PLATFORM == PLATFORM_WIN
        struct tm tmNow_s;
        localtime_s(&tmNow_s, &tt);
        struct tm* now = &tmNow_s;
#else
        std::tm* now = std::localtime(&tt);
#endif

        return now->tm_year + 1900;
    };

    //����ʱ����õ���ǰ����
    static uint16 Get_Date_Month(const PSS_Time_Point& time_point)
    {
        auto tt = std::chrono::system_clock::to_time_t(time_point);
#if PSS_PLATFORM == PLATFORM_WIN
        struct tm tmNow_s;
        localtime_s(&tmNow_s, &tt);
        struct tm* now = &tmNow_s;
#else
        std::tm* now = std::localtime(&tt);
#endif

        return now->tm_mon + 1;
    };

    //����ʱ����õ���ǰ����
    static uint16 Get_Date_Day(const PSS_Time_Point& time_point)
    {
        auto tt = std::chrono::system_clock::to_time_t(time_point);
#if PSS_PLATFORM == PLATFORM_WIN
        struct tm tmNow_s;
        localtime_s(&tmNow_s, &tt);
        struct tm* now = &tmNow_s;
#else
        std::tm* now = std::localtime(&tt);
#endif
        return now->tm_mday;
    };

    //�õ�����ʱ����λ�Ǻ���
    static int64 Get_Time_Difference(const PSS_Time_Point& time_end, const PSS_Time_Point& time_begin)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count();
    }; 

    //���Ӻ�����
    static PSS_Time_Point Add_Time_Milliseconds(const PSS_Time_Point& time_now, const uint32& milliseconds)
    {
        auto tt_new_time = std::chrono::system_clock::time_point(time_now + std::chrono::milliseconds(milliseconds));
        return std::chrono::time_point_cast<std::chrono::milliseconds>(tt_new_time);
    };

    //�õ���ǰʱ��ķ�����
    static uint8 Get_Time_of_Minute(const PSS_Time_Point& time_point)
    {
        auto tt = std::chrono::system_clock::to_time_t(time_point);
#if PSS_PLATFORM == PLATFORM_WIN
        struct tm tmNow_s;
        localtime_s(&tmNow_s, &tt);
        struct tm* now = &tmNow_s;
#else
        std::tm* now = std::localtime(&tt);
#endif

        return now->tm_min;
    };

    //�õ���ǰ����ֵ
    static uint64 Get_Time_use_second(const PSS_Time_Point& time_point)
    {
        auto ms = time_point.time_since_epoch();  // �����Ԫʱ��
        return std::chrono::duration_cast<std::chrono::seconds>(ms).count();
    };
};

#endif
