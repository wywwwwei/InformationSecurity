#include<iostream>
#include<bitset>
#include<string>

using std::string;
using std::bitset;
using std::cout;
using std::cin;
using std::endl;

const int IP[] =
{
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};

const int IP_Inverse[] =
{
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
};

const int E[] =
{
	32,1,2,3,4,5,
	4,5,6,7,8,9,
	8,9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32,1
};

const int SBox[8][4][16] =
{
	{
		{ 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
		{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
		{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
		{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 }
	},
	{
		{ 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
		{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
		{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
		{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 }
	},
	{
		{ 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
		{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
		{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
		{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 }
	},
	{
		{ 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
		{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
		{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
		{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 }
	},
	{
		{ 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
		{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
		{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
		{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 }
	},
	{
		{ 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
		{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
		{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
		{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 }
	},
	{
		{ 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
		{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
		{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
		{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 }
	},
	{
		{ 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
		{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
		{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
		{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 }
	}
};

const int P[] =
{
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25
};

const int PC_1[] =
{
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
};

const int PC_2[] =
{
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
};

const int LeftShift[] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

bitset<48> SubKey[16];

const string dec2binStr[] =
{
	"0000","0001","0010","0011",
	"0100","0101","0110","0111",
	"1000","1001","1010","1011",
	"1100","1101","1110","1111"
};

const string hex2binStr(char hex)
{
	switch (hex)
	{
		case '0':
			return "0000";
		case '1':
			return "0001";
		case '2':
			return "0010";
		case '3':
			return "0011";
		case '4':
			return "0100";
		case '5':
			return "0101";
		case '6':
			return "0110";
		case '7':
			return "0111";
		case '8':
			return "1000";
		case '9':
			return "1001";
		case 'a' :
		case 'A' :
			return "1010";
		case 'b':
		case 'B':
			return "1011";
		case 'c':
		case 'C':
			return "1100";
		case 'd':
		case 'D':
			return "1101";
		case 'e':
		case 'E':
			return "1110";
		case 'f':
		case 'F':
			return "1111";
		default:
			return "0000";
	}
}

bitset<64> IPReplace(bitset<64> plain)
{
	bitset<64> result;
	for (int i = 1; i <= 64; i++)
	{
		result[64 - i] = plain[64 - IP[i - 1]];
	}
	return result;
}

bitset<64> IPRelpace_Inverse(bitset<64> plain)
{
	bitset<64> result;
	for (int i = 1; i <= 64; i++)
	{
		result[64 - i] = plain[64 - IP_Inverse[i - 1]];
	}
	return result;
}

bitset<32> PReplace(bitset<32> connectedGroup)
{
	bitset<32> result;
	for (int i = 1; i <= 32; i++)
	{
		result[32 - i] = connectedGroup[32 - P[i - 1]];
	}
	return result;
}

bitset<56> PC1Replace(bitset<64> Key)
{
	bitset<56> result;
	for (int i = 1; i <= 56; i++)
	{
		result[56 - i] = Key[64 - PC_1[i - 1]];
	}
	return result;
}

bitset<48> PC2Replace(bitset<56> CiDi)
{
	bitset<48> result;
	for (int i = 1; i <= 48; i++)
	{
		result[48 - i] = CiDi[56 - PC_2[i - 1]];
	}
	return result;
}

bitset<64> string2bit(string plain)
{
	bitset<64> result;
	string temp;
	int i = 0;
	int j = 0;
	int len = plain.length();
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < 8; j++)
		{
			result[i * 8 + j] = ((plain[len - i - 1] >> j) & 1);
		}
	}

	return result;
}


bitset<48> EExpand(bitset<32> Ri_1)
{
	bitset<48> result;
	for (int i = 1; i <= 48; i++)
	{
		result[48 - i] = Ri_1[32 - E[i - 1]];
	}
	return result;
}

string Transform64(string beforeGroup)
{
	string result;
	for (int i = 0; i < 8; i++)
	{
		string group = beforeGroup.substr(6 * i, 6);
		int row = (group[0] - '0') * 2 + (group[5] - '0');
		int col = (group[1] - '0') * 8 + (group[2] - '0') * 4 + (group[3] - '0') * 2 + (group[4] - '0');
		result += dec2binStr[SBox[i][row][col]];
	}
	return result;
}

bitset<32> getFeistel(bitset<48>expand_Ri_1, bitset<48>Ki)
{
	
	bitset<48> beforeGroup = expand_Ri_1 ^ Ki;
	
	bitset<32> afterGroupAndSB(Transform64(beforeGroup.to_string()));

	return PReplace(afterGroupAndSB);
}

void getSubKey(bitset<64> Key)
{
	//PC1 Permutation
	bitset<56> afterPC1 = PC1Replace(Key);

	bitset<28> C0(afterPC1.to_string().substr(0, afterPC1.size() / 2));
	bitset<28> D0(afterPC1.to_string().substr(afterPC1.size() / 2, afterPC1.size() / 2));

	for (int i = 0; i < 16; i++)
	{
		//shifted left (circular shift) one or two bits
		bitset<28>newC = C0 << LeftShift[i] | C0 >> (28 - LeftShift[i]);
		bitset<28>newD = D0 << LeftShift[i] | D0 >> (28 - LeftShift[i]);
		//PC2 Permutation
		SubKey[i] = PC2Replace(bitset<56>(newC.to_string() + newD.to_string()));
		C0 = newC;
		D0 = newD;
	}
}

void fillData(string &data_block)
{
	int dataLen = data_block.length();
	if (dataLen % 8 == 0)
	{
		data_block += "88888888";
	}
	else
	{
		int fill_num = 8 - (dataLen % 8);
		for (int i = 0; i < fill_num; i++)
		{
			data_block += char('0' + fill_num);
		}
	}
}

void cutData(string &data)
{
	int len = data.length();
	int last_num = data[len - 1] - '0';
	data = data.substr(0, len - last_num);
}

string EncryptOrDecrypt(string plain_text, bitset<64> Key,bool Encrypt)
{
	//Byte padding
	if(Encrypt) fillData(plain_text);
	
	int block_num = plain_text.length() / 8;

	string cipher;
	//subkey generator
	getSubKey(Key);

	//Divided into several blocks
	for (int i = 0; i < block_num; i++)
	{
		//initial permutation
		bitset<64> M0 = IPReplace(string2bit(plain_text.substr(8 * i, 8)));

		bitset<32> Li_1(M0.to_string().substr(0, M0.size() / 2));
		bitset<32> Ri_1(M0.to_string().substr(M0.size() / 2, M0.size() / 2));
		bitset<32> Li;
		bitset<32> Ri;

		//16 rounds
		for (int i = 0; i < 16; i++)
		{
			Li = Ri_1;
			Ri = Li_1 ^ getFeistel(EExpand(Ri_1), (Encrypt ? SubKey[i] : SubKey[15 - i]));

			Ri_1 = Ri;
			Li_1 = Li;
		}
		bitset<64> after_W(Ri.to_string() + Li.to_string());
		//final permutation
		cipher += IPRelpace_Inverse(after_W).to_string();
	}

	return cipher;
}

string bit2str(string input)
{
	string result;
	int len = (input.length() / 8);
	for (int i = 0; i < len; i++)
	{
		int value = 0;
		string cur = input.substr(8 * i, 8);
		for (int j = 0; j < 8; j++)
		{
			value += (cur[j]-'0') * (1 << (7 - j));
		}
		result += (char)value;
	}
	return result;
}

int main()
{
	int if_encrypt;
	bool encrypt;
	string key;
	string data;
	cout << "Choose encrypt -> 0   decrypt -> 1" << endl;
	cin >> if_encrypt;
	encrypt = (if_encrypt == 0) ? true : false;
	cout << "Please input a 64-bits Key:" << endl;
	cin >> key;
	cout << "Please input the (string)data you need to encrypt/decrypt:" << endl;
	cout << "Encrypt -> string data   Decrypt -> bit set" << endl;
	cin.ignore();
	std::getline(cin, data);
	cout << endl;

	string result = data;
	if (encrypt)
	{
		cout << "Encrypt :" << endl;
		cout << "Result:" << endl;
		result = EncryptOrDecrypt(data, bitset<64>(key), encrypt);
		cout << "bits format:" << endl << result << endl;
		cout << endl;
	}

	cout << "Decrypt :" << endl;

	string temp = bit2str(result);
	result = EncryptOrDecrypt(temp, bitset<64>(key), encrypt ? (!encrypt) : encrypt);
	cout << "Result:" << endl;
	cout << "bits format:" << endl << result << endl;

	temp = bit2str(result);
	cutData(temp);
	cout << "string format:" << endl << temp << endl;
}