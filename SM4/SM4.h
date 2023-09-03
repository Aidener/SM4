#ifndef SM4_H
#include"ByteStream.h"
class SM4Exception
{
public:
    SM4Exception(std::string msg);
    std::string getmsg();
private:
    std::string msg;
};


class SM4 {
private:
    static const unsigned char sbox[16][16];
    static const unsigned int FK[4];
    static const unsigned int CK[32];
    unsigned int IV[4];
    unsigned int Clast[4];
    enum class TARG{
        REPEAT,
        KEYLOAD
    };
    enum class WAY{
        ENCRYPTED,
        DECRYPTED
    };
    void IVmove();
    unsigned int SBOX(unsigned int x);
    unsigned int T(unsigned int x, TARG arg);
    void keyExtend(unsigned int k[], int i);
    void repeat(unsigned int x[4], unsigned int rk);
    void ECBMOD(unsigned int C[], int len, unsigned int rk[]);
    void CBCMOD(unsigned int C[], int len, unsigned int rk[], WAY);
    void CFBMOD(unsigned int C[], int len, unsigned int rk[], WAY);
    void OFBMOD(unsigned int C[], int len, unsigned int rk[], WAY);
public:
    enum class WORKMOD{
        ECB,
        CBC,
        CFB,
        OFB,
    };
    SM4();
    ByteStream encrypted(const ByteStream& M,ByteStream key,WORKMOD MOD, const ByteStream iv="123456789123456")throw (SM4Exception);
    ByteStream decrypted(const ByteStream& C,ByteStream key,WORKMOD MOD, const ByteStream iv="123456789123456")throw (SM4Exception);
};

#endif // !SM4_H
