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

namespace WordPluginCheck
{

    public class Win32
    {
        public const int MOUSEEVENTF_ABSOLUTE = 0x8000;
        public const int MOUSEEVENTF_MOVE = 0x0001;
        public const int MOUSEEVENTF_LEFTDOWN = 0x0002;
        public const int MOUSEEVENTF_LEFTUP = 0x0004;
        public const int MAX_COORDINATE = 65535;

        [DllImport("user32.dll")]
        public static extern int FindWindow(string lpClassName, string lpWindowName);

        [DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        public static extern void mouse_event(long dwFlags, long dx, long dy, long cButtons, long dwExtraInfo);

        [DllImport("user32.dll")]
        public static extern int SetForegroundWindow(int hwnd);

        [DllImport("user32.dll")]
        public static extern bool SetCursorPos(int x, int y);

    }

    public class Global
    {
        public static string certificateName;
    }

    public class SelectCertificate
    {
        public volatile static int ParentWindowHandle_2010 = 0;

        public static int SelectingCertificate2010()
        {
            int iErrorCode = 1;

            while (true)
            {
                ParentWindowHandle_2010 = Win32.FindWindow("NUIDialog", "Подписание");

                if (ParentWindowHandle_2010 != 0) //нашли окно
                {
                    IntPtr hWnd = new IntPtr(ParentWindowHandle_2010);
                    AutomationElement aeParent = AutomationElement.FromHandle(hWnd);

                    AutomationElement aeBtnChange = aeParent.FindFirst(TreeScope.Descendants,
                                                           new PropertyCondition(AutomationElement.NameProperty, "Изменить..."));
                    if (aeBtnChange == null)
                    {
                        Console.WriteLine("aeBtnChange == null\n");
                    }

                    Win32.SetCursorPos((int)aeBtnChange.Current.BoundingRectangle.Location.X + 20, (int)aeBtnChange.Current.BoundingRectangle.Location.Y + 20);
                    Win32.mouse_event(Win32.MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    Win32.mouse_event(Win32.MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                    Thread.Sleep(3000);

                    iErrorCode = SelectingCertificate();

                    Thread.Sleep(2000);
                    if (iErrorCode != 4) { // сертификат найден
                        AutomationElement aeBtnSign = aeParent.FindFirst(TreeScope.Descendants,
                                                               new PropertyCondition(AutomationElement.NameProperty, "Подписать"));
                        if (aeBtnSign == null)
                        {
                            Console.WriteLine("aeBtnSign == null\n");
                        }
                        InvokePattern ipBtnSign = (InvokePattern)aeBtnSign.GetCurrentPattern(InvokePattern.Pattern);
                        ipBtnSign.Invoke();
                        Thread.Sleep(3000);

                        int ErrorWindowHandle = Win32.FindWindow("#32770", "Microsoft Word");
                        if (ErrorWindowHandle != 0)
                        {
                            Console.WriteLine("Error: Can't add a signature to the document.");
                            hWnd = new IntPtr(ErrorWindowHandle);
                            AutomationElement aeDialog = AutomationElement.FromHandle(hWnd);

                            AutomationElement aeBtnOK = aeDialog.FindFirst(TreeScope.Descendants,
                                                                   new PropertyCondition(AutomationElement.NameProperty, "ОК"));
                            InvokePattern ipBtnOK = (InvokePattern)aeBtnOK.GetCurrentPattern(InvokePattern.Pattern);
                            ipBtnOK.Invoke();
                           
                            goto err;
                        }

                        return iErrorCode;
                    }
                    else 
                    { 
                        AutomationElement aeBtnCancel = aeParent.FindFirst(TreeScope.Descendants,
                                                              new PropertyCondition(AutomationElement.NameProperty, "Отмена"));
                        InvokePattern ipBtnCancel = (InvokePattern)aeBtnCancel.GetCurrentPattern(InvokePattern.Pattern);
                        ipBtnCancel.Invoke();
                        goto err;
                    }
                }
            }
        err:
            iErrorCode = 4;
            return iErrorCode;
        }


        public volatile static int ParentWindowHandle_2007 = 0;

        public static int SelectingCertificate2007()
        {
            int iErrorCode = 1;

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
                    {
                        Console.WriteLine("aeBtnChange == null\n");
                    }
                    InvokePattern ipBtnChange = (InvokePattern)aeBtnChange.GetCurrentPattern(InvokePattern.Pattern);
                    ipBtnChange.Invoke();

                    iErrorCode = SelectingCertificate();

                    if (iErrorCode != 4)
                    {
                        AutomationElement aeBtnSign = aeParent.FindFirst(TreeScope.Descendants,
                                                               new PropertyCondition(AutomationElement.NameProperty, "Подписать"));
                        if (aeBtnSign == null)
                        {
                            Console.WriteLine("aeBtnSign == null\n");
                        }
                        InvokePattern ipBtnSign = (InvokePattern)aeBtnSign.GetCurrentPattern(InvokePattern.Pattern);
                        ipBtnSign.Invoke();

                        Thread.Sleep(3000);
                        int ErrorWindowHandle = Win32.FindWindow("#32770", "Microsoft Office Word");
                        if (ErrorWindowHandle != 0)
                        {
                            Console.WriteLine("Error: Can't add a signature to the document.");
                            hWnd = new IntPtr(ErrorWindowHandle);
                            AutomationElement aeDialog = AutomationElement.FromHandle(hWnd);

                            AutomationElement aeBtnOK = aeDialog.FindFirst(TreeScope.Descendants,
                                                                   new PropertyCondition(AutomationElement.NameProperty, "ОК"));
                            InvokePattern ipBtnOK = (InvokePattern)aeBtnOK.GetCurrentPattern(InvokePattern.Pattern);
                            ipBtnOK.Invoke();
                           
                            goto err;
                        }

                        return iErrorCode;
                    }
                    else
                    {
                        AutomationElement aeBtnCancel = aeParent.FindFirst(TreeScope.Descendants,
                                                              new PropertyCondition(AutomationElement.NameProperty, "Отмена"));
                        InvokePattern ipBtnCancel = (InvokePattern)aeBtnCancel.GetCurrentPattern(InvokePattern.Pattern);
                        ipBtnCancel.Invoke();
                        goto err;
                    }
                }
            }

        err:
            iErrorCode = 4;
            return iErrorCode;
        }

