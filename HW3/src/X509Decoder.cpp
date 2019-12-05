#include"X509Decoder.h"
#include<iostream>
#include<sstream>
#include<iomanip>

using std::cout;
using std::endl;
using std::to_string;
using std::stringstream;

#define VERSION_COUNT 0
#define SERIAL_COUNT 1
#define SIG_COUNT 2
#define ISSUER_COUNT 3
#define VALID_COUNT 4
#define SUBJ_COUNT 5
#define SUBJ_PUBLIC_KEY_COUNT 6
#define ISSUER_UNIQUE_COUNT 7
#define SUBJ_UNIQUE_COUNT 8
#define EXTENSIONS_COUNT 9
#define SIG_ALGORITHM_COUNT 10
#define SIG_VALUE_COUNT 11

map<string, string> X509Decoder::maps = {
	{"1.2.840.10040.4.1","DSA"},
	{"1.2.840.10040.4.3", "DSA with SHA-1"},
	{"1.2.840.113549.1.1.1", "RSA"},
	{"1.2.840.113549.1.1.2", "MD2 with RSA"},
	{"1.2.840.113549.1.1.3", "MD4 with RSA"},
	{"1.2.840.113549.1.1.4", "MD5 with RSA"},
	{"1.2.840.113549.1.1.5", "SHA-1 with RSA"},
	{"1.2.840.113549.1.1.11", "SHA-256 with RSA"},
	{"1.2.840.10045.2.1", "EC Public Key"},
	{"1.2.840.10045.3.1.7", "ECC"},
	{"2.5.4.3", "Common Name"},
	{"2.5.4.6", "Country Name"},
	{"2.5.4.7", "Locality Name" },
	{"2.5.4.8", "State or Province Name"},
	{"2.5.4.10", "Organization Name"},
	{"2.5.4.11", "Organization Unit Name"},
	{"2.5.4.15", "Business Category"},
	{ "1.3.6.1.5.5.7.1.1" ,"Authority Information Access"},
	{ "1.3.6.1.5.5.7.1.11" ,"Subject Information Access"},
	{"1.3.6.1.5.5.7.48.1", "Online Certificate Status Protocol (OCSP)"},
	{"1.3.6.1.5.5.7.48.2", "Certificate authority issuers" },
	{"1.3.6.1.5.5.7.2.1","CPS Qualifier"},
	//ExtensionOID
	{ "2.5.29.9" ,"Subject Directory Attributes"},
	{"2.5.29.14", "Subject Key Identifier" },
	{ "2.5.29.15", "Key Usage" },
	{ "2.5.29.17", "Subject Alternative Name" },
	{ "2.5.29.19", "Basic Constraints" },
	{ "2.5.29.28" ,"Issuing Distribution Point"},
	{ "2.5.29.30" ,"Name Constraints"},
	{ "2.5.29.31", "CRL Distribution Points" },
	{ "2.5.29.32", "Certificate Policies" },
	{ "2.5.29.33" ,"Policy Mapping"},
	{ "2.5.29.35", "Author Key Identifier" },
	{ "2.5.29.36" ,"Policy Constraints"},
	{ "2.5.29.37", "Extended Key Usage" },
	{ "2.5.29.46" ,"Freshest Crl"},
	{ "2.5.29.54" ,"Inhibit Any Policy"},
	{"1.3.6.1.5.5.7.3.1","Server authentication"},
	{"1.3.6.1.5.5.7.3.2","Client authentication"},
	{"1.3.6.1.5.5.7.3.3","Code signing"},
	{"1.3.6.1.5.5.7.3.4","Email"},
	{"1.3.6.1.5.5.7.3.8","Timestamping"},
	{"1.3.6.1.5.5.7.3.9","OCSP Signing"},
	{"1.3.6.1.4.1.311.10.3.1","Certificate trust list signing"},
	{"1.3.6.1.4.1.311.10.3.3","Microsoft Server Gated Crypto (SGC)"},
	{"1.3.6.1.4.1.311.10.3.4","Microsoft Encrypted File System"},
	{"1.3.6.1.4.1.11129.2.4.2","Precert Signed Certificate Timestamps"},
	{"1.3.6.1.4.1.311.60.2.1.1","Jurisdiction Locality Name"},
	{"1.3.6.1.4.1.311.60.2.1.2","Jurisdiction State Or Province Name"},
	{"1.3.6.1.4.1.311.60.2.1.3","Jurisdiction Country Name"},
	{"2.16.840.1.113730.4.1","Netscape SGC"}
};

