
//MD5.cpp
#include "MD5.h"

#include <exception>
#include <strstream>

using namespace std;

//CONSTRUCTOR
CMD5::CMD5()
{
	m_auiBuf[0] = 0x67452301;
	m_auiBuf[1] = 0xefcdab89;
	m_auiBuf[2] = 0x98badcfe;
	m_auiBuf[3] = 0x10325476;
	m_auiBits[0] = 0;
	m_auiBits[1] = 0;
}

//Update context to reflect the concatenation of another buffer of bytes.
void CMD5::AddData(char const* pcData, int iDataLength)
{
	if(iDataLength < 0)
		throw runtime_error(string("FileDigest ERROR: in CMD5::AddData(), Data Length should be >= 0!"));
	unsigned int uiT;
	//Update bitcount
	uiT = m_auiBits[0];
	if((m_auiBits[0] = uiT + ((unsigned int)iDataLength << 3)) < uiT)
		m_auiBits[1]++; //Carry from low to high
	m_auiBits[1] += iDataLength >> 29;
	uiT = (uiT >> 3) & (BLOCKSIZE-1); //Bytes already
	//Handle any leading odd-sized chunks
	if(uiT != 0)
	{
		unsigned char *puc = (unsigned char *)m_aucIn + uiT;
		uiT = BLOCKSIZE - uiT;
		if((unsigned int)iDataLength < uiT)
		{
			memcpy(puc, pcData, iDataLength);
			return;
		}
		memcpy(puc, pcData, uiT);
		Transform();
		pcData += uiT;
		iDataLength -= uiT;
	}
	//Process data in 64-byte chunks
	while(iDataLength >= BLOCKSIZE)
	{
		memcpy(m_aucIn, pcData, BLOCKSIZE);
		Transform();
		pcData += BLOCKSIZE;
		iDataLength -= BLOCKSIZE;
	}
	//Handle any remaining bytes of data
	memcpy(m_aucIn, pcData, iDataLength);
	//Set the flag
	m_bAddData = true;
}

//Final wrapup - pad to 64-byte boundary with the bit pattern 
//1 0*(64-bit count of bits processed, MSB-first)
void CMD5::FinalDigest(char* pcDigest)
{
	//Is the User's responsability to ensure that pcDigest has
	//at least 16 bytes allocated
	if(false == m_bAddData)
		throw runtime_error(string("FileDigest ERROR: in CMD5::FinalDigest(), No data Added before call!"));
	unsigned int uiCount;
	unsigned char* puc;
	//Compute number of bytes mod 64
	uiCount = (m_auiBits[0] >> 3) & (BLOCKSIZE-1);
	//Set the first char of padding to 0x80. This is safe since there is
	//always at least one byte free
	puc = m_aucIn + uiCount;
	*puc++ = 0x80;
	//Bytes of padding needed to make 64 bytes
	uiCount = BLOCKSIZE - uiCount - 1;
	//Pad out to 56 mod 64
	if(uiCount < 8)
	{
		//Two lots of padding:  Pad the first block to 64 bytes
		memset(puc, 0, uiCount);
		Transform();
		//Now fill the next block with 56 bytes
		memset(m_aucIn, 0, BLOCKSIZE-8);
	}
	else
	{
		//Pad block to 56 bytes
		memset(puc, 0, uiCount - 8);
	}
	//Append length in bits and transform
	((unsigned int*)m_aucIn)[(BLOCKSIZE>>2)-2] = m_auiBits[0];
	((unsigned int*)m_aucIn)[(BLOCKSIZE>>2)-1] = m_auiBits[1];
	Transform();
	memcpy(pcDigest, m_auiBuf, MD128LENGTH<<2);
	//Reinitialize
	Reset();
}

//Reset current operation in order to prepare a new one
void CMD5::Reset()
{
	//Reinitialize
	m_auiBuf[0] = 0x67452301;
	m_auiBuf[1] = 0xefcdab89;
	m_auiBuf[2] = 0x98badcfe;
	m_auiBuf[3] = 0x10325476;
	m_auiBits[0] = 0;
	m_auiBits[1] = 0;
	//Reset the flag
	m_bAddData = false;
}

