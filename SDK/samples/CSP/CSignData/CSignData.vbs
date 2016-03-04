'
' Copyright(C) 2000-2010 Проект ИОК
'
' Этот файл содержит информацию, являющуюся
' собственностью компании Крипто Про.
'
' Любая часть этого файла не может быть скопирована,
' исправлена, переведена на другие языки,
' локализована или модифицирована любым способом,
' откомпилирована, передана по сети с или на
' любую компьютерную систему без предварительного
' заключения соглашения с компанией Крипто Про.
'
' Программный код, содержащийся в этом файле, предназначен
' исключительно для целей обучения и не может быть использован
' для защиты информации.
'
' Компания Крипто-Про не несет никакой
' ответственности за функционирование этого кода.
'
' Начало примера (не следует удалять данный комментарий, он используется 
' для автоматической сборки документации)
'------------------------------------------------------------------------------
' В данном script-примере осуществляется подпись и проверка подписи
' содержимого файла при помощи инструментария CAPICOM.  
'------------------------------------------------------------------------------

Option Explicit

Const ForReading = 1, ForWriting = 2


' Команды.
Const Unknown                                               = 0
Const Sign		                                    = 1
Const CoSign                                                = 2
Const Verify                                                = 3
                                                            
' CAPICOM константы.                                                            
Const CAPICOM_CURRENT_USER_STORE                            = 2
                                                            
Const CAPICOM_CERTIFICATE_FIND_EXTENDED_PROPERTY            = 6
Const CAPICOM_CERTIFICATE_FIND_TIME_VALID                   = 9

Const CAPICOM_VERIFY_SIGNATURE_ONLY                         = 0

Const CAPICOM_CERTIFICATE_INCLUDE_CHAIN_EXCEPT_ROOT         = 0

Const CAPICOM_PROPID_KEY_PROV_INFO                          = 2

' Аргументы командной строки.
Dim Command          : Command                              = Unknown
Dim StoreLocation    : StoreLocation                        = Null
Dim VerifyFlag       : VerifyFlag                           = CAPICOM_VERIFY_SIGNATURE_ONLY
Dim IncludeOption    : IncludeOption                        = CAPICOM_CERTIFICATE_INCLUDE_CHAIN_EXCEPT_ROOT
Dim bDetached        : bDetached                            = False
                                                            
' Фильтр.                          
Dim FileNames()

' В первую очередь осуществляется проверка того, что script выполняется при помощи CScript.exe.
If InStr(1, UCase(Wscript.FullName), "CSCRIPT.EXE", vbTextCompare) = 0 Then
   Wscript.Echo "This script can only be executed by CScript.exe." & vbCRLF & vbCRLF &_
                "You can either:" & vbCRLF & vbCRLF & _
                "1. Set CScript.exe as the default (Run CScript //h:cscript), or" & vbCRLF & _
                "2. Run CScript.exe directly as in, CScript " & Wscript.ScriptName & "."
   Wscript.Quit(-1)
End If

' Разбор командной сроки.
ParseCommandLine

' Поиск сертификата, на котором будет осуществляться операция подписи.       
Dim Signer
Set Signer = CreateObject("CAPICOM.Signer")

' Определение свойств операции подписи.
If Command = Sign OR Command = CoSign Then
      Dim iIndex
      Dim Store
      Dim Certificates
      Dim StoreName : StoreName = "MY"

      ' Открытие соответствующего хранилища.
      Set Store = CreateObject("CAPICOM.Store")
      
      StoreLocation = CAPICOM_CURRENT_USER_STORE
      
      Store.Open StoreLocation, StoreName
      
      ' В качестве кандитатов на сертификат, на котором будет осуществлена подпись, 
      ' изначально определяются все сертификаты хранилища.
      Set Certificates = Store.Certificates
      
      ' Из них не рассматриваются сертификаты, в которых отсутствует закрытый ключ.
      If Certificates.Count > 0 Then
         Set Certificates = Certificates.Find(CAPICOM_CERTIFICATE_FIND_EXTENDED_PROPERTY, _
         CAPICOM_PROPID_KEY_PROV_INFO)
      End If
      
      ' Из них выбираются только сертификаты, действительные в настоящее время.
      If Certificates.Count > 0 Then
         Set Certificates = Certificates.Find(CAPICOM_CERTIFICATE_FIND_TIME_VALID, Now)
      End If
      
      ' Выбор сертификата для подписи. Если после проведенной фильтрации осталось несколько кандидатов,
      ' то пользователю предоставляется выбор одного из них (при помощи диалогового окна).
      Select Case Certificates.Count
      Case 0
         Wscript.Stdout.Writeline "Error: No signing certificate can be found."
         Wscript.Quit(1)
   
      Case 1
         Signer.Certificate = Certificates(1)
   
      Case Else
         Set Certificates = Certificates.Select("CSignData.vbs", "Please select a certificate to sign " _
         & FileNames(0) & ".")
         If Certificates.Count = 0 Then
            Wscript.Stdout.Writeline "Error: Certificate selection dialog was cancelled."
            Wscript.Quit(2)
         End If
         Signer.Certificate = Certificates(1)
       
      End Select
      
      Set Certificates = Nothing
      Set Store = Nothing
   
   Signer.Options = IncludeOption
   
   End If

