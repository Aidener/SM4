#include<iostream>
#include"SM4.h"

using namespace std;

//ECB使用案例
void one() {
	ofstream   out("C1.bin", std::ios_base::binary);
	ofstream   out2("C2.bin", std::ios_base::binary);
	ifstream   in("text.txt", std::ios_base::binary);
	ByteStream str;
	SM4 sm4;
	str.resize(2048);
	in >> str;                                                                        //重载>>自动读取2048字节即是128分组
	ByteStream C = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::ECB);
	str[16]++;                                                                        //明文第二分组改变1bit
	ByteStream C1 = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::ECB);

	out << C;
	out2 << C1;

	out.close();
	out2.close();
	out.open("M1.bin", std::ios_base::binary);
	out2.open("M2.bin", std::ios_base::binary);

	ByteStream M = sm4.decrypted(C, "123456789123456", SM4::WORKMOD::ECB);
	ByteStream M1 = sm4.decrypted(C1, "123456789123456", SM4::WORKMOD::ECB);
	out << M;
	out2 << M1;

	out.close();
	out2.close();
}

//CFB或CBC使用案例
void two() {
	ofstream   out("C1.bin", std::ios_base::binary);
	ofstream   out2("C2.bin", std::ios_base::binary);
	ifstream   in("text.txt", std::ios_base::binary);
	ByteStream str;
	SM4 sm4;
	str.resize(2048);
	in >> str;                                                                        //重载>>自动读取2048字节即是128分组
	ByteStream C = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::CFB);
	
	ByteStream C1 = sm4.encrypted(str, "120456789123456", SM4::WORKMOD::CFB);       //密钥改变

	out << C;
	out2 << C1;

	out.close();
	out2.close();
	out.open("M1.bin", std::ios_base::binary);
	out2.open("M2.bin", std::ios_base::binary);

	ByteStream M = sm4.decrypted(C, "123456789123456", SM4::WORKMOD::CFB);
	ByteStream M1 = sm4.decrypted(C1, "120456789123456", SM4::WORKMOD::CFB);
	out << M;
	out2 << M1;

	out.close();
	out2.close();
}

//OFB使用案例
void three() {                                          //如需其他模式结果，只需要改变SM4::WORKING::MOD
	ofstream   out("C1.bin", std::ios_base::binary);
	ofstream   out2("C2.bin", std::ios_base::binary);
	ifstream   in("text.txt", std::ios_base::binary);
	ByteStream str;
	SM4 sm4;                              //创建sm4对象
	str.resize(2048);
	in >> str;                                                                        //重载>>自动读取2048字节即是128分组
	ByteStream C = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::OFB);

	ByteStream C1 = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::OFB);       //密钥改变
	C1[16]++;                                                                        //第二组密钥改变1bit

	out << C;
	out2 << C1;

	out.close();
	out2.close();
	out.open("M1.bin", std::ios_base::binary);
	out2.open("M2.bin", std::ios_base::binary);

	ByteStream M = sm4.decrypted(C, "123456789123456", SM4::WORKMOD::OFB);
	ByteStream M1 = sm4.decrypted(C1, "123456789123456", SM4::WORKMOD::OFB);
	out << M;
	out2 << M1;

	out.close();
	out2.close();
}



int main() {
	three();
	return 0;
}


/*
//官方测试案例
int main() {
	Byte str[16] = { 0x01 ,0x23 ,0x45 ,0x67 ,0x89 ,0xab ,0xcd ,0xef ,0xfe ,0xdc ,0xba ,0x98 ,0x76 ,0x54 ,0x32 ,0x10 };
	ByteStream P(str,16);
	SM4 sm4;
	ByteStream C = sm4.encrypted(P,P,SM4::WORKMOD::ECB);
	ByteStream M = sm4.decrypted(C, P, SM4::WORKMOD::ECB);
	return 0;
}
*/
