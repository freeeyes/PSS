#include "GBKtoUTF8.h"
#include "GBKtoUTF8.inl"
#include <string>
#include <sstream>
#include <cstdlib>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#include <wchar.h>
#include <locale.h>
#else
#include <iconv.h>
#endif

#ifdef WIN32

wchar_t* ANSIToUnicode(const char* str)
{
    int textlen;
    wchar_t* result;
    textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
    memset(result, 0, (textlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
    return result;
}

char* UnicodeToANSI(const wchar_t* str)
{
    char* result;
    int textlen;
    textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    result = (char*)malloc((textlen + 1) * sizeof(char));
    memset(result, 0, sizeof(char) * (textlen + 1));
    WideCharToMultiByte(CP_ACP, 0, str, -1, result, textlen, NULL, NULL);
    return result;
}

wchar_t* UTF8ToUnicode(const char* str)
{
    int textlen;
    wchar_t* result;
    textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
    memset(result, 0, (textlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
    return result;
}

char* UnicodeToUTF8(const wchar_t* str)
{
    char* result;
    int textlen;
    textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    result = (char*)malloc((textlen + 1) * sizeof(char));
    memset(result, 0, sizeof(char) * (textlen + 1));
    WideCharToMultiByte(CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
    return result;
}

/*宽字符转换为多字符Unicode - ANSI*/
string w2m(const wchar_t* wcs)
{
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");                               //中文地区
    const wchar_t* _Source = wcs;
    size_t _Dsize = strlen((const char*)wcs) + 1;                          //比要转换的宽字符个数大1
    char* _Dest = new char[_Dsize];
    memset(_Dest, 0, _Dsize);                               //初始化缓冲区
    size_t   i;
    wcstombs_s(&i, _Dest, _Dsize, _Source, strlen((const char*)wcs));
    string result = _Dest;
    delete[]_Dest;
    setlocale(LC_ALL, curLocale.c_str());                   //设置回原来的locale
    return result;
}

/*多字符转换为宽字符ANSI - Unicode*/
wstring m2w(const char* mbs)
{
    std::string curlLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    const char* _Source = mbs;
    size_t _Dsize = strlen((const char*)mbs) + 1;
    wchar_t* _Dest = new wchar_t[_Dsize];
    size_t i;
    mbstowcs_s(&i, _Dest, _Dsize, _Source, strlen((const char*)mbs));
    wstring result = _Dest;
    delete[] _Dest;
    setlocale(LC_ALL, curlLocale.c_str());
    return result;
}

std::string ConvertGBKToUtf8(const std::string& strGBK)
{
    wchar_t* buf = ANSIToUnicode(strGBK.c_str());
    char* ret = UnicodeToUTF8(buf);
    std::string  strResult = ret;
    free(buf);
    free(ret);
    return strResult;
}

std::string ConvertUtf8ToGBK(const std::string& strUtf8)
{
    wchar_t* buf = UTF8ToUnicode(strUtf8.c_str());
    char* ret = UnicodeToANSI(buf);
    std::string  strResult = ret;
    free(buf);
    free(ret);
    return strResult;
}

#else
bool code_convert(const char* from_charset, const char* to_charset, char* inbuf, size_t  inlen, char* outbuf, size_t outlen )
{
    iconv_t cd ;
    char** pin = &inbuf ;
    char** pout = &outbuf ;

    cd = iconv_open( to_charset, from_charset );

    if( cd == 0 )
    {
        return false;
    }

    memset( outbuf, 0, outlen );
    int convert = iconv( cd, pin, &inlen, pout, &outlen );

    if( convert == -1 )
    {
        iconv_close( cd );
        return false ;
    }

    iconv_close(cd);

    return true ;
}

std::string ConvertGBKToUtf8(const std::string& strGBK)
{
    const size_t length = strGBK.length();

    char* cname = new char[length + 1];
    memset(cname, '\0', length + 1);
    memcpy(cname, strGBK.c_str(), length);

    char* cdst = new char[length * 3];
    memset(cdst, '\0', length * 3);

    bool code = code_convert( "gbk", "utf-8", cname, length, cdst, length * 3);
    std::string strUtf8;

    if (code)
    {
        strUtf8 = cdst;
    }
    else    //转换失败就使用原来的编码内容，有总比没有好吧，要是其他地方使用的话可未必如此哦
    {
        strUtf8 = cname;
    }

    delete[] cname;
    delete[] cdst;

    return strUtf8;
}

std::string ConvertUtf8ToGBK(const std::string& strUtf8)
{
    const size_t length = strUtf8.length();

    char* cname = new char[length + 1];
    memset(cname, '\0', length + 1);
    memcpy(cname, strUtf8.c_str(), length);

    char* cdst = new char[length + 1];
    memset(cdst, '\0', length + 1);

    bool code = code_convert( "utf-8", "gbk", cname, length, cdst, length);
    std::string strGBK;

    if (code)
    {
        strGBK = cdst;
    }
    else    //转换失败就使用原来的编码内容，有总比没有好吧，要是其他地方使用的话可未必如此哦
    {
        strGBK = cname;
    }

    delete[] cname;
    delete[] cdst;

    return strGBK;
}
#endif

/*下面两个IsTextUTF8都无法检测出gb2312编码的汉字“谢谢”*/
bool IsTextUTF8(const char* str, int length)
{
#ifndef OS_WINDOWS
    typedef unsigned long DWORD;
    typedef unsigned char UCHAR;
#endif
    int i;
    DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    UCHAR chr;
    bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8

    for(i = 0; i < length; i++)
    {
        chr = (UCHAR) * (str + i);

        if( (chr & 0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
        {
            bAllAscii = false;
        }

        if(nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
        {
            if(chr >= 0x80)
            {
                if(chr >= 0xFC && chr <= 0xFD)
                {
                    nBytes = 6;
                }
                else if(chr >= 0xF8)
                {
                    nBytes = 5;
                }
                else if(chr >= 0xF0)
                {
                    nBytes = 4;
                }
                else if(chr >= 0xE0)
                {
                    nBytes = 3;
                }
                else if(chr >= 0xC0)
                {
                    nBytes = 2;
                }
                else
                {
                    return false;
                }

                nBytes--;
            }
        }
        else //多字节符的非首字节,应为 10xxxxxx
        {
            if((chr & 0xC0) != 0x80 )
            {
                return false;
            }

            nBytes--;
        }
    }

    if( nBytes > 0 ) //违返规则
    {
        return false;
    }

    if(bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
    {
        return false;
    }

    return true;
}

int is_utf8_special_byte(unsigned char c)
{
    unsigned special_byte = 0X02; //binary 00000010

    if ((unsigned)(c >> 6) == special_byte)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool IsTextUTF8(const std::string& str)
{
    unsigned one_byte   = 0X00;    //binary 00000000
    unsigned two_byte   = 0X06;    //binary 00000110
    unsigned three_byte = 0X0E;    //binary 00001110
    unsigned four_byte  = 0X1E;    //binary 00011110
    unsigned five_byte  = 0X3E;    //binary 00111110
    unsigned six_byte   = 0X7E;    //binary 01111110

    unsigned char k = 0;
    unsigned char m = 0;
    unsigned char n = 0;
    unsigned char p = 0;
    unsigned char q = 0;

    unsigned char c = 0;
    bool    isUtf8 = false;

    for (size_t i = 0; i < str.size();)
    {
        c = (unsigned char)str[i];

        if ((unsigned)(c >> 7) == one_byte)
        {
            i++;
            continue;
        }
        else if ((unsigned)(c >> 5) == two_byte)
        {
            k = (unsigned char)str[i + 1];

            if ( is_utf8_special_byte(k) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 2;
                continue;
            }
        }
        else if ((unsigned)(c >> 4) == three_byte)
        {
            m = (unsigned char)str[i + 1];
            n = (unsigned char)str[i + 2];

            if ( is_utf8_special_byte(m) && is_utf8_special_byte(n) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 3;
                continue;
            }
        }
        else if ((unsigned)(c >> 3) == four_byte)
        {
            k = (unsigned char)str[i + 1];
            m = (unsigned char)str[i + 2];
            n = (unsigned char)str[i + 3];

            if ( is_utf8_special_byte(k)
                 && is_utf8_special_byte(m)
                 && is_utf8_special_byte(n) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 4;
                continue;
            }
        }
        else if ((unsigned)(c >> 2) == five_byte)
        {
            unsigned char k = (unsigned char)str[i + 1];
            unsigned char m = (unsigned char)str[i + 2];
            unsigned char n = (unsigned char)str[i + 3];
            unsigned char p = (unsigned char)str[i + 4];

            if ( is_utf8_special_byte(k)
                 && is_utf8_special_byte(m)
                 && is_utf8_special_byte(n)
                 && is_utf8_special_byte(p) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 5;
                continue;
            }
        }
        else if ((unsigned)(c >> 1) == six_byte)
        {
            k = (unsigned char)str[i + 1];
            m = (unsigned char)str[i + 2];
            n = (unsigned char)str[i + 3];
            p = (unsigned char)str[i + 4];
            q = (unsigned char)str[i + 5];

            if ( is_utf8_special_byte(k)
                 && is_utf8_special_byte(m)
                 && is_utf8_special_byte(n)
                 && is_utf8_special_byte(p)
                 && is_utf8_special_byte(q) )
            {
                isUtf8 = true; //utf8_yes++;
                i += 6;
                continue;
            }
        }

        if(!isUtf8)
        {
            return false;
        }
        else
        {
            i++;
        }
    }

    return true;
}

/*****************************************************************************
* 该代码来自：http://blog.csdn.net/csfreebird/article/details/8571726
* 输出字符串的二进制格式
****************************************************************************/
// Get the bit value specified by the index,index starts with 0
template<class T>
int Bit_Value(T value, int index)
{
    return (value & (1 << index)) == 0 ? 0 : 1;
}

// T must be one of integer type
template<class T>
std::string PrintIntAsBinaryString(T v)
{
    std::stringstream stream;
    int i = sizeof(T) * 8 - 1;

    while (i >= 0)
    {
        stream << Bit_Value(v, i);
        --i;
    }

    return stream.str();
}

/**计算字符串的二进制表示*/
std::string PrintStringAsBinaryString(const char* p)
{
    std::stringstream stream;

    for (size_t i = 0; i < strlen(p); ++i)
    {
        stream << PrintIntAsBinaryString(p[i]);
        stream << " ";
    }

    return stream.str();
}

/**计算字符串的二进制表示*/
std::string PrintStringAsBinaryString(const std::string& str)
{
    std::stringstream stream;

    for (size_t i = 0; i < str.size(); ++i)
    {
        stream << PrintIntAsBinaryString(str[i]);
        stream << " ";
    }

    return stream.str();
}

extern const unsigned short gbk_to_unicode_table[];
extern const unicode_to_gbk_t unicode_to_gbk_table[];
#define U2W_LOBYTE(w)           ((unsigned char)(((unsigned short)(w)) & 0xff))
#define U2W_HIBYTE(w)           ((unsigned char)((((unsigned short)(w)) >> 8) & 0xff))

unsigned short one_gbk_to_unicode(unsigned char ch, unsigned char cl)
{
    ch -= 0x81;
    cl -= 0x40;
    return (ch <= 0x7d && cl <= 0xbe) ? gbk_to_unicode_table[ch * 0xbf + cl] : 0x1fff;
}

unsigned short one_unicode_to_gbk(unsigned short unicode)
{
    const static int TABLE_SIZE = (sizeof(unicode_to_gbk_table)/sizeof(unicode_to_gbk_table[0]));
    int i, b, e;
    b = 0;
    e = TABLE_SIZE - 1;

    while (b <= e)
    {
        i = (b + e) / 2;

        if (unicode_to_gbk_table[i].unicode == unicode)
        {
            return unicode_to_gbk_table[i].gbk;
        }

        if (unicode_to_gbk_table[i].unicode < unicode)
        {
            b = i + 1;
        }
        else
        {
            e = i - 1;
        }
    }

    return 0;
}

void unicode_to_gbk(unsigned short* punicode, char* pgbk, int len)
{
    while (*punicode && len > 0)
    {
        unsigned short dbcs;
        dbcs = one_unicode_to_gbk(*punicode);

        if (dbcs > 0x0ff || dbcs < 0)
        {
            *pgbk = dbcs >> 8;
            ++pgbk;
        }

        *pgbk = U2W_LOBYTE(dbcs);
        ++pgbk;
        --len;
        ++punicode;
    }

    *pgbk = 0x00;
}

/*****************************************************************************
 * 该代码来自：http://blog.csdn.net/tge7618291/article/details/7608510
 * 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.
 *
 * 参数:
 *    unic     字符的Unicode编码值
 *    pOutput  指向输出的用于存储UTF8编码值的缓冲区的指针
 *    outsize  pOutput缓冲的大小
 *
 * 返回值:
 *    返回转换后的字符的UTF8编码所占的字节数, 如果出错则返回 0 .
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 *     2. 请保证 pOutput 缓冲区有最少有 6 字节的空间大小!
 ****************************************************************************/
int one_unicode_to_utf8(unsigned long unic, unsigned char* pOutput, int outSize)
{
    assert(pOutput != NULL);
    assert(outSize >= 6);

    if ( unic <= 0x0000007F )
    {
        // * U-00000000 - U-0000007F:  0xxxxxxx
        *pOutput     = (unic & 0x7F);
        return 1;
    }
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )
    {
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        *(pOutput + 1) = (unic & 0x3F) | 0x80;
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *(pOutput + 2) = (unic & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >>  6) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 3) = (unic & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 12) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 4) = (unic & 0x3F) | 0x80;
        *(pOutput + 3) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 18) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 5) = (unic & 0x3F) | 0x80;
        *(pOutput + 4) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput + 3) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput + 2) = ((unic >> 18) & 0x3F) | 0x80;
        *(pOutput + 1) = ((unic >> 24) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;
        return 6;
    }

    return 0;
}

/*****************************************************************************
 * 该代码来自：http://blog.csdn.net/coollofty/article/details/8058859
 * 参数1是UTF8字符串当前位置指针，这里必须要是指针，因为必须要通过第1个字符进行判断才知道一个完整的字符的编码要向后取多少个字符
 * 参数2是返回的UCS-2编码的Unicode字符
 ****************************************************************************/
int one_utf8_to_unicode(const char* utf8, unsigned short& wch)
{
    //首字符的Ascii码大于0xC0才需要向后判断，否则，就肯定是单个ANSI字符了
    unsigned char firstCh = utf8[0];

    if (firstCh >= 0xC0)
    {
        //根据首字符的高位判断这是几个字母的UTF8编码
        int afters, code;

        if ((firstCh & 0xE0) == 0xC0)
        {
            afters = 2;
            code = firstCh & 0x1F;
        }
        else if ((firstCh & 0xF0) == 0xE0)
        {
            afters = 3;
            code = firstCh & 0xF;
        }
        else if ((firstCh & 0xF8) == 0xF0)
        {
            afters = 4;
            code = firstCh & 0x7;
        }
        else if ((firstCh & 0xFC) == 0xF8)
        {
            afters = 5;
            code = firstCh & 0x3;
        }
        else if ((firstCh & 0xFE) == 0xFC)
        {
            afters = 6;
            code = firstCh & 0x1;
        }
        else
        {
            wch = firstCh;
            return 1;
        }

        //知道了字节数量之后，还需要向后检查一下，如果检查失败，就简单的认为此UTF8编码有问题，或者不是UTF8编码，于是当成一个ANSI来返回处理
        for(int k = 1; k < afters; ++ k)
        {
            if ((utf8[k] & 0xC0) != 0x80)
            {
                //判断失败，不符合UTF8编码的规则，直接当成一个ANSI字符返回
                wch = firstCh;
                return 1;
            }

            code <<= 6;
            code |= (unsigned char)utf8[k] & 0x3F;
        }

        wch = code;
        return afters;
    }
    else
    {
        wch = firstCh;
    }

    return 1;
}

/*****************************************************************************
 * 参数1是UTF8编码的字符串
 * 参数2是输出的UCS-2的Unicode字符串
 * 参数3是参数1字符串的长度
 * 使用的时候需要注意参数2所指向的内存块足够用。其实安全的办法是判断一下pUniBuf是否为NULL，如果为NULL则只统计输出长度不写pUniBuf，这样
 * 通过两次函数调用就可以计算出实际所需要的Unicode缓存输出长度。当然，更简单的思路是：无论如何转换，UTF8的字符数量不可能比Unicode少，所
 * 以可以简单的按照sizeof(wchar_t) * utf8Leng来分配pUniBuf的内存……
 ****************************************************************************/
int utf8_to_unicode(const char* utf8Buf, unsigned short* pUniBuf, int utf8Leng)
{
    int i = 0, count = 0;

    while(i < utf8Leng)
    {
        i += one_utf8_to_unicode(utf8Buf + i, pUniBuf[count]);
        count ++;
    }

    return count;
}

int unicode_to_utf8(unsigned short wchar, char* utf8)
{
    int len = 0;

    if (wchar < 0xC0)
    {
        utf8[len ++] = (char)wchar;
    }
    else if (wchar < 0x800)
    {
        utf8[len ++] = 0xc0 | (wchar >> 6);
        utf8[len ++] = 0x80 | (wchar & 0x3f);
    }
    else if (wchar < 0x10000)
    {
        utf8[len ++] = 0xe0 | (wchar >> 12);
        utf8[len ++] = 0x80 | ((wchar >> 6) & 0x3f);
        utf8[len ++] = 0x80 | (wchar & 0x3f);
    }
    else if (wchar < 0x200000)
    {
        utf8[len ++] = 0xf0 | ((int)wchar >> 18);
        utf8[len ++] = 0x80 | ((wchar >> 12) & 0x3f);
        utf8[len ++] = 0x80 | ((wchar >> 6) & 0x3f);
        utf8[len ++] = 0x80 | (wchar & 0x3f);
    }
    else if (wchar < 0x4000000)
    {
        utf8[len ++] = 0xf8 | ((int)wchar >> 24);
        utf8[len ++] = 0x80 | (((int)wchar >> 18) & 0x3f);
        utf8[len ++] = 0x80 | (((int)wchar >> 12) & 0x3f);
        utf8[len ++] = 0x80 | (((int)wchar >> 6) & 0x3f);
        utf8[len ++] = 0x80 | ((int)wchar & 0x3f);
    }
    else if (wchar < 0x80000000)
    {
        utf8[len ++] = 0xfc | ((int)wchar >> 30);
        utf8[len ++] = 0x80 | (((int)wchar >> 24) & 0x3f);
        utf8[len ++] = 0x80 | (((int)wchar >> 18) & 0x3f);
        utf8[len ++] = 0x80 | (((int)wchar >> 12) & 0x3f);
        utf8[len ++] = 0x80 | (((int)wchar >> 6) & 0x3f);
        utf8[len ++] = 0x80 | ((int)wchar & 0x3f);
    }

    return len;
}