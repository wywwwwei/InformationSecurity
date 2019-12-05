# X.509数字证书解析程序

[toc]

[Github项目地址]( https://github.com/wywwwwei/InformationSecurity/tree/master/HW3)



> 参考资料：
>
> [ASN.1 JavaScript decoder example]( https://lapo.it/asn1js/#MIAGCSqGSIb3DQEHAqCAMIACAQExCzAJBgUrDgMCGgUAMIAGCSqGSIb3DQEHAQAAoIAwggHvMIIBWKADAgECAhAvoXazbunwSfREtACZZhlFMA0GCSqGSIb3DQEBBQUAMAwxCjAIBgNVBAMMAWEwHhcNMDgxMDE1MTUwMzQxWhcNMDkxMDE1MTUwMzQxWjAMMQowCAYDVQQDDAFhMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCJUwlwhu5hR8X01f-vG0mKPRHsVRjpZNxSEmsmFPdDiD9kylE3ertTDf0gRkpIvWfNJ-eymuxoXF0Qgl5gXAVuSrjupGD6J-VapixJiwLXJHokmDihLs3zfGARz08O3qnO5ofBy0pRxq5isu_bAAcjoByZ1sI_g0iAuotC1UFObwIDAQABo1IwUDAOBgNVHQ8BAf8EBAMCBPAwHQYDVR0OBBYEFEIGXQB4h-04Z3y_n7Nv94-CqPitMB8GA1UdIwQYMBaAFEIGXQB4h-04Z3y_n7Nv94-CqPitMA0GCSqGSIb3DQEBBQUAA4GBAE0G7tAiaacJxvP3fhEj-yP9VDxL0omrRRAEaMXwWaBf_Ggk1T_u-8_CDAdjuGNCiF6ctooKc8u8KpnZJsGqnpGQ4n6L2KjTtRUDh-hija0eJRBFdirPQe2HAebQGFnmOk6Mn7KiQfBIsOzXim_bFqaBSbf06bLTQNwFouSO-jwOAAAxggElMIIBIQIBATAgMAwxCjAIBgNVBAMMAWECEC-hdrNu6fBJ9ES0AJlmGUUwCQYFKw4DAhoFAKBdMBgGCSqGSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZIhvcNAQkFMQ8XDTA4MTAxNTE1MDM0M1owIwYJKoZIhvcNAQkEMRYEFAAAAAAAAAAAAAAAAAAAAAAAAAAAMA0GCSqGSIb3DQEBAQUABIGAdB7ShyMGf5lVdZtvwKlnYLHMUqJWuBnFk7aQwHAmg3JnH6OcgId2F-xfg6twXm8hhUBkhHPlHGoWa5kQtN9n8rz3NorzvcM_1Xv9-0Eal7NYSn2Hb0C0DMj2XNIYH2C6CLIHkmy1egzUvzsomZPTkx5nGDWm-8WHCjWb9A6lyrMAAAAAAAA )
>
> [RFC 5280 Internet X.509 Public Key Infrastructure Certificate and Certificate Revocation List Profile]( https://tools.ietf.org/html/rfc5280 )
>
> [x.509 Certificates Explained —— Bjørn T. Nøstdahl Blog]( https://nostdahl.com/2017/08/11/x-509-certificates-explained/ )
>
> [X.509 Wikipedia]( https://en.wikipedia.org/wiki/X.509 )

## Introduction

> 在密码学中，X.509是定义公共密钥证书格式的标准。X.509证书用于许多Internet协议中，包括TLS / SSL，这是HTTPS（浏览网页的安全协议）的基础。它们还用于脱机应用程序，例如电子签名。 X.509证书包含公钥和身份（主机名，组织或个人），并且由证书颁发机构签名或自行签名。当证书由受信任的证书颁发机构签名或通过其他方式进行验证时，持有该证书的人可以依靠其包含的公钥与另一方建立安全通信，或验证由相应私钥进行数字签名的文档。

## Structure of a certificate

- Certificate（ 证书 ）
  - Version Number（ 版本号 ）
  - Serial Number（序列号）
  - Signature Algorithm ID（ 签名算法）
  - Issuer Name（ 颁发者 ）
  - Validity period（ 证书有效期 ）
    - Not Before
    - Not After
  - Subject name（ 主题 ）
  - Subject Public Key Info（ 主题公钥信息 ）
    - Public Key Algorithm
    - Subject Public Key
  - Issuer Unique Identifier (optional)（ 颁发者唯一身份信息 —— 可选）
  - Subject Unique Identifier (optional)（ 主题唯一身份信息  —— 可选）
  - Extensions (optional)（ 扩展信息 ）
    - ...
- Certificate Signature Algorithm（ 证书签名算法 ）
- Certificate Signature（ 数字签名 ）

> ### 证书文件扩展名
>
> -  .pem– (隐私增强型电子邮件)[DER](https://baike.baidu.com/item/DER)编码的证书再进行[Base64](https://baike.baidu.com/item/Base64)编码的数据存放在"-----BEGIN CERTIFICATE-----"和"-----END CERTIFICATE-----"之中 
> -  .cer,.crt,.der– 通常是[DER](https://baike.baidu.com/item/DER)二进制格式的，但Base64编码后也很常见。 
> - ……

## Data Structure

### ASN.1 DER

抽象语法表示法一（ASN.1）是一种标准的接口描述语言，用于定义可以以跨平台方式序列化和反序列化的数据结构。它广泛用于电信和计算机网络，尤其是在密码学中。  我们可以使用 ANS.1 描述独立于计算机硬件的对象结构 

- BER编码规则

  基本编码规则（BER）定义了一种自描述和自定界格式，用于对ASN.1数据结构进行编码。数据元素通过TLV（类型-长度-值）编码按以下顺序编码：标识符八位字节，长度八位字节，实际数据元素（内容八位字节），有时还包括内容结尾标记。它允许接收者从不完整的流中解码ASN.1数据，而与数据的内容，大小或语义含义无关。 

- DER编码规则

  可分辨编码规则（DER）用来满足x.509规范对安全数据传输的要求，并且被认为是在文件中存储X.509证书的最流行的编码格式。它们构成BER的受限变体，旨在为ASN.1所描述的数据结构生成明确的传输语法。当您需要唯一的编码时（例如在密码学中），可以使用DER。它可以确保经过数字签名的数据结构获得唯一的序列化表示形式。 

### Structure described by ANS.1 DER

#### Common type fields

```
BOOLEAN      0x01 Model logical, two-state variable values 
INTEGER      0x02 Model integer variable values 
BIT STRING   0x03 Model binary data of arbitrary length 
OCTET STRING 0x04 Model binary data whose length is a multiple of eight 
NULL         0x05 Indicate effective absence of a sequence element 
OBJECT       0x06 Name information objects 
REAL         0x09 Model real variable values 
ENUMERATED   0x0A Model values of variables with at least three states
SEQUENCE
SEQUENCE OF  0x10 Models an ordered collection of variables
SET
SET OF       0x11 Model an unordered collection of variables
PrintableString 	0x13	Describe data that consists only of a specific printable 							 subset of the ASCII character set.
T61String 			0x14	Encoding strings containing characters used in Western 								European languages.
IA5String			0x16	Represent ISO 646 (IA5) characters.
UTCTime 			0x17	The primary time standard by which the world regulates 								clocks and time.
GeneralizedTime		0x18	A string value representing the calendar date, as defined 							  in ISO 8601
```

 ASN.1编码始终按该顺序使用TLV（标签，长度，值）系统。标签和长度可以在多个字节上编码。 

 标签类型是一个字节字段，用于证书解析的最重要的值是以上所提到过的 

- 标签

  对于典型的标签（值<= 31），编码是一个字节，其中后5位指定“类型”，

  位6指定这是基本类型（例如整数或字符串）还是构造对象（例如序列或集合）

  ```
  CONSTRUCTED 0x20
  ```

  并且前两位指定“类”：

  ```
  UNIVERSAL = 0x00,
  APPLICATION = 0x40,
  CONTEXT_SPECIFIC = 0x80,
  PRIVATE = 0xC0
  ```

  举个例子：

  证书的第一个字节为0x30或0x10 |0x20，表示这是具有通用类类型的构造序列。 

- 长度

  - short form

     最高位为0，最多表示127个字节的值，低7位表示是具体的长度

  - long form

    第一个字节最高位为1，低七位表示接下来描述长度的字节数

    比如 82 04 80 则表示 0x82 & 0x80 = 0x02 个字节（04 80）用来描述长度，所以长度为 

    0x80 * 256 ^ 0 + 0x04 * 256 ^ 1 = 1152（个字节）

#### Structure

根据RFC5280中的定义

```
Certificate ::= SEQUENCE {
 tbsCertificate TBSCertificate,
 signatureAlgorithm AlgorithmIdentifier,
 signatureValue BIT STRING }
```

- TBSCertificate：

  ```
  TBSCertificate ::= SEQUENCE {
       version [0] EXPLICIT Version DEFAULT v1,
       serialNumber CertificateSerialNumber,
       signature AlgorithmIdentifier,
       issuer Name,
       validity Validity,
       subject Name,
       subjectPublicKeyInfo SubjectPublicKeyInfo,
       issuerUniqueID [1] IMPLICIT BIT STRING OPTIONAL,
       -- If present, version MUST be v2 or v3
       subjectUniqueID [2] IMPLICIT BIT STRING OPTIONAL,
       -- If present, version MUST be v2 or v3
       extensions [3] EXPLICIT Extensions OPTIONAL
       -- If present, version MUST be v3
  }
  ```

  ```
  Version ::= INTEGER { v1(0), v2(1), v3(2) }
   
  CertificateSerialNumber ::= INTEGER
  
  AlgorithmIdentifier ::= SEQUENCE {
       algorithm OBJECT IDENTIFIER,
       parameters ANY DEFINED BY algorithm OPTIONAL }
  
  Name ::= SEQUENCE OF RelativeDistinguishedName
   
  RelativeDistinguishedName ::= SET OF AttributeTypeAndValue
   
  AttributeTypeAndValue ::= SEQUENCE {
       type OBJECT IDENTIFIER,
       value ANY DEFINED BY type }
  
  Validity ::= SEQUENCE {
       notBefore Time,
       notAfter Time }
   
  Time ::= CHOICE {
       utcTime UTCTime,
       generalTime GeneralizedTime }
   
  AlgorithmIdentifier ::= SEQUENCE {
       algorithm OBJECT IDENTIFIER,
       parameters ANY DEFINED BY algorithm OPTIONAL }
   
  SubjectPublicKeyInfo ::= SEQUENCE {
       algorithm AlgorithmIdentifier,
       subjectPublicKey BIT STRING }
   
  Extensions ::= SEQUENCE OF Extension
  
  Extension ::= SEQUENCE {
       extnID OBJECT IDENTIFIER,
       critical BOOLEAN DEFAULT FALSE,
       extnValue OCTET STRING}
  ```

   通常，值类型是ASN.1字符串的某种形式。

  ```
  DirectoryString ::= CHOICE {
       teletexString TeletexString,
       printableString PrintableString,
       universalString UniversalString,
       utf8String UTF8String,
       bmpString BMPString }
  ```

- signatureAlgorithm

  ```
  AlgorithmIdentifier ::= SEQUENCE {
       algorithm OBJECT IDENTIFIER,
       parameters ANY DEFINED BY algorithm OPTIONAL }
  ```

- signatureValue

  ```
  BIT STRING
  ```

#### Parse of some special structures

- Object ID

   在ASN.1中，OID是代表分层分配方案的一系列整数值。 

  OID中的前两个值编码在前导的第一个字节中（OID始终至少包含2个组成部分）。这是通过对两个值编码进行40 * b [0] + b [1]完成的，所以这是可逆的，因为两个初始值都严格限制为小于40。 

  其余分量由可变长度编码方案编码。每个字节编码组件值的7位，高位用于指示后面是否还有其他字节。 

  例如，OID的值为  2A 86 48 86 F7 0D 01 01 0B 

   0x2A是1.2的编码，这是序列的第一部分，因为 0x2A / 40 = 1  0x2A %40 = 2

  然后 0x86 & 0x80 = 1 -> 0x48 & 0x80 = 0 所以第三部分 为

  0x48 * 128 ^ 0 + (0x86 & 0x80) * 128 ^ 1 =   840

  然后 0x86 & 0x80 = 1 -> 0xF7 & 0x80 = 1 -> 0x0D & 0x80 = 0，所以第四部分

  0x0D * 128 ^ 0 + (0xF7 & 0x80) * 128 ^ 1 + (0x86 & 0x80) * 128 ^ 2 =  113549

  因为 0x01 & 0x80 = 0 和 0x0B & 0x80 = 0，所以第五六七部分分别为 1 / 1 / 11

  所以OID 为 1.2.840.113549.1.1.11 表示 sha256WithRSAEncryption 

- Bit String

  比特串的长度可能不是8的倍数，而DER编码以字节为单位。故而，如果需要，则在比特串的最后填若干位“0”，使其长度达到8的倍数；在最前面增加1字节，写明填充的位数。特别注意：value部分的第一字节，即表示填充位数的那个字节，也要计入数据的总长度。如果不需要填充，则第一字节也需要用00来表示填充位数。 

  例如 03  03  06  B4 80 

  0x03表示Bit String 

  第二个0x03表示长度为3

  0x06表示最后一个字节补充了6个0，所以最后一个字节实际为 0x80 >> 6 = 0x02

  所以Bit string 为 0xB4 0x02 = 1011010010

## Source Code

- 数据结构声明

  ```c++
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
  ```

- 具体的解析

  说是解析证书，其实就是对ANS.1 DER语法进行类似于编译器的词法分析和语法分析，相比于正常的编译器，BIT STRING / OCTET STRING 使得解析起来更加有难度，不过我们只需要提取证书的有用的部分，很多结构（SEQUENCE / SET）可以不进行数据处理

  所以，解析证书首先得按证书的数据结构来，在不同的时候遇到同一个数据结构要进行不同的处理

  ```c++
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
  ```

  接下来，我们要按解析不同的类型（上面所提到的特别的解析方法Bit String和Octet String）来分

  - 正常数据类型的解析（这里具体操作忽略，只有大概框架）

    根据不同的证书的数据结构，进行不同的ANS.1 DER的解析

    ```c++
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
    			break;
    		case 0xa2:
    			this->count = SUBJ_UNIQUE_COUNT;
    			break;
    		case 0xa3:
    			this->count = EXTENSIONS_COUNT;
    			break;
    		case 0x01:
    			switch (this->count)
    			{
    			case EXTENSIONS_COUNT:
    				break;
    			default:
    				break;
    			}
    			break;
    		case 0x02:
    			switch (this->count)
    			{
    			case SERIAL_COUNT:
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
    				break;
    			case ISSUER_UNIQUE_COUNT:
    				break;
    			case SUBJ_UNIQUE_COUNT:
    				break;
    			case SIG_VALUE_COUNT:
    				break;
    			default:
    				this->logError();
    				break;
    			}
    			break;
    		case 0x04:
    			break;
    		case 0x06:
    			switch (this->count)
    			{
    			case SIG_COUNT:
    				break;
    			case ISSUER_COUNT:
    			case SUBJ_COUNT:
    			case EXTENSIONS_COUNT:
    				break;
    			case SUBJ_PUBLIC_KEY_COUNT:
    				break;
    			case SIG_ALGORITHM_COUNT:
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
    				break;
    			case SUBJ_COUNT:
    				break;
    			default:
    				break;
    			}
    			break;
    		case 0x17:
    			switch (this->count)
    			{
    			case VALID_COUNT:
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
    ```

  - 非正常数据类型的解析

    这里又需要通过any和len参数区分Bit String和Octet String，因为两种的处理方法不一样

    ```c++
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
    ```

    其他的更具体地对于不同类型的解析就不在报告中给出了

    最后写成的类为

    ```c++
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
    ```

## Result

   - 百度证书

     命令行openssl运行结果

     ```shell
     openssl x509 -inform DER -in baidu.cer -noout -text
     Certificate:
         Data:
             Version: 3 (0x2)
             Serial Number:
                 aa:9f:3f:e7:ff:c3:1a:87:d9:0d:cc:f8:82:76:97:06
             Signature Algorithm: sha256WithRSAEncryption
             Issuer: C = EN, CN = GlobalSign Root CA
             Validity
                 Not Before: May  9 01:22:02 2019 GMT
                 Not After : Jun 25 05:31:02 2020 GMT
             Subject: C = CN, ST = beijing, L = beijing, OU = service operation department, O = "Beijing Baidu Netcom Science Technology Co., Ltd", CN = baidu.com
             Subject Public Key Info:
                 Public Key Algorithm: rsaEncryption
                     RSA Public-Key: (1024 bit)
                     Modulus:
                         00:d3:5e:23:1c:85:ee:ac:2b:5b:d6:2e:b1:b3:f6:
                         b8:3d:be:2e:f6:4e:44:aa:56:92:1c:b8:8f:a1:82:
                         ff:f3:0a:c9:95:49:04:0a:2c:d1:04:71:cb:8e:49:
                         75:c9:9f:b9:b1:5d:b2:4b:1e:e9:ff:e9:6c:0f:70:
                         c5:89:38:6b:ab:79:03:ba:c9:84:69:c1:6e:23:3e:
                         cf:6a:13:d4:2a:5f:9d:59:4f:dd:bb:b3:7c:26:96:
                         b1:7c:56:fc:95:df:8c:02:8b:ac:df:6e:94:f6:9d:
                         fb:f4:bd:26:35:0f:ef:41:da:a8:0e:5e:08:a9:83:
                         81:3c:fd:9e:2f:be:8e:ff:ad
                     Exponent: 65537 (0x10001)
             X509v3 extensions:
                 X509v3 Subject Alternative Name:
                     DNS:baidu.com, DNS:click.hm.baidu.com, DNS:cm.pos.baidu.com, DNS:log.hm.baidu.com, DNS:update.pan.baidu.com, DNS:wn.pos.baidu.com, DNS:*.91.com, DNS:*.aipage.cn, DNS:*.aipage.com, DNS:*.apollo.auto, DNS:*.baidu.com, DNS:*.baidubce.com, DNS:*.baiducontent.com, DNS:*.baidupcs.com, DNS:*.baidustatic.com, DNS:*.baifae.com, DNS:*.baifubao.com, DNS:*.bce.baidu.com, DNS:*.bcehost.com, DNS:*.bdimg.com, DNS:*.bdstatic.com, DNS:*.bdtjrcv.com, DNS:*.bj.baidubce.com, DNS:*.chuanke.com, DNS:*.dlnel.com, DNS:*.dlnel.org, DNS:*.dueros.baidu.com, DNS:*.eyun.baidu.com, DNS:*.fanyi.baidu.com, DNS:*.gz.baidubce.com, DNS:*.hao123.baidu.com, DNS:*.hao123.com, DNS:*.hao222.com, DNS:*.im.baidu.com, DNS:*.map.baidu.com, DNS:*.mbd.baidu.com, DNS:*.mipcdn.com, DNS:*.news.baidu.com, DNS:*.nuomi.com, DNS:*.safe.baidu.com, DNS:*.smartapps.cn, DNS:*.ssl2.duapps.com, DNS:*.su.baidu.com, DNS:*.trustgo.com, DNS:*.xueshu.baidu.com, DNS:apollo.auto, DNS:baifae.com, DNS:baifubao.com, DNS:dwz.cn, DNS:mct.y.nuomi.com, DNS:www.baidu.cn, DNS:www.baidu.com.cn
                 X509v3 Basic Constraints:
                     CA:FALSE
                 X509v3 Extended Key Usage:
                     TLS Web Server Authentication, TLS Web Client Authentication
         Signature Algorithm: sha256WithRSAEncryption
              cb:85:23:25:e3:59:fa:14:58:5a:0a:34:fb:df:89:db:b6:68:
              95:62:b8:41:fc:51:bf:34:16:fd:e7:fc:8d:f6:90:89:7c:a4:
              ca:3e:44:10:d9:a8:d1:a7:8f:43:4b:c7:2c:6c:37:f3:b5:58:
              51:b4:90:33:e6:8d:00:d8:f5:5e:ca:28:a7:54:0c:b5:63:2d:
              75:95:2c:9d:1d:cb:43:0a:29:5d:8d:b7:83:98:6b:d1:01:37:
              bc:10:7d:07:93:9a:b3:0a:e0:a9:75:07:6d:d0:c6:94:18:eb:
              d5:eb:5d:0f:e1:03:16:2b:78:06:65:82:1d:be:82:27:a4:3b:
              a2:18:83:67:04:2a:09:78:07:72:4c:3e:98:07:ef:b9:fc:61:
              dc:f5:91:d5:13:76:38:08:70:b2:d8:11:19:4a:5d:35:8b:e0:
              12:ff:5e:30:1d:51:dd:0f:77:20:54:67:ad:4e:ea:f5:6d:e4:
              da:46:4e:b2:61:b1:f9:a8:73:83:41:48:90:49:14:37:6a:7e:
              df:97:59:1b:3a:d7:b3:49:ec:07:02:95:29:35:f8:15:cf:a2:
              c5:1a:5d:ef:e0:96:8e:26:e0:7e:1c:33:5b:ea:b7:88:02:1e:
              4e:0c:44:5b:6d:2b:bb:b5:92:26:aa:76:0d:98:08:98:15:60:
              59:4e:61:07
     ```

     程序运行结果

     ```shell
     ---------------Input-------------------
     E:\VSprogram\X509Decoder\Debug\baidu.cer
     ---------------Output-------------------
     Open certificate successfully
     Certificate:
     Version:
             V3
     Serial Number:
             00 aa 9f 3f e7 ff c3 1a 87 d9 0d cc f8 82 76 97 06
     Signature Algorithm:
             SHA-256 with RSA
     Parameters:
             NULL
     
     Issuer Name:
             Country Name: EN
             Common Name: GlobalSign Root CA
     Validity period:
             Not Before: 2019/05/09 01:22:02Z UTC
             Not After: 2020/06/25 05:31:02Z UTC
     Subject Name:
             Country Name CN
             State or Province Name beijing
             Locality Name beijing
             Organization Unit Name service operation department
             Organization Name Beijing Baidu Netcom Science Technology Co., Ltd
             Common Name baidu.com
     Subject Public Key Info:
             Public Key Algorithm:RSA
             Parameters:NULL
     
             Subject Public Key:
     00 d3 5e 23 1c 85 ee ac 2b 5b d6 2e b1 b3 f6 b8
     3d be 2e f6 4e 44 aa 56 92 1c b8 8f a1 82 ff f3
     0a c9 95 49 04 0a 2c d1 04 71 cb 8e 49 75 c9 9f
     b9 b1 5d b2 4b 1e e9 ff e9 6c 0f 70 c5 89 38 6b
     ab 79 03 ba c9 84 69 c1 6e 23 3e cf 6a 13 d4 2a
     5f 9d 59 4f dd bb b3 7c 26 96 b1 7c 56 fc 95 df
     8c 02 8b ac df 6e 94 f6 9d fb f4 bd 26 35 0f ef
     41 da a8 0e 5e 08 a9 83 81 3c fd 9e 2f be 8e ff
     ad
     01 00 01
     
     Extensions:
             Extension ID: Subject Alternative Name
             Critical: False
             Extension Value:
     baidu.com
     click.hm.baidu.com
     cm.pos.baidu.com
     log.hm.baidu.com
     update.pan.baidu.com
     wn.pos.baidu.com
     *.91.com
     *.aipage.cn
     *.aipage.com
     *.apollo.auto
     *.baidu.com
     *.baidubce.com
     *.baiducontent.com
     *.baidupcs.com
     *.baidustatic.com
     *.baifae.com
     *.baifubao.com
     *.bce.baidu.com
     *.bcehost.com
     *.bdimg.com
     *.bdstatic.com
     *.bdtjrcv.com
     *.bj.baidubce.com
     *.chuanke.com
     *.dlnel.com
     *.dlnel.org
     *.dueros.baidu.com
     *.eyun.baidu.com
     *.fanyi.baidu.com
     *.gz.baidubce.com
     *.hao123.baidu.com
     *.hao123.com
     *.hao222.com
     *.im.baidu.com
     *.map.baidu.com
     *.mbd.baidu.com
     *.mipcdn.com
     *.news.baidu.com
     *.nuomi.com
     *.safe.baidu.com
     *.smartapps.cn
     *.ssl2.duapps.com
     *.su.baidu.com
     *.trustgo.com
     *.xueshu.baidu.com
     apollo.auto
     baifae.com
     baifubao.com
     dwz.cn
     mct.y.nuomi.com
     www.baidu.cn
     www.baidu.com.cn
     
             Extension ID: Basic Constraints
             Critical: False
             Extension Value:
     NULL
     
             Extension ID: Extended Key Usage
             Critical: False
             Extension Value:
     Server authentication: Client authentication:
     Certificate Signature Algorithm:
     SHA-256 with RSA
     Parameters:NULL
     
     Certificate Signature:
     cb 85 23 25 e3 59 fa 14 58 5a 0a 34 fb df 89 db
     b6 68 95 62 b8 41 fc 51 bf 34 16 fd e7 fc 8d f6
     90 89 7c a4 ca 3e 44 10 d9 a8 d1 a7 8f 43 4b c7
     2c 6c 37 f3 b5 58 51 b4 90 33 e6 8d 00 d8 f5 5e
     ca 28 a7 54 0c b5 63 2d 75 95 2c 9d 1d cb 43 0a
     29 5d 8d b7 83 98 6b d1 01 37 bc 10 7d 07 93 9a
     b3 0a e0 a9 75 07 6d d0 c6 94 18 eb d5 eb 5d 0f
     e1 03 16 2b 78 06 65 82 1d be 82 27 a4 3b a2 18
     83 67 04 2a 09 78 07 72 4c 3e 98 07 ef b9 fc 61
     dc f5 91 d5 13 76 38 08 70 b2 d8 11 19 4a 5d 35
     8b e0 12 ff 5e 30 1d 51 dd 0f 77 20 54 67 ad 4e
     ea f5 6d e4 da 46 4e b2 61 b1 f9 a8 73 83 41 48
     90 49 14 37 6a 7e df 97 59 1b 3a d7 b3 49 ec 07
     02 95 29 35 f8 15 cf a2 c5 1a 5d ef e0 96 8e 26
     e0 7e 1c 33 5b ea b7 88 02 1e 4e 0c 44 5b 6d 2b
     bb b5 92 26 aa 76 0d 98 08 98 15 60 59 4e 61 07
     ```

   - 谷歌证书

     命令函openssl运行结果

     ```shell
     openssl x509 -inform DER -in google.cer -noout -text
     Certificate:
         Data:
             Version: 3 (0x2)
             Serial Number:
                 7a:0f:be:81:39:7f:aa:a4:02:00:00:00:00:4b:8d:23
             Signature Algorithm: sha256WithRSAEncryption
             Issuer: C = US, O = Google Trust Services, CN = GTS CA 1O1
             Validity
                 Not Before: Nov  5 07:50:06 2019 GMT
                 Not After : Jan 28 07:50:06 2020 GMT
             Subject: C = US, ST = California, L = Mountain View, O = Google LLC, CN = *.google.com.hk
             Subject Public Key Info:
                 Public Key Algorithm: rsaEncryption
                     RSA Public-Key: (2048 bit)
                     Modulus:
                         00:d3:fe:be:d9:6f:d7:83:47:48:f4:12:f4:5c:10:
                         76:86:c1:8b:d2:7d:fb:32:0b:f1:57:1a:8f:3a:0c:
                         a3:01:94:56:2d:5a:e6:38:31:97:68:f9:16:a4:1f:
                         21:37:b3:02:83:7e:88:f6:30:e8:ae:03:d6:78:d3:
                         00:e7:b9:72:13:05:81:bb:03:d0:fc:77:36:a5:6d:
                         6f:ca:f8:e1:02:f7:1f:4a:ae:1b:ef:ae:bf:68:99:
                         1f:59:ae:6a:6a:35:22:08:2e:ae:00:0e:62:13:fd:
                         15:2b:07:47:51:eb:3b:14:fe:ea:b2:ab:d4:a6:1d:
                         7d:3a:d7:da:d0:d6:a3:67:8c:85:e4:ea:a5:b9:2c:
                         46:b6:ee:a8:ac:86:90:79:ae:10:82:92:d7:c2:18:
                         dc:66:d2:fe:35:61:12:11:ff:07:86:4d:97:e5:d4:
                         be:5c:e3:ff:79:93:83:e2:86:f7:14:65:e1:bd:ee:
                         b7:54:ba:2e:4c:47:64:f5:4e:1f:78:d3:7e:68:4b:
                         6e:4a:41:c6:3e:fb:52:c3:eb:af:09:5f:3a:40:a3:
                         0c:73:aa:b5:33:ea:6e:04:39:de:73:9c:2b:93:a8:
                         ab:e1:4f:07:2d:69:05:59:a2:20:4e:a4:fe:48:28:
                         83:6a:9c:ba:88:d0:df:38:c6:e7:ab:ec:49:5d:11:
                         23:f9
                     Exponent: 65537 (0x10001)
             X509v3 extensions:
                 X509v3 Key Usage: critical
                     Digital Signature, Key Encipherment
                 X509v3 Extended Key Usage:
                     TLS Web Server Authentication
                 X509v3 Basic Constraints: critical
                     CA:FALSE
                 X509v3 Subject Key Identifier:
                     FB:BE:10:14:8E:F1:DF:C4:79:80:BB:FE:76:AE:64:C3:F2:AC:A4:0E
                 X509v3 Authority Key Identifier:
                     keyid:98:D1:F8:6E:10:EB:CF:9B:EC:60:9F:18:90:1B:A0:EB:7D:09:FD:2B
     
                 Authority Information Access:
                     OCSP - URI:http://ocsp.pki.goog/gts1o1
                     CA Issuers - URI:http://pki.goog/gsr2/GTS1O1.crt
     
                 X509v3 Subject Alternative Name:
                     DNS:*.google.com.hk, DNS:google.com.hk
                 X509v3 Certificate Policies:
                     Policy: 2.23.140.1.2.2
                     Policy: 1.3.6.1.4.1.11129.2.5.3
     
                 X509v3 CRL Distribution Points:
     
                     Full Name:
                       URI:http://crl.pki.goog/GTS1O1.crl
     
                 CT Precertificate SCTs:
                     Signed Certificate Timestamp:
                         Version   : v1 (0x0)
                         Log ID    : B2:1E:05:CC:8B:A2:CD:8A:20:4E:87:66:F9:2B:B9:8A:
                                     25:20:67:6B:DA:FA:70:E7:B2:49:53:2D:EF:8B:90:5E
                         Timestamp : Nov  5 08:50:07.112 2019 GMT
                         Extensions: none
                         Signature : ecdsa-with-SHA256
                                     30:45:02:20:1C:05:65:04:13:1A:19:FE:46:5F:B2:6B:
                                     64:41:B9:11:26:5A:66:73:AD:37:F7:38:B9:CF:43:43:
                                     94:AE:60:2A:02:21:00:97:61:21:CA:FB:35:7E:5F:DD:
                                     8A:54:8B:C9:6A:A8:2D:84:7A:9F:1B:67:25:FB:5A:12:
                                     16:9F:40:1E:21:31:E1
                     Signed Certificate Timestamp:
                         Version   : v1 (0x0)
                         Log ID    : 5E:A7:73:F9:DF:56:C0:E7:B5:36:48:7D:D0:49:E0:32:
                                     7A:91:9A:0C:84:A1:12:12:84:18:75:96:81:71:45:58
                         Timestamp : Nov  5 08:50:07.164 2019 GMT
                         Extensions: none
                         Signature : ecdsa-with-SHA256
                                     30:45:02:21:00:E7:5B:B3:AB:F3:85:4C:7C:91:5E:F0:
                                     1C:55:CC:E7:7C:88:46:81:7F:D7:24:4A:FC:A0:98:AF:
                                     5B:A1:45:4F:C0:02:20:2B:28:39:FD:E4:74:00:BA:92:
                                     A8:3F:6B:CE:AE:A7:B1:53:77:DE:EE:2C:86:F2:F0:80:
                                     3C:B9:5C:AE:FD:93:8A
         Signature Algorithm: sha256WithRSAEncryption
              81:e8:7f:e1:a6:7b:b3:4d:a5:78:55:52:8b:b7:90:33:51:c6:
              9a:47:89:81:36:fd:9e:69:23:d1:a3:25:8c:02:cc:93:da:f1:
              87:8a:2c:00:31:b0:a0:4e:6d:83:8c:9c:38:9d:07:8a:f3:0d:
              b0:b0:61:be:9c:08:c9:f9:00:3f:55:56:91:3a:e1:66:68:50:
              2b:60:a9:42:d4:19:cc:ea:eb:28:2b:80:6f:28:7d:d4:2e:e4:
              14:fe:6b:9e:47:ef:f4:14:54:a5:ed:39:4f:81:f1:27:57:10:
              97:07:ad:0d:a8:d1:df:b7:4a:4d:31:fb:1c:64:cc:5b:a1:5c:
              2b:dc:0a:11:6f:9a:c9:a4:01:e5:c0:cb:80:77:49:73:47:df:
              d0:c8:57:3f:a1:78:80:3f:b6:69:2c:19:27:4c:b0:d9:e5:70:
              db:d0:f0:3a:b0:ce:cc:d4:12:5a:b7:d6:e8:01:e8:db:9e:88:
              1b:de:de:43:44:04:13:e2:2f:f2:7b:0f:4f:5a:06:5e:d9:0f:
              c6:b0:4d:5d:6f:d6:3d:19:92:77:db:d5:5c:fa:c4:d1:ab:09:
              20:56:f5:c1:a1:a3:61:47:b0:af:ed:1a:ee:f5:d1:47:38:99:
              d3:a0:21:aa:60:f2:0d:d8:ef:ad:b3:34:f6:42:cc:a1:49:e4:
              1b:30:2a:bb
     ```

     程序运行结果：

     ```shell
     ---------------Input-------------------
     E:\VSprogram\X509Decoder\Debug\google.cer
     ---------------Output-------------------
     Open certificate successfully
     Certificate:
     Version:
             V3
     Serial Number:
             7a 0f be 81 39 7f aa a4 02 00 00 00 00 4b 8d 23
     Signature Algorithm:
             SHA-256 with RSA
     Parameters:
             NULL
     
     Issuer Name:
             Country Name: US
             Organization Name: Google Trust Services
             Common Name: GTS CA 1O1
     Validity period:
             Not Before: 2019/11/05 07:50:06Z UTC
             Not After: 2020/01/28 07:50:06Z UTC
     Subject Name:
             Country Name US
             State or Province Name California
             Locality Name Mountain View
             Organization Name Google LLC
             Common Name *.google.com.hk
     Subject Public Key Info:
             Public Key Algorithm:RSA
             Parameters:NULL
     
             Subject Public Key:
     00 d3 fe be d9 6f d7 83 47 48 f4 12 f4 5c 10 76
     86 c1 8b d2 7d fb 32 0b f1 57 1a 8f 3a 0c a3 01
     94 56 2d 5a e6 38 31 97 68 f9 16 a4 1f 21 37 b3
     02 83 7e 88 f6 30 e8 ae 03 d6 78 d3 00 e7 b9 72
     13 05 81 bb 03 d0 fc 77 36 a5 6d 6f ca f8 e1 02
     f7 1f 4a ae 1b ef ae bf 68 99 1f 59 ae 6a 6a 35
     22 08 2e ae 00 0e 62 13 fd 15 2b 07 47 51 eb 3b
     14 fe ea b2 ab d4 a6 1d 7d 3a d7 da d0 d6 a3 67
     8c 85 e4 ea a5 b9 2c 46 b6 ee a8 ac 86 90 79 ae
     10 82 92 d7 c2 18 dc 66 d2 fe 35 61 12 11 ff 07
     86 4d 97 e5 d4 be 5c e3 ff 79 93 83 e2 86 f7 14
     65 e1 bd ee b7 54 ba 2e 4c 47 64 f5 4e 1f 78 d3
     7e 68 4b 6e 4a 41 c6 3e fb 52 c3 eb af 09 5f 3a
     40 a3 0c 73 aa b5 33 ea 6e 04 39 de 73 9c 2b 93
     a8 ab e1 4f 07 2d 69 05 59 a2 20 4e a4 fe 48 28
     83 6a 9c ba 88 d0 df 38 c6 e7 ab ec 49 5d 11 23
     f9
     01 00 01
     
     Extensions:
             Extension ID: Key Usage
             Critical: True
             Extension Value:
     05
     
     
             Extension ID: Extended Key Usage
             Critical: False
             Extension Value:
     Server authentication:
             Extension ID: Basic Constraints
             Critical: True
             Extension Value:
     NULL
     
             Extension ID: Subject Key Identifier
             Critical: False
             Extension Value:
     fb be 10 14 8e f1 df c4 79 80 bb fe 76 ae 64 c3
     f2 ac a4 0e
     
             Extension ID: Author Key Identifier
             Critical: False
             Extension Value:
     98 d1 f8 6e 10 eb cf 9b ec 60 9f 18 90 1b a0 eb
     7d 09 fd 2b
     
             Extension ID: Authority Information Access
             Critical: False
             Extension Value:
     Online Certificate Status Protocol (OCSP): http://ocsp.pki.goog/gts1o1
     Certificate authority issuers: http://pki.goog/gsr2/GTS1O1.crt
     
             Extension ID: Subject Alternative Name
             Critical: False
             Extension Value:
     *.google.com.hk
     google.com.hk
     
             Extension ID: Certificate Policies
             Critical: False
             Extension Value:
     2.23.140.1.2.2: 1.3.6.1.4.1.11129.2.5.3:
             Extension ID: CRL Distribution Points
             Critical: False
             Extension Value:
     http://crl.pki.goog/GTS1O1.crl
     
             Extension ID: Precert Signed Certificate Timestamps
             Critical: False
             Extension Value:
     00 f0 00 76 00 b2 1e 05 cc 8b a2 cd 8a 20 4e 87
     66 f9 2b b9 8a 25 20 67 6b da fa 70 e7 b2 49 53
     2d ef 8b 90 5e 00 00 01 6e 3a c1 c2 88 00 00 04
     03 00 47 30 45 02 20 1c 05 65 04 13 1a 19 fe 46
     5f b2 6b 64 41 b9 11 26 5a 66 73 ad 37 f7 38 b9
     cf 43 43 94 ae 60 2a 02 21 00 97 61 21 ca fb 35
     7e 5f dd 8a 54 8b c9 6a a8 2d 84 7a 9f 1b 67 25
     fb 5a 12 16 9f 40 1e 21 31 e1 00 76 00 5e a7 73
     f9 df 56 c0 e7 b5 36 48 7d d0 49 e0 32 7a 91 9a
     0c 84 a1 12 12 84 18 75 96 81 71 45 58 00 00 01
     6e 3a c1 c2 bc 00 00 04 03 00 47 30 45 02 21 00
     e7 5b b3 ab f3 85 4c 7c 91 5e f0 1c 55 cc e7 7c
     88 46 81 7f d7 24 4a fc a0 98 af 5b a1 45 4f c0
     02 20 2b 28 39 fd e4 74 00 ba 92 a8 3f 6b ce ae
     a7 b1 53 77 de ee 2c 86 f2 f0 80 3c b9 5c ae fd
     93 8a
     
     Certificate Signature Algorithm:
     SHA-256 with RSA
     Parameters:NULL
     
     Certificate Signature:
     81 e8 7f e1 a6 7b b3 4d a5 78 55 52 8b b7 90 33
     51 c6 9a 47 89 81 36 fd 9e 69 23 d1 a3 25 8c 02
     cc 93 da f1 87 8a 2c 00 31 b0 a0 4e 6d 83 8c 9c
     38 9d 07 8a f3 0d b0 b0 61 be 9c 08 c9 f9 00 3f
     55 56 91 3a e1 66 68 50 2b 60 a9 42 d4 19 cc ea
     eb 28 2b 80 6f 28 7d d4 2e e4 14 fe 6b 9e 47 ef
     f4 14 54 a5 ed 39 4f 81 f1 27 57 10 97 07 ad 0d
     a8 d1 df b7 4a 4d 31 fb 1c 64 cc 5b a1 5c 2b dc
     0a 11 6f 9a c9 a4 01 e5 c0 cb 80 77 49 73 47 df
     d0 c8 57 3f a1 78 80 3f b6 69 2c 19 27 4c b0 d9
     e5 70 db d0 f0 3a b0 ce cc d4 12 5a b7 d6 e8 01
     e8 db 9e 88 1b de de 43 44 04 13 e2 2f f2 7b 0f
     4f 5a 06 5e d9 0f c6 b0 4d 5d 6f d6 3d 19 92 77
     db d5 5c fa c4 d1 ab 09 20 56 f5 c1 a1 a3 61 47
     b0 af ed 1a ee f5 d1 47 38 99 d3 a0 21 aa 60 f2
     0d d8 ef ad b3 34 f6 42 cc a1 49 e4 1b 30 2a bb
     ```

     