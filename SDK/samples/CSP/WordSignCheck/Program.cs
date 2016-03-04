using System;
using System.Collections.Generic;
using System.Text;
using Office = Microsoft.Office.Core;
using Word = Microsoft.Office.Interop.Word;
using System.Reflection;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.IO;
using System.Security;
using Accessibility;
using System.Windows.Forms;
using System.Windows.Automation;

[assembly: AllowPartiallyTrustedCallers]

public delegate bool WindowEnumDelegate(int hwnd, int lParam);


namespace WordSignCheck
{
    public class Win32 //имитация функций FindWindow
    {
        [DllImport("user32.dll")]
        public static extern int FindWindow(string lpClassName, string lpWindowName);
    }
    public class Global
    {
        public static string certificateName;
    }
    public class SelectCertificate
    {   

        public volatile static int ParentWindowHandle_2007 = 0;

        public void SelectingCertificate2007()
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            while (true)
            {
                ParentWindowHandle_2007 = Win32.FindWindow("NUIDialog", "Подписание");

                if (ParentWindowHandle_2007 != 0) //нашли окно
                {
                    IntPtr hWnd = new IntPtr(ParentWindowHandle_2007);
                    AutomationElement aeParent = AutomationElement.FromHandle(hWnd);

                    AutomationElement aeBtnChange = aeParent.FindFirst(TreeScope.Descendants,
                                                           new PropertyCondition(AutomationElement.NameProperty, "Изменить..."));
                    if (aeBtnChange == null)
                        Console.WriteLine("aeBtnChange == null\n");
                    InvokePattern ipBtnChange = (InvokePattern)aeBtnChange.GetCurrentPattern(InvokePattern.Pattern);
                    ipBtnChange.Invoke();
                    
                    SelectingCertificate();

                    AutomationElement aeBtnSign = aeParent.FindFirst(TreeScope.Descendants,
                                                           new PropertyCondition(AutomationElement.NameProperty, "Подписать"));
                    if (aeBtnSign == null)
                        Console.WriteLine("aeBtnSign == null\n");
                    InvokePattern ipBtnSign = (InvokePattern)aeBtnSign.GetCurrentPattern(InvokePattern.Pattern);
                    ipBtnSign.Invoke();
                    
                    break;
                }
            }
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }

        public volatile static int ParentSelectCertWindowHandle = 0;

