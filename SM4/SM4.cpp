#include "SM4.h"

SM4Exception::SM4Exception(std::string msg) :msg(msg) {

}

std::string SM4Exception::getmsg() {
    return msg;
}

int cycleLeft(unsigned int x, int j) {
    return (x << j) | (x >> (32 - j));
}

const unsigned char SM4::sbox[16][16] = {                                                          //s盒
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
    0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
    0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
    0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
    0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
    0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
    0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
    0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
    0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
    0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
    0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
    0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
    0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
    0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
    0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
    0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48 };

const unsigned int SM4::FK[4] = { 0xA3B1BAC6,0x56AA3350, 0x677D9197,0xB27022DC };

const unsigned int SM4::CK[32] = {        //为提高效率直接给出，也可自行计算CK[i][j]=((4i+j)*7)%256,其中i为轮数，j为ck[i]第几个字节
    0x00070E15,0x1C232A31,0x383F464D,0x545B6269,
    0x70777E85,0x8C939AA1,0xA8AFB6BD,0xC4CBD2D9,
    0xE0E7EEF5,0xFC030A11,0x181F262D,0x343B4249,
    0x50575E65,0x6C737A81,0x888F969D,0xA4ABB2B9,
    0xC0C7CED5,0xDCE3EAF1,0xF8FF060D,0x141B2229,
    0x30373E45,0x4C535A61,0x686F767D,0x848B9299,
    0xA0A7AEB5,0xBCC3CAD1,0xD8DFE6ED,0xF4FB0209,
    0x10171E25,0x2C333A41,0x484F565D,0x646B7279
};


void SM4::IVmove()
{
    for (int i = 0; i < 2; i++) {
        IV[i] = IV[i + 2];
    }
}

unsigned int SM4::SBOX(unsigned int x)
{
    unsigned char* c = (unsigned char*)&x;
    c[0] = sbox[(c[0] & 0xf0)>>4][c[0] & 0xf];
    c[1] = sbox[(c[1] & 0xf0)>>4][c[1] & 0xf];
    c[2] = sbox[(c[2] & 0xf0)>>4][c[2] & 0xf];
    c[3] = sbox[(c[3] & 0xf0)>>4][c[3] & 0xf];
    return x;
}

unsigned int SM4::T(unsigned int x, TARG arg)
{
    x = SBOX(x);

    if (arg == TARG::REPEAT)
        return x ^ cycleLeft(x, 2) ^ cycleLeft(x, 10) ^ cycleLeft(x, 18) ^ cycleLeft(x, 24);
    else if (arg == TARG::KEYLOAD)
        return x ^ cycleLeft(x, 13) ^ cycleLeft(x, 23);
    else
        return 0;
}


void SM4::keyExtend(unsigned int k[], int i)
{
    unsigned int x = k[0] ^ T(k[1] ^ k[2] ^ k[3] ^ CK[i], TARG::KEYLOAD);
    for (int i = 0; i < 3; i++) {
        k[i] = k[i + 1];
    }
    k[3] = x;
}

void SM4::repeat(unsigned int x[4], unsigned int rk)
{
    unsigned int n = x[0] ^ T(x[1] ^ x[2] ^ x[3] ^ rk, TARG::REPEAT);
    for (int i = 0; i < 3; i++) {
        x[i] = x[i + 1];
    }
    x[3] = n;
}

void SM4::ECBMOD(unsigned int C[], int len, unsigned int rk[])
{
    unsigned int x = 0;
    for (int i = 0; i < len; i += 4) {
        for (int j = 0; j < 32; j++)                                 //32轮加密
            repeat(&C[i], rk[j]);

        x = C[i];                                      //翻转
        C[i] = C[i + 3];
        C[i + 3] = x;
        x = C[i + 1];
        C[i + 1] = C[i + 2];
        C[i + 2] = x;
    }
}

