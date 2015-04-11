// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "InputDlg.h"
#include "SelectFolderDlg.h"
#include ".\lib\LDF.h"
#include "LDFProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//可修改以增加Input中添加按钮所能添加的文件数目上限
#define MAX_CFileDialog_FILE_COUNT 999//由下行可知，理论意义为：在文件全路径名称达到最长时，可支持选取999个文件（实际上多于此）
#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog


CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDlg)
	m_inputfolder = _T("");
	m_minLon = _T("");
	m_maxLon = _T("");
	m_resLon = _T("");
	m_minLat = _T("");
	m_maxLat = _T("");
	m_resLat = _T("");
	m_selectinputstyle = 0;//0为文件夹，1为文件组
	//}}AFX_DATA_INIT
	rectSmall.left=0;
	rectSmall.top=0;
	rectSmall.right=0;
	rectSmall.bottom=0;
	rectLarge.left=0;
	rectLarge.top=0;
	rectLarge.right=0;
	rectLarge.bottom=0;
	m_InputWindow_Extend=FALSE;//默认为0（不扩展输入窗口，即不显示地图信息）
}


void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDlg)
	DDX_Control(pDX, IDC_LIST_FILE, m_listFile);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_inputfolder);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_minLon);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_maxLon);
	DDX_Text(pDX, IDC_EDIT_LONRES, m_resLon);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_minLat);
	DDX_Text(pDX, IDC_EDIT_TOP, m_maxLat);
	DDX_Text(pDX, IDC_EDIT_LATRES, m_resLat);
	DDX_Radio(pDX, IDC_RADIO_Folder, m_selectinputstyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDlg)
	ON_BN_CLICKED(IDC_BUT_SEL, OnButSel)
	ON_BN_CLICKED(IDC_RADIO_Folder, OnRADIOFolder)
	ON_BN_CLICKED(IDC_RADIO_Files, OnRADIOFiles)
	ON_BN_CLICKED(IDC_BUT_Add, OnBUTAdd)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, OnSelchangeListFile)
	ON_BN_CLICKED(IDC_BUT_Del, OnBUTDel)
	ON_BN_CLICKED(IDC_BUT_Clear, OnBUTClear)
	ON_BN_CLICKED(IDC_BUT_Extend, OnBUTExtend)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT, &CInputDlg::OnEnKillfocusEditInput)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg message handlers

void CInputDlg::OnButSel() 
{
	// TODO: Add your control notification handler code here
	//首次打开时需要设置一下默认目录，之后会自动记住上次选择的文件夹
	if (CSelectFolderDlg::m_filePath.GetLength()==0) CSelectFolderDlg::m_filePath=m_inputfolder;
	m_inputfolder=CSelectFolderDlg::Show();
    CFileFind ff;
    BOOL ret = ff.FindFile(m_inputfolder+"\\*.LDF");
    while(ret)
    {
        ret = ff.FindNextFile();
		if(!m_repeatcheck.Lookup(m_inputfolder+"\\"+ff.GetFileName(),m_inputfolder+"\\"+ff.GetFileName())){
			m_repeatcheck.SetAt(m_inputfolder+"\\"+ff.GetFileName(),m_inputfolder+"\\"+ff.GetFileName());
			m_listFile.AddString(m_inputfolder+"\\"+ff.GetFileName());
		}
    }
    ff.Close();

	//显示选择的文件数目
	ShowFilesNum();

	UpdateData(FALSE);
}

//选取数据风格：文件夹
void CInputDlg::OnRADIOFolder() 
{
	// TODO: Add your control notification handler code here
	m_selectinputstyle=0;
	GetDlgItem(IDC_EDIT_INPUT)->EnableWindow();
	GetDlgItem(IDC_BUT_SEL)->EnableWindow();

	GetDlgItem(IDC_BUT_Add)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_Del)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_Clear)->EnableWindow(FALSE);
}

//选取数据风格：文件组
void CInputDlg::OnRADIOFiles() 
{
	// TODO: Add your control notification handler code here
	m_selectinputstyle=1;
	GetDlgItem(IDC_BUT_Add)->EnableWindow();
	GetDlgItem(IDC_BUT_Del)->EnableWindow();
	GetDlgItem(IDC_BUT_Clear)->EnableWindow();

	GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_SEL)->EnableWindow(FALSE);
}

