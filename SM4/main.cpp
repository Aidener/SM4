#include<iostream>
#include"SM4.h"

using namespace std;

//ECBʹ�ð���
void one() {
	ofstream   out("C1.bin", std::ios_base::binary);
	ofstream   out2("C2.bin", std::ios_base::binary);
	ifstream   in("text.txt", std::ios_base::binary);
	ByteStream str;
	SM4 sm4;
	str.resize(2048);
	in >> str;                                                                        //����>>�Զ���ȡ2048�ֽڼ���128����
	ByteStream C = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::ECB);
	str[16]++;                                                                        //���ĵڶ�����ı�1bit
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

//CFB��CBCʹ�ð���
void two() {
	ofstream   out("C1.bin", std::ios_base::binary);
	ofstream   out2("C2.bin", std::ios_base::binary);
	ifstream   in("text.txt", std::ios_base::binary);
	ByteStream str;
	SM4 sm4;
	str.resize(2048);
	in >> str;                                                                        //����>>�Զ���ȡ2048�ֽڼ���128����
	ByteStream C = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::CFB);
	
	ByteStream C1 = sm4.encrypted(str, "120456789123456", SM4::WORKMOD::CFB);       //��Կ�ı�

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

//OFBʹ�ð���
void three() {                                          //��������ģʽ�����ֻ��Ҫ�ı�SM4::WORKING::MOD
	ofstream   out("C1.bin", std::ios_base::binary);
	ofstream   out2("C2.bin", std::ios_base::binary);
	ifstream   in("text.txt", std::ios_base::binary);
	ByteStream str;
	SM4 sm4;                              //����sm4����
	str.resize(2048);
	in >> str;                                                                        //����>>�Զ���ȡ2048�ֽڼ���128����
	ByteStream C = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::OFB);

	ByteStream C1 = sm4.encrypted(str, "123456789123456", SM4::WORKMOD::OFB);       //��Կ�ı�
	C1[16]++;                                                                        //�ڶ�����Կ�ı�1bit

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
//�ٷ����԰���
int main() {
	Byte str[16] = { 0x01 ,0x23 ,0x45 ,0x67 ,0x89 ,0xab ,0xcd ,0xef ,0xfe ,0xdc ,0xba ,0x98 ,0x76 ,0x54 ,0x32 ,0x10 };
	ByteStream P(str,16);
	SM4 sm4;
	ByteStream C = sm4.encrypted(P,P,SM4::WORKMOD::ECB);
	ByteStream M = sm4.decrypted(C, P, SM4::WORKMOD::ECB);
	return 0;
}
*/