X509Decoder::X509Decoder(Byte *input,unsigned int _len) {
	this->input = input;
	this->index = 0;
	this->fileLen = _len;
}

unsigned int X509Decoder::getLen() {
	unsigned int len = this->input[this->index++];
	if (len > 0x7f) {
		unsigned int trueLen = 0;
		len = len & 0x7f;
		for (int i = 0; i < len; i++) {
			trueLen = trueLen << 8;
			trueLen += this->input[this->index++];
		}
		return trueLen;
	}
	else {
		return len;
	}
}

void X509Decoder::decode() {
	string id;
	bool critical = false;
	bool inSet = false;
	unsigned int offset = 0;
	unsigned int firstIndex = 0;
	unsigned int curSetLen = 0;
	unsigned int tempIndex = 0;
	while (this->index < this->fileLen) {
		Byte type = this->input[this->index++];
		switch (type)
		{
		case 0x31:
			curSetLen = this->getLen();
			tempIndex = this->index;
			break;
		case 0x30:
			if (this->count > SERIAL_COUNT) {
				if (offset == 0) {
					offset = this->getLen();
					firstIndex = this->index;
				}
				else {
					if (this->index >= offset + firstIndex) {
						if (this->count < SUBJ_PUBLIC_KEY_COUNT) {
							this->count++;
						}
						else if (this->count == SUBJ_PUBLIC_KEY_COUNT) {
							this->count = SIG_ALGORITHM_COUNT;
						}
						else if (this->count == ISSUER_UNIQUE_COUNT || this->count == SUBJ_UNIQUE_COUNT || this->count == EXTENSIONS_COUNT) {
							this->count = SIG_ALGORITHM_COUNT;
						}
						offset = this->getLen();
						firstIndex = this->index;
					}
					else {
						if (this->count == SUBJ_PUBLIC_KEY_COUNT) {
							curSetLen = this->getLen();
							tempIndex = this->index;
						}
						else {
							this->getLen();
						}
					}
				}
			}
			else {
				this->getLen();
			}
			break;
		case 0xa0:
			this->count = VERSION_COUNT;
			decodeVersion();
			break;
		case 0xa1:
			this->count = ISSUER_UNIQUE_COUNT;
			offset = this->getLen();
			firstIndex = this->index;
			break;
		case 0xa2:
			this->count = SUBJ_UNIQUE_COUNT;
			offset = this->getLen();
			firstIndex = this->index;
			break;
		case 0xa3:
			this->count = EXTENSIONS_COUNT;
			offset = this->getLen();
			firstIndex = this->index;
			break;
		case 0x01:
			switch (this->count)
			{
			case EXTENSIONS_COUNT:
				this->index++;
				if (this->input[index++] == 0xFF) {
					critical = true;
				}
				break;
			default:
				break;
			}
			break;
		case 0x02:
			switch (this->count)
			{
			case SERIAL_COUNT:
				this->result.tbscertificate.serialNumber = this->decodeInts(this->getLen());
				this->count++;
				break;
			default:
				logError();
				break;
			}
			break;
		case 0x03:
			switch (this->count)
			{
			case SUBJ_PUBLIC_KEY_COUNT:
				this->result.tbscertificate.subjectPublicKeyInfo.subjectPublicKey = this->decodeBitstring();
				break;
			case ISSUER_UNIQUE_COUNT:
				this->result.tbscertificate.issuerUniqueID = this->decodeBitstring();
				break;
			case SUBJ_UNIQUE_COUNT:
				this->result.tbscertificate.subjectUniqueID = this->decodeBitstring();
				break;
			case SIG_VALUE_COUNT:
				this->result.signatureValue = this->decodeBitstring();
				break;
			default:
				this->logError();
				break;
			}
			break;
		case 0x04:
			this->result.tbscertificate.extensions.emplace_back(Extension(id, critical, this->decodeBitstring(true, this->getLen())));
			critical = false;
			break;
		case 0x06:
			switch (this->count)
			{
			case SIG_COUNT:
				this->result.tbscertificate.signature.algorithm = X509Decoder::maps[this->decodeOID()];
				this->result.tbscertificate.signature.parameters = this->decodeBitstring(true, offset - (this->index - firstIndex));
				break;
			case ISSUER_COUNT:
			case SUBJ_COUNT:
			case EXTENSIONS_COUNT:
				id = X509Decoder::maps[this->decodeOID()];
				break;
			case SUBJ_PUBLIC_KEY_COUNT:
				this->result.tbscertificate.subjectPublicKeyInfo.algorithm.algorithm = X509Decoder::maps[this->decodeOID()];
				this->result.tbscertificate.subjectPublicKeyInfo.algorithm.parameters = this->decodeBitstring(true,curSetLen-(this->index-tempIndex));
				break;

			case SIG_ALGORITHM_COUNT:
				this->result.signatureAlgorithm.algorithm = X509Decoder::maps[this->decodeOID()];
				this->result.signatureAlgorithm.parameters = this->decodeBitstring(true, offset - (this->index - firstIndex));
				this->count++;
				break;
			default:
				break;
			}
			break;
		case 0x0C:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x16:
		case 0x1A:
		case 0x1E:
			switch (this->count)
			{
			case ISSUER_COUNT:
				this->result.tbscertificate.issuer.emplace_back(AttributeTypeAndValue(id, this->decodePrintableStr()));
				break;
			case SUBJ_COUNT:
				this->result.tbscertificate.subject.emplace_back(AttributeTypeAndValue(id, this->decodePrintableStr()));
				break;
			default:
				break;
			}
			break;
		case 0x17:
			switch (this->count)
			{
			case VALID_COUNT:
				this->result.tbscertificate.validity.notBefore = this->decodeTime();
				this->index++;
				this->result.tbscertificate.validity.notAfter = this->decodeTime();
				break;
			default:
				break;
			}
			break;
		default:
			this->logError();
		}
	}
}