void CInputDlg::OnBUTAdd() 
{
	
	if(m_strLastDir_InputFiles.GetLength()==0)
		{m_strLastDir_InputFiles = _T("D:\\aTemp\\aa"); }//以前"D:/Temp/aa"这种写法也是可以的

	// TODO: Add your control notification handler code here
	DWORD flag = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
	CString str_Selectfile;

	CString fileName;
	wchar_t * p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);

	CFileDialog dlgFile(TRUE,  _T("LDF"), _T("*.LDF"), flag,
		TEXT("支持的文件 (*.ldf)|*.ldf|下步支持的文件(*.gpf,*.tiff;*.tif)|*.gpf,*.tiff;*.tif|所有文件(*.*)|*.*||"),NULL);//_T("LDF(*.LDF)|*.ldf||"));

	OPENFILENAME& ofn = dlgFile.m_ofn;

	ofn.lStructSize = 88;//76将是win98风格
	ofn.lpstrTitle=TEXT("添加LDF文件...\0"); 
	ofn.lpstrFile = p;
	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;
	ofn.lpstrInitialDir=m_strLastDir_InputFiles;//上一次打开的文件夹
	/**************************************
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	int nResult = ::GetOpenFileName(&ofn);
	/*************************************/
	if (dlgFile.DoModal() == IDOK){
		POSITION pos = dlgFile.GetStartPosition();
		while (pos){
			str_Selectfile = dlgFile.GetNextPathName(pos);
			if(!m_repeatcheck.Lookup(str_Selectfile,str_Selectfile)){
				m_repeatcheck.SetAt(str_Selectfile,str_Selectfile);
				m_listFile.AddString(str_Selectfile);
			}
		}
		//存储上次打开文件夹
		m_strLastDir_InputFiles = str_Selectfile.Left(str_Selectfile.ReverseFind('\\'));
		UpdateData(FALSE);
	}

	//显示选择的文件数目
	ShowFilesNum();

	//fileName.ReleaseBuffer();

	if (m_listFile.GetCount() >= 1){
		//默认显示第一个文件的信息及图像
		LDF ldf1;
		CString str;
		m_listFile.GetText(0, str);

		//由头文件确定地图信息的6个参数
		if (!ldf1.readHdr(str)){
			return;
		}
		m_resLon.Format(_T("%.4f"),ldf1.m_hdr.fLonSolution);
		m_resLat.Format(_T("%.4f"),ldf1.m_hdr.fLatSolution);
		m_minLon.Format(_T("%.4f"),ldf1.m_hdr.fMinLon);
		m_maxLon.Format(_T("%.4f"),ldf1.m_hdr.fMaxLon);
		m_minLat.Format(_T("%.4f"),ldf1.m_hdr.fMinLat);
		m_maxLat.Format(_T("%.4f"),ldf1.m_hdr.fMaxLat);
		UpdateData(FALSE);
	}
}

void CInputDlg::OnSelchangeListFile() 
{
	// TODO: Add your control notification handler code here
	LDF ldf1;
	CString str;
	int n = m_listFile.GetCurSel();
	if (n <= m_listFile.GetCount()-1 && n!=-1)
	{
		m_listFile.GetText(n, str);
		if (!ldf1.readHdr(str)){
			return;
		}
		m_resLon.Format(_T("%.4f"),ldf1.m_hdr.fLonSolution);
		m_resLat.Format(_T("%.4f"),ldf1.m_hdr.fLatSolution);
		m_minLon.Format(_T("%.4f"),ldf1.m_hdr.fMinLon);
		m_maxLon.Format(_T("%.4f"),ldf1.m_hdr.fMaxLon);
		m_minLat.Format(_T("%.4f"),ldf1.m_hdr.fMinLat);
		m_maxLat.Format(_T("%.4f"),ldf1.m_hdr.fMaxLat);
	}
	UpdateData(FALSE);
}

void CInputDlg::OnBUTDel() 
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel = m_listFile.GetCurSel();
	if (nSel == -1){
		return;
	}
	m_listFile.GetText(nSel,str);
	m_repeatcheck.RemoveKey(str); 
	m_listFile.DeleteString(nSel);
	if (nSel > 0)
	{
		m_listFile.SetCurSel(nSel - 1);
	}
	else{
		m_listFile.SetCurSel(0);
	}
	//重新计算选择的文件数目
	ShowFilesNum();
}

