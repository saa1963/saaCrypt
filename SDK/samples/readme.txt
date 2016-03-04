
Для сборки примеров под Windows необходимо:
___________________________________________

- установить переменные окружения VisualStudio .Net (vcvars.bat);
- установить переменные окружения PlatformSDK (SetEnv.bat) для соответствующей ОС;
- установить переменные окружения DDK (setenv.bat) для соответствующей ОС;
  (требуется, при сборке драйверов);
- добавить в переменную окружения INCLUDE путь к INCLUDE файлам CSP;
- добавить в переменную INCLUDE путь к msxml5.dll, capicom.dll
  (требуется для примеров подписи xml);

- запусть nmake 
- для очистки сборки nmake clean
- для сборки драйвера - build в папке KernelMode

При сборке из среды VisualStudio:
_________________________________

- Убедиться, что прописаны все необходимые пути в 
  tools->options->Projects and Solutions->VC++ Directories
- Для проектов установить нужные пути к include файлам CSP (sdk\include)

  
Для сборки примеров под UNIX необходимо:
________________________________________

- установить пакет разработчика (cpcsp-devel или CPROcspd, по ситуации)
- в папке с примерами (/opt/cprocsp/src/doxygen/"CSP и SSPI")
  eval `../setenv.sh --64`; make -f Makefile.unix
- для очистки сборки eval `../setenv.sh --64`; make -f Makefile.unix clean

- Примеры CEnvelop, CSignData, Request, XMLSignForVerify, 
  XMLVerify, XMLVerifyTwoSigs предназначенны только для win платформ.


Рецепт для сборки и запуска тестов (unix вариант):
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
/opt/cprocsp/bin/amd64/csptestf -keyset -newkeyset -container '\\.\hdimage\test' # ввести пустой пароль
/opt/cprocsp/src/doxygen/CSP/CryptAcquireContext/CryptAcquireContext # ввести пустой пароль
/opt/cprocsp/src/doxygen/CSP/CryptMessages/CryptMessages
/opt/cprocsp/src/doxygen/CSP/CryptMsgSign/CryptMsgSign
/opt/cprocsp/src/doxygen/CSP/DerivingSessionKey/DerivingSessionKey
/opt/cprocsp/src/doxygen/CSP/DuplicatingHash/DuplicatingHash
/opt/cprocsp/src/doxygen/CSP/DuplicatingSessionKey/DuplicatingSessionKey
/opt/cprocsp/src/doxygen/CSP/EnumeratingProviders/EnumeratingProviders
/opt/cprocsp/src/doxygen/CSP/SessionKeyParameters/SessionKeyParameters
/opt/cprocsp/src/doxygen/CSP/SigningHash/SigningHash
/opt/cprocsp/src/doxygen/CSP/SILENT/SILENT
/opt/cprocsp/bin/amd64/csptestf -keyset -newkeyset -container '\\.\hdimage\Sender' # ввести пустой пароль
/opt/cprocsp/bin/amd64/csptestf -keyset -newkeyset -container '\\.\hdimage\Responder' # ввести пустой пароль
/opt/cprocsp/src/doxygen/CSP/ExportInFile/ExportInFile Sender
/opt/cprocsp/src/doxygen/CSP/ExportInFile/ExportInFile Responder
cp test.dat source.txt
/opt/cprocsp/src/doxygen/CSP/EncryptFile/EncryptFile
/opt/cprocsp/src/doxygen/CSP/DecryptFile/DecryptFile
/opt/cprocsp/src/doxygen/CSP/ExportingSessionKey/ExportingSessionKey
/opt/cprocsp/bin/amd64/csptestf -keyset -silent -deletekeyset -container '\\.\hdimage\test'
/opt/cprocsp/bin/amd64/csptestf -keyset -silent -deletekeyset -container '\\.\hdimage\Sender'
/opt/cprocsp/bin/amd64/csptestf -keyset -silent -deletekeyset -container '\\.\hdimage\Responder'
/opt/cprocsp/src/doxygen/SSPI/WebClient/WebClient -sимя_сервера


Сборка примеров с фреймворком для Mac OS
__________________________________________________
На Mac OS поддерживается два вариант включения CSP: линковка с библиотеками, как на остальных Unix, и включение
фреймворка.
Для сборки на Mac OS с фреймворком замените файл /opt/cprocsp/src/doxygen/examples.unix.mk на 
/opt/cprocsp/src/doxygen/examples.macos.mk и соберите аналогично рецепту для сборки и запуска тестов на Unix.