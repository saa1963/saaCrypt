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
' В данном script-примере осуществляется зашифрование и расшифрование
' содержимого файла при помощи инструментария CAPICOM.  
'------------------------------------------------------------------------------

Option Explicit

Const ForReading = 1, ForWriting = 2

' Команды.
Const Unknown                                      = 0
Const Encrypt                                      = 1
Const Decrypt                                      = 2
                                                   
' CAPICOM константы.                                                   
Const CAPICOM_CURRENT_USER_STORE                   = 2
                                                   
Const CAPICOM_CERTIFICATE_FIND_TIME_VALID          = 9

Const CAPICOM_ENCRYPTION_ALGORITHM_RC2             = 0
        
Const CAPICOM_ENCRYPTION_KEY_LENGTH_MAXIMUM        = 0

' Аргументы командной строки.
Dim Command       : Command        = Unknown
Dim StoreLocation : StoreLocation  = CAPICOM_CURRENT_USER_STORE
Dim StoreName     : StoreName      = "MY"
Dim CertFile      : CertFile       = Null
Dim Algorithm     : Algorithm      = Null 'CAPICOM_ENCRYPTION_ALGORITHM_RC2 - в CryptoPro CSP не поддерживается
Dim KeyLength     : KeyLength      = Null 'CAPICOM_ENCRYPTION_KEY_LENGTH_MAXIMUM - в CryptoPro CSP не поддерживается
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

' Поиск сертификата (сертификатов) получателя.
Dim Certificates
If Command = Encrypt Then
   
   ' Открытие хранилища сертификатов.
   Dim Store
   Set Store = CreateObject("CAPICOM.Store")
   Store.Open StoreLocation, StoreName
   
  ' В качестве кандитатов на сертификаты получателя 
  ' изначально определяются все сертификаты хранилища.
   Set Certificates = Store.Certificates
   
   ' Из них выбираются только сертификаты, действительные в настоящее время.
   If Certificates.Count > 0 Then
      Set Certificates = Certificates.Find(CAPICOM_CERTIFICATE_FIND_TIME_VALID, Now)
   End If
   
   ' Выбор сертификата (сертификатов). Если после проведенной фильтрации осталось несколько кандидатов,
   ' то пользователю предоставляется выбор одного из них или нескольких (при помощи диалогового окна).
   Select Case Certificates.Count
   Case 0
      Wscript.Stdout.Writeline "Error: No recipient's certificate can be found."
      Wscript.Quit(1)
   
   Case 1
      ' Остался только один, потому он и будет использоваться.
         
   Case Else
      ' Пользователь выбирает сертификат (сертификаты) получателя.
      Set Certificates = Certificates.Select("CEnvelop.vbs", "Please select one or more recipient's certificate(s).", True)
      If Certificates.Count = 0 Then
         Wscript.Stdout.Writeline "Error: Certificate selection dialog was cancelled."
         Wscript.Quit(2)
      End If
    
   End Select

   ' Освобождение ресурсов.
   Set Store = Nothing   
End If

' Выполнение требуемой операции.
Select Case Command
Case Encrypt   
   DoEncryptCommand FileNames, Certificates, Algorithm, KeyLength

Case Decrypt
   DoDecryptCommand FileNames
      
End Select

' Освобождение ресурсов.
Set Certificates = Nothing
     
Wscript.Quit(0)

' Конец Main


'------------------------------------------------------------------------------
' Функиция DoSignCommand  зашифровывает содержимое файла FileNames(0) 
' и записывает зашифрованные данные в FileNames(1).
'------------------------------------------------------------------------------

Sub DoEncryptCommand (FileNames, Certificates, Algorithm, KeyLength)
   Dim Content
   Dim Message
   Dim EnvelopedData
   
   ' Создание объекта EnvelopedData.
   Set EnvelopedData = CreateObject("CAPICOM.EnvelopedData")
   
   ' Определение алгоритма и размера ключа зашифрования.
   'EnvelopedData.Algorithm.Name = Algorithm - в CryptoPro CSP не поддерживается
   'EnvelopedData.Algorithm.KeyLength = KeyLength - - в CryptoPro CSP не поддерживается
   
   ' Добавление получателей.
   Dim Recipient
   For Each Recipient In Certificates 
      EnvelopedData.Recipients.Add Recipient
   Next
   
   ' Распечатка информативного сообщения.
   Wscript.Stdout.Writeline "Enveloping text file " & FileNames(0) & "."
   Wscript.Stdout.Writeline
     
   ' Загрузка содержимого текстового файла для зашифрования.
   LoadFile FileNames(0), Content
   
   ' Зашифрование.
   EnvelopedData.Content = Content
   Message = EnvelopedData.Encrypt
   
   ' Сохранение зашифрованных данных в FileNames(1).
   SaveFile FileNames(1), Message
   Wscript.Stdout.Writeline "Successful - Enveloped message saved to " & FileNames(1) & "."

   ' Освобождение ресурсов.
   Set EnvelopedData = Nothing
   
