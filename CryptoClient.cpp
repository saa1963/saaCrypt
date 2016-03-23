// CryptoClient.cpp: implementation of the CCryptoClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "WinCryptEx.h"
#include "CryptoClient.h"
#include <vector>

using namespace std;

//#define _CRT_SECURE_NO_WARNING
#pragma warning( disable : 4996 )

CHAR *hex_table[] = {	
	"00","01","02","03","04","05","06","07","08","09","0A","0B","0C","0D","0E","0F",
	"10","11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1E","1F",
	"20","21","22","23","24","25","26","27","28","29","2A","2B","2C","2D","2E","2F",
	"30","31","32","33","34","35","36","37","38","39","3A","3B","3C","3D","3E","3F",
	"40","41","42","43","44","45","46","47","48","49","4A","4B","4C","4D","4E","4F",
	"50","51","52","53","54","55","56","57","58","59","5A","5B","5C","5D","5E","5F",
	"60","61","62","63","64","65","66","67","68","69","6A","6B","6C","6D","6E","6F",
	"70","71","72","73","74","75","76","77","78","79","7A","7B","7C","7D","7E","7F",
	"80","81","82","83","84","85","86","87","88","89","8A","8B","8C","8D","8E","8F",
	"90","91","92","93","94","95","96","97","98","99","9A","9B","9C","9D","9E","9F",
	"A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","AA","AB","AC","AD","AE","AF",
	"B0","B1","B2","B3","B4","B5","B6","B7","B8","B9","BA","BB","BC","BD","BE","BF",
	"C0","C1","C2","C3","C4","C5","C6","C7","C8","C9","CA","CB","CC","CD","CE","CF",
	"D0","D1","D2","D3","D4","D5","D6","D7","D8","D9","DA","DB","DC","DD","DE","DF",
	"E0","E1","E2","E3","E4","E5","E6","E7","E8","E9","EA","EB","EC","ED","EE","EF",
	"F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","FA","FB","FC","FD","FE","FF"
};

const DWORD ProviderType = 71;	// ГОСТ Р 34.10-94 
const DWORD ProviderType2001 = PROV_GOST_2001_DH;	// ГОСТ Р 34.10-2001 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCryptoClient::CCryptoClient(LPCWSTR pszContainer, LPCWSTR pszProvider)
{
	if (pszContainer != NULL){
		container = new wchar_t[wcslen(pszContainer) + 1];
		wcscpy(container, pszContainer);
	}
	else
		container = NULL;
	DWORD prtype;
	if (wcsstr(pszProvider, L"2001"))
	{
		prtype = ProviderType2001;
	}
	else
	{
		prtype = ProviderType;
	}
	nError = GetHandleCSP(pszContainer, pszProvider);
}

CCryptoClient::CCryptoClient(LPCWSTR pszContainer, LPCWSTR pszProvider, BOOL flag)
{
	if (pszContainer != NULL){
		container = new wchar_t[wcslen(pszContainer) + 1];
		wcscpy(container, pszContainer);
	}
	else
		container = NULL;
	DWORD prtype;
	if (wcsstr(pszProvider, L"2001"))
	{
		prtype = ProviderType2001;
	}
	else
	{
		prtype = ProviderType;
	}
	nError = GetHandleCSP0(pszContainer, pszProvider);
}

CCryptoClient::~CCryptoClient()
{
	if (container != NULL)
		delete container;
	CryptReleaseContext(phProv, 0);
}

ECryptoClientErrors CCryptoClient::GetHandleCSP(LPCWSTR pszContainer, LPCWSTR pszProvider)
{
	char er[2000];
	int nError;
	DWORD prtype;
	if (wcsstr(pszProvider, L"2001"))
	{
		prtype = ProviderType2001;
	}
	else
	{
		prtype = ProviderType;
	}
	// Если не задано имя контейнера, то приложение не будет работать с закрытыми
	// ключами и dwFlags должен быть установлен в CRYPT_VERIFYCONTEXT
	// Применяется для проверки подписи
	if (pszContainer == NULL){
		if (CryptAcquireContextW(
			&phProv,
			NULL,
			NULL,
			prtype,
			CRYPT_VERIFYCONTEXT
			))
			return CC_NOERRORS;
		else
			return CC_NOACCESS_CONTEXT;
	}


	if (CryptAcquireContextW(
			&phProv,
			pszContainer,
			NULL,
			prtype,
			0
			))
		return CC_NOERRORS;
	else
	{
		if (CryptAcquireContextW(
		&phProv,
		pszContainer,
		NULL,
		prtype,
		CRYPT_NEWKEYSET
		))
			return CC_NOERRORS;
		else
		{
			return CC_NOACCESS_CONTEXT;
		}
	}
	return CC_NOACCESS_CONTEXT;
}

ECryptoClientErrors CCryptoClient::GetHandleCSP0(LPCWSTR pszContainer, LPCWSTR pszProvider)
{
	char er[2000];
	DWORD prtype;
	if (wcsstr(pszProvider, L"2001"))
	{
		prtype = ProviderType2001;
	}
	else
	{
		prtype = ProviderType;
	}
	if (CryptAcquireContextW(
		&phProv,
		pszContainer,
		NULL,
		prtype,
		0
		))
		return CC_NOERRORS;
	else
	{
		MyCodeError(er, GetLastError());
		MessageBox(NULL, er, "", MB_OK);
		return CC_NOACCESS_CONTEXT;
	}
}

ECryptoClientErrors CCryptoClient::CreateNewKeys(ECryptoKeyTypes kt)
{

	HCRYPTKEY phUserKey;
	DWORD dwKeySpec;
	switch (kt){
	case KT_EXCHANGE_KEY:
		dwKeySpec = AT_KEYEXCHANGE;
		break;
	case KT_SIGN_KEY:
		dwKeySpec = AT_SIGNATURE;
		break;
	case KT_SYMMETRIC_KEY:
		dwKeySpec = AT_UECSYMMETRICKEY;
		break;
	default:
		break;
	}

	if (CryptGenKey(
		phProv, 
		dwKeySpec, 
		0, 
		&phUserKey))
		return CC_NOERRORS;
	else
		return CC_NOCREATE_KEYS;
}