        public volatile static int ParentSelectCertWindowHandle = 0;
        public volatile static int ParentSelectCertWindowHandleRu = 0;
        public volatile static int ParentSelectCertWindowHandleEn = 0;
        public volatile static int ParentSelectCertWindowHandleNewDlg = 0;

        public static int SelectingCertificate() 
        {
            int iErrorCode = 1;
            string cancelBtnName = "";
            string okBtnName = "";
            bool isNewDlg = false;

            while (true)
            {
                Thread.Sleep(1000);
                ParentSelectCertWindowHandleNewDlg = Win32.FindWindow("#32770", "Безопасность Windows");
                ParentSelectCertWindowHandleRu = Win32.FindWindow("#32770", "Выбор сертификата");
                ParentSelectCertWindowHandleEn = Win32.FindWindow("#32770", "Select Certificate");

                if (ParentSelectCertWindowHandleRu == 0 && ParentSelectCertWindowHandleEn == 0 && ParentSelectCertWindowHandleNewDlg == 0)
                    continue;

                if (ParentSelectCertWindowHandleRu != 0)
                {
                    cancelBtnName = "Отмена";
                    okBtnName = "ОК";
                    ParentSelectCertWindowHandle = ParentSelectCertWindowHandleRu;
                }
                if (ParentSelectCertWindowHandleEn != 0) 
                {
                    cancelBtnName = "Cancel";
                    okBtnName = "OK";
                    ParentSelectCertWindowHandle = ParentSelectCertWindowHandleEn;
                }

                if (ParentSelectCertWindowHandleNewDlg != 0)
                {
                    cancelBtnName = "Отмена";
                    okBtnName = "ОК";
                    ParentSelectCertWindowHandle = ParentSelectCertWindowHandleNewDlg;
                    isNewDlg = true;
                }

                IntPtr hWnd = new IntPtr(ParentSelectCertWindowHandle);
                AutomationElement aeParent = AutomationElement.FromHandle(hWnd);

                AutomationElement aeList = aeParent.FindFirst(TreeScope.Descendants,
                                                           new PropertyCondition(AutomationElement.NameProperty, Global.certificateName));
                    
                //если сертификат не найден, нажать кнопку "Отмена"
                if (aeList == null)
                {
                    Console.WriteLine("aeList == null");
                    Console.WriteLine("No certificates to choose!");
                    AutomationElement aeBtnCancel = aeParent.FindFirst(TreeScope.Descendants,
                                                        new PropertyCondition(AutomationElement.NameProperty, cancelBtnName));
                    InvokePattern ipBtnCancel = (InvokePattern)aeBtnCancel.GetCurrentPattern(InvokePattern.Pattern);
                    ipBtnCancel.Invoke();
                    goto err;
                }

                if (isNewDlg == false)
                {
                    SelectionItemPattern ipList = (SelectionItemPattern)aeList.GetCurrentPattern(SelectionItemPattern.Pattern);
                    ipList.Select();
                }
                else
                {
                    aeList.SetFocus();
                }

                AutomationElement aeBtn = aeParent.FindFirst(TreeScope.Descendants | TreeScope.Children,
                                                           new PropertyCondition(AutomationElement.NameProperty, okBtnName));
                if (aeBtn == null)
                {
                    Console.WriteLine("aeBtn == null\n");
                }
                InvokePattern ipBtn = (InvokePattern)aeBtn.GetCurrentPattern(InvokePattern.Pattern);
                ipBtn.Invoke();

                return iErrorCode;
            }
        err:
            iErrorCode = 4;
            return iErrorCode;
        }

    }