End Sub ' Конец DoEncryptCommand


'------------------------------------------------------------------------------
' Функция DoCoSignCommand расшифровывает зашифрованный файл FileNames(0)и сохраняет
' расшифрвоанные данные в FileNames(1).
'------------------------------------------------------------------------------

Sub DoDecryptCommand (FileNames)
   Dim Message
   Dim EnvelopedData
   
   ' Создание объекта EnvelopedData.
   Set EnvelopedData = CreateObject("CAPICOM.EnvelopedData")
   
   ' Распечатка информативного сообщения.
   Wscript.Stdout.Writeline "Decrypting enveloped text file " & FileNames(0) & "."
   Wscript.Stdout.Writeline
   
   ' Загрузка зашифрованных данных.
   LoadFile FileNames(0), Message
   
   ' Расшифрование.
   EnvelopedData.Decrypt(Message)
   
         
   ' Сохранение расшифрованных данных в FileNames(1).
   SaveFile FileNames(1), EnvelopedData.Content
   Wscript.Stdout.Writeline "Successful - Decrypted content saved to " & FileNames(1) & "."

   ' Освобождение ресурсов.
   Set EnvelopedData = Nothing
   
End Sub ' Конец DoDecryptCommand


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
    
End Sub ' End SaveFile


'------------------------------------------------------------------------------
' Функция ParseCommandLine разбирает командную строку и устанавливает
' опции согласно ей.
'------------------------------------------------------------------------------

Sub ParseCommandLine

   ' Константы для разбора состояний, задаваемых командной строкой.
   Const ARG_STATE_COMMAND    = 0
   Const ARG_STATE_OPTIONS    = 1    
   Const ARG_STATE_FILENAME   = 8
   
   ' Разбор командной строки.
   Dim Arg
   Dim ArgState : ArgState = ARG_STATE_COMMAND
   
   For Each Arg In Wscript.Arguments
      Select Case ArgState
      Case ARG_STATE_COMMAND
         Select Case UCase(Arg) 
         Case "ENCRYPT"
            Command = Encrypt

         Case "DECRYPT"
            Command = Decrypt

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
         Wscript.Stdout.Writeline "Internal script error: Unknown argument state (" & CStr(ArgState) & ") encountered."
         Wscript.Quit(-3)
         
      End Select
   Next
   
   ' Проверка правильности состояния.
   If ArgState <> ARG_STATE_FILENAME Then
      DisplayUsage
   End If

   ' Проверка опций.
   Select Case Command
   Case Encrypt
     
      ' Функция зашифрвоаниядолжна иметь входной и выходной файлы.
      If (UBound(FileNames) - LBound(FileNames) + 1) <> 2 Then
         DisplayUsage
      End If
   
   Case Decrypt
      ' Функция расшифрования должна иметь входной и выходной файлы.
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
         Wscript.Stdout.Writeline "Usage: CEnvelop Command InFile OutFile"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Command:"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  Encrypt                    -- Envelop a text file"
         Wscript.Stdout.Writeline "  Decrypt                    -- Decrypt an enveloped text file"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "For help on a specific command, enter ""CEnvelop Command -?"""
         
      Case Encrypt
         Wscript.Stdout.Writeline "Usage: CEnvelop Encrypt ContentFile EnvelopedFile"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "The Encrypt command is used to envelop a text file to multiple recipients, of"
         Wscript.Stdout.Writeline "which the certificates must either reside in the AddressBook store, or in the"
         Wscript.Stdout.Writeline "specified certificate file. Enveloping protects the data from being read by"
         Wscript.Stdout.Writeline "others except the intended recipients."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Parameters:"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  -?                         -- This help screen"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  ContentFile                -- Text file to be enveloped"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  EnvelopedFile              -- Enveloped text file"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Note: All non-fatal invalid options for this specific command will be ignored,"
         Wscript.Stdout.Writeline "      and the ** symbol indicates option can be listed multiple times."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "      If there is only one certificate found matching the requirement, that"
         Wscript.Stdout.Writeline "      particular certificate will be used as the recipient's certificate."
	 Wscript.Stdout.Writeline "      However, if there is more than one certificate matching the requirement,"
         Wscript.Stdout.Writeline "      a dialog will be displayed to allow selection of multiple recipient's"
         Wscript.Stdout.Writeline "      certificates."
         Wscript.Stdout.Writeline
        
       Case Decrypt
         Wscript.Stdout.Writeline "Usage: CEnvelop Decrypt EnvelopedFile ContentFile"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "The Decrypt command is used to decrypt an enveloped text file. The decryption"
         Wscript.Stdout.Writeline "process will automatically search for the recipient's certificate in MY store"
         Wscript.Stdout.Writeline "(Current User or Local Machine)."
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "Parameters:"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  -?                         -- This help screen"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  EnvelopedFile              -- Enveloped text file"
         Wscript.Stdout.Writeline
         Wscript.Stdout.Writeline "  ContentFile                -- Decrypted text file"
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

