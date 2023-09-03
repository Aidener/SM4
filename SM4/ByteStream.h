#ifndef BYTE_STREAM
#include<string>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstring>
typedef unsigned char Byte;
class ByteStream {
    Byte* ptr;
    unsigned int capcity;
    unsigned int size;
public:
    ByteStream();
    ByteStream(const char*);
    ByteStream(const Byte[],unsigned int length);
    ByteStream(const ByteStream&);
    ByteStream(const std::string&);
    //移动复制构造函数，降低内存重分配开销
    ByteStream(ByteStream&&);            
    Byte* const c_str() const;
    //此函数用于设置字节串的实际大小，注意，一般情况下，本函数不会初始化新开辟空间的值，主要用于设置文件读入或写入的字节数
    void resize(unsigned int);
    void reserve(unsigned int);
    Byte& operator[](unsigned int i);
    const ByteStream& operator=(const std::string&);
    int length()const;
    void copy(Byte* src, unsigned int begin, int count);   
    void copy(const std::string& str, unsigned int begin, unsigned int count);
    void clear();
    ~ByteStream();
    const ByteStream& operator =(const ByteStream&);
    const ByteStream& operator =(const ByteStream);
    ByteStream& operator +(const ByteStream&);
    ByteStream& operator +(ByteStream);
    std::string toString();
    friend std::ifstream& operator>>(std::ifstream& in, ByteStream& b);
};
std::ostream& operator<< (std::ostream& out, ByteStream& b);
std::ofstream& operator<<(std::ofstream& out, ByteStream& b);
std::ifstream& operator>>(std::ifstream& in, ByteStream& b);

void ByteAdjustToInteger_OnLittleEndian(Byte* s, unsigned int length);
void IntegerAdjustToByte_OnLittleEndian(unsigned int* a, unsigned int len);
#endif // !BYTE_STREAM



