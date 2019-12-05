#include"X509Decoder.h"
#include<iostream>
#include<fstream>
#include<iomanip>

using std::cin;
using std::hex;
using std::streampos;

int main() {
	string filename;
	cin >> filename;
	std::ifstream fp(filename, std::ios::binary | std::ios::in);
	if (fp.is_open()) {
		std::cout << "Open certificate successfully" << std::endl;
	}
	streampos firstPos = fp.tellg();
	fp.seekg(0, std::ios::end);
	unsigned int len = fp.tellg() - firstPos;
	fp.seekg(0, std::ios::beg);

	Byte *input = new Byte[len];
	fp.read((char*)input, len);
	X509Decoder decoder(input,len);
	decoder.print();
	delete []input;
}