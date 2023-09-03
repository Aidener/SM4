#include"ByteStream.h"

ByteStream::ByteStream(const char* c)
{
	unsigned int len = 0;
	ptr = NULL;
	size = 0;
	capcity = 0;
	for (; c[len] != '\0'; len++);
	ptr = new Byte[len + 1];
	memcpy(ptr, c, len+1);
	size = len + 1;
	capcity = size;
}
ByteStream::ByteStream(const Byte c[], unsigned int length)
{
	ptr = NULL;
	size = 0;
	capcity = 0;
	ptr = new Byte[length];
	memcpy(ptr, c, length );
	size = length;
	capcity = size;
}
ByteStream::ByteStream(const ByteStream& B)
{
	ptr = NULL;
	size = 0;
	capcity = 0;
	ptr=new Byte[B.size];
	memcpy(ptr, B.ptr, B.size);
	size = B.size;
	capcity = size;
}
ByteStream::ByteStream(const std::string& str)
{
	ptr = NULL;
	size = 0;
	capcity = 0;
	ptr = new Byte[str.length()];
	memcpy(ptr, str.c_str(), str.length());
	size = str.length();
	capcity = size;
}
ByteStream::ByteStream(ByteStream&& s)
{
	ptr = s.ptr;
	capcity = s.capcity;
	size = s.size;
	s.ptr = NULL;
}
 Byte* const ByteStream::c_str()const
{
	return ptr;
}

void ByteStream::resize(unsigned int p)
{
	if (p < capcity)
		size = p;
	else {
		reserve(p);
		size = p;
	}
}

void ByteStream::reserve(unsigned int c)
{
	if (c < capcity)
		return;
	unsigned int i = 1;
	for ( ;i <= c; i *= 2);
	capcity = i-1;
	Byte* p = new Byte[capcity];
	memcpy(p, ptr, size);
	delete[]ptr;
	ptr = p;
}

Byte& ByteStream::operator[](unsigned int i)
{
		return ptr[i];	
}
const ByteStream& ByteStream::operator=(const std::string& str)
{
	if (capcity < str.length()) {
		reserve(str.length());
	}
	memcpy(ptr, str.c_str(), str.length());
	size = str.length();
	return *this;
}
 int ByteStream::length()const
{
	return size;
}
void ByteStream::copy(Byte* src, unsigned int begin, int count)
{
	if (size < count)
		reserve(count);
	memcpy(ptr, &src[begin], count);
	size = count;
}
void ByteStream::copy(const std::string& str,unsigned int begin,unsigned int count)
{
	if (count >size)
		reserve(count);
	memcpy(ptr, &str.c_str()[begin], count);
	size = count;
}
void ByteStream::clear()
{
	size = 0;
}
ByteStream::ByteStream()
{
	capcity = 15;
	size = 0;
	ptr = new Byte[capcity];
}

ByteStream::~ByteStream()
{
	if(ptr)
	 delete[]ptr;
}

const ByteStream& ByteStream::operator=(const ByteStream& src)
{
	if (capcity < src.size) {
		reserve(src.size);
	}
	memcpy(ptr, src.c_str(), src.size);
	size = src.size;
	return *this;
}

const ByteStream& ByteStream::operator=(const ByteStream src)
{
	if (capcity < src.size) {
		reserve(src.size);
	}
	memcpy(ptr, src.c_str(), src.size);
	size = src.size;
	return *this;
}

ByteStream& ByteStream::operator+(const ByteStream& add)
{
	if (capcity < size + add.size)
		resize(size + add.size);
	memcpy(&ptr[size], add.ptr, add.size);
	return *this;
}

ByteStream& ByteStream::operator+(ByteStream add)
{
	if (capcity < size + add.size)
		resize(size + add.size);
	memcpy(&ptr[size], add.ptr, add.size);
	return *this;
}

std::string ByteStream::toString()
{
	return std::string((char*)ptr);
}

std::ostream& operator<<(std::ostream& out, ByteStream& b)
{
		out << b.c_str();
	return out;
}

std::ofstream& operator<<(std::ofstream& out, ByteStream& b)
{
		out.write((const char*)b.c_str(),b.length());
		return out;
}

std::ifstream& operator>>(std::ifstream& in, ByteStream& b)
{
	in.read((char*)b.ptr, b.length());
	return in;
}

void ByteAdjustToInteger_OnLittleEndian(Byte *s,unsigned int length)
{
	Byte tem = '\0';
	for (int i = 0; i < length/4; i++) {
		tem = s[4*i];
		s[4*i] = s[4 * i + 3];
		s[4 * i + 3] = tem;
		tem = s[4 * i + 1];
		s[4 * i + 1] = s[4 * i + 2];
		s[4 * i + 2] = tem;
	}
}

void IntegerAdjustToByte_OnLittleEndian(unsigned int* a,unsigned int len)
{
	for (int i = 0; i < len; i++) {
		a[i] = ((a[i] >> 24) & 0xFF) | ((a[i] >> 8) & 0xFF00) | ((a[i] << 8) & 0xFF0000) | ((a[i] << 24) & 0xFF000000);
	}
}