' Выполнение требуемой операции.
Select Case Command
Case Sign   
   DoSignCommand FileNames, bDetached, Signer

Case CoSign
   DoCoSignCommand FileNames, bDetached, Signer
   
Case Verify
   DoVerifyCommand FileNames, bDetached, VerifyFlag
   
End Select

' Освобождение ресурсов.
Set Signer = Nothing
     
Wscript.Quit(0)

' Конец main

 
'------------------------------------------------------------------------------
' Функиция DoSignCommand  подписывает содержимое файла FileNames(0) 
' и записывает подписанные данные в FileNames(1).
'------------------------------------------------------------------------------

Sub DoSignCommand (FileNames, bDetached, Signer)
   Dim Content
   Dim Message
   Dim SignedData

   ' Создание объекта SignedData.
   Set SignedData = CreateObject("CAPICOM.SignedData")
   
   ' Распечатка информативного сообщения.
   Wscript.Stdout.Writeline "Signing text file " & FileNames(0) & "."
   Wscript.Stdout.Writeline
   
   ' Загрузка содержимого файла для подписи.
   LoadFile FileNames(0), Content
   
   ' Подпись.
   SignedData.Content = Content
   Message = SignedData.Sign(Signer, bDetached)
   
   ' Сохранение подписанных данных в FileNames(1).
   SaveFile FileNames(1), Message
   Wscript.Stdout.Writeline "Successful - Signed message saved to " & FileNames(1) & "."

   ' Освобождение ресурсов.
   Set SignedData = Nothing

End Sub 'Конец DoSignCommand


'------------------------------------------------------------------------------
' Функция DoCoSignCommand подписывает текстовый файл FileNames(0)и сохраняет
' заново подписанное содержимое в FileNames(1).
'------------------------------------------------------------------------------

Sub DoCoSignCommand (FileNames, bDetached, Signer)
   Dim Content
   Dim Message
   Dim SignedData
   
   ' Создание объекта SignedData.
   Set SignedData = CreateObject("CAPICOM.SignedData")
   
   ' Распечатка информативного сообщения.
   Wscript.Stdout.Writeline "CoSigning text file " & FileNames(0) & "."
   Wscript.Stdout.Writeline
   
   
   ' Загрузка подписанных данных.
   LoadFile FileNames(0), Message
   
   ' Обработка отделенных данных
   If True = bDetached Then
      LoadFile FileNames(1), Content
      SignedData.Content = Content
      Wscript.Stdout.Writeline "content file " & FileNames(1) & "." 
   End If

   ' Проверка подписанных данных.
   SignedData.Verify Message, bDetached, VerifyFlag

   ' Повторная подпись.
   Message = SignedData.CoSign(Signer, bDetached)
   
   ' Сохранение внось подписанных данных в FileNames(0).
   SaveFile FileNames(0), Message
   Wscript.Stdout.Writeline "Successful - CoSigned message saved to " & FileNames(0) & "."
   
   ' Освобождение ресурсов.
   Set SignedData = Nothing

End Sub ' Конец DoCoSignCommand


'------------------------------------------------------------------------------
' Функция DoVerifyCommand проверяется подписанный текстовый файл.
'------------------------------------------------------------------------------

