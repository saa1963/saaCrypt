

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Mar 04 12:21:37 2016
 */
/* Compiler settings for saaCrypt.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __saaCrypt_h__
#define __saaCrypt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICrClass_FWD_DEFINED__
#define __ICrClass_FWD_DEFINED__
typedef interface ICrClass ICrClass;

#endif 	/* __ICrClass_FWD_DEFINED__ */


#ifndef __CrClass_FWD_DEFINED__
#define __CrClass_FWD_DEFINED__

#ifdef __cplusplus
typedef class CrClass CrClass;
#else
typedef struct CrClass CrClass;
#endif /* __cplusplus */

#endif 	/* __CrClass_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICrClass_INTERFACE_DEFINED__
#define __ICrClass_INTERFACE_DEFINED__

/* interface ICrClass */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICrClass;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A0EB4B35-BEC0-4F6C-900C-93A905881102")
    ICrClass : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ BSTR Container,
            /* [in] */ BSTR Provider,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateNewKeys( 
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExportPublicKey( 
            /* [retval][out] */ BSTR *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SignMessage( 
            /* [in] */ BSTR msg,
            /* [retval][out] */ BSTR *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VerifyMessage( 
            /* [in] */ BSTR msg,
            /* [in] */ BSTR sign,
            /* [in] */ BSTR key,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateNewExchangeKey( 
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExportExchangeKey( 
            /* [retval][out] */ BSTR *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeMessage( 
            /* [in] */ BSTR msg,
            /* [in] */ BSTR pkey,
            /* [retval][out] */ BSTR *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecodeMessage( 
            /* [in] */ BSTR msg,
            /* [in] */ BSTR pkey,
            /* [retval][out] */ BSTR *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeFile( 
            /* [in] */ BSTR fname,
            BSTR pkey,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecodeFile( 
            /* [in] */ BSTR fname,
            /* [in] */ BSTR pkey,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveRegister( 
            /* [in] */ BSTR fname) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init0( 
            /* [in] */ BSTR Container,
            /* [in] */ BSTR Provider,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init1( 
            /* [in] */ BSTR container,
            /* [in] */ BSTR provider,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE InitExistsContainer( 
            /* [in] */ BSTR container,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateNewSymmetricKey( 
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE EnumContainers( 
            /* [retval][out] */ VARIANT *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeMessageBin( 
            /* [in] */ VARIANT msg,
            /* [in] */ VARIANT pkey,
            /* [retval][out] */ VARIANT *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecodeMessageBin( 
            /* [in] */ VARIANT msg,
            /* [in] */ VARIANT pkey,
            /* [retval][out] */ VARIANT *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncodeFile2( 
            /* [in] */ BSTR fname,
            BSTR pkey,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecodeFile2( 
            /* [in] */ BSTR fname,
            /* [in] */ BSTR pkey,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SignMessageBin( 
            /* [in] */ VARIANT msg,
            /* [retval][out] */ VARIANT *sign) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE VerifyMessageBin( 
            /* [in] */ VARIANT msg,
            /* [in] */ VARIANT sign,
            /* [in] */ VARIANT key,
            /* [retval][out] */ VARIANT_BOOL *retval) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICrClassVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICrClass * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICrClass * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICrClass * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICrClass * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICrClass * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICrClass * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICrClass * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            ICrClass * This,
            /* [in] */ BSTR Container,
            /* [in] */ BSTR Provider,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNewKeys )( 
            ICrClass * This,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExportPublicKey )( 
            ICrClass * This,
            /* [retval][out] */ BSTR *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SignMessage )( 
            ICrClass * This,
            /* [in] */ BSTR msg,
            /* [retval][out] */ BSTR *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VerifyMessage )( 
            ICrClass * This,
            /* [in] */ BSTR msg,
            /* [in] */ BSTR sign,
            /* [in] */ BSTR key,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNewExchangeKey )( 
            ICrClass * This,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExportExchangeKey )( 
            ICrClass * This,
            /* [retval][out] */ BSTR *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncodeMessage )( 
            ICrClass * This,
            /* [in] */ BSTR msg,
            /* [in] */ BSTR pkey,
            /* [retval][out] */ BSTR *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecodeMessage )( 
            ICrClass * This,
            /* [in] */ BSTR msg,
            /* [in] */ BSTR pkey,
            /* [retval][out] */ BSTR *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncodeFile )( 
            ICrClass * This,
            /* [in] */ BSTR fname,
            BSTR pkey,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecodeFile )( 
            ICrClass * This,
            /* [in] */ BSTR fname,
            /* [in] */ BSTR pkey,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveRegister )( 
            ICrClass * This,
            /* [in] */ BSTR fname);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init0 )( 
            ICrClass * This,
            /* [in] */ BSTR Container,
            /* [in] */ BSTR Provider,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init1 )( 
            ICrClass * This,
            /* [in] */ BSTR container,
            /* [in] */ BSTR provider,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *InitExistsContainer )( 
            ICrClass * This,
            /* [in] */ BSTR container,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateNewSymmetricKey )( 
            ICrClass * This,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *EnumContainers )( 
            ICrClass * This,
            /* [retval][out] */ VARIANT *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncodeMessageBin )( 
            ICrClass * This,
            /* [in] */ VARIANT msg,
            /* [in] */ VARIANT pkey,
            /* [retval][out] */ VARIANT *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecodeMessageBin )( 
            ICrClass * This,
            /* [in] */ VARIANT msg,
            /* [in] */ VARIANT pkey,
            /* [retval][out] */ VARIANT *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncodeFile2 )( 
            ICrClass * This,
            /* [in] */ BSTR fname,
            BSTR pkey,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecodeFile2 )( 
            ICrClass * This,
            /* [in] */ BSTR fname,
            /* [in] */ BSTR pkey,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SignMessageBin )( 
            ICrClass * This,
            /* [in] */ VARIANT msg,
            /* [retval][out] */ VARIANT *sign);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *VerifyMessageBin )( 
            ICrClass * This,
            /* [in] */ VARIANT msg,
            /* [in] */ VARIANT sign,
            /* [in] */ VARIANT key,
            /* [retval][out] */ VARIANT_BOOL *retval);
        
        END_INTERFACE
    } ICrClassVtbl;

    interface ICrClass
    {
        CONST_VTBL struct ICrClassVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICrClass_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICrClass_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICrClass_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICrClass_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICrClass_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICrClass_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICrClass_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICrClass_Init(This,Container,Provider,retval)	\
    ( (This)->lpVtbl -> Init(This,Container,Provider,retval) ) 

#define ICrClass_CreateNewKeys(This,retval)	\
    ( (This)->lpVtbl -> CreateNewKeys(This,retval) ) 

#define ICrClass_ExportPublicKey(This,retval)	\
    ( (This)->lpVtbl -> ExportPublicKey(This,retval) ) 

#define ICrClass_SignMessage(This,msg,retval)	\
    ( (This)->lpVtbl -> SignMessage(This,msg,retval) ) 

#define ICrClass_VerifyMessage(This,msg,sign,key,retval)	\
    ( (This)->lpVtbl -> VerifyMessage(This,msg,sign,key,retval) ) 

#define ICrClass_CreateNewExchangeKey(This,retval)	\
    ( (This)->lpVtbl -> CreateNewExchangeKey(This,retval) ) 

#define ICrClass_ExportExchangeKey(This,retval)	\
    ( (This)->lpVtbl -> ExportExchangeKey(This,retval) ) 

#define ICrClass_EncodeMessage(This,msg,pkey,retval)	\
    ( (This)->lpVtbl -> EncodeMessage(This,msg,pkey,retval) ) 

#define ICrClass_DecodeMessage(This,msg,pkey,retval)	\
    ( (This)->lpVtbl -> DecodeMessage(This,msg,pkey,retval) ) 

#define ICrClass_EncodeFile(This,fname,pkey,retval)	\
    ( (This)->lpVtbl -> EncodeFile(This,fname,pkey,retval) ) 

#define ICrClass_DecodeFile(This,fname,pkey,retval)	\
    ( (This)->lpVtbl -> DecodeFile(This,fname,pkey,retval) ) 

#define ICrClass_SaveRegister(This,fname)	\
    ( (This)->lpVtbl -> SaveRegister(This,fname) ) 

#define ICrClass_Init0(This,Container,Provider,retval)	\
    ( (This)->lpVtbl -> Init0(This,Container,Provider,retval) ) 

#define ICrClass_Init1(This,container,provider,retval)	\
    ( (This)->lpVtbl -> Init1(This,container,provider,retval) ) 

#define ICrClass_InitExistsContainer(This,container,retval)	\
    ( (This)->lpVtbl -> InitExistsContainer(This,container,retval) ) 

#define ICrClass_CreateNewSymmetricKey(This,retval)	\
    ( (This)->lpVtbl -> CreateNewSymmetricKey(This,retval) ) 

#define ICrClass_EnumContainers(This,retval)	\
    ( (This)->lpVtbl -> EnumContainers(This,retval) ) 

#define ICrClass_EncodeMessageBin(This,msg,pkey,retval)	\
    ( (This)->lpVtbl -> EncodeMessageBin(This,msg,pkey,retval) ) 

#define ICrClass_DecodeMessageBin(This,msg,pkey,retval)	\
    ( (This)->lpVtbl -> DecodeMessageBin(This,msg,pkey,retval) ) 

#define ICrClass_EncodeFile2(This,fname,pkey,retval)	\
    ( (This)->lpVtbl -> EncodeFile2(This,fname,pkey,retval) ) 

#define ICrClass_DecodeFile2(This,fname,pkey,retval)	\
    ( (This)->lpVtbl -> DecodeFile2(This,fname,pkey,retval) ) 

#define ICrClass_SignMessageBin(This,msg,sign)	\
    ( (This)->lpVtbl -> SignMessageBin(This,msg,sign) ) 

#define ICrClass_VerifyMessageBin(This,msg,sign,key,retval)	\
    ( (This)->lpVtbl -> VerifyMessageBin(This,msg,sign,key,retval) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICrClass_INTERFACE_DEFINED__ */



#ifndef __SAACRYPTLib_LIBRARY_DEFINED__
#define __SAACRYPTLib_LIBRARY_DEFINED__

/* library SAACRYPTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SAACRYPTLib;

EXTERN_C const CLSID CLSID_CrClass;

#ifdef __cplusplus

class DECLSPEC_UUID("1B4F737C-1E35-41BB-846B-D7D8EF17D5C0")
CrClass;
#endif
#endif /* __SAACRYPTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


