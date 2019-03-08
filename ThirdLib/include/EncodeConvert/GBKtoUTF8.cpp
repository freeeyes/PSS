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

/*���ַ�ת��Ϊ���ַ�Unicode - ANSI*/
string w2m(const wchar_t* wcs)
{
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");                               //���ĵ���
    const wchar_t* _Source = wcs;
    size_t _Dsize = strlen((const char*)wcs) + 1;                          //��Ҫת���Ŀ��ַ�������1
    char* _Dest = new char[_Dsize];
    memset(_Dest, 0, _Dsize);                               //��ʼ��������
    size_t   i;
    wcstombs_s(&i, _Dest, _Dsize, _Source, strlen((const char*)wcs));
    string result = _Dest;
    delete[]_Dest;
    setlocale(LC_ALL, curLocale.c_str());                   //���û�ԭ����locale
    return result;
}

/*���ַ�ת��Ϊ���ַ�ANSI - Unicode*/
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
    else    //ת��ʧ�ܾ�ʹ��ԭ���ı������ݣ����ܱ�û�кðɣ�Ҫ�������ط�ʹ�õĻ���δ�����Ŷ
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
    else    //ת��ʧ�ܾ�ʹ��ԭ���ı������ݣ����ܱ�û�кðɣ�Ҫ�������ط�ʹ�õĻ���δ�����Ŷ
    {
        strGBK = cname;
    }

    delete[] cname;
    delete[] cdst;

    return strGBK;
}
#endif

/*��������IsTextUTF8���޷�����gb2312����ĺ��֡�лл��*/
bool IsTextUTF8(const char* str, int length)
{
#ifndef OS_WINDOWS
    typedef unsigned long DWORD;
    typedef unsigned char UCHAR;
#endif
    int i;
    DWORD nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
    UCHAR chr;
    bool bAllAscii = true; //���ȫ������ASCII, ˵������UTF-8

    for(i = 0; i < length; i++)
    {
        chr = (UCHAR) * (str + i);

        if( (chr & 0x80) != 0 ) // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx
        {
            bAllAscii = false;
        }

        if(nBytes == 0) //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
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
        else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
        {
            if((chr & 0xC0) != 0x80 )
            {
                return false;
            }

            nBytes--;
        }
    }

    if( nBytes > 0 ) //Υ������
    {
        return false;
    }

    if(bAllAscii) //���ȫ������ASCII, ˵������UTF-8
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
* �ô������ԣ�http://blog.csdn.net/csfreebird/article/details/8571726
* ����ַ����Ķ����Ƹ�ʽ
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

/**�����ַ����Ķ����Ʊ�ʾ*/
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

/**�����ַ����Ķ����Ʊ�ʾ*/
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
 * �ô������ԣ�http://blog.csdn.net/tge7618291/article/details/7608510
 * ��һ���ַ���Unicode(UCS-2��UCS-4)����ת����UTF-8����.
 *
 * ����:
 *    unic     �ַ���Unicode����ֵ
 *    pOutput  ָ����������ڴ洢UTF8����ֵ�Ļ�������ָ��
 *    outsize  pOutput����Ĵ�С
 *
 * ����ֵ:
 *    ����ת������ַ���UTF8������ռ���ֽ���, ��������򷵻� 0 .
 *
 * ע��:
 *     1. UTF8û���ֽ�������, ����Unicode���ֽ���Ҫ��;
 *        �ֽ����Ϊ���(Big Endian)��С��(Little Endian)����;
 *        ��Intel�������в���С�˷���ʾ, �ڴ˲���С�˷���ʾ. (�͵�ַ���λ)
 *     2. �뱣֤ pOutput �������������� 6 �ֽڵĿռ��С!
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
 * �ô������ԣ�http://blog.csdn.net/coollofty/article/details/8058859
 * ����1��UTF8�ַ�����ǰλ��ָ�룬�������Ҫ��ָ�룬��Ϊ����Ҫͨ����1���ַ������жϲ�֪��һ���������ַ��ı���Ҫ���ȡ���ٸ��ַ�
 * ����2�Ƿ��ص�UCS-2�����Unicode�ַ�
 ****************************************************************************/
int one_utf8_to_unicode(const char* utf8, unsigned short& wch)
{
    //���ַ���Ascii�����0xC0����Ҫ����жϣ����򣬾Ϳ϶��ǵ���ANSI�ַ���
    unsigned char firstCh = utf8[0];

    if (firstCh >= 0xC0)
    {
        //�������ַ��ĸ�λ�ж����Ǽ�����ĸ��UTF8����
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

        //֪�����ֽ�����֮�󣬻���Ҫ�����һ�£�������ʧ�ܣ��ͼ򵥵���Ϊ��UTF8���������⣬���߲���UTF8���룬���ǵ���һ��ANSI�����ش���
        for(int k = 1; k < afters; ++ k)
        {
            if ((utf8[k] & 0xC0) != 0x80)
            {
                //�ж�ʧ�ܣ�������UTF8����Ĺ���ֱ�ӵ���һ��ANSI�ַ�����
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
 * ����1��UTF8������ַ���
 * ����2�������UCS-2��Unicode�ַ���
 * ����3�ǲ���1�ַ����ĳ���
 * ʹ�õ�ʱ����Ҫע�����2��ָ����ڴ���㹻�á���ʵ��ȫ�İ취���ж�һ��pUniBuf�Ƿ�ΪNULL�����ΪNULL��ֻͳ��������Ȳ�дpUniBuf������
 * ͨ�����κ������þͿ��Լ����ʵ������Ҫ��Unicode����������ȡ���Ȼ�����򵥵�˼·�ǣ��������ת����UTF8���ַ����������ܱ�Unicode�٣���
 * �Կ��Լ򵥵İ���sizeof(wchar_t) * utf8Leng������pUniBuf���ڴ桭��
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