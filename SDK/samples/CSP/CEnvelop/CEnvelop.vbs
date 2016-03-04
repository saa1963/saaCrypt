'
' Copyright(C) 2000-2010 ������ ���
'
' ���� ���� �������� ����������, ����������
' �������������� �������� ������ ���.
'
' ����� ����� ����� ����� �� ����� ���� �����������,
' ����������, ���������� �� ������ �����,
' ������������ ��� �������������� ����� ��������,
' ���������������, �������� �� ���� � ��� ��
' ����� ������������ ������� ��� ����������������
' ���������� ���������� � ��������� ������ ���.
'
' ����������� ���, ������������ � ���� �����, ������������
' ������������� ��� ����� �������� � �� ����� ���� �����������
' ��� ������ ����������.
'
' �������� ������-��� �� ����� �������
' ��������������� �� ���������������� ����� ����.
'
' ������ ������� (�� ������� ������� ������ �����������, �� ������������ 
' ��� �������������� ������ ������������)
'------------------------------------------------------------------------------
' � ������ script-������� �������������� ������������ � �������������
' ����������� ����� ��� ������ �������������� CAPICOM.  
'------------------------------------------------------------------------------

Option Explicit

Const ForReading = 1, ForWriting = 2

' �������.
Const Unknown                                      = 0
Const Encrypt                                      = 1
Const Decrypt                                      = 2
                                                   
' CAPICOM ���������.                                                   
Const CAPICOM_CURRENT_USER_STORE                   = 2
                                                   
Const CAPICOM_CERTIFICATE_FIND_TIME_VALID          = 9

Const CAPICOM_ENCRYPTION_ALGORITHM_RC2             = 0
        
Const CAPICOM_ENCRYPTION_KEY_LENGTH_MAXIMUM        = 0

' ��������� ��������� ������.
Dim Command       : Command        = Unknown
Dim StoreLocation : StoreLocation  = CAPICOM_CURRENT_USER_STORE
Dim StoreName     : StoreName      = "MY"
Dim CertFile      : CertFile       = Null
Dim Algorithm     : Algorithm      = Null 'CAPICOM_ENCRYPTION_ALGORITHM_RC2 - � CryptoPro CSP �� ��������������
Dim KeyLength     : KeyLength      = Null 'CAPICOM_ENCRYPTION_KEY_LENGTH_MAXIMUM - � CryptoPro CSP �� ��������������
Dim FileNames()
                                                   

' � ������ ������� �������������� �������� ����, ��� script ����������� ��� ������ CScript.exe.
If InStr(1, UCase(Wscript.FullName), "CSCRIPT.EXE", vbTextCompare) = 0 Then
   Wscript.Echo "This script can only be executed by CScript.exe." & vbCRLF & vbCRLF &_
                "You can either:" & vbCRLF & vbCRLF & _
                "1. Set CScript.exe as the default (Run CScript //h:cscript), or" & vbCRLF & _
                "2. Run CScript.exe directly as in, CScript " & Wscript.ScriptName & "."
   Wscript.Quit(-1)
End If

' ������ ��������� �����.
ParseCommandLine         

' ����� ����������� (������������) ����������.
Dim Certificates
If Command = Encrypt Then
   
   ' �������� ��������� ������������.
   Dim Store
   Set Store = CreateObject("CAPICOM.Store")
   Store.Open StoreLocation, StoreName
   
  ' � �������� ���������� �� ����������� ���������� 
  ' ���������� ������������ ��� ����������� ���������.
   Set Certificates = Store.Certificates
   
   ' �� ��� ���������� ������ �����������, �������������� � ��������� �����.
   If Certificates.Count > 0 Then
      Set Certificates = Certificates.Find(CAPICOM_CERTIFICATE_FIND_TIME_VALID, Now)
   End If
   
   ' ����� ����������� (������������). ���� ����� ����������� ���������� �������� ��������� ����������,
   ' �� ������������ ��������������� ����� ������ �� ��� ��� ���������� (��� ������ ����������� ����).
   Select Case Certificates.Count
   Case 0
      Wscript.Stdout.Writeline "Error: No recipient's certificate can be found."
      Wscript.Quit(1)
   
   Case 1
      ' ������� ������ ����, ������ �� � ����� ��������������.
         
   Case Else
      ' ������������ �������� ���������� (�����������) ����������.
      Set Certificates = Certificates.Select("CEnvelop.vbs", "Please select one or more recipient's certificate(s).", True)
      If Certificates.Count = 0 Then
         Wscript.Stdout.Writeline "Error: Certificate selection dialog was cancelled."
         Wscript.Quit(2)
      End If
    
   End Select

   ' ������������ ��������.
   Set Store = Nothing   
End If

' ���������� ��������� ��������.
Select Case Command
Case Encrypt   
   DoEncryptCommand FileNames, Certificates, Algorithm, KeyLength

Case Decrypt
   DoDecryptCommand FileNames
      
End Select

