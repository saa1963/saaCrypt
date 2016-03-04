
��� ������ �������� ��� Windows ����������:
___________________________________________

- ���������� ���������� ��������� VisualStudio .Net (vcvars.bat);
- ���������� ���������� ��������� PlatformSDK (SetEnv.bat) ��� ��������������� ��;
- ���������� ���������� ��������� DDK (setenv.bat) ��� ��������������� ��;
  (���������, ��� ������ ���������);
- �������� � ���������� ��������� INCLUDE ���� � INCLUDE ������ CSP;
- �������� � ���������� INCLUDE ���� � msxml5.dll, capicom.dll
  (��������� ��� �������� ������� xml);

- ������� nmake 
- ��� ������� ������ nmake clean
- ��� ������ �������� - build � ����� KernelMode

��� ������ �� ����� VisualStudio:
_________________________________

- ���������, ��� ��������� ��� ����������� ���� � 
  tools->options->Projects and Solutions->VC++ Directories
- ��� �������� ���������� ������ ���� � include ������ CSP (sdk\include)

  
��� ������ �������� ��� UNIX ����������:
________________________________________

- ���������� ����� ������������ (cpcsp-devel ��� CPROcspd, �� ��������)
- � ����� � ��������� (/opt/cprocsp/src/doxygen/"CSP � SSPI")
  eval `../setenv.sh --64`; make -f Makefile.unix
- ��� ������� ������ eval `../setenv.sh --64`; make -f Makefile.unix clean

- ������� CEnvelop, CSignData, Request, XMLSignForVerify, 
  XMLVerify, XMLVerifyTwoSigs �������������� ������ ��� win ��������.


������ ��� ������ � ������� ������ (unix �������):
__________________________________________________

cd /opt/cprocsp/src/doxygen/CSP
eval `../setenv.sh --64`; make -f Makefile.unix clean
eval `../setenv.sh --64`; make -f Makefile.unix
/opt/cprocsp/src/doxygen/SSPI
eval `../setenv.sh --64`; make -f Makefile.unix clean
eval `../setenv.sh --64`; make -f Makefile.unix
cd /tmp
echo a > /tmp/test.dat
/opt/cprocsp/src/doxygen/CSP/CreatingHash/CreatingHash /tmp/test.dat
/opt/cprocsp/bin/amd64/csptestf -keyset -newkeyset -container '\\.\hdimage\test' # ������ ������ ������
/opt/cprocsp/src/doxygen/CSP/CryptAcquireContext/CryptAcquireContext # ������ ������ ������
/opt/cprocsp/src/doxygen/CSP/CryptMessages/CryptMessages
/opt/cprocsp/src/doxygen/CSP/CryptMsgSign/CryptMsgSign
/opt/cprocsp/src/doxygen/CSP/DerivingSessionKey/DerivingSessionKey
/opt/cprocsp/src/doxygen/CSP/DuplicatingHash/DuplicatingHash
/opt/cprocsp/src/doxygen/CSP/DuplicatingSessionKey/DuplicatingSessionKey
/opt/cprocsp/src/doxygen/CSP/EnumeratingProviders/EnumeratingProviders
/opt/cprocsp/src/doxygen/CSP/SessionKeyParameters/SessionKeyParameters
/opt/cprocsp/src/doxygen/CSP/SigningHash/SigningHash
/opt/cprocsp/src/doxygen/CSP/SILENT/SILENT
/opt/cprocsp/bin/amd64/csptestf -keyset -newkeyset -container '\\.\hdimage\Sender' # ������ ������ ������
/opt/cprocsp/bin/amd64/csptestf -keyset -newkeyset -container '\\.\hdimage\Responder' # ������ ������ ������
/opt/cprocsp/src/doxygen/CSP/ExportInFile/ExportInFile Sender
/opt/cprocsp/src/doxygen/CSP/ExportInFile/ExportInFile Responder
cp test.dat source.txt
/opt/cprocsp/src/doxygen/CSP/EncryptFile/EncryptFile
/opt/cprocsp/src/doxygen/CSP/DecryptFile/DecryptFile
/opt/cprocsp/src/doxygen/CSP/ExportingSessionKey/ExportingSessionKey
/opt/cprocsp/bin/amd64/csptestf -keyset -silent -deletekeyset -container '\\.\hdimage\test'
/opt/cprocsp/bin/amd64/csptestf -keyset -silent -deletekeyset -container '\\.\hdimage\Sender'
/opt/cprocsp/bin/amd64/csptestf -keyset -silent -deletekeyset -container '\\.\hdimage\Responder'
/opt/cprocsp/src/doxygen/SSPI/WebClient/WebClient -s���_�������


������ �������� � ����������� ��� Mac OS
__________________________________________________
�� Mac OS �������������� ��� ������� ��������� CSP: �������� � ������������, ��� �� ��������� Unix, � ���������
����������.
��� ������ �� Mac OS � ����������� �������� ���� /opt/cprocsp/src/doxygen/examples.unix.mk �� 
/opt/cprocsp/src/doxygen/examples.macos.mk � �������� ���������� ������� ��� ������ � ������� ������ �� Unix.