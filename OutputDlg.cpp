// OutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "OutputDlg.h"
#include "SelectFolderDlg.h"
#include "LDFProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputDlg dialog


COutputDlg::COutputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutputDlg::IDD, pParent)
	, m_selectoutputstyle(0)
	, m_CHEK_Clddetection2(FALSE)
	, m_CHEK_Pseudocolor(FALSE)
	, m_CHEK_Kpseudocolor(FALSE)
	, m_CHEK_Statistics(FALSE)
	, m_CHEK_Compound(FALSE)
{
	//{{AFX_DATA_INIT(COutputDlg)
	m_outputfolder = _T("");
	//}}AFX_DATA_INIT
	m_bkgndfile = _T("");
}


void COutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutputDlg)
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_outputfolder);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_background, m_selectoutputstyle);
	DDX_Check(pDX, IDC_CHEK_Clddetection2, m_CHEK_Clddetection2);
	DDX_Check(pDX, IDC_CHEK_Pseudocolor, m_CHEK_Pseudocolor);
	DDX_Check(pDX, IDC_CHEK_Kpseudocolor, m_CHEK_Kpseudocolor);
	DDX_Check(pDX, IDC_CHEK_Statistics, m_CHEK_Statistics);
	DDX_Check(pDX, IDC_CHEK_Compound, m_CHEK_Compound);
	DDX_Text(pDX, IDC_EDIT_BKGND, m_bkgndfile);
}


BEGIN_MESSAGE_MAP(COutputDlg, CDialog)
	//{{AFX_MSG_MAP(COutputDlg)
	ON_BN_CLICKED(IDC_BUT_OUTSEL, OnButOutsel)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_background, &COutputDlg::OnBnClickedRadiobackground)
	ON_BN_CLICKED(IDC_RADIO_abnormaldetection, &COutputDlg::OnBnClickedRadioabnormaldetection)
	ON_BN_CLICKED(IDC_CHEK_Pseudocolor, &COutputDlg::OnBnClickedChekPseudocolor)
	ON_BN_CLICKED(IDC_CHEK_Kpseudocolor, &COutputDlg::OnBnClickedChekKpseudocolor)
	ON_BN_CLICKED(IDC_BUT_SELBKGND, &COutputDlg::OnBnClickedButSelbkgnd)
	ON_BN_CLICKED(IDC_CHEK_Statistics, &COutputDlg::OnBnClickedChekStatistics)
	ON_BN_CLICKED(IDC_CHEK_Compound, &COutputDlg::OnBnClickedChekCompound)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputDlg message handlers

void COutputDlg::OnButOutsel() 
{
	// TODO: Add your control notification handler code here
	//首次打开时需要设置一下默认目录，之后会自动记住上次选择的文件夹
	if (CSelectFolderDlg::m_filePath.GetLength()==0) CSelectFolderDlg::m_filePath=m_outputfolder;
	m_outputfolder=CSelectFolderDlg::Show();
	UpdateData(FALSE);
}

BOOL COutputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_outputfolder=((CLDFProcessDlg*)GetParent())->m_outputfolder;


	if(0==((CLDFProcessDlg*)GetParent())->m_selectoutputstyle){
		//((CButton*)GetDlgItem(IDC_RADIO_Folder))->SetCheck(TRUE);
		m_selectoutputstyle=0;
		GetDlgItem(IDC_CHEK_Pseudocolor)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHEK_Kpseudocolor)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHEK_Statistics)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHEK_Compound)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BKGND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_SELBKGND)->EnableWindow(FALSE);}
	else{
		//((CButton*)GetDlgItem(IDC_RADIO_Files))->SetCheck(TRUE);
		m_selectoutputstyle=1;
		GetDlgItem(IDC_CHEK_Clddetection2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_together)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_split)->EnableWindow(FALSE);}

	m_CHEK_Clddetection2=((CLDFProcessDlg*)GetParent())->m_CHEK_Clddetection2;
	m_CHEK_Pseudocolor=((CLDFProcessDlg*)GetParent())->m_CHEK_Pseudocolor;
	m_CHEK_Kpseudocolor=((CLDFProcessDlg*)GetParent())->m_CHEK_Kpseudocolor;
	m_CHEK_Statistics=((CLDFProcessDlg*)GetParent())->m_CHEK_Statistics;
	m_CHEK_Compound=((CLDFProcessDlg*)GetParent())->m_CHEK_Compound;
	
	m_bkgndfile=((CLDFProcessDlg*)GetParent())->m_bkgndfile;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COutputDlg::OnBnClickedRadiobackground()
{
	// TODO: 在此添加控件通知处理程序代码
	m_selectoutputstyle=0;
	GetDlgItem(IDC_CHEK_Clddetection2)->EnableWindow();
	GetDlgItem(IDC_RADIO_together)->EnableWindow();
	GetDlgItem(IDC_RADIO_split)->EnableWindow();

	GetDlgItem(IDC_CHEK_Pseudocolor)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHEK_Kpseudocolor)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHEK_Statistics)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHEK_Compound)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BKGND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_SELBKGND)->EnableWindow(FALSE);

	m_CHEK_Pseudocolor=FALSE;
	m_CHEK_Kpseudocolor=FALSE;
	m_CHEK_Statistics=FALSE;
	m_CHEK_Compound=FALSE;
	UpdateData(FALSE);
}