//The core of the MD5 algorithm, this alters an existing MD5 hash to
//reflect the addition of 16 longwords of new data. MD5Update blocks
//the data and converts bytes into longwords for this routine.
void CMD5::Transform()
{
	unsigned int* puiIn = (unsigned int*)m_aucIn;
	register unsigned int a, b, c, d;
	a = m_auiBuf[0];
	b = m_auiBuf[1];
	c = m_auiBuf[2];
	d = m_auiBuf[3];
	//
	MD5STEP(F1, a, b, c, d, puiIn[0] + 0xd76aa478, 7);
	MD5STEP(F1, d, a, b, c, puiIn[1] + 0xe8c7b756, 12);
	MD5STEP(F1, c, d, a, b, puiIn[2] + 0x242070db, 17);
	MD5STEP(F1, b, c, d, a, puiIn[3] + 0xc1bdceee, 22);
	MD5STEP(F1, a, b, c, d, puiIn[4] + 0xf57c0faf, 7);
	MD5STEP(F1, d, a, b, c, puiIn[5] + 0x4787c62a, 12);
	MD5STEP(F1, c, d, a, b, puiIn[6] + 0xa8304613, 17);
	MD5STEP(F1, b, c, d, a, puiIn[7] + 0xfd469501, 22);
	MD5STEP(F1, a, b, c, d, puiIn[8] + 0x698098d8, 7);
	MD5STEP(F1, d, a, b, c, puiIn[9] + 0x8b44f7af, 12);
	MD5STEP(F1, c, d, a, b, puiIn[10] + 0xffff5bb1, 17);
	MD5STEP(F1, b, c, d, a, puiIn[11] + 0x895cd7be, 22);
	MD5STEP(F1, a, b, c, d, puiIn[12] + 0x6b901122, 7);
	MD5STEP(F1, d, a, b, c, puiIn[13] + 0xfd987193, 12);
	MD5STEP(F1, c, d, a, b, puiIn[14] + 0xa679438e, 17);
	MD5STEP(F1, b, c, d, a, puiIn[15] + 0x49b40821, 22);
	//
	MD5STEP(F2, a, b, c, d, puiIn[1] + 0xf61e2562, 5);
	MD5STEP(F2, d, a, b, c, puiIn[6] + 0xc040b340, 9);
	MD5STEP(F2, c, d, a, b, puiIn[11] + 0x265e5a51, 14);
	MD5STEP(F2, b, c, d, a, puiIn[0] + 0xe9b6c7aa, 20);
	MD5STEP(F2, a, b, c, d, puiIn[5] + 0xd62f105d, 5);
	MD5STEP(F2, d, a, b, c, puiIn[10] + 0x02441453, 9);
	MD5STEP(F2, c, d, a, b, puiIn[15] + 0xd8a1e681, 14);
	MD5STEP(F2, b, c, d, a, puiIn[4] + 0xe7d3fbc8, 20);
	MD5STEP(F2, a, b, c, d, puiIn[9] + 0x21e1cde6, 5);
	MD5STEP(F2, d, a, b, c, puiIn[14] + 0xc33707d6, 9);
	MD5STEP(F2, c, d, a, b, puiIn[3] + 0xf4d50d87, 14);
	MD5STEP(F2, b, c, d, a, puiIn[8] + 0x455a14ed, 20);
	MD5STEP(F2, a, b, c, d, puiIn[13] + 0xa9e3e905, 5);
	MD5STEP(F2, d, a, b, c, puiIn[2] + 0xfcefa3f8, 9);
	MD5STEP(F2, c, d, a, b, puiIn[7] + 0x676f02d9, 14);
	MD5STEP(F2, b, c, d, a, puiIn[12] + 0x8d2a4c8a, 20);
	//
	MD5STEP(F3, a, b, c, d, puiIn[5] + 0xfffa3942, 4);
	MD5STEP(F3, d, a, b, c, puiIn[8] + 0x8771f681, 11);
	MD5STEP(F3, c, d, a, b, puiIn[11] + 0x6d9d6122, 16);
	MD5STEP(F3, b, c, d, a, puiIn[14] + 0xfde5380c, 23);
	MD5STEP(F3, a, b, c, d, puiIn[1] + 0xa4beea44, 4);
	MD5STEP(F3, d, a, b, c, puiIn[4] + 0x4bdecfa9, 11);
	MD5STEP(F3, c, d, a, b, puiIn[7] + 0xf6bb4b60, 16);
	MD5STEP(F3, b, c, d, a, puiIn[10] + 0xbebfbc70, 23);
	MD5STEP(F3, a, b, c, d, puiIn[13] + 0x289b7ec6, 4);
	MD5STEP(F3, d, a, b, c, puiIn[0] + 0xeaa127fa, 11);
	MD5STEP(F3, c, d, a, b, puiIn[3] + 0xd4ef3085, 16);
	MD5STEP(F3, b, c, d, a, puiIn[6] + 0x04881d05, 23);
	MD5STEP(F3, a, b, c, d, puiIn[9] + 0xd9d4d039, 4);
	MD5STEP(F3, d, a, b, c, puiIn[12] + 0xe6db99e5, 11);
	MD5STEP(F3, c, d, a, b, puiIn[15] + 0x1fa27cf8, 16);
	MD5STEP(F3, b, c, d, a, puiIn[2] + 0xc4ac5665, 23);
	//
	MD5STEP(F4, a, b, c, d, puiIn[0] + 0xf4292244, 6);
	MD5STEP(F4, d, a, b, c, puiIn[7] + 0x432aff97, 10);
	MD5STEP(F4, c, d, a, b, puiIn[14] + 0xab9423a7, 15);
	MD5STEP(F4, b, c, d, a, puiIn[5] + 0xfc93a039, 21);
	MD5STEP(F4, a, b, c, d, puiIn[12] + 0x655b59c3, 6);
	MD5STEP(F4, d, a, b, c, puiIn[3] + 0x8f0ccc92, 10);
	MD5STEP(F4, c, d, a, b, puiIn[10] + 0xffeff47d, 15);
	MD5STEP(F4, b, c, d, a, puiIn[1] + 0x85845dd1, 21);
	MD5STEP(F4, a, b, c, d, puiIn[8] + 0x6fa87e4f, 6);
	MD5STEP(F4, d, a, b, c, puiIn[15] + 0xfe2ce6e0, 10);
	MD5STEP(F4, c, d, a, b, puiIn[6] + 0xa3014314, 15);
	MD5STEP(F4, b, c, d, a, puiIn[13] + 0x4e0811a1, 21);
	MD5STEP(F4, a, b, c, d, puiIn[4] + 0xf7537e82, 6);
	MD5STEP(F4, d, a, b, c, puiIn[11] + 0xbd3af235, 10);
	MD5STEP(F4, c, d, a, b, puiIn[2] + 0x2ad7d2bb, 15);
	MD5STEP(F4, b, c, d, a, puiIn[9] + 0xeb86d391, 21);
	//
	m_auiBuf[0] += a;
	m_auiBuf[1] += b;
	m_auiBuf[2] += c;
	m_auiBuf[3] += d;
}


void CMD5::ENCODE_DATA(const char* pInput, int nLength, char* pOutput)
{
	char szDigest[256] = {0};// 4*4 바이트 이상 이면 된다.

	Reset();
	AddData(pInput, nLength);
	FinalDigest(szDigest);
	Binary2Hex(reinterpret_cast<unsigned char*>(szDigest), 16, pOutput);
}

void CMD5::ENCODE_DATA_LOW(const char* pInput, int nLength, char* pOutput)
{
	char szDigest[256] = {0};// 4*4 바이트 이상 이면 된다.

	Reset();
	AddData(pInput, nLength);
	FinalDigest(szDigest);
	Binary2HexLow(reinterpret_cast<unsigned char*>(szDigest), 16, pOutput);
}