Sub DoVerifyCommand (FileNames, bDetached, VerifyFlag)
   Dim Content
   Dim Message
   Dim SignedData

   ' Создание объекта SignedData.
   Set SignedData = CreateObject("CAPICOM.SignedData")
   
   ' Распечатка информативного сообщения.
   Wscript.Stdout.Writeline "Verifying signed text file " & FileNames(0) & ", please wait..."
   Wscript.Stdout.Writeline
   
   ' Загрузка подписанных данных для проверки.
   LoadFile FileNames(0), Message

   ' Обработка отделенных данных
   If True = bDetached Then
   LoadFile FileNames(1), Content
   SignedData.Content = Content
   End If
   
   
   ' Проверка подписи.
   SignedData.Verify Message, bDetached, VerifyFlag

      
   ' Сохранение проверенного содержимого в FileNames(1).
   If False = bDetached Then
      SaveFile FileNames(1), SignedData.Content
      Wscript.Stdout.Writeline "Successful - Verified content saved to " & FileNames(1) & "."
   Else
      Wscript.Stdout.Writeline "Successful."
   End If
      
   ' Освобождение ресурсов.
   Set SignedData = Nothing

End Sub ' Конец DoVerifyCommand

'------------------------------------------------------------------------------
'Функция LoadFile читает содержимое текстового файла.
'------------------------------------------------------------------------------

Sub LoadFile (FileName, Buffer)   
    Dim fso
    Set fso = CreateObject("Scripting.FileSystemObject")

    If Not fso.FileExists(FileName) Then
        Wscript.Stdout.Writeline "Error: File " & FileName & " not found."
        Wscript.Quit(-5)
    End If
   
    Dim ts
    Set ts = fso.OpenTextFile(FileName, ForReading)  
    Buffer = ts.ReadAll
    
End Sub ' Конец LoadFile


'------------------------------------------------------------------------------
' Функция SaveFile сохраняет строку в файл.
'------------------------------------------------------------------------------

Sub SaveFile (FileName, Buffer)
    Dim fso
    Set fso = CreateObject("Scripting.FileSystemObject")
    
    Dim ts
    Set ts = fso.OpenTextFile(FileName, ForWriting, True)
    ts.Write Buffer
    
End Sub ' Конец SaveFile


'------------------------------------------------------------------------------
' Функция ParseCommandLine разбирает командную строку и устанавливает
' опции согласно ей.
'------------------------------------------------------------------------------

Sub ParseCommandLine

   ' Константы для разбора состояний, задаваемых командной строкой.
   Const ARG_STATE_COMMAND                   = 0
   Const ARG_STATE_OPTIONS                   = 1    
   Const ARG_STATE_FILENAME                  = 13
   
   ' Разбор командной строки.
   Dim Arg
   Dim ArgState : ArgState = ARG_STATE_COMMAND
   
   For Each Arg In Wscript.Arguments
      Select Case ArgState
      Case ARG_STATE_COMMAND
         Select Case UCase(Arg) 
         Case "SIGN"
            Command = Sign

         Case "COSIGN"
            Command = CoSign

         Case "VERIFY"
            Command = Verify

         Case Else
            DisplayUsage
            
         End Select
         
         ArgState = ARG_STATE_OPTIONS
         
      Case ARG_STATE_OPTIONS
         Select Case UCase(Arg) 
         Case "-?", "/?"
            DisplayUsage
            
         Case Else
            If Left(Arg, 1) = "-"  OR Left(Arg, 1) = "/" Then
               DisplayUsage
            Else
               ReDim FileNames(0)              
               FileNames(0) = Arg
            End If
            ArgState = ARG_STATE_FILENAME
            
         End Select

      Case ARG_STATE_FILENAME
         If Left(Arg, 1) = "-"  OR Left(Arg, 1) = "/" Then
            DisplayUsage
         Else
            ReDim Preserve FileNames(UBound(FileNames) + 1)
         End If
         FileNames(UBound(FileNames)) = Arg
          
      Case Else
         Wscript.Stdout.Writeline "Internal script error: Unknown argument state (" _
         & CStr(ArgState) & ") encountered."
         Wscript.Quit(-3)
         
      End Select
   Next
   
   ' Проверка правильности состояния.
   If ArgState <> ARG_STATE_FILENAME Then
      DisplayUsage
   End If

   ' Проверка опций.
   Select Case Command
   Case Sign
      ' Функция подписи должна иметь входной и выходной файлы.
      If (UBound(FileNames) - LBound(FileNames) + 1) <> 2 Then
         DisplayUsage
      End If
   
   
   Case Verify
      ' Функция проверки подписи должна иметь входной и выходной файлы.
      If (UBound(FileNames) - LBound(FileNames) + 1) <> 2 Then
         DisplayUsage
      End If
      
   End Select
   