void COutputDlg::OnBnClickedRadioabnormaldetection()
{
	// TODO: 在此添加控件通知处理程序代码
	m_selectoutputstyle=1;
	GetDlgItem(IDC_CHEK_Pseudocolor)->EnableWindow();
	GetDlgItem(IDC_CHEK_Kpseudocolor)->EnableWindow();
	GetDlgItem(IDC_CHEK_Statistics)->EnableWindow();
	GetDlgItem(IDC_CHEK_Compound)->EnableWindow();

	GetDlgItem(IDC_CHEK_Clddetection2)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_together)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_split)->EnableWindow(FALSE);

	m_CHEK_Clddetection2=FALSE;

	UpdateData(FALSE);
}


void COutputDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	((CLDFProcessDlg*)GetParent())->m_outputfolder=m_outputfolder;
	((CLDFProcessDlg*)GetParent())->m_selectoutputstyle=m_selectoutputstyle;
	((CLDFProcessDlg*)GetParent())->m_CHEK_Clddetection2=m_CHEK_Clddetection2;
	((CLDFProcessDlg*)GetParent())->m_CHEK_Pseudocolor=m_CHEK_Pseudocolor;
	((CLDFProcessDlg*)GetParent())->m_CHEK_Kpseudocolor=m_CHEK_Kpseudocolor;
	((CLDFProcessDlg*)GetParent())->m_CHEK_Statistics=m_CHEK_Statistics;
	((CLDFProcessDlg*)GetParent())->m_CHEK_Compound=m_CHEK_Compound;
	((CLDFProcessDlg*)GetParent())->m_bkgndfile=m_bkgndfile;
	CDialog::OnOK();
}


void COutputDlg::OnBnClickedChekPseudocolor()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_CHEK_Pseudocolor)
	{
		GetDlgItem(IDC_EDIT_BKGND)->EnableWindow();
		GetDlgItem(IDC_BUT_SELBKGND)->EnableWindow();
	}
	else if(!m_CHEK_Kpseudocolor)
	{
		GetDlgItem(IDC_EDIT_BKGND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_SELBKGND)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}


void COutputDlg::OnBnClickedChekKpseudocolor()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_CHEK_Kpseudocolor)
	{
		GetDlgItem(IDC_EDIT_BKGND)->EnableWindow();
		GetDlgItem(IDC_BUT_SELBKGND)->EnableWindow();
	}
	else if(!m_CHEK_Pseudocolor)
	{
		GetDlgItem(IDC_EDIT_BKGND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_SELBKGND)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}


void COutputDlg::OnBnClickedButSelbkgnd()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD flag = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CFileDialog dlgFile(TRUE,  _T("*.tif"), _T("*.tif"), flag,
		TEXT("支持的文件 (*.tiff;*.tif)|*.tiff;*.tif|下步支持的文件(*.gpf)|*.gpf|所有文件(*.*)|*.*||"),NULL);

    if (dlgFile.DoModal() == IDOK)
    {
        m_bkgndfile = dlgFile.GetPathName();
    }
    UpdateData(FALSE);
}



void COutputDlg::OnBnClickedChekStatistics()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
}


void COutputDlg::OnBnClickedChekCompound()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
}


