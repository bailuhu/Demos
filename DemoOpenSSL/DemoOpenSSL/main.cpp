
#include "openssl/opensslconf.h"
#include "openssl/e_os2.h"
#include "openssl/md5.h"    
#include "openssl/sha.h"    
#include "openssl/des.h"    
#include "openssl/rsa.h"    
#include "openssl/pem.h"



#include <string>

std::string EncodeRSAKeyFile(const std::string& strPemFileName, const std::string& strData)
{
	if (strPemFileName.empty() || strData.empty())
	{
		assert(false);
		return "";
	}
	FILE* hPubKeyFile = NULL;
	if (fopen_s(&hPubKeyFile, strPemFileName.c_str(), "rb") || hPubKeyFile == NULL)
	{
		assert(false);
		return "";
	}
	std::string strRet;
	RSA* pRSAPublicKey = RSA_new();
	if (PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
	{
		assert(false);
		return "";
	}

	int nLen = RSA_size(pRSAPublicKey);
	char* pEncode = new char[nLen + 1];
	int ret = RSA_public_encrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pEncode, pRSAPublicKey, RSA_PKCS1_PADDING);
	if (ret >= 0)
	{
		strRet = std::string(pEncode, ret);
	}
	delete[] pEncode;
	RSA_free(pRSAPublicKey);
	fclose(hPubKeyFile);
	CRYPTO_cleanup_all_ex_data();
	return strRet;
}

std::string DecodeRSAKeyFile(const std::string& strPemFileName, const std::string& strData)
{
	if (strPemFileName.empty() || strData.empty())
	{
		assert(false);
		return "";
	}
	FILE* hPriKeyFile = NULL;
	if (fopen_s(&hPriKeyFile, strPemFileName.c_str(), "rb") || hPriKeyFile == NULL)
	{
		assert(false);
		return "";
	}
	std::string strRet;
	RSA* pRSAPriKey = RSA_new();
	if (PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
	{
		assert(false);
		return "";
	}
	int nLen = RSA_size(pRSAPriKey);
	char* pDecode = new char[nLen + 1];

	int ret = RSA_private_decrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING);
	if (ret >= 0)
	{
		strRet = std::string((char*)pDecode, ret);
	}
	delete[] pDecode;
	RSA_free(pRSAPriKey);
	fclose(hPriKeyFile);
	CRYPTO_cleanup_all_ex_data();
	return strRet;
}

int main()
{

	return 0;
}