void SM4::CBCMOD(unsigned int C[], int len, unsigned int rk[], WAY way)
{
    unsigned int x = 0;
    unsigned int Tlast[4] = { 0 };                 //局部临时密文存储


    for (int i = 0; i < len; i += 4) {
        for (int j = 0; j < 4 && i>3 && way == WAY::ENCRYPTED; j++)                               //与上次密文亦或
            C[i + j] = C[i + j] ^ C[i + j - 4];

        for (int j = 0; j < 4 && way == WAY::DECRYPTED; j++) {
            Tlast[j] = C[i + j];                                                                  //临时存储本次密文
        }

        for (int j = 0; j < 32; j++)                                  //32轮加密
            repeat(&C[i], rk[j]);

        x = C[i];                                      //翻转
        C[i] = C[i + 3];
        C[i + 3] = x;
        x = C[i + 1];
        C[i + 1] = C[i + 2];
        C[i + 2] = x;

        for (int j = 0; j < 4 && i > 3 && way == WAY::DECRYPTED; j++) {
            C[i + j] = C[i + j] ^ Clast[j];                                                           //使用上次密文解密
        }

        for (int j = 0; j < 4 && way == WAY::DECRYPTED; j++) {
            Clast[j] = Tlast[j];                                                                     //存储本次密文
        }

    }
}

void SM4::CFBMOD(unsigned int C[], int len, unsigned int rk[], WAY way)
{
    unsigned int x = 0;
    for (int i = 0; i < len; i += 2) {                            //分组为2字
        if (i > 1 && way == WAY::ENCRYPTED) {
            IVmove();
            for (int j = 0; j < 2; j++) {                           //使用上次密文补充IV后两字
                IV[j + 2] = C[i + j - 2];
            }
        }
        if (i > 1 && way == WAY::DECRYPTED) {
            IVmove();
            IV[2] = Clast[0];                                   //使用上次密文补充IV后两字
            IV[3] = Clast[1];
        }
        if (way == WAY::DECRYPTED && i < len - 2) {             //存储本次密文
            Clast[0] = C[i];
            Clast[1] = C[i + 1];
        }

        for (int j = 0; j < 32; j++)                                  //32轮加密
            repeat(IV, rk[j]);

        x = IV[0];                                      //翻转
        IV[0] = IV[3];
        IV[3] = x;
        x = IV[1];
        IV[1] = IV[2];
        IV[2] = x;

        for (int j = 0; j < 2; j++) {
            C[i + j] = IV[j] ^ C[i + j];                      //取前两字加解密分组
        }
    }
}

void SM4::OFBMOD(unsigned int C[], int len, unsigned int rk[], WAY way)
{
    unsigned int x = 0;
    for (int i = 0; i < len; i += 2) {                            //分组为2字
        if (i > 1) {
            for (int j = 0; j < 2; j++)                         //循环移位2字
            {
                x = IV[j];
                IV[j] = IV[j + 2];
                IV[j + 2] = x;
            }
        }

        for (int j = 0; j < 32; j++)                                  //32轮加密
            repeat(IV, rk[j]);

        x = IV[0];                                      //翻转
        IV[0] = IV[3];
        IV[3] = x;
        x = IV[1];
        IV[1] = IV[2];
        IV[2] = x;

        for (int j = 0; j < 2; j++) {
            C[i + j] = IV[j] ^ C[i + j];                      //取前两字加解密分组
        }
    }
}


SM4::SM4() 
{
    for (int i = 0; i < 4; i++) {
        IV[i] = 0;
        Clast[i] = 0;
    }
}

