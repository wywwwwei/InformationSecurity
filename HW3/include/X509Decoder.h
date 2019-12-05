#pragma once
#include<string>
#include<vector>
#include<map>

using std::string;
using std::vector;
using std::map;

typedef unsigned char Byte;
typedef enum { V1, V2, V3 } Version;

typedef struct {
	string algorithm;
	string parameters;
}AlgorithmIdentifier;

typedef struct {
	string notBefore;
	string notAfter;
}Validity;

typedef struct {
	AlgorithmIdentifier algorithm;
	string subjectPublicKey;
}SubjectPublicKeyInfo;

typedef struct Extension{
	string extnID;
	bool critical;
	string extnValue;
	Extension(string _extnID, bool _critical, string _extnValue) {
		this->extnID = _extnID;
		this->critical = _critical;
		this->extnValue = _extnValue;
	}
}Extension;

typedef struct AttributeTypeAndValue {
	string type;
	string value;
	AttributeTypeAndValue(string _type,string _value){
		this->type = _type;
		this->value = _value;
	}
}AttributeTypeAndValue;

typedef struct {
	Version version;
	vector<Byte> serialNumber;
	AlgorithmIdentifier signature;
	vector<AttributeTypeAndValue> issuer;
	Validity validity;
	vector<AttributeTypeAndValue> subject;
	SubjectPublicKeyInfo subjectPublicKeyInfo;
	string issuerUniqueID;
	string subjectUniqueID;
	vector<Extension> extensions;
}TBSCertificate;

typedef struct {
	TBSCertificate tbscertificate;
	AlgorithmIdentifier signatureAlgorithm;
	string signatureValue;
}Certificate;

class X509Decoder {
public:
	static map<string, string> maps;
	explicit X509Decoder(Byte *input,unsigned int _len);
	void print();
private:
	void decode();
	unsigned int getLen();
	void decodeVersion();
	unsigned int decodeInt();
	vector<Byte> decodeInts(unsigned int len);
	string decodeOID();
	string decodePrintableStr();
	string decodeTime();
	string decodeBitstring(bool any = false, unsigned int _len = 0);
	void logError();

	Certificate result;
	unsigned int index;
	Byte *input;
	unsigned int count;
	unsigned int fileLen;
};