    class Program
    {
        static int Main(string[] args)
        {
            if (args.Length != 1) 
            {
                System.Console.WriteLine("USAGE: WordPluginCheck.exe  <certificate_name>");
                return 1;
            }
            Global.certificateName = args[0];

            string fileDirectory = @"C:\Temp\WordPluginTest";
            if (!Directory.Exists(fileDirectory))
            {
                Directory.CreateDirectory(fileDirectory);
            }

            object Missing = System.Reflection.Missing.Value;
            Word._Application oWordApplication = new Word.ApplicationClass();
            oWordApplication.Visible = true;
            int iErrorCode = 1;

            try
            {   //TODO: сделать проверку на единственность документа
                object fileToSave = fileDirectory + "\\sample.docx";

                WordDocumentEx oWordDocumentEx = new WordDocumentEx(oWordApplication);
                oWordDocumentEx.AddSomeText();
                oWordDocumentEx.SaveAs(fileToSave);

                if (oWordApplication.Version == "14.0") //  Office 2010 
                {
                    iErrorCode = oWordDocumentEx.SignaturesAdd2010();
                    if (iErrorCode != 1)
                    {
                        goto err;
                    }
                    //TODO: замечено, что подпись cln512b не работает 
                    iErrorCode = SelectCertificate.SelectingCertificate2010();

                    if (iErrorCode != 1)
                    {
                        goto err;
                    }
                }

                if (oWordApplication.Version == "12.0") // Office 2007
                {
                    iErrorCode = oWordDocumentEx.SignaturesAdd2007();
                    if (iErrorCode != 1)
                    {
                        goto err;
                    }
                    //TODO: замечено, что подпись cln512b не работает 
                    iErrorCode = SelectCertificate.SelectingCertificate2007();

                    if (iErrorCode != 1)
                    {
                        goto err;
                    }
                }

                Thread.Sleep(5000);
                oWordDocumentEx.Commit();
                oWordDocumentEx.SaveAs(fileToSave);
                oWordDocumentEx.Close();

                //теперь необходимо проверить созданную подпись.
                oWordDocumentEx.Open(fileToSave);
                Thread.Sleep(10000);
                iErrorCode = oWordDocumentEx.SignaturesCheck();
                if (iErrorCode != 0)
                {
                    Console.Write("SignatureCheck error\n");
                    goto err;
                }
                else
                {
                    Console.Write("SignatureCheck with valid signature passed\n");
                }
                oWordDocumentEx.Close();

                if (oWordApplication.Version == "12.0" || oWordApplication.Version == "14.0") //Office 2007 и 2010
                {
                    //теперь попробуем "сломать" документ, поменяв содержание текста, тем самым подпись должна стать неверное
                    oWordDocumentEx.HackerChangeText(fileDirectory, fileToSave);
                    oWordDocumentEx.Open(fileToSave);
                    Thread.Sleep(5000);
                    iErrorCode = oWordDocumentEx.SignaturesCheck();
                    if (iErrorCode != 1)
                    {
                        Console.Write("SignatureCheck unexpected success\n");
                    }
                    else
                    {
                        iErrorCode = 0;
                        Console.Write("SignatureCheck with invalid signature passed\n");
                    }
                    oWordDocumentEx.Close();
                }
                //Thread.Sleep(2000);
                //File.Delete(fileDirectory + @"\sample.docx");
                //iErrorCode = 0;
            }
            catch (System.Exception e)
            {
                Console.Write(e.Message.ToString() + "\n");
                iErrorCode = 2;
            }

     err:
            oWordApplication.Quit(ref Missing, ref Missing, ref Missing);
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
            Word.Paragraph oParagraph;
            oParagraph = document.Content.Paragraphs.Add(ref Missing);
            oParagraph.Range.Text = "Privet!";
            oParagraph.Range.Font.Bold = 1;
            oParagraph.Format.SpaceAfter = 24;    //24 pt spacing after paragraph.
        }

