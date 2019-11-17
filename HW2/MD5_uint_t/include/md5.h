#pragma once
#pragma once

#include<cstdint>
#include<string>

using std::string;

#define BLOCKSIZE (512 / 8)

class MD5 {
private:
	uint32_t IV[4] = { 0x67452301,0xefcdab89,0x98badcfe,0x10325476 };
	uint32_t MD[4];

	//MD5 encryption related
	static uint32_t(*fistleFunc[4])(uint32_t, uint32_t, uint32_t);
	static uint32_t table_int[64];
	static int leftShift[16];
	static int X[64];
	static inline uint32_t functionF(uint32_t b, uint32_t c, uint32_t d);
	static inline uint32_t functionG(uint32_t b, uint32_t c, uint32_t d);
	static inline uint32_t functionH(uint32_t b, uint32_t c, uint32_t d);
	static inline uint32_t functionI(uint32_t b, uint32_t c, uint32_t d);
	static inline uint32_t loopLeftShift(uint32_t input, int n);

	//radix and character position related
	static void littleEndian(uint32_t output[], uint8_t input[], int size);
	static void deLittleEndian(uint8_t output[], uint32_t input[], int size);

	void chunkProcess(uint8_t trunk[]);
	void padding(uint32_t len[2]);
	void transform(string plain);
	void fillAndEncrypt(uint8_t plain[], unsigned int len);

	uint8_t buffer[BLOCKSIZE] = { '0' };//Record the last not aligned block
	uint64_t messageLen;
public:
	explicit MD5(string plain);
	void getCipher();
};