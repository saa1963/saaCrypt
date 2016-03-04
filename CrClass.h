// CrClass.h : Declaration of the CCrClass

#ifndef __CRCLASS_H_
#define __CRCLASS_H_

#include "resource.h"       // main symbols
#include "CryptoClient.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CCrClass
class ATL_NO_VTABLE CCrClass : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCrClass, &CLSID_CrClass>,
	public IDispatchImpl<ICrClass, &IID_ICrClass, &LIBID_SAACRYPTLib>
{
public:
	CCrClass()
	{
	}
	~CCrClass();

DECLARE_REGISTRY_RESOURCEID(IDR_CRCLASS)
DECLARE_NOT_AGGREGATABLE(CCrClass)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCrClass)
	COM_INTERFACE_ENTRY(ICrClass)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// ICrClass
public:
	STDMETHOD(Init0)(/*[in]*/ BSTR Container, /*[in]*/ BSTR Provider, /*[out, ret]*/ VARIANT_BOOL *retval);
	STDMETHOD(SaveRegister)(/*[in]*/ BSTR fname);
	STDMETHOD(DecodeFile)(/*[in]*/ BSTR fname, /*[in]*/ BSTR pkey, /*[out,retval]*/ VARIANT_BOOL *retval);
	STDMETHOD(EncodeFile)(/*[in]*/ BSTR fname, BSTR pkey, /*[out, retval]*/ VARIANT_BOOL *retval);
	STDMETHOD(DecodeMessage)(/*[in]*/ BSTR msg, /*[in]*/ BSTR pkey, /*[out,retval]*/ BSTR *retval);
	STDMETHOD(EncodeMessage)(/*[in]*/ BSTR msg, /*[in]*/ BSTR pkey, /*[out,retval]*/ BSTR *retval);
	STDMETHOD(ExportExchangeKey)(BSTR *retval);
	STDMETHOD(CreateNewExchangeKey)(VARIANT_BOOL *retval);
	STDMETHOD(VerifyMessage)(/*[in]*/ BSTR msg, /*[in]*/ BSTR sign, /*[in]*/ BSTR key, /*[out,retval]*/ VARIANT_BOOL *retval);
	STDMETHOD(SignMessage)(/*[in]*/ BSTR msg, /*[out,retval]*/ BSTR *retval);
	STDMETHOD(ExportPublicKey)(/*[out,retval]*/ BSTR *retval);
	STDMETHOD(CreateNewKeys)(/*[out,retval]*/ VARIANT_BOOL *retval);
	STDMETHOD(CreateNewSymmetricKey)(/*[out,retval]*/ VARIANT_BOOL *retval);
	//STDMETHOD(EnumContainers)(/*[out,retval]*/ SAFEARRAY *retval);
	STDMETHOD(Init)(/*[in]*/ BSTR Container, /*[in]*/ BSTR Provider, /*[out,retval]*/ VARIANT_BOOL *retval);
private:
	CCryptoClient *m_cclient;
public:
	STDMETHOD(Init1)(BSTR container, BSTR provider, VARIANT_BOOL* retval);
	STDMETHOD(InitExistsContainer)(BSTR container, VARIANT_BOOL* retval);
	STDMETHOD(EnumContainers)(VARIANT* retval);
	STDMETHOD(DecodeMessageBin)(/*[in]*/ VARIANT msg, /*[in]*/ VARIANT pkey, /*[out,retval]*/ VARIANT *retval);
	STDMETHOD(EncodeMessageBin)(/*[in]*/ VARIANT msg, /*[in]*/ VARIANT pkey, /*[out,retval]*/ VARIANT *retval);
	STDMETHOD(DecodeFile2)(/*[in]*/ BSTR fname, /*[in]*/ BSTR pkey, /*[out,retval]*/ VARIANT_BOOL *retval);
	STDMETHOD(EncodeFile2)(/*[in]*/ BSTR fname, BSTR pkey, /*[out, retval]*/ VARIANT_BOOL *retval);
	STDMETHOD(SignMessageBin)(VARIANT msg, VARIANT* sign);
	STDMETHOD(VerifyMessageBin)(VARIANT msg, VARIANT sign, VARIANT key, VARIANT_BOOL* retval);
};

#endif //__CRCLASS_H_
