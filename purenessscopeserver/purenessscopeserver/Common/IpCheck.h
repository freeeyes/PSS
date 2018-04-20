#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <string>

using namespace std;

enum EM_IP_TYPE
{
    IP_V4 = 0,
    IP_V6,
    IP_UNKNOW,
};

static void str_split(const string& src, const string& separator, vector<string>& dest)
{
    string str = src;
    string substring;
    string::size_type start = 0, index;

    do
    {
        index = str.find(separator, start);

        if (index != string::npos)
        {
            substring = str.substr(start, index - start);
            dest.push_back(substring);
            start = str.find_first_not_of(separator, index);

            if (start == string::npos)
            {
                return;
            }
        }
    }
    while (index != string::npos);

    //the last token
    substring = str.substr(start);
    dest.push_back(substring);
}

static EM_IP_TYPE Check_IP_V6(vector<string> vecIpSection)
{
    for (int i = 0; i < (int)vecIpSection.size(); i++)
    {
        if (vecIpSection[i].c_str() == (string)"")
        {
            continue;
        }

        char* pStop = NULL;
        long int nSection = (long int)strtol(vecIpSection[i].c_str(), &pStop, 16);

        int nDataLength = (int)(pStop - vecIpSection[i].c_str());

        if (nDataLength != (int)vecIpSection[i].length())
        {
            printf("[Check_IP_V6]error Section[%s].\n", vecIpSection[i].c_str());
            return IP_UNKNOW;
        }

        if (nSection < 0x0000 || nSection > 0xffff)
        {
            printf("[Check_IP_V6]error Section IP_UNKNOW[%s].\n", vecIpSection[i].c_str());
            return IP_UNKNOW;
        }
    }

    return IP_V6;
}

static EM_IP_TYPE Check_IP_V4(vector<string> vecIpSection)
{
    if (vecIpSection.size() != 4)
    {
        return IP_UNKNOW;
    }


    for (int i = 0; i < (int)vecIpSection.size(); i++)
    {
        char* pStop = NULL;
        long int nSection = (long int)strtol(vecIpSection[i].c_str(), &pStop, 10);

        int nDataLength = (int)(pStop - vecIpSection[i].c_str());

        if (nDataLength != (int)vecIpSection[i].length())
        {
            printf("[Check_IP_V4]error Section[%s].\n", vecIpSection[i].c_str());
            return IP_UNKNOW;
        }

        if (nSection < 0 || nSection > 255)
        {
            printf("[Check_IP_V4]error Section IP_UNKNOW[%s].\n", vecIpSection[i].c_str());
            return IP_UNKNOW;
        }
    }

    return IP_V4;
}

inline EM_IP_TYPE Check_IP(string strIP)
{
    if (strIP == "INADDR_ANY")
    {
        return IP_V4;
    }

    vector<string> vecIpSection;

    str_split(strIP, ".", vecIpSection);

    if (vecIpSection.size() > 1)
    {
        return Check_IP_V4(vecIpSection);
    }

    vecIpSection.clear();
    str_split(strIP, "::", vecIpSection);

    if (vecIpSection.size() > 2)
    {
        return IP_UNKNOW;
    }

    if (vecIpSection.size() == 1)
    {
        vecIpSection.clear();
        str_split(strIP, ":", vecIpSection);

        if (vecIpSection.size() == 8)
        {
            return Check_IP_V6(vecIpSection);
        }
    }
    else
    {
        //printf("[Check_IP]IPv6 vecIpSection(%d).\n", vecIpSection.size());
        for (int i = 0; i < (int)vecIpSection.size(); i++)
        {
            //printf("[Check_IP]IPv6(%s).\n", vecIpSection[i].c_str());
            if (vecIpSection[i].length() == 0)
            {
                continue;
            }
            else
            {
                vector<string> vecIpSubSection;
                str_split(vecIpSection[i], ":", vecIpSubSection);

                if (IP_UNKNOW == Check_IP_V6(vecIpSubSection))
                {
                    return IP_UNKNOW;
                }
            }
        }

        return IP_V6;
    }

    return IP_UNKNOW;
}

inline string Get_Type_Name(EM_IP_TYPE emType)
{
    if (IP_V4 == emType)
    {
        return (string)"IPV4";
    }
    else if (IP_V6 == emType)
    {
        return (string)"IPV6";
    }
    else
    {
        return (string)"IP Unknow";
    }
}