void X509Decoder::decodeVersion() {
	unsigned int len = this->input[this->index++];
	if (len != 0x03) {
		this->logError();
	}
	if (this->input[this->index++] != 0x02) {
		this->logError();
	}
	if (this->getLen() != 0x01) {
		this->logError();
	}
	this->result.tbscertificate.version = (Version)this->decodeInt();
	this->count = SERIAL_COUNT;
}

void X509Decoder::logError() {
	cout << "An unknown error occurred" << endl;
	exit(1);
}

unsigned int X509Decoder::decodeInt() {
	return this->input[this->index++];
}

vector<Byte> X509Decoder::decodeInts(unsigned int len) {
	vector<Byte> number;
	for (int i = 0; i < len; i++) {
		number.emplace_back(this->input[this->index++]);
	}
	return number;
}

string X509Decoder::decodeOID() {
	unsigned int lens = this->getLen();
	unsigned int result = this->input[this->index++];
	string val = to_string(result / 40) + '.' + to_string(result % 40) + '.';
	result = 0;
	for (unsigned int i = 0; i < lens - 1; i++) {
		if (this->input[this->index] & 0x80) {
			result = (result << 7) + (this->input[this->index++] & 0x7F);
		}
		else {
			result = (result << 7) + this->input[this->index++];
			val += to_string(result);
			if (i + 1 != lens -1) {
				val += '.';
			}
			result = 0;
		}
	}
	if (X509Decoder::maps.count(val) == 0) {
		X509Decoder::maps.insert({ val,val });
	}
	return val;
}

string X509Decoder::decodePrintableStr(){
	unsigned int lens = this->getLen();
	string result;
	for (int i = 0; i < lens; i++) {
		result += (char)this->input[this->index++];
	}
	return result;
}

string X509Decoder::decodeTime() {
	unsigned int lens = this->getLen();
	string result;
	for (int i = 0; i < lens; i++) {
		if (i == 0) {
			if (this->input[this->index] < (unsigned char)'5') {
				result += "20";
			}
			else {
				result += "19";
			}
		}
		result += (char)this->input[this->index++];
		if (i == 1 || i == 3) {
			result += '/';
		}
		else if (i == 5) {
			result += ' ';
		}
		else if (i == 7 || i == 9) {
			result += ":";
		}
	}
	result += " UTC";
	return result;
}

