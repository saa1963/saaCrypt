/*
 * Copyright(C) 2000 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 */

#include "sststprj.h"

/*++++
 * ������� �������� ��������� ���������� � ������.
 ++++*/

TSupErr
supsys_test_info_version(const TSupSysEContext *context)
{
  unsigned int version[4]; /* ������. */
  TSupErr code; /* ��� ��������. */
  
  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting version in progress...")));
        
  code = supsys_version( context, version );
  if( code )
    return supsys_test_error( context, code );

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item version:%u.%u.%u.%u."),version[0], version[1], version[2],version[3]));
     
  return SUP_ERR_NO;
}


/*++++
 * ������� �������� ��������.
 ++++*/
//

TSupErr
supsys_test_info_nickname(const TSupSysEContext *context)
{
  TSupErr code; /* ��� ��������. */
  TSupSysENickname nickname; /* ��������. */   

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting nickname in progress...")));    

  code = supsys_nickname( context, nickname );
  if( code )
    return supsys_test_error( context, code );

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item nickname:%s."),nickname));
  return SUP_ERR_NO;
}


/*++++
 * ������� ������������ ����� �����������.
 ++++*/
TSupErr
supsys_test_info_device_name(const TSupSysEContext *context)
{
  TSupErr code; /* ��� ��������. */
  size_t length = 0; /* ����� ����� �����������. */
  TCHAR *name; /* ��� �����������. */

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting device name in progress...")));    

  code = supsys_name( context, &length, NULL );
  if( code )
    return supsys_test_error( context, code );
  name = malloc( ( length + 1 ) * sizeof( TCHAR ) );
  if( name == NULL )
    return supsys_test_error( context, code );
  code = supsys_name( context, &length, name );
  if( code )
    {
      free( name );
      return supsys_test_error( context, code );
    }
  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item name:%s."),name));
  free( name );
  return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ������� ����� �����������.
 ++++*/
TSupErr
supsys_test_info_company(const TSupSysEContext *context)
{
  TSupErr code; /* ��� ��������. */
  size_t length = 0; /* ����� ����� �����������. */
  TCHAR *name; /* ��� �����������. */

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting company in progress...")));    

  code = supsys_company( context, &length, NULL );
  if( code )
    return supsys_test_error( context, code );
  name = malloc( ( length + 1 ) * sizeof( TCHAR ) );
  if( name == NULL )
    return supsys_test_error( context, code );
  code = supsys_company( context, &length, name );
  if( code )
    {
      free( name );
      return supsys_test_error( context, code );
    }

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Comapny:%s."),name));

  free( name );
  return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ������� ����� �����������.
 ++++*/
TSupErr
supsys_test_info_detail_name(const TSupSysEContext *context)
{
  TSupErr code; /* ��� ��������. */
  size_t length = 0; /* ����� ����� �����������. */
  TCHAR *name; /* ��� �����������. */

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting detail name in progress...")));    
  code = supsys_detail( context, &length, NULL );
  if( code )
    return supsys_test_error( context, code );
  name = malloc( ( length + 1 ) * sizeof( TCHAR ) );
  if( name == NULL )
    return supsys_test_error( context, code );
  code = supsys_detail( context, &length, name );
  if( code )
    {
      free( name );
      return supsys_test_error( context, code );
    }

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Detail name:%s."),name));
  free( name );
  return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ����� ������������� �����������.
 ++++*/
TSupErr
supsys_test_info_producer(const TSupSysEContext *context)
{
  TSupErr code; /* ��� ��������. */
  size_t length = 0; /* ����� ����� �����������. */
  TCHAR *text; /* ��� �����������. */

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting producer in progress...")));

  code = supsys_producer( context, &length, NULL );
  if( code == SUP_ERR_UNSUPPORTED )
    {
      DbTrace(DB_TRACE, (FTEXT(support_context_io, "Soft item or producer unsupported.")));
      return SUP_ERR_NO;
    }
  if( code )
    return supsys_test_error( context, code );
  text = malloc( ( length + 1 ) * sizeof( TCHAR ) );
  if( text == NULL )
    return supsys_test_error( context, code );
  code = supsys_producer( context, &length, text );
  if( code )
    {
      free( text );
      return supsys_test_error( context, code );
    }

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Producer:%s."),text));
  free( text );

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting producer version in progress...")));

  code = supsys_producer_version( context, &length, NULL );
  if( code == SUP_ERR_UNSUPPORTED ){
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Unknown producer version.")));
    return SUP_ERR_NO;
  }
  if( code )
    return supsys_test_error( context, code );
  text = malloc( ( length + 1 ) * sizeof( TCHAR ) );
  if( text == NULL )
    return supsys_test_error( context, code );
  code = supsys_producer_version( context, &length, text );
  if( code ){
    free( text );
    return supsys_test_error( context, code );
  }

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Producer version:%s."),text));
  free( text );
  return SUP_ERR_NO;
}

/*++++
 * ������� ������������ ��������� �������������� ������ ������ ��������� � 
 *  ����������� ��� ���� ������ ���������� �����.
 ++++*/
TSupErr
supsys_test_info_copyright(const TSupSysEContext *context)
{
  TCHAR *copyright; /* Copyright info */
  TSupErr code; /* ��� ��������. */
  size_t length; /* ������ copyright. */
    
  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting copyright information.")));
  code = supsys_copyright( context, &length, NULL );
  if( code )
    return supsys_test_error( context, code );
  copyright = malloc( ( length + 1 ) * sizeof( TCHAR ) );
  if( copyright == NULL )
    return supsys_test_error( context, SUP_ERR_MEMORY );
  code = supsys_copyright( context, &length, copyright );
  if( code ){
    free( copyright );
    return supsys_test_error( context, code );
  }

  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Copyright info: %s"),copyright));
  free( copyright );
  return SUP_ERR_NO;
}


/*++++
 * ������� ������������ ��������� ��������� ������.
 ++++*/
TSupErr
supsys_test_info_flags(const TSupSysEContext *context)
{
  TSupErr code; /* ��� ��������. */
  int flag; /* ���� �����������. */
  //    int ids;

  /*+ ����� ��������� � ������ �����. +*/
  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting flags...")));    

  /*+ ����� ��������� � ��������� ���������� ����������. +*/
  code = supsys_flag( context, SUPSYS_FLAG_BLOCK_CONNECT, 
		      SUPSYS_FLAG_CONNECT_MANY, &flag );
  if( code )
    return supsys_test_error( context, code );
  if( flag ){
    //	ids = IDS_RESULT_FLAG_MANY_SUPPORTED;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item support multy connections.")));
  }    
  else{
    //	ids = IDS_RESULT_FLAG_MANY_NOT;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item isn't supported multy connections.")));
  }

  /*+ ����� ��������� � ��������� ���������� COM. +*/
  code = supsys_flag( context, SUPSYS_FLAG_BLOCK_CONNECT, 
		      SUPSYS_FLAG_CONNECT_COM, &flag );
  if( code )
    return supsys_test_error( context, code );
  if( flag ){
    //	ids = IDS_RESULT_FLAG_COM_SUPPORTED;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item support com connections.")));
  }
  else{
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item isn't supported com connections.")));
    //	ids = IDS_RESULT_FLAG_COM_NOT;
  }
        
  /*+ ����� ��������� � ��������� ���������� COM. +*/
  code = supsys_flag( context, SUPSYS_FLAG_BLOCK_CONNECT, 
		      SUPSYS_FLAG_CONNECT_LPT, &flag );
  if( code )
    return supsys_test_error( context, code );
  if( flag ){
    //	ids = IDS_RESULT_FLAG_LPT_SUPPORTED;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item support lpt connections.")));
  }
  else{
    //	ids = IDS_RESULT_FLAG_LPT_NOT;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item isn't supported lpt connections.")));
  }

  /*+ ����� ��������� � ��������� ���������� auto. +*/
  code = supsys_flag( context, SUPSYS_FLAG_BLOCK_CONNECT, 
		      SUPSYS_FLAG_CONNECT_AUTO, &flag );
  if( code )
    return supsys_test_error( context, code );
  if( flag ){
    //	ids = IDS_RESULT_FLAG_AUTO_SUPPORTED;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item support auto connections.")));
  }
  else{
    //	ids = IDS_RESULT_FLAG_AUTO_NOT;
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Item isn't supported auto connections.")));
  } 

  return SUP_ERR_NO;
}

TSupErr
supsys_test_list_connects(const TSupSysEContext *context)
{
  TSupErr code; /* ��� ��������. */
  size_t length; /* ������ ���� ����������. */
  int flag;
  TSupSysEConnectContext *list;
  TCHAR *connect;

  code = supsys_flag( context, SUPSYS_FLAG_BLOCK_CONNECT, 
		      SUPSYS_FLAG_CONNECT_MANY, &flag );
  if( code )
    return supsys_test_error( context, code );
  if( !flag )
    return SUP_ERR_NO;

  /*+ ����� ��������� � ������ �����. +*/
  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting available connections.")));

  code = supsys_connect_list_open( context, &length, &list );
  if( code )
    return supsys_test_error( context, code );
  connect = malloc( ( length + 1 ) * sizeof( TCHAR ) );
  if( !connect )
    {
      supsys_connect_list_close( list );
      return supsys_test_error( context, code );
    }
  code = supsys_connect_list_next( list, connect );
  while( !code )
    {
      DbTrace(DB_TRACE, (FTEXT(support_context_io, "%s\n"),connect));

      code = supsys_connect_list_next( list, connect );
    }
  free( connect );
  supsys_connect_list_close( list );
  DbTrace(DB_TRACE, (FTEXT(support_context_io, "Checking connection type OK.")));
  return SUP_ERR_NO;
}

/*++++
 * ������� ��������� ���� ���������� � �����������, ������� �� ���������
 * ��������.
 +*/
TSupErr supsys_test_info( 
    const TSupSysEContext *context )
{
    TSupErr code; /* ��� ��������. */

    /*+ ������������ ��������� ���������� � ������. +*/
    /*
    code = supsys_test_info_version( context );
    if( code )
	return code;
    */
    
    /*+ ������������ ��������� ��������. +*/
    code = supsys_test_info_nickname( context );
    if( code )
	return code;
    
    /*+ ������������ ��������� ����� ����������. +*/
    code = supsys_test_info_device_name( context );
    if( code )
	return code;
    
    /*+ ������������ ��������� �������� ������������� DLL. +*/
    /*
    code = supsys_test_info_company( context );
    if( code )
	return code;
    */
    
    /*+ ������������ ��������� ������� ����� �����������. +*/
    code = supsys_test_info_detail_name( context );
    if( code )
	return code;
    
    /*+ ������������ ��������� ���������� �������������. +*/
    code = supsys_test_info_producer( context );
    if( code )
	return code;
    
    /*+ ����������� ������� ��������� Copyright. +*/
    /*
    code = supsys_test_info_copyright( context );
    if( code )
	return code;
    */

    /*+ ������������ ������� ��������� ��������� ������. +*/
    code = supsys_test_info_flags( context );
    if( code )
	return code;

    code = supsys_test_list_connects( context );
    if( code )
	return code;

    return SUP_ERR_NO;
}

TSupErr supsys_test_resolve_auto(
    const TSupSysEContext *context )
{
    TSupErr code; /* ��� ��������. */
    size_t length; /* ������ ���� ����������. */
    int flag;
    TSupSysEConnectContext *list;
    TCHAR *connect;

    code = supsys_flag( context, SUPSYS_FLAG_BLOCK_CONNECT, 
			SUPSYS_FLAG_CONNECT_AUTO, &flag );
    if( code )
	return supsys_test_error( context, code );
    if( !flag )
	return SUP_ERR_NO;

    /*+ ����� ��������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Getting auto connection.")));

    code = supsys_connect_list_open( context, &length, &list );
    if( code )
	return supsys_test_error( context, code );
    supsys_connect_list_close( list );
    connect = malloc( ( length + 1 ) * sizeof( TCHAR ) );
    if( !connect )
	return supsys_test_error( context, code );

    code = supsys_connect_auto( context, &length, connect, 0, NULL );
    if( code )
	return supsys_test_error( context, code );

    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Auto connection is:%s"),connect));

    return SUP_ERR_NO;
}

TSupErr supsys_test_connect(
    TSupSysEContext *context )
{
    TSupErr code; /* ��� ��������. */
    /*+ ����� ��������� � ������ �����. +*/
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Connecting...")));

    code = supsys_connect( context );
    if( code )
	return supsys_test_error( context, code );
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Connect success.")));
    return SUP_ERR_NO;
}

TSupErr supsys_test_disconnect(
    TSupSysEContext *context )
{
    TSupErr code; /* ��� ��������. */
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Disconnecting...")));
    code = supsys_disconnect( context );
    if( code )
	return supsys_test_error( context, code );
    DbTrace(DB_TRACE, (FTEXT(support_context_io, "Disconnect success.")));
    return SUP_ERR_NO;
}
/* end of file: $Id: sstst.c 54318 2009-05-11 19:09:23Z dim $ */