        public void SaveAs(object fileToSave)
        {
            document.SaveAs(ref fileToSave, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing);
        }

        public void Commit()
        {
            document.Signatures.Commit();
        }

        public void Open(object fileToOpen)
        {
            document = application.Documents.Open(ref fileToOpen, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing, ref Missing);
        }

        public void Maximize(AutomationElement aeParent)
        {
            object aeParentPatternObj;
            WindowPattern aeParentPattern;
            if (true == aeParent.TryGetCurrentPattern(WindowPattern.Pattern, out aeParentPatternObj))
            {
                aeParentPattern = aeParentPatternObj as WindowPattern;
                if (aeParentPattern.Current.CanMaximize)
                {
                    aeParentPattern.SetWindowVisualState(WindowVisualState.Maximized);
                }
            }
        }
        public void Close()
        {
            document.Close(ref Missing, ref Missing, ref Missing);
        }

        public int SignaturesAdd2010()
        {
                int iErrorCode = 1;
                AutomationElement aeParent = AutomationElement.RootElement.FindFirst(TreeScope.Children, new PropertyCondition(AutomationElement.NameProperty, "sample.docx - Microsoft Word"));

                if (aeParent != null)
                {
                    Maximize(aeParent);

                    AutomationElement aeBtnFile = aeParent.FindFirst(TreeScope.Descendants,
                                                                new PropertyCondition(AutomationElement.NameProperty, "Вкладка \"Файл\""));
                    if (aeBtnFile == null)
                    {
                        Console.WriteLine("aeBtnChange == null\n");
                        goto err;
                    }

                    InvokePattern ipBtnFile = (InvokePattern)aeBtnFile.GetCurrentPattern(InvokePattern.Pattern);
                    ipBtnFile.Invoke();

                    AutomationElement aeBtnInsert = aeParent.FindFirst(TreeScope.Descendants,
                                                               new PropertyCondition(AutomationElement.NameProperty, "Сведения"));
                    if (aeBtnInsert == null)
                    {
                        Console.WriteLine("aeBtnInsert == null\n");
                        goto err;
                    }
                    aeBtnInsert.SetFocus();

                    AutomationElement aeBtnPlugin = null;
                    AutomationElement aeBtnPluginRu = aeParent.FindFirst(TreeScope.Descendants,
                                                                new PropertyCondition(AutomationElement.NameProperty, "Добавить цифровую подпись (КРИПТО-ПРО)"));
                    AutomationElement aeBtnPluginEn = aeParent.FindFirst(TreeScope.Descendants,
                                                                new PropertyCondition(AutomationElement.NameProperty, "Add NonVisible digital signature(CRYPTO-PRO)"));
                    if (aeBtnPluginRu == null && aeBtnPluginEn == null)
                    {
                        Console.WriteLine("aeBtnPlugin == null\n");
                        goto err;
                    }
                    if (aeBtnPluginRu != null)
                        aeBtnPlugin = aeBtnPluginRu;
                    if (aeBtnPluginEn != null)
                        aeBtnPlugin = aeBtnPluginEn;

                    Win32.SetCursorPos((int)aeBtnPlugin.Current.BoundingRectangle.Location.X + 20, (int)aeBtnPlugin.Current.BoundingRectangle.Location.Y + 20);
                    Win32.mouse_event(Win32.MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    Win32.mouse_event(Win32.MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                    return iErrorCode;
                }
           err:
                iErrorCode = 4;
                return iErrorCode;
        }


        public int SignaturesAdd2007()
        {
            int iErrorCode = 1;
            AutomationElement aeParent = AutomationElement.RootElement.FindFirst(TreeScope.Children, new PropertyCondition(AutomationElement.NameProperty, "sample.docx - Microsoft Word"));

            if (aeParent != null)
            {
                Maximize(aeParent);

                AutomationElement aeBtnOffice = aeParent.FindFirst(TreeScope.Descendants,
                                                            new PropertyCondition(AutomationElement.NameProperty, "Кнопка \"Office\""));
                if (aeBtnOffice == null)
                {   
                    Console.WriteLine("aeBtnOffice == null\n");
                    goto err;
                }

                InvokePattern ipBtnOffice = (InvokePattern)aeBtnOffice.GetCurrentPattern(InvokePattern.Pattern);
                ipBtnOffice.Invoke();

                AutomationElement aeBtnSelect = aeParent.FindFirst(TreeScope.Descendants,
                                                                           new PropertyCondition(AutomationElement.NameProperty, "Подготовить"));
                if (aeBtnSelect == null)
                {
                    Console.WriteLine("aeBtnSelect == null\n");
                    goto err;
                }
                InvokePattern ipBtnSelect = (InvokePattern)aeBtnSelect.GetCurrentPattern(InvokePattern.Pattern);
                ipBtnSelect.Invoke();

                AutomationElement aeBtnPlugin = aeParent.FindFirst(TreeScope.Descendants,
                                                            new PropertyCondition(AutomationElement.NameProperty, "Добавить цифровую подпись (КРИПТО-ПРО)"));
                if (aeBtnPlugin == null)
                {
                    Console.WriteLine("aeBtnPlugin == null\n");
                    goto err;
                }

                InvokePattern ipBtnPlugin = (InvokePattern)aeBtnPlugin.GetCurrentPattern(InvokePattern.Pattern);
                ipBtnPlugin.Invoke();

                return iErrorCode;
            }
        err:
            iErrorCode = 4;
            return iErrorCode;
        }

        public int SignaturesCheck()
        {
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
            return iErrorCode;
        }

        public void HackerChangeText(string fileDirectory, object fileToChange) //метод хитрым способом меняет содержимое документа (распаковывает, меняет xml и запаковывает обратно), тем самым если существовала подпись, она становится недействительной
        {
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
            ZipProcess.WaitForExit(); //ждем пока заспаковка не закончится

            File.Delete(fileDirectory + @"\word\document.xml");
            Directory.Delete(fileDirectory + @"\word");
        }
    }
}