
saaCryptps.dll: dlldata.obj saaCrypt_p.obj saaCrypt_i.obj
	link /dll /out:saaCryptps.dll /def:saaCryptps.def /entry:DllMain dlldata.obj saaCrypt_p.obj saaCrypt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del saaCryptps.dll
	@del saaCryptps.lib
	@del saaCryptps.exp
	@del dlldata.obj
	@del saaCrypt_p.obj
	@del saaCrypt_i.obj
