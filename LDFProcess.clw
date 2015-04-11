; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=COutputDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "LDFProcess.h"

ClassCount=7
Class1=CLDFProcessApp
Class2=CLDFProcessDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_DIALOG_OUTPUT
Resource2=IDR_MAINFRAME
Resource3=IDD_LDFPROCESS_DIALOG
Resource4=IDD_DIALOG_abnormalmodel
Resource5=IDD_DIALOG_clouddetection
Class4=COutputDlg
Class5=CInputDlg
Class6=CCldetcDlg
Class7=CAbnmdlDlg
Resource6=IDD_DIALOG_INPUT
Resource7=IDD_ABOUTBOX

[CLS:CLDFProcessApp]
Type=0
HeaderFile=LDFProcess.h
ImplementationFile=LDFProcess.cpp
Filter=N

[CLS:CLDFProcessDlg]
Type=0
HeaderFile=LDFProcessDlg.h
ImplementationFile=LDFProcessDlg.cpp
Filter=D
LastObject=CLDFProcessDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=LDFProcessDlg.h
ImplementationFile=LDFProcessDlg.cpp
Filter=D

[DLG:IDD_LDFPROCESS_DIALOG]
Type=1
Class=CLDFProcessDlg
ControlCount=6
Control1=IDC_BUT_CLOUDetection,button,1342242816
Control2=IDC_BUT_Output,button,1342242816
Control3=IDC_BUT_Input,button,1342242816
Control4=IDC_BUT_Browser,button,1342242816
Control5=IDC_BUT_Abnormal,button,1342242816
Control6=IDC_BUT_CAL,button,1342242816

[DLG:IDD_DIALOG_abnormalmodel]
Type=1
Class=CAbnmdlDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RADIO_KValue,button,1342308361
Control4=IDC_RADIO_RST,button,1342177289
Control5=IDC_RADIO_Wavelet,button,1342177289

[DLG:IDD_DIALOG_clouddetection]
Type=1
Class=CCldetcDlg
ControlCount=27
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308354
Control5=IDC_STATIC,static,1342308354
Control6=IDC_STATIC,static,1342308354
Control7=IDC_STATIC,static,1342308354
Control8=IDC_STATIC,static,1342308354
Control9=IDC_STATIC,static,1342308354
Control10=IDC_EDIT_MONTH1,edit,1350631552
Control11=IDC_EDIT_MONTH2,edit,1350631552
Control12=IDC_EDIT_MONTH3,edit,1350631552
Control13=IDC_EDIT_MONTH4,edit,1350631552
Control14=IDC_EDIT_MONTH5,edit,1350631552
Control15=IDC_EDIT_MONTH6,edit,1350631552
Control16=IDC_STATIC,static,1342308354
Control17=IDC_STATIC,static,1342308354
Control18=IDC_STATIC,static,1342308354
Control19=IDC_STATIC,static,1342308354
Control20=IDC_STATIC,static,1342308354
Control21=IDC_STATIC,static,1342308354
Control22=IDC_EDIT_MONTH7,edit,1350631552
Control23=IDC_EDIT_MONTH8,edit,1350631552
Control24=IDC_EDIT_MONTH9,edit,1350631552
Control25=IDC_EDIT_MONTH10,edit,1350631552
Control26=IDC_EDIT_MONTH11,edit,1350631552
Control27=IDC_EDIT_MONTH12,edit,1350631552

[DLG:IDD_DIALOG_OUTPUT]
Type=1
Class=COutputDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_OUTPUT,edit,1350631552
Control5=IDC_CHEK_Pseudocolor,button,1342242819
Control6=IDC_CHEK_Kpseudocolor,button,1342242819
Control7=IDC_CHEK_Statistics,button,1342242819
Control8=IDC_STATIC,button,1342177287
Control9=IDC_BUT_OUTSEL,button,1342242816

[DLG:IDD_DIALOG_INPUT]
Type=1
Class=CInputDlg
ControlCount=27
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_INPUT,edit,1350631552
Control5=IDC_BUT_SEL,button,1342242816
Control6=IDC_RADIO_Folder,button,1342308361
Control7=IDC_RADIO_Files,button,1342177289
Control8=IDC_BUT_Extend,button,1342242816
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308354
Control11=IDC_STATIC,static,1342308354
Control12=IDC_STATIC,static,1342308354
Control13=IDC_STATIC,static,1342308354
Control14=IDC_STATIC,static,1342308354
Control15=IDC_STATIC,static,1342308354
Control16=IDC_EDIT_LEFT,edit,1350631554
Control17=IDC_EDIT_RIGHT,edit,1350631554
Control18=IDC_EDIT_LONRES,edit,1350631554
Control19=IDC_EDIT_BOTTOM,edit,1350631554
Control20=IDC_EDIT_TOP,edit,1350631554
Control21=IDC_EDIT_LATRES,edit,1350631554
Control22=IDC_BUT_Add,button,1342242816
Control23=IDC_SEPARATOR,static,1342181383
Control24=IDC_LIST_FILE,listbox,1352728835
Control25=IDC_BUT_Del,button,1342242816
Control26=IDC_BUT_Clear,button,1342242816
Control27=IDC_STATIC_COUNT,static,1342308352

[CLS:COutputDlg]
Type=0
HeaderFile=OutputDlg.h
ImplementationFile=OutputDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=COutputDlg
VirtualFilter=dWC

[CLS:CInputDlg]
Type=0
HeaderFile=InputDlg.h
ImplementationFile=InputDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUT_Add
VirtualFilter=dWC

[CLS:CCldetcDlg]
Type=0
HeaderFile=CldetcDlg.h
ImplementationFile=CldetcDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[CLS:CAbnmdlDlg]
Type=0
HeaderFile=AbnmdlDlg.h
ImplementationFile=AbnmdlDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAbnmdlDlg
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

