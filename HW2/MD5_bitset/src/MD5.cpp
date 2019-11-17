#include"MD5.h"
#include<cmath>
#include<iostream>


//Static member initialization
decltype(MD5::functionF) *MD5::fistleFunc[4] = { MD5::functionF,MD5::functionG,MD5::functionH,MD5::functionI };

int MD5::leftShift[16] = { 7,12,17,22,5,9,14,20,4,11,16,23,6,10,15,21 };

int MD5::X[64] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
	5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
	0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
};

unsigned int MD5::table_int[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

//static function
inline bitset<32> MD5::functionF(bitset<32>b, bitset<32>c, bitset<32>d){
	return (b & c) | (~b & d);
}

inline bitset<32> MD5::functionG(bitset<32>b, bitset<32>c, bitset<32>d){
	return (b & d) | (c & ~d);
}

inline bitset<32> MD5::functionH(bitset<32>b, bitset<32>c, bitset<32>d){
	return b ^ c ^ d;
}

inline bitset<32> MD5::functionI(bitset<32>b, bitset<32>c, bitset<32>d){
	return c ^ (b | ~d);
}

inline bitset<32> MD5::loopLeftShift(bitset<32>input, int n){
	return (input >> (32 - n)) | (input << n);
}

//Associated with radix
//dec264bitsbin return in little-endian.
string MD5::dec264bitsbin(int input)
{
	string result="";
	string temp = "";
	while (input > 0) {
		temp = std::to_string(input % 2) + temp;
		if (temp.length() == 8) {
			result += temp;
			temp = "";
		}
		input /= 2;
	}
	temp.insert(0, 8 - (temp.length() % 8), '0');
	result += temp;
	result.append(64 - result.length(), '0');
	return result;
}

string MD5::string2bit(string &plain) {
	string result;
	int i = 0;
	int j = 0;
	int len = plain.length();
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < 8; j++)
		{
			result += ((plain[i] >> (7 - j)) & 1) == 1 ? '1' : '0';
		}
	}
	return result;
}

string MD5::littleEndian(string input) {
	return input.substr(24, 8) + input.substr(16, 8) + input.substr(8, 8) + input.substr(0, 8);
}

string MD5::deLittleEndian(bitset<32>&input) {
	string temp = input.to_string();
	return temp.substr(24, 8) + temp.substr(16, 8) + temp.substr(8, 8) + temp.substr(0, 8);
}

string MD5::bits2hex(string &input) {
	char match[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
	string result;
	for (int i = 0; i < input.length(); i += 4) {
		result += match[(input[i] - '0')*(1 << 3) + (input[i + 1] - '0')*(1 << 2) + (input[i + 2] - '0')*(1 << 1) + (input[i + 3] - '0')];
	}
	return result;
}

//Specific encryption process
void MD5::padding(string &input) {
	string fill = "1";
	int origin_len = input.length();
	int len = (input.length() % 512 - 448);
	len = (len >= 0) ? 512 - len : -len;
	for (int i = 1; i < len; i++) {
		fill += "0";
	}
	input += fill + dec264bitsbin(origin_len);
}

void MD5::chunkProcess(string &trunk)
{
	bitset<32> Message[16];
	for (int i = 0; i < 16; i++) {
		Message[i] = bitset<32>(MD5::littleEndian(trunk.substr(32 * i, 32)));
	}

	bitset<32>MD[4] = { IV[0] ,IV[1] ,IV[2] ,IV[3] };
	for (int i = 0; i < 64; i++) {
		bitset<32> fistleRes = MD5::fistleFunc[i / 16](MD[1], MD[2], MD[3]);
		bitset<32> tempRes = (fistleRes.to_ulong() + MD[0].to_ulong() + Message[MD5::X[i]].to_ulong() + MD5::table_int[i])&0xffffffff;

		MD[0] = MD[3];
		MD[3] = MD[2];
		MD[2] = MD[1];
		MD[1] = MD[1].to_ulong() + MD5::loopLeftShift(tempRes, MD5::leftShift[4*(i/16)+(i%16)%4]).to_ulong();
	}

	IV[0] = (unsigned int)MD[0].to_ulong() + (unsigned int)IV[0].to_ulong();
	IV[1] = (unsigned int)MD[1].to_ulong() + (unsigned int)IV[1].to_ulong();
	IV[2] = (unsigned int)MD[2].to_ulong() + (unsigned int)IV[2].to_ulong();
	IV[3] = (unsigned int)MD[3].to_ulong() + (unsigned int)IV[3].to_ulong();

}

void MD5::transform(string plain)
{
	string plainBits = string2bit(plain);
	padding(plainBits);
	int len = plainBits.length();
	for (int i = 0; i <= (len - 1) / 512; i++) {
		string toEncrypt = plainBits.substr(i * 512, 512);
		chunkProcess(toEncrypt);
	}
	plain.clear();
	plainBits.clear();
}

MD5::MD5(string plain) {
	transform(plain);
}
string MD5::getCipher() {
	string cipher = MD5::deLittleEndian(IV[0]) + MD5::deLittleEndian(IV[1]) + MD5::deLittleEndian(IV[2]) + MD5::deLittleEndian(IV[3]);
	return bits2hex(cipher);
}