string X509Decoder::decodeBitstring(bool any ,unsigned int _len ) {
	stringstream ss;
	unsigned int len;
	if (!any) {
		len = this->getLen() - 1;
		Byte bits = this->input[this->index++];
		this->input[this->index + len - 1] = this->input[this->index + len - 1] >> bits;
	}
	else {
		len = _len;
	}
	unsigned int i = this->index;
	unsigned int count = 0;
	vector<Byte> res;
	while (this->index < i + len) {
		if (this->index == i + len - 1) {
			ss << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)this->input[this->index++] << endl;
			continue;
		}
		switch (this->input[this->index++]) {
		case 0x30:
		case 0x31:
		case 0xa0:
			this->getLen();
			break;
		case 0x01:
			this->index++;
			if (this->input[this->index++] == 0xFF) {
				ss << "True" << endl;
			}
			else {
				ss << "False" << endl;
			}
			break;
		case 0x02:
		case 0x04:
		case 0x80:
			res = this->decodeInts(this->getLen());
			count = 0;
			for (vector<Byte>::iterator iter = res.begin(); iter != res.end(); iter++) {
				count++;
				ss << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)*iter << " ";
				if (count % 16 == 0) {
					ss << endl;
				}
			}
			res.clear();
			ss << endl;
			break;
		case 0x03:
			ss << this->decodeBitstring() << endl;
			break;
		case 0x05:
			this->index++;
			ss << "NULL" << endl;
			break;
		case 0x06:
			ss << X509Decoder::maps[this->decodeOID()] << ": ";
			break;
		case 0x13:
		case 0x82:
		case 0x86:
			ss << this->decodePrintableStr() << endl;
			break;
		case 0x17:
			ss << this->decodeTime() << endl;
			break;
		default:
			this->index--;
			res = this->decodeInts(len);
			count = 0;
			for (vector<Byte>::iterator iter = res.begin(); iter != res.end(); iter++) {
				count++;
				ss << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)*iter << " ";
				if (count % 16 == 0) {
					ss << endl;
				}
			}
			res.clear();
			ss << endl;
			break;
		}
	}
	if (ss.rdbuf()->in_avail()==NULL) {
		ss << "NULL" << endl;
	}
	return ss.str();
}

void X509Decoder::print() {
	decode();

	cout << "Certificate:" << endl;
	cout << "Version:" << endl << "\tV" << this->result.tbscertificate.version + 1 << endl;
	cout << "Serial Number:" << endl << "\t";
	unsigned int count = 0;
	for (vector<Byte>::iterator iter = this->result.tbscertificate.serialNumber.begin(); iter != this->result.tbscertificate.serialNumber.end(); iter++) {
		cout << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)*iter << " ";
		count++;
		if (count % 32 == 0) {
			cout << endl << "\t";
		}
	}
	cout << endl;
	cout << "Signature Algorithm:" << endl << "\t" << this->result.tbscertificate.signature.algorithm << endl;
	cout << "Parameters:" << endl << "\t" << this->result.tbscertificate.signature.parameters << endl;
	cout << "Issuer Name:" << endl;
	for (vector<AttributeTypeAndValue>::iterator iter = this->result.tbscertificate.issuer.begin(); iter != this->result.tbscertificate.issuer.end(); iter++) {
		cout << "\t" << iter->type << ": " << iter->value << endl;
	}
	cout << "Validity period:" << endl << "\tNot Before: " << this->result.tbscertificate.validity.notBefore << endl << "\tNot After: " << this->result.tbscertificate.validity.notAfter << endl;
	cout << "Subject Name:" << endl;
	for (vector<AttributeTypeAndValue>::iterator iter = this->result.tbscertificate.subject.begin(); iter != this->result.tbscertificate.subject.end(); iter++) {
		cout << "\t" << iter->type << " " << iter->value << endl;
	}
	cout << "Subject Public Key Info:" << endl <<
		"\tPublic Key Algorithm:" << this->result.tbscertificate.subjectPublicKeyInfo.algorithm.algorithm << endl <<
		"\tParameters:" << this->result.tbscertificate.subjectPublicKeyInfo.algorithm.parameters << endl <<
		"\tSubject Public Key:\n" << this->result.tbscertificate.subjectPublicKeyInfo.subjectPublicKey << endl;
	if (this->result.tbscertificate.issuerUniqueID != "") {
		cout << "Issuer Unique Identifier:" << endl << this->result.tbscertificate.issuerUniqueID << endl;
	}
	if (this->result.tbscertificate.subjectUniqueID != "") {
		cout << "Subject Unique Identifier:" << endl << this->result.tbscertificate.subjectUniqueID << endl;
	}
	if (!this->result.tbscertificate.extensions.empty()) {
		cout << "Extensions:" << endl;
		for (vector<Extension>::iterator iter = this->result.tbscertificate.extensions.begin(); iter != this->result.tbscertificate.extensions.end(); iter++) {
			cout << "\tExtension ID: " << iter->extnID << endl <<
				"\tCritical: " << ((iter->critical) ? "True" : "False") << endl <<
				"\tExtension Value:\n" << iter->extnValue << endl;
		}
	}
	cout << "Certificate Signature Algorithm:" << endl << this->result.signatureAlgorithm.algorithm << endl <<
		"Parameters:" << this->result.signatureAlgorithm.parameters << endl;
	cout << "Certificate Signature:" << endl << this->result.signatureValue << endl;
}