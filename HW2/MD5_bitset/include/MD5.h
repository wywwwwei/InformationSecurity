#pragma once
#include<string>
#include<bitset>

using std::string;
using std::bitset;

class MD5 {
private:
	bitset<32>IV[4] = { 0x67452301,0xefcdab89,0x98badcfe,0x10325476 };
	bitset<32>MD[4];
	
	//MD5 encryption related
	static bitset<32>(*fistleFunc[4])(bitset<32>, bitset<32>, bitset<32>);
	static unsigned int table_int[64];
	static int leftShift[16];
	static int X[64];
	static inline bitset<32> functionF(bitset<32>b, bitset<32>c, bitset<32>d);
	static inline bitset<32> functionG(bitset<32>b, bitset<32>c, bitset<32>d);
	static inline bitset<32> functionH(bitset<32>b, bitset<32>c, bitset<32>d);
	static inline bitset<32> functionI(bitset<32>b, bitset<32>c, bitset<32>d);
	static inline bitset<32> loopLeftShift(bitset<32>input,int n);
	
	//radix and character position related
	static string dec264bitsbin(int input);
	static string string2bit(string &plain);
	static string littleEndian(string input);
	static string deLittleEndian(bitset<32> &input);
	static string bits2hex(string &input);

	void chunkProcess(string &chunk);
	void padding(string &input);
	void transform(string plain);
public:
	explicit MD5(string plain);
	string getCipher();
};