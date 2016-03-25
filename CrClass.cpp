// CrClass.cpp : Implementation of CCrClass
#include "stdafx.h"
#include "SaaCrypt.h"
#include "CrClass.h"
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CCrClass

CCrClass::~CCrClass(){
	delete m_cclient;
}

STDMETHODIMP CCrClass::Init(BSTR Container, BSTR Provider, VARIANT_BOOL *retval)
{
	// TODO: Add your implementation code here

	m_cclient = new CCryptoClient(Container, Provider);
	if (m_cclient->nError == CC_NOERRORS)
		*retval = VARIANT_TRUE;
	else{
		*retval = VARIANT_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CCrClass::CreateNewKeys(VARIANT_BOOL *retval)
{
	// TODO: Add your implementation code here
	if (m_cclient->CreateNewKeys(KT_SIGN_KEY) != CC_NOERRORS)
		*retval = VARIANT_FALSE;
	else
		*retval = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CCrClass::ExportPublicKey(BSTR *retval)
{
	// TODO: Add your implementation code here

	BYTE *pbData;
	DWORD dwDataLen;
	if (m_cclient->ExportSignPublicKey(NULL, &dwDataLen) != CC_NOERRORS){
		*retval = SysAllocString((OLECHAR*)"");
		goto done;
	}
	pbData = new BYTE[dwDataLen + 2];
	if (m_cclient->ExportSignPublicKey(pbData, &dwDataLen) != CC_NOERRORS){
		*retval = SysAllocString((OLECHAR*)"");
		delete pbData;
		goto done;
	}
	*retval = SysAllocString((wchar_t*)pbData);
	delete pbData;
done:
	return S_OK;
}

STDMETHODIMP CCrClass::ExportExchangeKey(BSTR *retval)
{
	// TODO: Add your implementation code here

	BYTE *pbData;
	DWORD dwDataLen;
	if (m_cclient->ExportExchangePublicKey(NULL, &dwDataLen) != CC_NOERRORS) {
		*retval = SysAllocString((OLECHAR*)"");
		goto done;
	}

	pbData = new BYTE[dwDataLen + 2];
	if (m_cclient->ExportExchangePublicKey(pbData, &dwDataLen) != CC_NOERRORS) {
		*retval = SysAllocString((OLECHAR*)"");
		delete pbData;
		goto done;
	}
	*retval = SysAllocString((wchar_t*)pbData);
	delete pbData;
done:
	return S_OK;
}

STDMETHODIMP CCrClass::SignMessage(BSTR msg, BSTR *retval)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCrClass::VerifyMessage(BSTR msg, BSTR sign, BSTR key, VARIANT_BOOL *retval)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCrClass::CreateNewExchangeKey(VARIANT_BOOL *retval)
{
	// TODO: Add your implementation code here
	if (m_cclient->CreateNewKeys(KT_EXCHANGE_KEY) != CC_NOERRORS)
		*retval = VARIANT_FALSE;
	else
		*retval = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CCrClass::CreateNewSymmetricKey(VARIANT_BOOL *retval)
{
	// TODO: Add your implementation code here
	if (m_cclient->CreateNewKeys(KT_SYMMETRIC_KEY) != CC_NOERRORS)
		*retval = VARIANT_FALSE;
	else
		*retval = VARIANT_TRUE;
	return S_OK;
}



STDMETHODIMP CCrClass::EncodeMessage(BSTR msg, BSTR pkey, BSTR *retval)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCrClass::DecodeMessage(BSTR msg, BSTR pkey, BSTR *retval)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCrClass::EncodeMessageBin(VARIANT msg, VARIANT pkey, VARIANT *retval)
{
	// TODO: Add your implementation code here
	VARIANT _retval;
	_retval.vt = VT_EMPTY;
	ECryptoClientErrors e;

	LONG ubound_msg;
	SafeArrayGetUBound(msg.parray, 1, &ubound_msg);
	LONG ubound_pkey;
	SafeArrayGetUBound(pkey.parray, 1, &ubound_pkey);

	BYTE HUGEP *pmsg;
	BYTE HUGEP *ppkey;
	
	SafeArrayAccessData(msg.parray, (void HUGEP**)&pmsg);
	SafeArrayAccessData(pkey.parray, (void HUGEP**)&ppkey);

	BYTE *msg1 = NULL;
	DWORD dwLenMsg1;
	e = m_cclient->CipherMessageBin(pmsg, ubound_msg + 1, ppkey, ubound_pkey + 1, &msg1, &dwLenMsg1);
	if (e != CC_NOERRORS){
		//MessageBox(NULL, "Error---", "", MB_OK);
		if (msg1 != NULL)
			delete msg1;
		SafeArrayUnaccessData(pkey.parray);
		SafeArrayUnaccessData(msg.parray);
		goto done;
	}
	SafeArrayUnaccessData(pkey.parray);
	SafeArrayUnaccessData(msg.parray);
	SAFEARRAY *rtarray = SafeArrayCreateVector(VT_UI1, 0, dwLenMsg1);
	BYTE HUGEP *pbyte;
	SafeArrayAccessData(rtarray, (void HUGEP**)&pbyte);
	memcpy(pbyte, msg1, dwLenMsg1);
	SafeArrayUnaccessData(rtarray);
	_retval.vt = VT_ARRAY | VT_UI1;
	_retval.parray = rtarray;
	*retval = _retval;
	if (msg1 != NULL)
		delete msg1;
done:
	return S_OK;
}

STDMETHODIMP CCrClass::DecodeMessageBin(VARIANT msg, VARIANT pkey, VARIANT *retval)
{
	// TODO: Add your implementation code here
	VARIANT _retval;
	_retval.vt = VT_EMPTY;
	ECryptoClientErrors e;

	LONG ubound_msg;
	SafeArrayGetUBound(msg.parray, 1, &ubound_msg);
	LONG ubound_pkey;
	SafeArrayGetUBound(pkey.parray, 1, &ubound_pkey);

	BYTE HUGEP *pmsg;
	BYTE HUGEP *ppkey;
	
	SafeArrayAccessData(msg.parray, (void HUGEP**)&pmsg);
	SafeArrayAccessData(pkey.parray, (void HUGEP**)&ppkey);

	BYTE *msg1 = NULL;
	DWORD dwLenMsg1;

	e = m_cclient->DeCipherMessageBin(pmsg, ubound_msg + 1, ppkey, ubound_pkey + 1, &msg1, &dwLenMsg1);
	if (e != CC_NOERRORS){
		if (msg1 != NULL)
			delete msg1;
		SafeArrayUnaccessData(pkey.parray);
		SafeArrayUnaccessData(msg.parray);
		goto done;
	}
	SafeArrayUnaccessData(pkey.parray);
	SafeArrayUnaccessData(msg.parray);
	SAFEARRAY *rtarray = SafeArrayCreateVector(VT_UI1, 0, dwLenMsg1);
	BYTE HUGEP *pbyte;
	SafeArrayAccessData(rtarray, (void HUGEP**)&pbyte);
	memcpy(pbyte, msg1, dwLenMsg1);
	SafeArrayUnaccessData(rtarray);
	_retval.vt = VT_ARRAY | VT_UI1;
	_retval.parray = rtarray;
	*retval = _retval;
	if (msg1 != NULL)
		delete msg1;
done:

	return S_OK;	
}

STDMETHODIMP CCrClass::EncodeFile(BSTR fname, BSTR pkey, VARIANT_BOOL *retval)
{
	return E_NOTIMPL;	
}

STDMETHODIMP CCrClass::DecodeFile(BSTR fname, BSTR pkey, VARIANT_BOOL *retval)
{
	return E_NOTIMPL;	
}

STDMETHODIMP CCrClass::EncodeFile2(BSTR fname, BSTR pkey, VARIANT_BOOL *retval)
{
	// TODO: Add your implementation code here
	BYTE *msg1 = NULL;
	DWORD dwLenMsg1;
	BYTE *buf;

	ECryptoClientErrors e;
	
	DWORD dwLn = 0;
	if (!FromFile(fname, NULL, &dwLn)){
		
		*retval = VARIANT_FALSE;
		goto done;
	}
	buf = new BYTE[dwLn];
	if (!FromFile(fname, buf, &dwLn)){
		
		*retval = VARIANT_FALSE;
		goto done0;
	}
	BYTE *_pkey = NULL;
	DWORD dwLenPKey = lstrlenW(pkey) / 2;
	_pkey = new BYTE[dwLenPKey];
	FromHex1(_pkey, pkey, dwLenPKey);
	e = m_cclient->CipherMessageBin((BYTE*)buf, dwLn, (BYTE*)_pkey, dwLenPKey, &msg1, &dwLenMsg1);
	delete _pkey;
	if (e != CC_NOERRORS){
		
		*retval = VARIANT_FALSE;
		goto done0;
	}
	if (!ToFile(fname, msg1, dwLenMsg1 - 1)){
		
		*retval = VARIANT_FALSE;
		goto done00;
	}
	*retval = VARIANT_TRUE;;
done00:
	delete msg1;
done0:
	delete buf;
done:
	return S_OK;	
}

STDMETHODIMP CCrClass::DecodeFile2(BSTR fname, BSTR pkey, VARIANT_BOOL *retval)
{
	// TODO: Add your implementation code here

	BYTE *msg1 = NULL;
	DWORD dwLenMsg1;
	BYTE *buf;

	ECryptoClientErrors e;
	DWORD dwLn = 0;
	if (!FromFile(fname, NULL, &dwLn)){
		*retval = VARIANT_FALSE;
		goto done;
	}
	buf = new BYTE[dwLn];
	if (!FromFile(fname, buf, &dwLn)){
		*retval = VARIANT_FALSE;
		goto done0;
	}
	BYTE *_pkey = NULL;
	DWORD dwLenPKey = lstrlenW(pkey) / 2;
	_pkey = new BYTE[dwLenPKey];
	FromHex1(_pkey, pkey, dwLenPKey);
	e = m_cclient->DeCipherMessageBin((BYTE*)buf, dwLn, (BYTE*)_pkey, dwLenPKey, &msg1, &dwLenMsg1);
	delete _pkey;
	if (e != CC_NOERRORS){
		*retval = VARIANT_FALSE;
		goto done0;
	}

	if (!ToFile(fname, msg1, dwLenMsg1 - 1)){
		*retval = VARIANT_FALSE;
		goto done00;
	}
	*retval = VARIANT_TRUE;;
done00:
	delete msg1;
done0:
	delete buf;
done:
	return S_OK;	
}

STDMETHODIMP CCrClass::SaveRegister(BSTR fname)
{
	// TODO: Add your implementation code here
	m_cclient->SaveRegister(fname);
	return S_OK;
}

STDMETHODIMP CCrClass::Init0(BSTR Container, BSTR Provider, VARIANT_BOOL *retval)
{
	// TODO: Add your implementation code here

	m_cclient = new CCryptoClient(Container, Provider, TRUE);
	if (m_cclient->nError == CC_NOERRORS)
		*retval = VARIANT_TRUE;
	else{
		*retval = VARIANT_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CCrClass::Init1(BSTR container, BSTR provider, VARIANT_BOOL* retval)
{
	// TODO: Add your implementation code here

	m_cclient = new CCryptoClient(container, provider, TRUE);
	if (m_cclient->nError == CC_NOERRORS)
		*retval = VARIANT_TRUE;
	else{
		*retval = VARIANT_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CCrClass::InitExistsContainer(BSTR container, VARIANT_BOOL* retval)
{
	// TODO: Add your implementation code here

	m_cclient = new CCryptoClient(container, 0);
	if (m_cclient->nError == CC_NOERRORS)
		*retval = VARIANT_TRUE;
	else{
		*retval = VARIANT_FALSE;
	}
	return S_OK;
}

#pragma warning(disable : 4996)
STDMETHODIMP CCrClass::EnumContainers(VARIANT* retval)
{
	vector<string> containers;
	vector<string>::iterator it;
	ECryptoClientErrors err;
	SAFEARRAY *array;
	BSTR v;
	OLECHAR* wdata;
	int ln;
	// заполняем вектор наименованиями контейнеров
	err = m_cclient->EnumContainers(containers);
	if (err == CC_NOERRORS)
	{
		array = SafeArrayCreateVector(VT_BSTR, 0, containers.size());
		LONG ind[1];
		it = containers.begin();
		for (ind[0] = 0; it != containers.end(); it++, ind[0]++)
		{
			ln = mbstowcs(NULL, (*it).c_str(), (*it).length() + 1);
			wdata = new OLECHAR[ln];
			mbstowcs(wdata, (*it).c_str(), (*it).length() + 1);
			v = SysAllocString(wdata);
			SafeArrayPutElement(array, ind, v);
			delete wdata;
		}
		VARIANT vr;
		vr.vt = VT_ARRAY | VT_BSTR;
		vr.parray = array;
		*retval = vr;
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


STDMETHODIMP CCrClass::SignMessageBin(VARIANT msg, VARIANT* sign)
{
	
	VARIANT _sign;
	_sign.vt = VT_EMPTY;
	LONG ubound;
	SafeArrayGetUBound(msg.parray, 1, &ubound);
	//BYTE *msgb = new BYTE[ubound + 1];
	BYTE HUGEP *pbyte;
	SafeArrayAccessData(msg.parray, (void HUGEP**)&pbyte);

	//ToFile("c:\\qqq.bin", pbyte, ubound + 1);

	DWORD dwSignLen;
	ECryptoClientErrors ner = m_cclient->SignMessageBin(pbyte, ubound + 1, NULL, &dwSignLen);
	if (ner != CC_NOERRORS){
		SafeArrayUnaccessData(msg.parray);
		*sign = _sign;
		goto done;
	}
	BYTE *pbSign = new BYTE[dwSignLen];
	ner = m_cclient->SignMessageBin(pbyte, ubound + 1, pbSign, &dwSignLen);
	if (ner != CC_NOERRORS){
		

		SafeArrayUnaccessData(msg.parray);
		delete pbSign;
		*sign = _sign;
		goto done;
	}
	SafeArrayUnaccessData(msg.parray);
	SAFEARRAY *rtarray = SafeArrayCreateVector(VT_UI1, 0, dwSignLen);
	SafeArrayAccessData(rtarray, (void HUGEP**)&pbyte);
	memcpy(pbyte, pbSign, dwSignLen);
	//ToFile("c:\\qqq.bin", pbyte, ubound + 1);
	//pbyte = pbSign;
	SafeArrayUnaccessData(rtarray);
	_sign.vt = VT_ARRAY | VT_UI1;
	_sign.parray = rtarray;
	*sign = _sign;
done:
	return S_OK;
}


STDMETHODIMP CCrClass::VerifyMessageBin(VARIANT msg, VARIANT sign, VARIANT key, VARIANT_BOOL* retval)
{
	// TODO: Add your implementation code here
	ECryptoClientErrors e;

	LONG ubound_msg;
	SafeArrayGetUBound(msg.parray, 1, &ubound_msg);
	LONG ubound_sign;
	SafeArrayGetUBound(sign.parray, 1, &ubound_sign);
	LONG ubound_key;
	SafeArrayGetUBound(key.parray, 1, &ubound_key);

	BYTE HUGEP *pmsg;
	BYTE HUGEP *psign;
	BYTE HUGEP *pkey;
	SafeArrayAccessData(msg.parray, (void HUGEP**)&pmsg);
	SafeArrayAccessData(sign.parray, (void HUGEP**)&psign);
	SafeArrayAccessData(key.parray, (void HUGEP**)&pkey);

	e = m_cclient->VerifyMessageBin(pmsg, ubound_msg + 1, 
		psign, ubound_sign + 1, pkey, ubound_key + 1);

	SafeArrayUnaccessData(key.parray);
	SafeArrayUnaccessData(sign.parray);
	SafeArrayUnaccessData(msg.parray);

	if (e == CC_BAD_SIGN)
		*retval = VARIANT_FALSE;
	else if (e == CC_NOERRORS)
		*retval = VARIANT_TRUE;
	else
	{
		*retval = VARIANT_FALSE;
	}

	return S_OK;
}
