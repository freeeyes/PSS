#ifndef _AES_H
#define _AES_H

//AES加解密算法，建议秘钥超过1024字节
#include <stdio.h>
#include <memory>
#include <string>
#include <iostream>
#include <cstring>

class AES
{
public:
    AES();
    virtual ~AES();
    bool SetKey(const unsigned char* szKey);
    unsigned char* Cipher(unsigned char* input);
    unsigned char* InvCipher(unsigned char* input);
    char* Cipher(char* input, int length=0);
    char* InvCipher(char* input, int length);

private:
    unsigned char Sbox[256];
    unsigned char InvSbox[256];
    unsigned char w[11][4][4];

    void KeyExpansion(const unsigned char* key);
    void KeyExpansion_single(int i, int j, int r,  const unsigned char* rc);
    unsigned char FFmul(unsigned char a, unsigned char b) const;

    void SubBytes(unsigned char state[][4]) const;
    void ShiftRows(unsigned char state[][4]) const;
    void MixColumns(unsigned char state[][4]) const;
    void AddRoundKey(unsigned char state[][4], unsigned char k[][4]) const;

    void InvSubBytes(unsigned char state[][4]) const;
    void InvShiftRows(unsigned char state[][4]) const;
    void InvMixColumns(unsigned char state[][4]) const;
};


#endif