ByteStream SM4::encrypted(const ByteStream& M, ByteStream key, WORKMOD MOD, const ByteStream iv) throw(SM4Exception)
{
    if (key.length() != 16) {
        throw SM4Exception("初始化秘钥不满足16字节或128bit");
        return ByteStream();
    }

    if (iv.length() != 16) {
        throw SM4Exception("初始化向量iv不满足16字节或128bit");
        return ByteStream();
    }

    int len = M.length() / 4;
    int byteappend = 0;
    if (M.length() % 16)
        byteappend = 16 - M.length() % 16;
    else
        byteappend = 16;                                      
    len += (byteappend+3)/4;                                                 //补全128
    unsigned int* CINT = new unsigned int[len]();            //初始化为0
    memcpy(CINT, M.c_str(), M.length());                   //明文分片
    ByteAdjustToInteger_OnLittleEndian((Byte*)CINT, len*4);
    for(int i=0;i<byteappend;i++)
       ((Byte*)CINT)[len*4-1-i] = byteappend;                            //PKCS#7填充

    unsigned int K[4] = { 0 };                                //四个秘钥分片,扩展32个rk
    ByteAdjustToInteger_OnLittleEndian(key.c_str(),key.length());
    memcpy(K, key.c_str(), key.length());
    //填入初始化向量IV
    ByteAdjustToInteger_OnLittleEndian(iv.c_str(), iv.length());
    memcpy(IV, iv.c_str(), iv.length());
    unsigned int rk[32] = { 0 };
    for (int i = 0; i < 4; i++) {
        K[i] = K[i] ^ FK[i];
    }
    for (int i = 0; i < 32; i++) {
        keyExtend(K, i);
        rk[i] = K[3];
    }
    if (MOD == WORKMOD::CBC)
        CBCMOD(CINT, len, rk, WAY::ENCRYPTED);                                           //分组反馈模式
    if (MOD == WORKMOD::CFB) {
        memcpy(IV, iv.c_str(), 16);
        CFBMOD(CINT, len, rk, WAY::ENCRYPTED);                                           //密码反馈模式
    }
    if (MOD == WORKMOD::ECB)
        ECBMOD(CINT, len, rk);                                                           //电码本模式
    if (MOD == WORKMOD::OFB) {
        memcpy(IV, iv.c_str(), 16);
        OFBMOD(CINT, len, rk, WAY::ENCRYPTED);                                           //输出反馈模式
    }

    ByteStream result;
    IntegerAdjustToByte_OnLittleEndian(CINT, len);
    result.copy((Byte*)CINT,0, len * 4);
    delete[]CINT;
    return result;
}

ByteStream SM4::decrypted(const ByteStream& C, const ByteStream key, WORKMOD MOD, const ByteStream iv)throw (SM4Exception)
{
    if (key.length() != 16) {
        throw SM4Exception("初始化秘钥不满足16字节或128bit");
        return ByteStream();
    }
    
    if (iv.length() != 16) {
        throw SM4Exception("初始化向量iv不满足16字节或128bit");
        return ByteStream();
    }

    int len = C.length() / 4;
    if (C.length() % 16) {
        throw SM4Exception("密文长度不满足16字节或128bit");
        return ByteStream();
    }

    unsigned int* M = new unsigned int[len]();          //初始化
    memcpy(M, C.c_str(), C.length());                   //明文分片
    ByteAdjustToInteger_OnLittleEndian((Byte*)M, C.length());
    
    unsigned int K[4] = { 0 };                  //四个秘钥分片,扩展32个rk
    ByteAdjustToInteger_OnLittleEndian(key.c_str(), key.length());
    memcpy(K, key.c_str(), key.length());
    //填入初始化向量IV
    ByteAdjustToInteger_OnLittleEndian(iv.c_str(), iv.length());
    memcpy(IV, iv.c_str(), iv.length());

    unsigned int rk[32] = { 0 };
    for (int i = 0; i < 4; i++) {
        K[i] = K[i] ^ FK[i];
    }
    for (int i = 0; i < 32; i++) {
        keyExtend(K, i);
        rk[i] = K[3];
    }
    unsigned int tem = 0;
    for (int i = 0; i < 16 && (MOD == WORKMOD::CBC || MOD == WORKMOD::ECB); i++) {             //秘钥逆序  IV不能逆序
        tem = rk[i];
        rk[i] = rk[31 - i];
        rk[31 - i] = tem;
    }
    if (MOD == WORKMOD::CBC)                                           //分组反馈模式
        CBCMOD(M, len, rk, WAY::DECRYPTED);
    if (MOD == WORKMOD::CFB) {                                           //密码反馈模式
        memcpy(IV, iv.c_str(), 16);
        CFBMOD(M, len, rk, WAY::DECRYPTED);
    }
    if (MOD == WORKMOD::ECB)                                        //电码本模式
        ECBMOD(M, len, rk);

    if (MOD == WORKMOD::OFB) {                                        //输出反馈模式
        memcpy(IV, iv.c_str(), 16);
        OFBMOD(M, len, rk, WAY::DECRYPTED);
    }

    ByteStream result;
    IntegerAdjustToByte_OnLittleEndian(M, len);
    int byteappend = ((Byte*)M)[len * 4-1];
    if (byteappend != ((Byte*)M)[len*4-1 - ((byteappend + 1) / 2)] || byteappend>16)
        throw SM4Exception("密文填充受损，解密可能不完整");
    result.copy((Byte*)M, 0, len * 4 - (byteappend > 16 ? 16 : byteappend));          //PKCS#7填充去除
    delete[]M;
    return result;
}






