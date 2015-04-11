// AbnmdlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "AbnmdlDlg.h"
#include "LDFProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbnmdlDlg dialog


CAbnmdlDlg::CAbnmdlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAbnmdlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbnmdlDlg)
	m_algorithm = -1;
	//}}AFX_DATA_INIT
}


void CAbnmdlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbnmdlDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_algorithm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbnmdlDlg, CDialog)
	//{{AFX_MSG_MAP(CAbnmdlDlg)
	ON_BN_CLICKED(IDC_RADIO_KValue, OnRADIOKValue)
	ON_BN_CLICKED(IDC_RADIO_RST, OnRadioRst)
	ON_BN_CLICKED(IDC_RADIO_Wavelet, OnRADIOWavelet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbnmdlDlg message handlers

void CAbnmdlDlg::OnRADIOKValue() 
{
	// TODO: Add your control notification handler code here
	m_algorithm=0;
}

void CAbnmdlDlg::OnRadioRst() 
{
	// TODO: Add your control notification handler code here
	m_algorithm=1;
}

void CAbnmdlDlg::OnRADIOWavelet() 
{
	// TODO: Add your control notification handler code here
	m_algorithm=2;
}

void CAbnmdlDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	((CLDFProcessDlg*)GetParent())->m_algorithm=m_algorithm;

	CDialog::OnOK();
}

BOOL CAbnmdlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_algorithm=((CLDFProcessDlg*)GetParent())->m_algorithm;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