End Sub ' Конец ParseCommandLine


'------------------------------------------------------------------------------
' Функция DisplayUsage распечатывает информацию об использовании данного
' примера, и затем осуществляет выход с ошибкой.
'------------------------------------------------------------------------------

Sub DisplayUsage

   Select Case Command
      Case Unknown
         Wscript.Stdout.Writeline "Usage: CSignData Command File1 [File2]"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Command:"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  Sign                   -- Sign a text file"
         Wscript.Stdout.Writeline "  CoSign                 -- CoSign a signed text file"
         Wscript.Stdout.Writeline "  Verify                 -- Verify a signed text file"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "For help on a specific command, enter ""CSignData Command -?"""
         
      Case Sign
         Wscript.Stdout.Writeline "Usage: CSignData Sign ContentFile SignedFile"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "The Sign command is used to sign a text file. Signing protects a file from"
         Wscript.Stdout.Writeline "tampering, and allows user to verify the signer based on signing certificate."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "For non-detached signing, both the content and signature will be saved to"
         Wscript.Stdout.Writeline "SignedFile. For detached signing, only the signature is saved to SignedFile."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Parameters:"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  -?                     -- This help screen"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  ContentFile            -- Text file to be signed"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  SignedFile             -- Signed file (contains signature only if detached)"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Note: All non-fatal invalid options for this specific command will be ignored,"
         Wscript.Stdout.Writeline "      and the ** symbol indicates option can be listed multiple times."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "      If there is only one certificate found in the MY store or PFX that"
         Wscript.Stdout.Writeline "      matches the requirement, that particular certificate will be used."
	 Wscript.Stdout.Writeline "      However, if there is more than one certificate matching the requirement,"
         Wscript.Stdout.Writeline "      a dialog will be displayed to allow selection of the signing certificate."
         Wscript.Stdout.Writeline
         
      Case CoSign
         Wscript.Stdout.Writeline "Usage: CSignData CoSign SignedFile [ContentFile]"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "The CoSign command is used to cosign a signed text file. CoSigning provides the"
         Wscript.Stdout.Writeline "same type of benefits as signing, with an additional signature."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "For non-detached cosigning, both the content and signatures will be saved to"
         Wscript.Stdout.Writeline "SignedFile. For detached cosigning, only the signatures are saved to"
         Wscript.Stdout.Writeline "SignedFile."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Parameters:"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  -?                     -- This help screen"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  SignedFile             -- Signed file (contains signature only if detached)"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  ContentFile            -- Text file (required if detached)"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Note: All non-fatal invalid options for this specific command will be ignored,"
         Wscript.Stdout.Writeline "      and the ** symbol indicates option can be listed multiple times."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "      If there is only one certificate found in the MY store or PFX that"
         Wscript.Stdout.Writeline "      matches the requirement, that particular certificate will be used."
	 Wscript.Stdout.Writeline "      However, if there is more than one certificate matching the requirement,"
         Wscript.Stdout.Writeline "      a dialog will be displayed to allow selection of the signing certificate."
         Wscript.Stdout.Writeline
         
       Case Verify
         Wscript.Stdout.Writeline "Usage: CSignData Verify SignedFile ContentFile"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "The Verify command is used to verify signed text file. Verification checks"
         Wscript.Stdout.Writeline "integrity of the signed file and determines if the signing certificate is"
         Wscript.Stdout.Writeline "valid and issued by a trusted party."
         Wscript.Stdout.Writeline 
         Wscript.Stdout.Writeline "For non-detached signed file, the content will be extracted and saved to"
         Wscript.Stdout.Writeline "ContentFile. For detached signed file, the ContentFile is not modified."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Parameters:"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  -?                     -- This help screen"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  SignedFile             -- Signed file (contains signature only if detached)"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  ContentFile            -- Text file (will not be over written if detached)"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Note: All non-fatal invalid options for this specific command will be ignored."
         Wscript.Stdout.Writeline
         
      Case Else
         Wscript.Stdout.Writeline "Internal script error: Unknown help state (Command = " & CStr(Command) & ")."
         Wscript.Quit(-2)
         
   End Select

   Wscript.Quit(-1)
   
End Sub ' Конец DisplayUsage
' Конец примера 
' (не следует удалять данный комментарий, он используется 
'  для автоматической сборки документации)
