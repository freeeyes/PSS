#ifndef _LCM_COMMON_H
#define _LCM_COMMON_H

#include <stdio.h>
#include <vector>

struct _Lcm_Info
{
    int m_nID;       //当前ID
    int m_nIndex;    //之前的在列表中的索引Index
    int m_nTimeout;  //时间参数

    _Lcm_Info()
    {
        m_nID      = 0;
        m_nIndex   = 0;
        m_nTimeout = 0;
    }
};

inline int gcd(int nA, int nB)
{
    if (nA < 1)
    {
        return 0;
    }

    int remainder = 0;

    do
    {
        remainder = nA % nB;
        nA = nB;
        nB = remainder;
    }
    while (nB != 0);

    return nA;
}

inline int lcm(int nA, int nB)
{
    if (nA == 0 || nB == 0 || gcd(nA, nB) == 0)
    {
        return 0;
    }
    else
    {
        return (nA * nB) / gcd(nA, nB);
    }
}

//计算最大公倍数
inline int Get_LeastCommonMultiple(std::vector<_Lcm_Info>& vec_Lcm_Info)
{
    if (vec_Lcm_Info.size() <= 1)
    {
        return vec_Lcm_Info[0].m_nTimeout;
    }

    int nData = lcm(vec_Lcm_Info[0].m_nTimeout, vec_Lcm_Info[1].m_nTimeout);

    if (0 == nData)
    {
        return 0;
    }

    for (int i = 2; i < (int)vec_Lcm_Info.size(); i++)
    {
        nData = lcm(nData, vec_Lcm_Info[i].m_nTimeout);

        if (0 == nData)
        {
            return 0;
        }
    }

    return nData;
}

inline void Get_Min_Set(std::vector<_Lcm_Info>& vec_Lcm_Info_Temp, std::vector<_Lcm_Info>& vec_Lcm_Info, int& nMinStep, int& nMinData)
{
    int nMin = vec_Lcm_Info_Temp[0].m_nTimeout;
    nMinData = 0;

    //计算最小步进
    for (int i = 1; i < (int)vec_Lcm_Info_Temp.size(); i++)
    {
        if (nMin > vec_Lcm_Info_Temp[i].m_nTimeout)
        {
            nMin = vec_Lcm_Info_Temp[i].m_nTimeout;
        }
    }

    nMinData = vec_Lcm_Info[0].m_nTimeout - nMin;

    //计算步进子集
    for (int i = 0; i < (int)vec_Lcm_Info_Temp.size(); i++)
    {
        vec_Lcm_Info_Temp[i].m_nTimeout -= nMin;

        if (nMinData > vec_Lcm_Info_Temp[i].m_nTimeout)
        {
            nMinData = vec_Lcm_Info_Temp[i].m_nTimeout;
        }
    }

    nMinStep = nMin;
}

//计算最小集
inline int Get_Minimum_Set(std::vector<_Lcm_Info>& vec_Lcm_Info, int nLeastCommonMultiple, std::vector<std::vector<_Lcm_Info> >& vecList)
{
    int  nTotal    = 0;
    int  nMinSetp  = 0;
    int  nMinData  = 0;
    std::vector<_Lcm_Info> vec_Lcm_Info_Temp;

    vec_Lcm_Info_Temp.assign(vec_Lcm_Info.begin(), vec_Lcm_Info.end());

    while (true)
    {
        if (nTotal >= nLeastCommonMultiple)
        {
            break;
        }

        Get_Min_Set(vec_Lcm_Info_Temp, vec_Lcm_Info, nMinSetp, nMinData);

        std::vector<_Lcm_Info> vecCurrData;

        for (int i = 0; i < (int)vec_Lcm_Info_Temp.size(); i++)
        {
            if (vec_Lcm_Info_Temp[i].m_nTimeout == nMinData)
            {
                _Lcm_Info obj_Lcm_Info;
                obj_Lcm_Info.m_nID      = vec_Lcm_Info_Temp[i].m_nID;
                obj_Lcm_Info.m_nIndex   = i;
                obj_Lcm_Info.m_nTimeout = nMinSetp;
                vecCurrData.push_back(obj_Lcm_Info);
                vec_Lcm_Info_Temp[i].m_nTimeout += vec_Lcm_Info[i].m_nTimeout;
            }
        }

        nTotal += nMinSetp;

        vecList.push_back(vecCurrData);
    }

    return 0;
}

#endif