ECryptoClientErrors CCryptoClient::ExportSignPublicKey(BYTE *pbData, DWORD *pdwDataLen)
{
	return _ExportPublicKey(pbData, pdwDataLen, AT_SIGNATURE);
}

ECryptoClientErrors CCryptoClient::ExportExchangePublicKey(BYTE *pbData, DWORD *pdwDataLen)
{
	return _ExportPublicKey(pbData, pdwDataLen, AT_KEYEXCHANGE);
}

ECryptoClientErrors CCryptoClient::ImportPublicKey(BYTE *pbData, DWORD dwDataLen, HCRYPTKEY *phUserKey)
{
	DWORD dwRealLen = dwDataLen / 2;
	BYTE *pbTemp;
	char cTemp[5] = {'\0', '\0', '\0', '\0', '\0'};

	if ((dwRealLen * 2) != dwDataLen)
		return CC_INVALID_PARAMETER;
	pbTemp = new BYTE[dwRealLen];

	FromHex(pbTemp, pbData, dwRealLen);

	// OK! в pbTemp находится BLOB
	if (!CryptImportKey(phProv, pbTemp, dwRealLen, 0, 0, phUserKey)){
		delete pbTemp;
		return CC_UNKNOWN_ERROR;
	}
	delete pbTemp;
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::ImportPublicKeyBin(BYTE *pbData, DWORD dwDataLen, HCRYPTKEY *phUserKey)
{
	if (!CryptImportKey(phProv, pbData, dwDataLen, 0, 0, phUserKey)){
		return CC_UNKNOWN_ERROR;
	}
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::SignMessage(BYTE *msg, DWORD dwLenMsg, BYTE *sign, DWORD *pdwSignLen)
{
	char er[4000];
	DWORD nError;
	HCRYPTHASH hHash;
	BYTE *pbTemp;
	DWORD dwTemp;

	// CALG_GR3411 Идентификатор алгоритма хеширования по ГОСТ Р 34.11-94. 
	if (!CryptCreateHash(phProv, CALG_GR3411, 0, 0, &hHash))
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptCreateHash Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	if (!CryptHashData(hHash, msg, dwLenMsg, 0))
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptHashData Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	if (!CryptSignHash(hHash, AT_KEYEXCHANGE, NULL, 0, NULL, &dwTemp))
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptSignHash Error 1", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	if (!sign){
		if (!CryptDestroyHash(hHash))
		{
			nError = GetLastError();
			MyCodeError(er, nError);
			MessageBox(NULL, "CryptDestroyHash Error", "", MB_OK);
			MessageBox(NULL, er, "", MB_OK);
			return CC_UNKNOWN_ERROR;
		}
		*pdwSignLen = dwTemp * 2;
		return CC_NOERRORS;
	}
	pbTemp = new BYTE[dwTemp];
	if (!CryptSignHash(hHash, AT_KEYEXCHANGE, NULL, 0, pbTemp, &dwTemp)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptSignHash Error 2", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		delete pbTemp;
		return CC_UNKNOWN_ERROR;
	}
	if (!CryptDestroyHash(hHash)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptDestroyHash Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		delete pbTemp;
		return CC_UNKNOWN_ERROR;
	}

	ToHex(sign, pbTemp, dwTemp);

	delete pbTemp;
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::SignMessageBin(BYTE *msg, DWORD dwLenMsg, BYTE *sign, DWORD *pdwSignLen)
{
	HCRYPTHASH hHash;
	BYTE *pbTemp;
	DWORD dwTemp;
	char er[4000];
	DWORD nError;

	// CALG_GR3411 Идентификатор алгоритма хеширования по ГОСТ Р 34.11-94. 
	if (!CryptCreateHash(phProv, CALG_GR3411, 0, 0, &hHash))
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptCreateHash Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	if (!CryptHashData(hHash, msg, dwLenMsg, 0))
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptHashData Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	if (!CryptSignHash(hHash, AT_KEYEXCHANGE, NULL, 0, NULL, &dwTemp))
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptSignHash Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	if (!sign){
		if (!CryptDestroyHash(hHash))
		{
			nError = GetLastError();
			MyCodeError(er, nError);
			MessageBox(NULL, "CryptDestroyHash Error", "", MB_OK);
			MessageBox(NULL, er, "", MB_OK);
			return CC_UNKNOWN_ERROR;
		}
		*pdwSignLen = dwTemp;
		return CC_NOERRORS;
	}
	pbTemp = new BYTE[dwTemp];
	if (!CryptSignHash(hHash, AT_KEYEXCHANGE, NULL, 0, pbTemp, &dwTemp)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptSignHash Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		delete pbTemp;
		return CC_UNKNOWN_ERROR;
	}
	if (!CryptDestroyHash(hHash)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptDestroyHash Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		delete pbTemp;
		return CC_UNKNOWN_ERROR;
	}

	//ToHex(sign, pbTemp, dwTemp);
	memcpy(sign, pbTemp, dwTemp);

	delete pbTemp;
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::VerifyMessage(BYTE *msg, DWORD dwLenMsg, BYTE *sign, 
	DWORD dwSignLen, BYTE *key, DWORD dwKeyLen)
{
	HCRYPTKEY hUserKey;
	HCRYPTHASH hHash;
	DWORD nError;
	DWORD dwRealSignLen = dwSignLen / 2;
	BYTE *pbTemp;
	char er[500];
	
	if ((dwRealSignLen * 2) != dwSignLen)
		return CC_INVALID_PARAMETER;
	pbTemp = new BYTE[dwRealSignLen];

	FromHex(pbTemp, sign, dwRealSignLen);
	
	__try{
		if (ImportPublicKey(key, dwKeyLen, &hUserKey) != CC_NOERRORS) 
			return CC_UNKNOWN_ERROR;
		// CALG_GR3411 Идентификатор алгоритма хеширования по ГОСТ Р 34.11-94. 
		if (!CryptCreateHash(phProv, CALG_GR3411, 0, 0, &hHash))
			return CC_UNKNOWN_ERROR;
		if (!CryptHashData(hHash, msg, dwLenMsg, 0))
			return CC_UNKNOWN_ERROR;
		if (!CryptVerifySignature(hHash, pbTemp, dwRealSignLen, hUserKey, NULL, 0)){
			nError = GetLastError();
			if (nError == NTE_BAD_SIGNATURE)
				return CC_BAD_SIGN;
			else
			{
				nError = GetLastError();
				MyCodeError(er, nError);
				MessageBox(NULL, "CryptVerifySignature Error", "", MB_OK);
				MessageBox(NULL, er, "", MB_OK);
				return CC_UNKNOWN_ERROR;
			}
		}
	}
	__finally{
		delete pbTemp;
		CryptDestroyHash(hHash);
		CryptDestroyKey(hUserKey);
	}
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::VerifyMessageBin(BYTE *msg, DWORD dwLenMsg, BYTE *sign, 
	DWORD dwSignLen, BYTE *key, DWORD dwKeyLen)
{
	HCRYPTKEY hUserKey;
	HCRYPTHASH hHash;
	DWORD nError;
	char er[500];
	
	__try{
		if (ImportPublicKeyBin(key, dwKeyLen, &hUserKey) != CC_NOERRORS) 
		{
			nError = GetLastError();
			MyCodeError(er, nError);
			MessageBox(NULL, "ImportPublicKey Error", "", MB_OK);
			MessageBox(NULL, er, "", MB_OK);
			return CC_UNKNOWN_ERROR;
		}
		// CALG_GR3411 Идентификатор алгоритма хеширования по ГОСТ Р 34.11-94. 
		if (!CryptCreateHash(phProv, CALG_GR3411, 0, 0, &hHash))
		{
			nError = GetLastError();
			MyCodeError(er, nError);
			MessageBox(NULL, "CryptCreateHash Error", "", MB_OK);
			MessageBox(NULL, er, "", MB_OK);
			return CC_UNKNOWN_ERROR;
		}
		if (!CryptHashData(hHash, msg, dwLenMsg, 0))
		{
			nError = GetLastError();
			MyCodeError(er, nError);
			MessageBox(NULL, "CryptHashData Error", "", MB_OK);
			MessageBox(NULL, er, "", MB_OK);
			return CC_UNKNOWN_ERROR;
		}
		if (!CryptVerifySignature(hHash, sign, dwSignLen, hUserKey, NULL, 0)){
			nError = GetLastError();
			if (nError == NTE_BAD_SIGNATURE)
				return CC_BAD_SIGN;
			else
			{
				nError = GetLastError();
				MyCodeError(er, nError);
				MessageBox(NULL, "CryptVerifySignature Error", "", MB_OK);
				MessageBox(NULL, er, "", MB_OK);
				return CC_UNKNOWN_ERROR;
			}
		}
	}
	__finally{
		CryptDestroyHash(hHash);
		CryptDestroyKey(hUserKey);
	}
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::CreateNewSessionKey(HCRYPTKEY *phUserKey)
{
	// CALG_G28147 Идентификатор алгоритма шифрования по ГОСТ 28147 89. 
	if (!CryptGenKey(phProv, CALG_G28147, CRYPT_EXPORTABLE, phUserKey))
		return CC_NOCREATE_KEYS;
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::_ExportPublicKey(BYTE *pbData, DWORD *pdwDataLen, DWORD dwKeySpec)
{
	HCRYPTKEY phUserKey;
	BYTE *pbTemp;
	DWORD dwTemp;
	// получаем дескриптор ключа
	if (!CryptGetUserKey(phProv, dwKeySpec, &phUserKey))
		return CC_NOEXIST_KEYS;
	// определяем размер буфера
	if (!CryptExportKey(phUserKey, 0, PUBLICKEYBLOB, 0, NULL, &dwTemp))
		return CC_UNKNOWN_ERROR;
	// размер буфера определен
	if (!pbData){
		*pdwDataLen = dwTemp * 4;
		return CC_NOERRORS;
	}
	pbTemp = new BYTE[dwTemp];	
	if (!CryptExportKey(phUserKey, 0, PUBLICKEYBLOB, 0, pbTemp, &dwTemp))
		return CC_UNKNOWN_ERROR;

	ToHex1(pbData, pbTemp, dwTemp);

	delete pbTemp;
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::CipherMessage(BYTE *pbMsg, DWORD *pdwLenMsg, BYTE *pbPKey, DWORD dwLenPKey, BYTE **pbMsg1, DWORD *pdwLenMsg1)
{
	DWORD nError;
	HCRYPTKEY hUserKey;
	HCRYPTKEY hSessionKey;
	// буфер для хранения блоба сессионного ключа
	BYTE *_pbData = NULL;
	DWORD _dwDataLen = 0;
	BYTE *pbData = NULL;
	DWORD dwDataLen = 0;
	DWORD dwOldLenMsg;
	char er[2000];

	
	if (CreateNewSessionKey(&hSessionKey) != CC_NOERRORS)
		return CC_UNKNOWN_ERROR;

	// получаем размер вектора инициализации ключа KP_IV
	DWORD dwIV;
	if (!CryptGetKeyParam(hSessionKey, KP_IV, NULL, &dwIV, 0))
		return CC_UNKNOWN_ERROR;
	DWORD _dwIV = dwIV * 2;

	// получаем вектор инициализации ключа KP_IV
	BYTE *_pbIV = new BYTE[dwIV];
	if (!CryptGetKeyParam(hSessionKey, KP_IV, _pbIV, &dwIV, 0))
		return CC_UNKNOWN_ERROR;
	BYTE *pbIV = new BYTE[dwIV * 2];
	ToHex(pbIV, _pbIV, dwIV);
	delete _pbIV;

	if (ImportPublicKeyOnExchangeKey(pbPKey, dwLenPKey, &hUserKey) != CC_NOERRORS)
		return CC_UNKNOWN_ERROR;
	
	if (!CryptExportKey(hSessionKey, hUserKey, SIMPLEBLOB, 0, NULL, &_dwDataLen)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}

	
	dwDataLen = _dwDataLen * 2;
	_pbData = new BYTE[_dwDataLen];
	pbData = new BYTE[dwDataLen];
	
	if (!CryptExportKey(hSessionKey, hUserKey, SIMPLEBLOB, 0, _pbData, &_dwDataLen)){
		delete _pbData;
		delete pbData;
		return CC_UNKNOWN_ERROR;
	}

	
	
	ToHex(pbData, _pbData, _dwDataLen);
	
	delete _pbData;
	// Ура! в pbData экспортированный сессионный ключ
	// определяем длину буфера для зашифрованных данных
	dwOldLenMsg = *pdwLenMsg;
	if (!CryptEncrypt(hSessionKey, 0, TRUE, 0, NULL, pdwLenMsg, *pdwLenMsg)){
		delete pbData;
		return CC_UNKNOWN_ERROR;
	}
	// Ура! в *pdwLenMsg количество байт в зашифрованном сообщении

	// 8 байт размер сессионного ключа + 8 байт размер IV + KP_IV + сессионный ключ + шифрованное сообщение
	*pdwLenMsg1 = dwIV * 2 + 8 + *pdwLenMsg * 2 + dwDataLen + 8 + 1;
	BYTE *LenKey = new BYTE[8];
	ToHex(LenKey, (BYTE*)&dwDataLen, 4);
	BYTE *LenIV = new BYTE[8];
	ToHex(LenIV, (BYTE*)&_dwIV, 4);
	*pbMsg1 = new BYTE[*pdwLenMsg1];
	// длина ключа
	memcpy((void*)*pbMsg1, (void*)LenKey, 8);
	// сам ключ
	memcpy((void*)(*pbMsg1 + 8), (void*)pbData, dwDataLen);
	// длина IV
	memcpy((void*)(*pbMsg1 + 8 + dwDataLen), (void*)LenIV, 8);
	// сам IV
	memcpy((void*)(*pbMsg1 + 8 + dwDataLen + 8), (void*)pbIV, dwIV * 2);
	//BYTE *tmp = new BYTE[*pdwLenMsg * 2];    // ????? зачем такой большой буфер
	DWORD dwRealLen;
	if (*pdwLenMsg >= dwOldLenMsg)
		dwRealLen = *pdwLenMsg;
	else
		dwRealLen = dwOldLenMsg;
	BYTE *tmp = new BYTE[dwRealLen];
	memcpy((void*)tmp, (void*)pbMsg, dwOldLenMsg);
	delete LenKey;
	delete pbData;
	delete LenIV;
	delete pbIV;
	
	//
	if (!CryptEncrypt(hSessionKey, 0, TRUE, 0, tmp, &dwOldLenMsg, dwRealLen)){
		delete tmp;
		delete pbMsg1;
		return CC_UNKNOWN_ERROR;
	}

	*pdwLenMsg = dwOldLenMsg;

	ToHex((BYTE*)(*pbMsg1 + 8 + dwDataLen + 8 + dwIV * 2), tmp, *pdwLenMsg);
	delete tmp;
	(*pbMsg1)[*pdwLenMsg1 - 1] = 0;

	return CC_NOERRORS;
}

//////////////////////////////////////////////////////
//
//    DeCipherMessage
//
//////////////////////////////////////////////////////
ECryptoClientErrors CCryptoClient::DeCipherMessage(BYTE *pbMsg, DWORD *pdwLenMsg, BYTE *pbPKey, DWORD dwLenPKey, BYTE **pbMsg1, DWORD *pdwLenMsg1)
{
	DWORD nError;
	HCRYPTKEY hUserKey;
	HCRYPTKEY hSessionKey;
	char er[2000];

	// импортируем открытый ключ отправителя на своей ключевой паре обмена
	if (ImportPublicKeyOnExchangeKey(pbPKey, dwLenPKey, &hUserKey) != CC_NOERRORS)
		return CC_UNKNOWN_ERROR;

	// разбираем сообщение на BLOB сессионного ключа и само сообщение

	

	// длина сессионного ключа
	BYTE *pbLenKey = new BYTE[4];
	FromHex(pbLenKey, pbMsg, 4);
	DWORD dwLenSessionKey = *((DWORD*)pbLenKey) / 2;
	delete pbLenKey;
	

	// BLOB сессионного ключа
	BYTE *pbSessionKey = new BYTE[dwLenSessionKey];
	FromHex(pbSessionKey, pbMsg + 8, dwLenSessionKey);
	
	//ToFile("", pbSessionKey, dwLenSessionKey);

	// длина IV
	BYTE *pbLenIV = new BYTE[4];
	FromHex(pbLenIV, pbMsg + 8 + dwLenSessionKey * 2, 4);
	DWORD dwLenIV = *((DWORD*)pbLenIV) / 2;
	delete pbLenIV;
	
	// сам IV
	BYTE *pbIV = new BYTE[dwLenIV];
	FromHex(pbIV, pbMsg + 8 + dwLenSessionKey * 2 + 8, dwLenIV);
	
	

	// Длина зашифрованного сообщения
	DWORD dwLenCryptMessage = (*pdwLenMsg - 8 - dwLenSessionKey * 2 - 8 - dwLenIV * 2) / 2;

	// зашифрованное сообщение
	BYTE *pbCryptMessage = new BYTE[dwLenCryptMessage];
	FromHex(pbCryptMessage, pbMsg + 8 + dwLenSessionKey * 2 + 8 + dwLenIV * 2, dwLenCryptMessage);

	
	
	// импортируем сессионный ключ на открытом ключе отправителя
	if (!CryptImportKey(phProv, pbSessionKey, dwLenSessionKey, hUserKey, 0, &hSessionKey)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, er, "", MB_OK);
		delete pbSessionKey;
		delete pbCryptMessage;
		delete pbIV;
		return CC_UNKNOWN_ERROR;
	}
	
	// уcтанавливаем IV
	if (!CryptSetKeyParam(hSessionKey, KP_IV, pbIV, 0)){
		delete pbSessionKey;
		delete pbCryptMessage;
		delete pbIV;	
		return CC_UNKNOWN_ERROR;
	}
	
	//ToFile("c:\\1\\s.txt", pbCryptMessage, dwLenCryptMessage);	

	// расшифровываем сообщение
	if (!CryptDecrypt(hSessionKey, 0, TRUE, 0, pbCryptMessage, &dwLenCryptMessage)){
		delete pbSessionKey;
		delete pbCryptMessage;
		delete pbIV;	
		return CC_UNKNOWN_ERROR;
	}

	//ToFile("c:\\1\\r.txt", pbCryptMessage, dwLenCryptMessage);	

	*pbMsg1 = new BYTE[dwLenCryptMessage + 1];	
	memcpy(*pbMsg1, pbCryptMessage, dwLenCryptMessage);
	
	(*pbMsg1)[dwLenCryptMessage] = 0;
	*pdwLenMsg1 = dwLenCryptMessage + 1;

	delete pbSessionKey;
	//MessageBox(NULL, "111", "", MB_OK);
	delete pbCryptMessage;
	delete pbIV;	
	
	return CC_NOERRORS;
}

////////////////////////////////////////////////////
///
/// Шифрование версия 2 (без преобразования)
///
////////////////////////////////////////////////////
ECryptoClientErrors CCryptoClient::CipherMessageBin(BYTE *pbMsg, DWORD dwLenMsg, BYTE *pbPKey, 
	DWORD dwLenPKey, BYTE **pbMsg1, DWORD *pdwLenMsg1)
{
	DWORD nError;
	HCRYPTKEY hUserKey;
	HCRYPTKEY hSessionKey;
	// буфер для хранения блоба сессионного ключа
	BYTE *_pbData = NULL;
	DWORD _dwDataLen = 0;
	BYTE *pbData = NULL;
	DWORD dwDataLen = 0;
	DWORD dwOldLenMsg;
	char er[2000];

	
	if (CreateNewSessionKey(&hSessionKey) != CC_NOERRORS)
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CreateNewSessionKey Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}

	// получаем размер вектора инициализации ключа KP_IV
	DWORD dwIV;
	if (!CryptGetKeyParam(hSessionKey, KP_IV, NULL, &dwIV, 0))
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptGetKeyParam Error 1", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	//DWORD _dwIV = dwIV * 2;

	// получаем вектор инициализации ключа KP_IV
	BYTE *_pbIV = new BYTE[dwIV];
	if (!CryptGetKeyParam(hSessionKey, KP_IV, _pbIV, &dwIV, 0))
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptGetKeyParam Error 2", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	//BYTE *pbIV = new BYTE[dwIV * 2];
	//ToHex(pbIV, _pbIV, dwIV);
	//delete _pbIV;

	if (ImportPublicKeyOnExchangeKeyBin(pbPKey, dwLenPKey, &hUserKey) != CC_NOERRORS)
	{
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "ImportPublicKeyOnExchangeKey Error", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	
	if (!CryptExportKey(hSessionKey, hUserKey, SIMPLEBLOB, 0, NULL, &_dwDataLen)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptExportKey Error 1", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}

	
	//dwDataLen = _dwDataLen * 2;
	_pbData = new BYTE[_dwDataLen];
	//pbData = new BYTE[dwDataLen];
	
	if (!CryptExportKey(hSessionKey, hUserKey, SIMPLEBLOB, 0, _pbData, &_dwDataLen)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptExportKey Error 2", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		delete _pbData;
		//delete pbData;
		return CC_UNKNOWN_ERROR;
	}

	
	
	//ToHex(pbData, _pbData, _dwDataLen);
	
	//delete _pbData;
	// Ура! в pbData экспортированный сессионный ключ
	// определяем длину буфера для зашифрованных данных
	dwOldLenMsg = dwLenMsg;
	if (!CryptEncrypt(hSessionKey, 0, TRUE, 0, NULL, &dwLenMsg, dwLenMsg)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptEncrypt Error 1", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		delete _pbData;
		return CC_UNKNOWN_ERROR;
	}
	// Ура! в *pdwLenMsg количество байт в зашифрованном сообщении

	/////////  8 байт размер сессионного ключа + 8 байт размер IV + KP_IV + сессионный ключ + шифрованное сообщение
	// 4 байта размер сессионного ключа
	// сессионный ключ
	// 4 байта размер IV
	// сам IV
	// 4 байт размер сообщения
	// само сообщение
	*pdwLenMsg1 = 4 + _dwDataLen + 4 + dwIV + 4 + dwLenMsg; //dwIV + 4 + *pdwLenMsg + _dwDataLen + 4 + 1;
	BYTE *LenKey = new BYTE[4];
	memcpy(LenKey, (BYTE*)&_dwDataLen, 4);
	BYTE *LenIV = new BYTE[4];
	memcpy(LenIV, (BYTE*)&dwIV, 4);
	*pbMsg1 = new BYTE[*pdwLenMsg1];
	// длина ключа
	memcpy((void*)*pbMsg1, (void*)LenKey, 4);
	// сам ключ
	memcpy((void*)(*pbMsg1 + 4), (void*)_pbData, _dwDataLen);
	// длина IV
	memcpy((void*)(*pbMsg1 + 4 + _dwDataLen), (void*)LenIV, 4);
	// сам IV
	memcpy((void*)(*pbMsg1 + 4 + _dwDataLen + 4), (void*)_pbIV, dwIV);
	//BYTE *tmp = new BYTE[*pdwLenMsg * 2];    // ????? зачем такой большой буфер
	DWORD dwRealLen;
	if (dwLenMsg >= dwOldLenMsg)
		dwRealLen = dwLenMsg;
	else
		dwRealLen = dwOldLenMsg;
	BYTE *tmp = new BYTE[dwRealLen];
	memcpy((void*)tmp, (void*)pbMsg, dwOldLenMsg);
	delete LenKey;
	delete _pbData;
	delete LenIV;
	delete _pbIV;
	
	//
	if (!CryptEncrypt(hSessionKey, 0, TRUE, 0, tmp, &dwOldLenMsg, dwRealLen)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptEncrypt Error 2", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		delete tmp;
		delete pbMsg1;
		return CC_UNKNOWN_ERROR;
	}

	dwLenMsg = dwOldLenMsg;

	// длина сообщения
	memcpy((BYTE*)(*pbMsg1 + 4 + _dwDataLen + 4 + dwIV), (BYTE*)&dwLenMsg, 4);
	memcpy((BYTE*)(*pbMsg1 + 4 + _dwDataLen + 4 + dwIV + 4), tmp, dwLenMsg);
	delete tmp;
	//(*pbMsg1)[*pdwLenMsg1 - 1] = 0;

	return CC_NOERRORS;
}

//////////////////////////////////////////////////////
//
//    DeCipherMessageBin без преобразования
//
//////////////////////////////////////////////////////
ECryptoClientErrors CCryptoClient::DeCipherMessageBin(BYTE *pbMsg, DWORD dwLenMsg, BYTE *pbPKey, 
	DWORD dwLenPKey, BYTE **pbMsg1, DWORD *pdwLenMsg1)
{
	DWORD nError;
	HCRYPTKEY hUserKey;
	HCRYPTKEY hSessionKey;
	char er[2000];

	// импортируем открытый ключ отправителя на своей ключевой паре обмена
	if (ImportPublicKeyOnExchangeKeyBin(pbPKey, dwLenPKey, &hUserKey) != CC_NOERRORS)
		return CC_UNKNOWN_ERROR;

	// разбираем сообщение на BLOB сессионного ключа и само сообщение

	

	// длина сессионного ключа
	BYTE *pbLenKey = new BYTE[4];
	memcpy(pbLenKey, pbMsg, 4);
	DWORD dwLenSessionKey = *((DWORD*)pbLenKey);
	delete pbLenKey;
	

	// BLOB сессионного ключа
	BYTE *pbSessionKey = new BYTE[dwLenSessionKey];
	memcpy(pbSessionKey, pbMsg + 4, dwLenSessionKey);
	
	//ToFile("", pbSessionKey, dwLenSessionKey);

	// длина IV
	BYTE *pbLenIV = new BYTE[4];
	memcpy(pbLenIV, pbMsg + 4 + dwLenSessionKey, 4);
	DWORD dwLenIV = *((DWORD*)pbLenIV);
	delete pbLenIV;
	
	// сам IV
	BYTE *pbIV = new BYTE[dwLenIV];
	memcpy(pbIV, pbMsg + 4 + dwLenSessionKey + 4, dwLenIV);

	// Длина зашифрованного сообщения + его размер
	DWORD dwLenCryptMessage = (dwLenMsg - 4 - dwLenSessionKey - 4 - dwLenIV - 4);

	// зашифрованное сообщение
	BYTE *pbCryptMessage = new BYTE[dwLenCryptMessage];
	memcpy(pbCryptMessage, pbMsg + 4 + dwLenSessionKey + 4 + dwLenIV + 4, dwLenCryptMessage);

	
	
	// импортируем сессионный ключ на открытом ключе отправителя
	if (!CryptImportKey(phProv, pbSessionKey, dwLenSessionKey, hUserKey, 0, &hSessionKey)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, er, "", MB_OK);
		delete pbSessionKey;
		delete pbCryptMessage;
		delete pbIV;
		return CC_UNKNOWN_ERROR;
	}
	
	// уcтанавливаем IV
	if (!CryptSetKeyParam(hSessionKey, KP_IV, pbIV, 0)){
		delete pbSessionKey;
		delete pbCryptMessage;
		delete pbIV;	
		return CC_UNKNOWN_ERROR;
	}
	
	//ToFile("c:\\1\\s.txt", pbCryptMessage, dwLenCryptMessage);	

	// расшифровываем сообщение
	if (!CryptDecrypt(hSessionKey, 0, TRUE, 0, pbCryptMessage, &dwLenCryptMessage)){
		delete pbSessionKey;
		delete pbCryptMessage;
		delete pbIV;	
		return CC_UNKNOWN_ERROR;
	}

	//ToFile("c:\\1\\r.txt", pbCryptMessage, dwLenCryptMessage);	

	*pbMsg1 = new BYTE[dwLenCryptMessage + 1];	
	memcpy(*pbMsg1, pbCryptMessage, dwLenCryptMessage);
	
	(*pbMsg1)[dwLenCryptMessage] = 0;
	*pdwLenMsg1 = dwLenCryptMessage + 1;

	delete pbSessionKey;
	//MessageBox(NULL, "111", "", MB_OK);
	delete pbCryptMessage;
	delete pbIV;	
	
	return CC_NOERRORS;
}


void MyCodeError(char *er, DWORD nError){
	switch (nError){
	case ERROR_INVALID_HANDLE:
		sprintf(er, "%s", "ERROR_INVALID_HANDLE");
		break;
	case ERROR_INVALID_PARAMETER:
		sprintf(er, "%s", "ERROR_INVALID_PARAMETER");
		break;
	case ERROR_MORE_DATA:
		sprintf(er, "%s", "ERROR_MORE_DATA");
		break;
	case NTE_BAD_FLAGS:
		sprintf(er, "%s", "NTE_BAD_FLAGS");
		break;
	case NTE_BAD_KEY:
		sprintf(er, "%s", "NTE_BAD_KEY");
		break;
	case NTE_BAD_KEY_STATE:
		sprintf(er, "%s", "NTE_BAD_KEY_STATE");
		break;
	case NTE_BAD_PUBLIC_KEY:
		sprintf(er, "%s", "NTE_BAD_PUBLIC_KEY");
		break;
	case NTE_BAD_DATA:
		sprintf(er, "%s", "NTE_BAD_PUBLIC_KEY");
		break;
	case NTE_BAD_TYPE:
		sprintf(er, "%s", "NTE_BAD_TYPE");
		break;
	case NTE_BAD_UID:
		sprintf(er, "%s", "NTE_BAD_UID");
		break;
	case NTE_NO_KEY:
		sprintf(er, "%s", "NTE_NO_KEY");
		break;
	case ERROR_BUSY:
		sprintf(er, "%s", "ERROR_BUSY");
		break;
	case NTE_BAD_KEYSET:
		sprintf(er, "%s", "NTE_BAD_KEYSET");
		break;
	case NTE_BAD_KEYSET_PARAM:
		sprintf(er, "%s", "NTE_BAD_KEYSET_PARAM");
		break;
	case NTE_BAD_SIGNATURE:
		sprintf(er, "%s", "NTE_BAD_SIGNATURE");
		break;
	case NTE_EXISTS:
		sprintf(er, "%s", "NTE_EXISTS");
		break;
	case NTE_KEYSET_ENTRY_BAD:
		sprintf(er, "%s", "NTE_KEYSET_ENTRY_BAD");
		break;
	case NTE_KEYSET_NOT_DEF:
		sprintf(er, "%s", "NTE_KEYSET_NOT_DEF");
		break;
	case NTE_NO_MEMORY:
		sprintf(er, "%s", "NTE_NO_MEMORY");
		break;
	case ERROR_NOT_ENOUGH_MEMORY:
		sprintf(er, "%s", "ERROR_NOT_ENOUGH_MEMORY");
		break;
	case NTE_BAD_PROV_TYPE:
		sprintf(er, "%s", "NTE_BAD_PROV_TYPE");
		break;
	case NTE_PROV_DLL_NOT_FOUND:
		sprintf(er, "%s", "NTE_PROV_DLL_NOT_FOUND");
		break;
	case NTE_PROV_TYPE_ENTRY_BAD:
		sprintf(er, "%s", "NTE_PROV_TYPE_ENTRY_BAD");
		break;
	case NTE_PROV_TYPE_NO_MATCH:
		sprintf(er, "%s", "NTE_PROV_TYPE_NO_MATCH");
		break;
	case NTE_PROV_TYPE_NOT_DEF:
		sprintf(er, "%s", "NTE_PROV_TYPE_NOT_DEF");
		break;
	case NTE_PROVIDER_DLL_FAIL:
		sprintf(er, "%s", "NTE_PROVIDER_DLL_FAIL");
		break;
	case NTE_SIGNATURE_FILE_BAD:
		sprintf(er, "%s", "NTE_SIGNATURE_FILE_BAD");
		break;
	default:
		sprintf(er, "%s", "Неизвестная ошибка");
		break;
	}
}

ECryptoClientErrors CCryptoClient::ImportPublicKeyOnExchangeKey(BYTE *pbData, DWORD dwDataLen, HCRYPTKEY *phUserKey){
	DWORD dwRealLen = dwDataLen / 2;
	BYTE *pbTemp;
	HCRYPTKEY hUserKey;

	if ((dwRealLen * 2) != dwDataLen)
		return CC_INVALID_PARAMETER;
	pbTemp = new BYTE[dwRealLen];

	FromHex(pbTemp, pbData, dwRealLen);

	if (!CryptGetUserKey(phProv, AT_KEYEXCHANGE, &hUserKey)){
		delete pbTemp;
		return CC_UNKNOWN_ERROR;
	}
	// OK! в pbTemp находится BLOB
	if (!CryptImportKey(phProv, pbTemp, dwRealLen, hUserKey, 0, phUserKey)){
		delete pbTemp;
		return CC_UNKNOWN_ERROR;
	}
	delete pbTemp;
	return CC_NOERRORS;
}

ECryptoClientErrors CCryptoClient::ImportPublicKeyOnExchangeKeyBin(BYTE *pbData, DWORD dwDataLen, HCRYPTKEY *phUserKey){
	DWORD nError;
	char er[2000];
	HCRYPTKEY hUserKey;

	if (!CryptGetUserKey(phProv, AT_KEYEXCHANGE, &hUserKey)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptGetUserKey Error 1", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}

	if (!CryptImportKey(phProv, pbData, dwDataLen, hUserKey, 0, phUserKey)){
		nError = GetLastError();
		MyCodeError(er, nError);
		MessageBox(NULL, "CryptImportKey Error 1", "", MB_OK);
		MessageBox(NULL, er, "", MB_OK);
		return CC_UNKNOWN_ERROR;
	}
	return CC_NOERRORS;
}

void ToHex(BYTE *dest, BYTE *src, DWORD ln){
	for (DWORD i = 0; i < ln; i++){
		dest[i * 2] = *hex_table[src[i]];
		dest[i * 2 + 1] = *(hex_table[src[i]] + 1);
	}
}

void ToHex1(BYTE *dest, BYTE *src, DWORD ln) {
	wchar_t *d = (wchar_t*)dest;
	for (DWORD i = 0; i < ln; i++) {
		swprintf(d, ln * 4 + 2, L"%02X", src[i]);
		d += 4;
	}
}

void FromHex(BYTE *dest, BYTE *src, DWORD ln){
	char cTemp[5];
	for (DWORD i = 0; i < ln; i++){
		cTemp[0] = '0';
		cTemp[1] = 'x';
		cTemp[2] = '\0';
		strncat(cTemp, (const char*)(src + (i * 2)), 2);
		dest[i] = (BYTE)strtoul(cTemp, NULL, 0);
	}
}

char *Number2String(DWORD n){
	char *ccc = new char[50];
	return _itoa(n, ccc, 10);
}

bool ToFile(char *fname, BYTE *buf, DWORD lenbuf){
	DWORD nWritten;
	HANDLE h = CreateFile(fname, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if (h == INVALID_HANDLE_VALUE){
		//MessageBox(NULL, "11", "", MB_OK);
		return false;
	}
	if (!WriteFile(h, buf, lenbuf, &nWritten, 0)){
		CloseHandle(h);
		return false;
	}
	CloseHandle(h);
	return true;
}

bool FromFile(char *fname, BYTE *buf, DWORD *lenbuf){
	DWORD dwHigh;
	DWORD dwRead;
	if (!FileExist(fname))
		return false;
	HANDLE h = CreateFile(fname, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (h == INVALID_HANDLE_VALUE){
		
		return false;
	}
	if (!buf){
		*lenbuf = GetFileSize(h, &dwHigh);
		CloseHandle(h);
		if (dwHigh == 0)
			return true;
		else{
			return false;
		}
	}
	if (!ReadFile(h, buf, *lenbuf, &dwRead, 0)){
		CloseHandle(h);
		return false;
	}

	CloseHandle(h);
	return true;
}

bool FileExist(char *fname){
	WIN32_FIND_DATA ffd;
	HANDLE h = FindFirstFile(fname, &ffd);
	if (h == INVALID_HANDLE_VALUE)
		return false;
	else
		return true;
}

bool CCryptoClient::SaveRegister(char *fname)
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tp;
	LONG rc;
	HKEY hKeyToSave;
	DWORD dwDisposition;
	wchar_t *subkey0 = L"SOFTWARE\\Crypto Pro\\Settings\\KeyDevices\\passwords\\";
	wchar_t *subkey;

	// 
    // enable backup privilege
    // 
    if(!OpenProcessToken(GetCurrentProcess(),
                        TOKEN_ADJUST_PRIVILEGES,
                        &hToken ))
    {
        MessageBox(NULL, "Невозможно открыть маркер доступа для текущего процесса", "", MB_OK);
        return false;
    }

	if(!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &luid))

    {
        MessageBox(NULL, "Не найден идентификатор для привилегии", "", MB_OK);
        return false;
    }

	tp.PrivilegeCount           = 1;
    tp.Privileges[0].Luid       = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
                                NULL, NULL );

	if (GetLastError() != ERROR_SUCCESS)
    {
        MessageBox(NULL, "Не могу установить привилегию", "", MB_OK);
        return false;
    }


	subkey = new wchar_t[wcslen(subkey0) + wcslen(container) + 1];
	wcscpy(subkey, subkey0);
	wcscat(subkey, container);
	rc=RegCreateKeyExW(HKEY_LOCAL_MACHINE,
                          subkey, // Name of subkey to open
                          0,
                          NULL,
                          REG_OPTION_BACKUP_RESTORE, // in winnt.h
                          KEY_QUERY_VALUE, // minimal access
                          NULL,
                          &hKeyToSave,
                          &dwDisposition);
	delete subkey;

	if (rc != ERROR_SUCCESS){
		MessageBox(NULL, "Не могу открыть ключ", "", MB_OK);
		return false;
	}

	rc=RegSaveKey(hKeyToSave, fname, NULL);
	//rc=RegSaveKeyEx(hKeyToSave, fname, NULL, REG_STANDARD_FORMAT);

	if (rc != ERROR_SUCCESS){
		MessageBox(NULL, "Не могу сохранить файл", "", MB_OK);
		return false;
	}

    // close registry key we just tried to save
    RegCloseKey(hKeyToSave);
	return true;
}

CCryptoClient::CCryptoClient(LPCWSTR pszContainer, LPCWSTR pszProvider, bool Is2001)
{
	if (pszContainer != NULL){
		container = new wchar_t[wcslen(pszContainer) + 1];
		wcscpy(container, pszContainer);
	}
	else
		container = NULL;
	//nError = GetHandleCSP(pszContainer, pszProvider);
	LPWSTR ProvName;
	DWORD cbProvName;
	CryptGetDefaultProviderW(ProviderType2001, NULL, CRYPT_MACHINE_DEFAULT, NULL, &cbProvName);
	ProvName = new wchar_t[cbProvName];
	CryptGetDefaultProviderW(ProviderType2001, NULL, CRYPT_MACHINE_DEFAULT, ProvName, &cbProvName);
	nError = GetHandleCSP1(pszContainer, ProvName);
	delete ProvName;
}

ECryptoClientErrors CCryptoClient::GetHandleCSP1(LPCWSTR pszContainer, LPCWSTR pszProvider)
{
	char er[2000];
	int nError;
	// Если не задано имя контейнера, то приложение не будет работать с закрытыми
	// ключами и dwFlags должен быть установлен в CRYPT_VERIFYCONTEXT
	// Применяется для проверки подписи
	if (pszContainer == NULL){
		if (CryptAcquireContextW(
			&phProv,
			NULL,
			NULL,
			ProviderType2001,
			CRYPT_VERIFYCONTEXT
			))
			return CC_NOERRORS;
		else
			return CC_NOACCESS_CONTEXT;
	}

	if (CryptAcquireContextW(
			&phProv,
			pszContainer,
			NULL,
			ProviderType2001,
			0
			))
			return CC_NOERRORS;
	nError = GetLastError();
	MyCodeError(er, nError);
	MessageBox(NULL, er, "", MB_OK);
	return CC_NOACCESS_CONTEXT;
}

CCryptoClient::CCryptoClient(LPCWSTR pszContainer, int vid)
{
	nError = GetHandleCSP2(pszContainer);
}

ECryptoClientErrors CCryptoClient::GetHandleCSP2(LPCWSTR pszContainer)
{
	if (CryptAcquireContextW(
			&phProv,
			pszContainer,
			NULL,
			ProviderType2001,
			0
			))
			return CC_NOERRORS;
		else
			return CC_ERROR;
}

// в качестве параметра передается пустой вектор строк, его надо заполнить
ECryptoClientErrors CCryptoClient::EnumContainers(vector<string>& containers)
{
	BYTE data[500];
	DWORD ldata = 500;
	int nError;
	if (CryptGetProvParam(phProv, PP_ENUMCONTAINERS, data, &ldata, CRYPT_FIRST))
	{
		containers.push_back((char*)data);
		while (CryptGetProvParam(phProv, PP_ENUMCONTAINERS, data, &ldata, CRYPT_NEXT))
		{
			containers.push_back((char*)data);
		}
		nError = GetLastError();
		if (nError == ERROR_NO_MORE_ITEMS)
			return CC_NOERRORS;
		else
			return CC_ERROR;
	}
	else
	{
		nError = GetLastError();
		if (nError == ERROR_NO_MORE_ITEMS)
			return CC_NOERRORS;
		else
			return CC_ERROR;
	}
}
