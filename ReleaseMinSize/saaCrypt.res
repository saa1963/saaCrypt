        ??  ??                  p  0   R E G I S T R Y   ??e       0        HKCR
{
	SaaCrypt.CrClass.1 = s 'CrClass Class'
	{
		CLSID = s '{1B4F737C-1E35-41BB-846B-D7D8EF17D5C0}'
	}
	SaaCrypt.CrClass = s 'CrClass Class'
	{
		CLSID = s '{1B4F737C-1E35-41BB-846B-D7D8EF17D5C0}'
		CurVer = s 'SaaCrypt.CrClass.1'
	}
	NoRemove CLSID
	{
		ForceRemove {1B4F737C-1E35-41BB-846B-D7D8EF17D5C0} = s 'CrClass Class'
		{
			ProgID = s 'SaaCrypt.CrClass.1'
			VersionIndependentProgID = s 'SaaCrypt.CrClass'
			ForceRemove 'Programmable'
			InprocServer32 = s '%MODULE%'
			{
				val ThreadingModel = s 'Apartment'
			}
			'TypeLib' = s '{7F5A44C7-37FD-4189-A2CA-9EC81EAB0AD5}'
		}
	}
}
?      ?? ??     0 	        ?4   V S _ V E R S I O N _ I N F O     ???               ?                         <   S t r i n g F i l e I n f o      0 4 0 9 0 4 B 0        C o m p a n y N a m e     H   F i l e D e s c r i p t i o n     s a a C r y p t   M o d u l e   6   F i l e V e r s i o n     1 ,   0 ,   0 ,   1     2 	  I n t e r n a l N a m e   s a a C r y p t     B   L e g a l C o p y r i g h t   C o p y r i g h t   2 0 0 3     B   O r i g i n a l F i l e n a m e   s a a C r y p t . D L L     @   P r o d u c t N a m e     s a a C r y p t   M o d u l e   :   P r o d u c t V e r s i o n   1 ,   0 ,   0 ,   1     (    O L E S e l f R e g i s t e r     D    V a r F i l e I n f o     $    T r a n s l a t i o n     	??  ,   T Y P E L I B   ??     0 	        MSFT      	      A                            ?       ????       ?             d   L  ?   ????   d     ????   p     ????   T     ????     ?   ????   ?  ?   ????   ?     ????   ?  8  ????   ?  ?  ????   D	     ????   ????    ????   T	  T   ????   ?	  $   ????   ????    ????   ????    ????   %"  ?	      ????                           `                        ????                  ????4" ?	                                 ?   @  ,       ,           ???? T             ????????    ?   ?????????   ????????????????????????????????????????????????x   ????`   ????????????H      ????????????????????0   ?DZ?7?A?ʞ??
?????????e?w?|Q???  ?w<?????????c?w?|Q???  ?w<?????????d?w?|Q???  ?w<?????????|sO5?A?k?????    ????0     ?      F   ????      ?      F   ????5K????lO????d   ????d      ????????      ?   x          - stdole2.tlbWWW?????????????????????????????????????????????   ?????????  ?????????????????????   ????????????,   ????????????????????????????????,  ????????????????L    ?  ????????????????????????????????  $  ?   ????????????????  ?   ?????????  ????????????????????????????????????????????????????????????????|   ?????????????   ?  ????????????l      ????   ????????@   ?????  ????????P   ?????????????????????????????????????????????  ????????????????????h   ???????????????????????????????????????????????????????? ׽SAACRYPTLibW    ????8?)CrClassWd   ????8? ICrClassd   ???? \[Init????????	 aContainerWWW???????? ?Provider???????? ?;retvalWWd   ???? 3CreateNewKeysWWWd   ???? DExportPublicKeyWd   ???? 9XSignMessageW???????? Q?msgWd   ???? ?VerifyMessageWWW???????? 8?sign???????? ??keyWd   ???? !CreateNewExchangeKeyd   ???? ?3ExportExchangeKeyWWWd   ???? ?vEncodeMessageWWW???????? ??pkeyd   ???? ރDecodeMessageWWWd   ????
 (oEncodeFileWW???????? R?fnameWWWd   ????
 ??DecodeFileWWd   ???? <NSaveRegisterd   ???? ??Init0WWWd   ???? ??Init1WWW saaCrypt 1.0 Type LibraryW CrClass ClassW ICrClass Interface method InitWWW method CreateNewKeysWW method ExportPublicKey method SignMessage method VerifyMessageWW method CreateNewExchangeKeyWWW method ExportExchangeKeyWW method EncodeMessageWW method DecodeMessageWW method EncodeFileW method DecodeFileW method SaveRegisterWWW method Init0WW method Init1 GOST 2001 @ ? @ ? >   Created by MIDL version 7.00.0500 at Wed Oct 15 13:34:10 2008
 ??HWW ? WW       ????0   D       H   L      @  D    ?     l 	D         @    ?P       ?h          |   
   ,   ?      L 	D        P       |   
   ,   ?    $ L 	D        h      |   
   8   ?    ( \ 	D        ?    ??         |   
   P   ?    , | 	D        ?    ??       ?      ?         |   
   ,   ?    0 L 	D        ?       |   
   ,   ?    4 L 	D        ?      |   
   D   ?    8 l 	D        ?    ??       ??        |   
   D   ?    < l 	D            ??       ??        |   
   D 	  ?    @ l 	D	           ??      ??          |   
   D 
  ?    D l 	D
        ,   ??      ??         |   
   ,   ?    H D 	        @   ??     D   ?    L l 	D        X   ?P       ?h          |   
   D   ?    P l 	D        h   ?P       ?h          |   
                           	   
               @   ?   ?   ?   ?   ,  L  l  ?  ?  ?  ?    $      D   p   ?   ?   $  P  |  ?    H  ?  ?  ?  0       ?? ??     0	                 s a a C r y p t                       