void CInputDlg::OnBUTClear() 
{
	// TODO: Add your control notification handler code here
	while (m_listFile.GetCount() > 0){
		m_listFile.DeleteString(m_listFile.GetCount() - 1);
		//重新计算选择的文件数目
		ShowFilesNum();
	}
	m_repeatcheck.RemoveAll();
}

BOOL CInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_selectinputstyle=((CLDFProcessDlg*)GetParent())->m_selectinputstyle;
	if(0==m_selectinputstyle){
		//((CButton*)GetDlgItem(IDC_RADIO_Folder))->SetCheck(TRUE);
		GetDlgItem(IDC_BUT_Add)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_Del)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_Clear)->EnableWindow(FALSE);
	}
	else{
		//((CButton*)GetDlgItem(IDC_RADIO_Files))->SetCheck(TRUE);
		GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_SEL)->EnableWindow(FALSE);}

	for(int i=0;i<((CLDFProcessDlg*)GetParent())->m_strArray.GetSize();i++){
		m_listFile.AddString(((CLDFProcessDlg*)GetParent())->m_strArray[i]);
		m_repeatcheck.SetAt(((CLDFProcessDlg*)GetParent())->m_strArray[i],((CLDFProcessDlg*)GetParent())->m_strArray[i]);
	}
	m_inputfolder=((CLDFProcessDlg*)GetParent())->m_inputfolder;
	m_InputWindow_Extend = ((CLDFProcessDlg*)GetParent())->m_InputWindow_Extend;
	
	//首先保存全窗口时的窗口大小信息
	CRect rectSeparator;
	GetWindowRect(&rectLarge);
	GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);

	rectSmall.left=rectLarge.left;
	rectSmall.top=rectLarge.top;
	rectSmall.right=rectSeparator.right;
	rectSmall.bottom=rectLarge.bottom;	

	//是全窗口显示还是小窗口显示
	if(!m_InputWindow_Extend)//rectLarge.IsRectNull()
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BUT_Extend,_T("Info>>"));
	}
	else
	{
		SetDlgItemText(IDC_BUT_Extend,_T("收缩<<"));
	}

	//显示选择的文件数目
	ShowFilesNum();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputDlg::OnBUTExtend() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if(GetDlgItemText(IDC_BUT_Extend,str),str=="收缩<<")
	{
		//修改
		SetDlgItemText(IDC_BUT_Extend,_T("Info>>"));
	}
	else
	{
		//修改
		SetDlgItemText(IDC_BUT_Extend,_T("收缩<<"));
	}

	if(str=="收缩<<")
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	if(m_InputWindow_Extend)
		m_InputWindow_Extend=FALSE;
	else
		m_InputWindow_Extend=TRUE;
}

void CInputDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	((CLDFProcessDlg*)GetParent())->m_selectinputstyle=m_selectinputstyle;
	((CLDFProcessDlg*)GetParent())->m_inputfolder=m_inputfolder;

	((CLDFProcessDlg*)GetParent())->m_strArray.RemoveAll();
	CString strtemp;
	for(int i=0;i<m_listFile.GetCount();i++){
		m_listFile.GetText(i,strtemp);
		((CLDFProcessDlg*)GetParent())->m_strArray.Add(strtemp);
	}

	((CLDFProcessDlg*)GetParent())->m_InputWindow_Extend = m_InputWindow_Extend;

	CDialog::OnOK();
}

void CInputDlg::OnEnKillfocusEditInput()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
    CFileFind ff;
    BOOL ret = ff.FindFile(m_inputfolder+"\\*.LDF");
    while(ret)
    {
        ret = ff.FindNextFile();
		if(!m_repeatcheck.Lookup(m_inputfolder+"\\"+ff.GetFileName(),m_inputfolder+"\\"+ff.GetFileName())){
			m_repeatcheck.SetAt(m_inputfolder+"\\"+ff.GetFileName(),m_inputfolder+"\\"+ff.GetFileName());
			m_listFile.AddString(m_inputfolder+"\\"+ff.GetFileName());
		}
    }
    ff.Close();

	//显示选择的文件数目
	ShowFilesNum();

	UpdateData(FALSE);
}


//显示选择的文件数目
void CInputDlg::ShowFilesNum() 
{
	//计算选择的文件数目
	int n = m_listFile.GetCount();
	CString buf;
	if (n !=0)
		buf.Format(_T("共 %d 个文件"), n);
	else
		buf=_T("");
	GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(buf);
}

