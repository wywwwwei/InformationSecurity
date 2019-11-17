#include"MD5.h"
#include<iostream>

using std::cin;
using std::getline;
using std::cout;
using std::endl;

int main() {
	string input;
	getline(cin, input);
	cout << MD5(input).getCipher() << endl;
}