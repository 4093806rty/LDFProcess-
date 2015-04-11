// CldetcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "CldetcDlg.h"
#include "LDFProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCldetcDlg dialog


CCldetcDlg::CCldetcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCldetcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCldetcDlg)
	m_threhold_Month1 = 0.0;
	m_threhold_Month10 = 0.0;
	m_threhold_Month11 = 0.0;
	m_threhold_Month12 = 0.0;
	m_threhold_Month2 = 0.0;
	m_threhold_Month3 = 0.0;
	m_threhold_Month4 = 0.0;
	m_threhold_Month5 = 0.0;
	m_threhold_Month6 = 0.0;
	m_threhold_Month7 = 0.0;
	m_threhold_Month8 = 0.0;
	m_threhold_Month9 = 0.0;
	//}}AFX_DATA_INIT
}


void CCldetcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCldetcDlg)
	DDX_Text(pDX, IDC_EDIT_MONTH1, m_threhold_Month1);
	DDX_Text(pDX, IDC_EDIT_MONTH10, m_threhold_Month10);
	DDX_Text(pDX, IDC_EDIT_MONTH11, m_threhold_Month11);
	DDX_Text(pDX, IDC_EDIT_MONTH12, m_threhold_Month12);
	DDX_Text(pDX, IDC_EDIT_MONTH2, m_threhold_Month2);
	DDX_Text(pDX, IDC_EDIT_MONTH3, m_threhold_Month3);
	DDX_Text(pDX, IDC_EDIT_MONTH4, m_threhold_Month4);
	DDX_Text(pDX, IDC_EDIT_MONTH5, m_threhold_Month5);
	DDX_Text(pDX, IDC_EDIT_MONTH6, m_threhold_Month6);
	DDX_Text(pDX, IDC_EDIT_MONTH7, m_threhold_Month7);
	DDX_Text(pDX, IDC_EDIT_MONTH8, m_threhold_Month8);
	DDX_Text(pDX, IDC_EDIT_MONTH9, m_threhold_Month9);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCldetcDlg, CDialog)
	//{{AFX_MSG_MAP(CCldetcDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCldetcDlg message handlers

BOOL CCldetcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_threhold_Month1 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[1];
	m_threhold_Month10 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[10];
	m_threhold_Month11 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[11];
	m_threhold_Month12 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[12];
	m_threhold_Month2 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[2];
	m_threhold_Month3 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[3];
	m_threhold_Month4 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[4];
	m_threhold_Month5 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[5];
	m_threhold_Month6 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[6];
	m_threhold_Month7 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[7];
	m_threhold_Month8 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[8];
	m_threhold_Month9 = ((CLDFProcessDlg*)GetParent())->m_thrld_Month[9];
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCldetcDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[1] = m_threhold_Month1;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[10] = m_threhold_Month10;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[11] = m_threhold_Month11;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[12] = m_threhold_Month12;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[2] = m_threhold_Month2;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[3] = m_threhold_Month3;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[4] = m_threhold_Month4;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[5] = m_threhold_Month5;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[6] = m_threhold_Month6;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[7] = m_threhold_Month7;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[8] = m_threhold_Month8;
	((CLDFProcessDlg*)GetParent())->m_thrld_Month[9] = m_threhold_Month9;

	CDialog::OnOK();
}