        public void SelectingCertificate() //щелкает Да по первому окну
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            while (true)
            {
                Thread.Sleep(1000);
                ParentSelectCertWindowHandle = Win32.FindWindow("#32770", "Выбор сертификата");

                if (ParentSelectCertWindowHandle != 0)
                {
                    IntPtr hWnd = new IntPtr(ParentSelectCertWindowHandle);
                    AutomationElement aeParent = AutomationElement.FromHandle(hWnd);

                    AutomationElement aeList = aeParent.FindFirst(TreeScope.Descendants,
                                                           new PropertyCondition(AutomationElement.NameProperty, Global.certificateName));
                    //TODO: добавить проверку на наличие сертификата
                    if (aeList == null)
                        Console.WriteLine("aeList == null\n");
                    SelectionItemPattern ipList = (SelectionItemPattern)aeList.GetCurrentPattern(SelectionItemPattern.Pattern);
                    ipList.Select();
                    AutomationElement aeBtn = aeParent.FindFirst(TreeScope.Children,
                                                           new PropertyCondition(AutomationElement.NameProperty, "ОК"));
                    if (aeBtn == null)
                        Console.WriteLine("aeBtn == null\n");
                    InvokePattern ipBtn = (InvokePattern)aeBtn.GetCurrentPattern(InvokePattern.Pattern);
                    ipBtn.Invoke();
                    //TODO: добавить проверку на успех подписания -  что не появилось сообщение про смарт-карту
                    break;
                }
            }
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }
    }

    class Program
    {
        static int Main(string[] args)
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            if (args.Length != 1)
            {
                System.Console.WriteLine("USAGE: WordSignCheck.exe  <certificate_name>");
                return 1;
            }
            Global.certificateName = args[0];

            string fileDirectory = @"C:\Temp\WordSignTest";
            if (!Directory.Exists(fileDirectory))
            {
                Directory.CreateDirectory(fileDirectory);
            }
            object Missing = System.Reflection.Missing.Value;
            Word._Application oWordApplication = new Word.ApplicationClass();
            oWordApplication.Visible = true;
            int iErrorCode = 1;

            try
            {
                object fileToSave = fileDirectory + "\\sample.docx";

                WordDocumentEx oWordDocumentEx = new WordDocumentEx(oWordApplication);
                oWordDocumentEx.AddSomeText();
                oWordDocumentEx.SaveAs(fileToSave);
                //Console.WriteLine((int)new System.Diagnostics.StackTrace(true).GetFrame(0).GetFileLineNumber());

                if (oWordApplication.Version == "12.0") //в Office 2007 нужно щелкнуть ОК в окне выбора 
                {
                    //создаем отдельный поток, который нажмет на кнопку выбора сертификата и указания цели подписи
                    SelectCertificate SelectCertificateObject = new SelectCertificate();
                    Thread SelectCertificateThread = new Thread(SelectCertificateObject.SelectingCertificate2007); //создаем новый поток, в котором щелкнем по кнопке
                    SelectCertificateThread.Start(); //запускаем

                    oWordDocumentEx.SignaturesAdd();
                    //TODO: замечено, что подпись cln512b не работает 

                    Thread.Sleep(1000);
                    while (SelectCertificateThread.IsAlive) ; //ждем пока он не отработает
                }
                else //в Office 2003 просто подписываем
                {
                    //создаем отдельный поток, который нажмет на кнопку выбора сертификата и указания цели подписи
                    SelectCertificate SelectCertificateObject = new SelectCertificate();
                    Thread SelectCertificateThread = new Thread(SelectCertificateObject.SelectingCertificate); //создаем новый поток, в котором щелкнем по кнопке
                    SelectCertificateThread.Start(); //запускаем

                    oWordDocumentEx.SignaturesAdd();
                    //TODO: замечено, что подпись cln512b не работает 

                    Thread.Sleep(1000);
                    while (SelectCertificateThread.IsAlive) ; //ждем пока он не отработает
                }

                oWordDocumentEx.Commit();
                oWordDocumentEx.SaveAs(fileToSave);
                oWordDocumentEx.Close();

                //теперь необходимо проверить созданную подпись.
                oWordDocumentEx.Open(fileToSave);
                iErrorCode = oWordDocumentEx.SignaturesCheck();
                if (iErrorCode != 0)
                {
                    Console.Write("SignatureCheck error!\n");
                    goto err;
                }
                else
                {
                    Console.Write("SignatureCheck with valid signature passed!\n");
                }
                oWordDocumentEx.Close();

                if (oWordApplication.Version == "12.0") //Office 2007 
                {
                    //теперь попробуем "сломать" документ, поменяв содержание текста, тем самым подпись должна стать неверное
                    oWordDocumentEx.HackerChangeText(fileDirectory, fileToSave);

                    oWordDocumentEx.Open(fileToSave);
                    iErrorCode = oWordDocumentEx.SignaturesCheck();
                    if (iErrorCode != 1)
                    {
                        Console.Write("SignatureCheck unexpected success!\n");
                    }
                    else
                    {
                        iErrorCode = 0;
                        Console.Write("SignatureCheck with invalid signature passed!\n");
                    }
                    oWordDocumentEx.Close();
                }
            }
            catch (System.Exception e)
            {
                Console.Write(e.Message.ToString() + "\n");
                iErrorCode = 2;
            }

        err:
            oWordApplication.Quit(ref Missing, ref Missing, ref Missing);
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            return iErrorCode;
        }
    }

    public class WordDocumentEx
    {
        private Word._Document document;
        Word._Application application;

        object Missing = System.Reflection.Missing.Value;

        public WordDocumentEx(Word._Application oWordApplication)
        {
            application = oWordApplication;
            document = new Word.Document();
        }

        public void AddSomeText()
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            Word.Paragraph oParagraph;
            oParagraph = document.Content.Paragraphs.Add(ref Missing);
            oParagraph.Range.Text = "Privet!";
            oParagraph.Range.Font.Bold = 1;
            oParagraph.Format.SpaceAfter = 24;    //24 pt spacing after paragraph.
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }

        public void SaveAs(object fileToSave)
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            document.SaveAs(ref fileToSave, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing);
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }

        public void Commit()
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            document.Signatures.Commit();
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }

        public void Open(object fileToOpen)
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            document = application.Documents.Open(ref fileToOpen, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing);
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }

        public void Close()
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            document.Close(ref Missing, ref Missing, ref Missing);
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }

        public void SignaturesAdd()
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            document.Signatures.Add();
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }

        public int SignaturesCheck()
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            object CertificateName = (object)@Global.certificateName; //название сертификата, при проверке смотрится, этим сертификатом подписано или нет
            int iErrorCode = 0;

            Microsoft.Office.Core.SignatureSet oSignatureSet = document.Signatures;

            foreach (Microsoft.Office.Core.Signature oSignatureToCheck in oSignatureSet) //перебираем сертификаты, т.к. у нас один сертификат
            {
                if (oSignatureToCheck.Signer != CertificateName.ToString())
                {
                    Console.Write("Signer is not valid. You must manually sign a document with " + CertificateName + " certificate once to make it default\n");
                    iErrorCode = 0;
                    goto err;
                }
                if (oSignatureToCheck.IsValid == false)
                {
                    iErrorCode = 1;
                    goto err;
                }
                else
                {
                    iErrorCode = 0;
                }
            }

        err:
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            return iErrorCode;
        }

        public void HackerChangeText(string fileDirectory, object fileToChange) //метод хитрым способом меняет содержимое документа (распаковывает, меняет xml и запаковывает обратно), тем самым если существовала подпись, она становится недействительной
        {
            Console.WriteLine("Start " + System.Reflection.MethodBase.GetCurrentMethod().Name);
            ProcessStartInfo UnzipProcessInfo = new ProcessStartInfo();
            UnzipProcessInfo.FileName = @"unzip.exe";
            UnzipProcessInfo.Arguments = " -o " + fileDirectory + @"\sample.docx word/document.xml -d " + fileDirectory;

            Process UnzipProcess = Process.Start(UnzipProcessInfo); //распаковываем
            UnzipProcess.WaitForExit(); //ждем пока распаковка не закончится

            StringBuilder newFile = new StringBuilder();
            string temp = "";
            string[] file = File.ReadAllLines(fileDirectory + @"\word\document.xml");

            foreach (string line in file)
            {
                if (line.Contains("Privet!"))
                {
                    temp = line.Replace("Privet", "Preved");
                    newFile.Append(temp + "\r\n");
                    continue;
                }
                newFile.Append(line + "\r\n");
            }
            File.Delete(fileDirectory + @"\word\document.xml"); //удаляем старый файл
            File.WriteAllText(fileDirectory + @"\word\document.xml", newFile.ToString()); //записываем новый

            Directory.SetCurrentDirectory(fileDirectory);
            ProcessStartInfo ZipProcessInfo = new ProcessStartInfo();
            ZipProcessInfo.FileName = @"zip.exe";
            ZipProcessInfo.Arguments = " -u " + @"sample.docx " + @"word\document.xml";

            Process ZipProcess = Process.Start(ZipProcessInfo); //запаковываем
            ZipProcess.WaitForExit(); //ждем пока запаковка не закончится

            File.Delete(fileDirectory + @"\word\document.xml");
            Directory.Delete(fileDirectory + @"\word");
            Console.WriteLine("Finish " + System.Reflection.MethodBase.GetCurrentMethod().Name);
        }
    }
}