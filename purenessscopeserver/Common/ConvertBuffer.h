#ifndef _CONVERTBUFFRE_H
#define _CONVERTBUFFRE_H

#include <iostream>

//二进制换砖类，负责二进制的代码转换
class CConvertBuffer
{
public:
    CConvertBuffer() = default;

    int GetBufferSize(const char* pData, int nSrcLen) const
    {
        char szData[3] = { '\0' };
        int nPos = 0;
        int nCurrSize = 0;
        int nConvertSize = 0;
        bool blState = false;   //转换后的字符串是否有效
        bool blSrcState = true;    //元字符串是否有效
        unsigned char cData;

        while (nPos < nSrcLen)
        {
            if (pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
            {
                if (nPos == nSrcLen - 1 && nCurrSize < 2)
                {
                    szData[nCurrSize++] = pData[nPos];
                }

                if (nCurrSize < 3)
                {
                    szData[nCurrSize] = '\0';
                }
                else
                {
                    blSrcState = false;
                }

                if (blSrcState == true)
                {
                    blState = ConvertStr2char(szData, cData);
                }

                if (blState == true)
                {
                    nConvertSize++;
                }

                nCurrSize = 0;
                blSrcState = true;
                nPos++;
            }
            else
            {
                if (nCurrSize < 2)
                {
                    szData[nCurrSize++] = pData[nPos];
                }
                else
                {
                    blSrcState = false;
                }

                nPos++;
            }
        }

        return nConvertSize;
    }

    bool Convertstr2charArray(const char* pData, int nSrcLen, unsigned char* pDes, int& nMaxLen) const
    {
        char szData[3] = { '\0' };
        int nPos = 0;
        int nCurrSize = 0;
        int nConvertSize = 0;
        bool blState = false;   //转换后的字符串是否有效
        bool blSrcState = true;    //元字符串是否有效

        while (nPos < nSrcLen)
        {
            if (pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
            {
                if (nPos == nSrcLen - 1 && nCurrSize < 2)
                {
                    szData[nCurrSize++] = pData[nPos];
                }

                if (nCurrSize < 3)
                {
                    szData[nCurrSize] = '\0';
                }
                else
                {
                    blSrcState = false;
                }

                if (nConvertSize < nMaxLen && blSrcState == true)
                {
                    blState = ConvertStr2char(szData, pDes[nConvertSize]);
                }

                if (blState == true)
                {
                    nConvertSize++;
                }

                nCurrSize = 0;
                blSrcState = true;
                nPos++;
            }
            else
            {
                if (nCurrSize < 2)
                {
                    szData[nCurrSize++] = pData[nPos];
                }
                else
                {
                    blSrcState = false;
                }

                nPos++;
            }
        }

        nMaxLen = nConvertSize;
        return true;
    }
private:
    bool Get_binary_Char(unsigned char cTag, unsigned char& cDes) const
    {
        if (cTag >= 'A' && cTag <= 'F')
        {
            cDes = cTag - 'A' + 10;
            return true;
        }
        else if (cTag >= 'a' && cTag <= 'f')
        {
            cDes = cTag - 'a' + 10;
            return true;
        }
        else if (cTag >= '0' && cTag <= '9')
        {
            cDes = cTag - '0';
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ConvertStr2char(const char* pData, unsigned char& cData) const
    {
        if (pData == nullptr || strlen(pData) != 2)
        {
            return false;
        }

        char cFirst = pData[1];
        unsigned char cTemp = 0;
        bool blStste = Get_binary_Char(cFirst, cTemp);

        if (false == blStste)
        {
            return false;
        }

        cData = cTemp;
        char cSecond = pData[0];
        blStste = Get_binary_Char(cSecond, cTemp);

        if (false == blStste)
        {
            return false;
        }

        cTemp = (unsigned char)(cTemp << 4);
        cData = cData | cTemp;

        return true;
    }
};

#endif
