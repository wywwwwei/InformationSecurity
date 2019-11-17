#include"md5.h"
#include<iostream>

int main() {
	string temp;
	std::getline(std::cin, temp);
	MD5(temp).getCipher();
}