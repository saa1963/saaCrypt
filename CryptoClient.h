// CryptoClient.h: interface for the CCryptoClient class.
//
//////////////////////////////////////////////////////////////////////

//#define _WIN32_WINNT 0x0400

#include <wincrypt.h>
#include "crypt32.h"
#include "WinCryptEx.h"
#include <vector>

using namespace std;


enum ECryptoClientErrors{
	CC_NOERRORS = 0,
	CC_NOACCESS_CONTEXT = 1,
	CC_NODELETE_KEYS = 2,
	CC_EXIST_KEYS = 3,
	CC_NOCREATE_KEYS = 4,
	CC_NOEXIST_KEYS = 5,
	CC_UNKNOWN_ERROR = 6,
	CC_INVALID_PARAMETER = 7,
	CC_BAD_SIGN = 8,
	CC_ERROR = 9
};

enum ECryptoKeyTypes{
	KT_EXCHANGE_KEY,
	KT_SIGN_KEY,
	KT_SYMMETRIC_KEY
};

void ToHex(BYTE *dest, BYTE *src, DWORD ln);
void FromHex(BYTE *dest, BYTE *src, DWORD ln);
void MyCodeError(char *er, DWORD nError);
char *Number2String(DWORD n);
bool ToFile(char *fname, BYTE *buf, DWORD lenbuf);
bool FromFile(char *fname, BYTE *buf, DWORD *lenbuf);
bool FileExist(char *fname);

class CCryptoClient  
{
public:
	bool SaveRegister(char *fname);
	ECryptoClientErrors CCryptoClient::CipherMessage(BYTE *pbMsg, DWORD *pdwLenMsg, BYTE *pbPKey, DWORD dwLenPKey, BYTE **pbMsg1, DWORD *pdwLenMsg1);
	ECryptoClientErrors CCryptoClient::CipherMessageBin(BYTE *pbMsg, DWORD dwLenMsg, BYTE *pbPKey, DWORD dwLenPKey, BYTE **pbMsg1, DWORD *pdwLenMsg1);
	ECryptoClientErrors CCryptoClient::DeCipherMessage(BYTE *pbMsg, DWORD *pdwLenMsg, BYTE *pbPKey, DWORD dwLenPKey, BYTE **pbMsg1, DWORD *pdwLenMsg1);
	ECryptoClientErrors CCryptoClient::DeCipherMessageBin(BYTE *pbMsg, DWORD dwLenMsg, BYTE *pbPKey, DWORD dwLenPKey, BYTE **pbMsg1, DWORD *pdwLenMsg1);
	ECryptoClientErrors ExportExchangePublicKey(BYTE *pbData, DWORD *pdwDataLen);
	ECryptoClientErrors CreateNewSessionKey(HCRYPTKEY *phUserKey);
	ECryptoClientErrors VerifyMessage(BYTE *msg, DWORD dwLenMsg, BYTE *sign, DWORD dwSignLen, BYTE *key, DWORD dwKeyLen);
	ECryptoClientErrors SignMessage(BYTE *msg, DWORD dwLenMsg, BYTE *sign, DWORD *pdwSignLen);
	ECryptoClientErrors ExportSignPublicKey(BYTE *pbData, DWORD *pdwDataLen);
	ECryptoClientErrors CreateNewKeys(ECryptoKeyTypes kt);
	ECryptoClientErrors nError;
	// ����������� ��� ������������� ��������� 2001
	CCryptoClient(LPCTSTR pszContainer, LPCTSTR pszProvider);
	CCryptoClient(LPCTSTR pszContainer, LPCTSTR pszProvider, BOOL flag);
	virtual ~CCryptoClient();
protected:
	ECryptoClientErrors ImportPublicKey(BYTE *pbData, DWORD dwDataLen, HCRYPTKEY *phUserKey);
	ECryptoClientErrors ImportPublicKeyBin(BYTE *pbData, DWORD dwDataLen, HCRYPTKEY *phUserKey);
	ECryptoClientErrors ImportPublicKeyOnExchangeKey(BYTE *pbData, DWORD dwDataLen, HCRYPTKEY *phUserKey);
	ECryptoClientErrors ImportPublicKeyOnExchangeKeyBin(BYTE *pbData, DWORD dwDataLen, HCRYPTKEY *phUserKey);
	ECryptoClientErrors GetHandleCSP(LPCTSTR pszContainer, LPCTSTR pszProvider);
	ECryptoClientErrors GetHandleCSP0(LPCTSTR pszContainer, LPCTSTR pszProvider);
	ECryptoClientErrors GetHandleCSP1(LPCTSTR pszContainer, LPCTSTR pszProvider);
	ECryptoClientErrors GetHandleCSP2(LPCTSTR pszContainer);
	HCRYPTPROV phProv;
	char *container;
private:
	ECryptoClientErrors _ExportPublicKey(BYTE *pbData, DWORD *pdwDataLen, DWORD dwKeySpec);
public:
	CCryptoClient(LPCTSTR pszContainer, LPCTSTR pszProvider, bool Is2001);
	CCryptoClient(LPCTSTR pszContainer, int vid);
	ECryptoClientErrors EnumContainers(vector<string>& containers);
	ECryptoClientErrors SignMessageBin(BYTE *msg, DWORD dwLenMsg, BYTE *sign, DWORD *pdwSignLen);
	ECryptoClientErrors VerifyMessageBin(BYTE *msg, DWORD dwLenMsg, BYTE *sign, DWORD dwSignLen, BYTE *key, DWORD dwKeyLen);
};
