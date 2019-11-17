#include"md5.h"
#include<iostream>
#include<iomanip>

using std::cout;
using std::endl;
//Static member initialization
decltype(MD5::functionF) *MD5::fistleFunc[4] = { MD5::functionF,MD5::functionG,MD5::functionH,MD5::functionI };

int MD5::leftShift[16] = { 7,12,17,22,5,9,14,20,4,11,16,23,6,10,15,21 };

int MD5::X[64] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
	5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
	0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
};

uint32_t MD5::table_int[64] = {
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
inline uint32_t MD5::functionF(uint32_t b, uint32_t c, uint32_t d) {
	return (b & c) | (~b & d);
}

inline uint32_t  MD5::functionG(uint32_t b, uint32_t c, uint32_t d) {
	return (b & d) | (c & ~d);
}

inline uint32_t MD5::functionH(uint32_t b, uint32_t c, uint32_t d) {
	return b ^ c ^ d;
}

inline uint32_t MD5::functionI(uint32_t b, uint32_t c, uint32_t d) {
	return c ^ (b | ~d);
}

inline uint32_t MD5::loopLeftShift(uint32_t input, int n) {
	return (input >> (32 - n)) | (input << n);
}

//Associated with radix
void MD5::littleEndian(uint32_t output[],uint8_t input[],int size) {
	for (int i = 0, j = 0; i < size; i += 4,j++) {
		output[j] = ((uint32_t)input[i] | (uint32_t)input[i + 1] << 8 | (uint32_t)input[i + 2] << 16 | (uint32_t)input[i + 3] << 24);
	}
}

void MD5::deLittleEndian(uint8_t output[],uint32_t input[],int size) {
	for (int i = 0, j = 0; i < size; i++, j += 4) {
		output[j] = input[i] & 0xff;
		output[j + 1] = (input[i] >> 8) & 0xff;
		output[j + 2] = (input[i] >> 16) & 0xff;
		output[j + 3] = (input[i] >> 24) & 0xff;
	}
}

//Specific encryption process
void MD5::padding(uint32_t len[2]) {
	static uint8_t paddingTable[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	uint8_t lenBits[64] = { '0' };
	deLittleEndian(lenBits, len, 2);

	unsigned int padLen = (len[0] % 512);
	padLen = (padLen < 448) ? (448 - padLen) : (448 + 512 - padLen);
	fillAndEncrypt(paddingTable, padLen / 8);
	fillAndEncrypt(lenBits, 64);

}

void MD5::chunkProcess(uint8_t trunk[])
{
	uint32_t  Message[16];
	MD5::littleEndian(Message, trunk, BLOCKSIZE);

	uint32_t MD[4] = { IV[0] ,IV[1] ,IV[2] ,IV[3] };
	for (int i = 0; i < 64; i++) {
		uint32_t  fistleRes = MD5::fistleFunc[i / 16](MD[1], MD[2], MD[3]);
		uint32_t  tempRes = fistleRes + MD[0] + Message[MD5::X[i]] + MD5::table_int[i];

		MD[0] = MD[3];
		MD[3] = MD[2];
		MD[2] = MD[1];
		MD[1] = MD[1] + MD5::loopLeftShift(tempRes, MD5::leftShift[4 * (i / 16) + (i % 16) % 4]);
	}

	IV[0] = MD[0] + IV[0];
	IV[1] = MD[1] + IV[1];
	IV[2] = MD[2] + IV[2];
	IV[3] = MD[3] + IV[3];

}

void MD5::transform(string plain)
{
	fillAndEncrypt((uint8_t*)plain.c_str(),plain.length());
	uint32_t plainBitsLen[2] = { messageLen & 0xFFFFFFFF,(messageLen >> 32) & 0xFFFFFFFF };
	padding(plainBitsLen);
	plain.clear();
}

MD5::MD5(string plain) {
	messageLen = 0;
	transform(plain);
}
void MD5::getCipher() {
	uint8_t output[16] = { '0' };
	deLittleEndian(output, IV, 4);
	//cout << std::hex;
	for (int i = 0; i < 16; i++) {
		cout << std::setw(2) << std::setfill('0') << std::hex << (int)output[i];
	}
}

void MD5::fillAndEncrypt(uint8_t plain[], unsigned int len){
	unsigned int curPos = messageLen / 8 % BLOCKSIZE;
	unsigned int firstToFill = BLOCKSIZE - curPos;
	unsigned int copyLen = 0;

	messageLen += len << 3;

	if (len >= firstToFill) {
		for (int i = curPos,j=0; i < BLOCKSIZE; i++,j++) {
			buffer[i] = plain[j];
			copyLen++;
		}
		chunkProcess(buffer);
		curPos = 0;

		for (int i = firstToFill; i + BLOCKSIZE <= len; i+=BLOCKSIZE) {
			chunkProcess(&plain[i]);
			copyLen += BLOCKSIZE;
		}
	}

	for (unsigned int i = curPos, j = copyLen; j < len; i++, j++) {
		buffer[i] = plain[j];
	}
}
