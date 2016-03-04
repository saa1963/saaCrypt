#if !defined( _SUPPORT_SSTEST_SSTST_H )
#define _SUPPORT_SSTEST_SSTST_H

#if !defined( _SSTEST_DECL )
#if defined( SSTEST_IMPORTS )
#define _SSTEST_DECL _STDIMPL_DECL
#else /* defined( SSTEST_IMPORTS ) */
#define _SSTEST_DECL
#endif /* !defined( SSTEST_IMPORTS ) */
#endif /* !defined( _SSTEST_DECL ) */

#if defined( __cplusplus )
extern "C" {
#endif /* defined( __cplusplus ) */

_SSTEST_DECL TSupErr supsys_test_error( 
  const TSupSysEContext *context,
  TSupErr code );

_SSTEST_DECL TSupErr supsys_test_info_version( 
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_info_nickname( 
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_info_device_name( 
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_info_company( 
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_info_detail_name( 
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_info_producer( 
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_info_copyright( 
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_info_flags(
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_list_connects(
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_info( 
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_resolve_auto(
  const TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_connect( 
  TSupSysEContext *context );

_SSTEST_DECL TSupErr supsys_test_disconnect( 
  TSupSysEContext *context );

#if defined( __cplusplus )
}
#endif /* defined( __cplusplus ) */

#endif /* !defined( _SUPPORT_SSTEST_SSTST_H ) */