' ������������ ��������.
Set Certificates = Nothing
     
Wscript.Quit(0)

' ����� Main


'------------------------------------------------------------------------------
' �������� DoSignCommand  ������������� ���������� ����� FileNames(0) 
' � ���������� ������������� ������ � FileNames(1).
'------------------------------------------------------------------------------

Sub DoEncryptCommand (FileNames, Certificates, Algorithm, KeyLength)
   Dim Content
   Dim Message
   Dim EnvelopedData
   
   ' �������� ������� EnvelopedData.
   Set EnvelopedData = CreateObject("CAPICOM.EnvelopedData")
   
   ' ����������� ��������� � ������� ����� ������������.
   'EnvelopedData.Algorithm.Name = Algorithm - � CryptoPro CSP �� ��������������
   'EnvelopedData.Algorithm.KeyLength = KeyLength - - � CryptoPro CSP �� ��������������
   
   ' ���������� �����������.
   Dim Recipient
   For Each Recipient In Certificates 
      EnvelopedData.Recipients.Add Recipient
   Next
   
   ' ���������� �������������� ���������.
   Wscript.Stdout.Writeline "Enveloping text file " & FileNames(0) & "."
   Wscript.Stdout.Writeline
     
   ' �������� ����������� ���������� ����� ��� ������������.
   LoadFile FileNames(0), Content
   
   ' ������������.
   EnvelopedData.Content = Content
   Message = EnvelopedData.Encrypt
   
   ' ���������� ������������� ������ � FileNames(1).
   SaveFile FileNames(1), Message
   Wscript.Stdout.Writeline "Successful - Enveloped message saved to " & FileNames(1) & "."

   ' ������������ ��������.
   Set EnvelopedData = Nothing
   
End Sub ' ����� DoEncryptCommand


'------------------------------------------------------------------------------
' ������� DoCoSignCommand �������������� ������������� ���� FileNames(0)� ���������
' �������������� ������ � FileNames(1).
'------------------------------------------------------------------------------

Sub DoDecryptCommand (FileNames)
   Dim Message
   Dim EnvelopedData
   
   ' �������� ������� EnvelopedData.
   Set EnvelopedData = CreateObject("CAPICOM.EnvelopedData")
   
   ' ���������� �������������� ���������.
   Wscript.Stdout.Writeline "Decrypting enveloped text file " & FileNames(0) & "."
   Wscript.Stdout.Writeline
   
   ' �������� ������������� ������.
   LoadFile FileNames(0), Message
   
   ' �������������.
   EnvelopedData.Decrypt(Message)
   
         
   ' ���������� �������������� ������ � FileNames(1).
   SaveFile FileNames(1), EnvelopedData.Content
   Wscript.Stdout.Writeline "Successful - Decrypted content saved to " & FileNames(1) & "."

   ' ������������ ��������.
   Set EnvelopedData = Nothing
   
End Sub ' ����� DoDecryptCommand


'------------------------------------------------------------------------------
'������� LoadFile ������ ���������� ���������� �����.
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
    
End Sub ' ����� LoadFile


'------------------------------------------------------------------------------
' ������� SaveFile ��������� ������ � ����.
'------------------------------------------------------------------------------

Sub SaveFile (FileName, Buffer)
    Dim fso
    Set fso = CreateObject("Scripting.FileSystemObject")
    
    Dim ts
    Set ts = fso.OpenTextFile(FileName, ForWriting, True)
    ts.Write Buffer
    
End Sub ' End SaveFile


'------------------------------------------------------------------------------
' ������� ParseCommandLine ��������� ��������� ������ � �������������
' ����� �������� ��.
'------------------------------------------------------------------------------

Sub ParseCommandLine

   ' ��������� ��� ������� ���������, ���������� ��������� �������.
   Const ARG_STATE_COMMAND    = 0
   Const ARG_STATE_OPTIONS    = 1    
   Const ARG_STATE_FILENAME   = 8
   
   ' ������ ��������� ������.
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
   
   ' �������� ������������ ���������.
   If ArgState <> ARG_STATE_FILENAME Then
      DisplayUsage
   End If

   ' �������� �����.
   Select Case Command
   Case Encrypt
     
      ' ������� ������������������ ����� ������� � �������� �����.
      If (UBound(FileNames) - LBound(FileNames) + 1) <> 2 Then
         DisplayUsage
      End If
   
   Case Decrypt
      ' ������� ������������� ������ ����� ������� � �������� �����.
      If (UBound(FileNames) - LBound(FileNames) + 1) <> 2 Then
         DisplayUsage
      End If
      
   End Select
  

End Sub ' ����� ParseCommandLine


'------------------------------------------------------------------------------
' ������� DisplayUsage ������������� ���������� �� ������������� �������
' �������, � ����� ������������ ����� � �������.
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
   
End Sub ' ����� DisplayUsage
' ����� ������� 
' (�� ������� ������� ������ �����������, �� ������������ 
'  ��� �������������� ������ ������������)

