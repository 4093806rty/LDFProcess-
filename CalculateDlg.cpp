// CalculateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "CalculateDlg.h"
#include "afxdialogex.h"


// CalculateDlg �Ի���

IMPLEMENT_DYNAMIC(CalculateDlg, CDialog)

CalculateDlg::CalculateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CalculateDlg::IDD, pParent)
	//, m_szSrcPath(_T(""))
	//, m_szDesPath(_T(""))
	, m_inputFile(_T(""))
	, m_Percent(_T(""))
{

}

CalculateDlg::~CalculateDlg()
{
}

void CalculateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_STATIC_INPUT, m_inputFile);
	DDX_Control(pDX, IDC_STATIC_TIME, m_stTime);
	DDX_Text(pDX, IDC_STATIC_PERCENT, m_Percent);
}


BEGIN_MESSAGE_MAP(CalculateDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CalculateDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CalculateDlg ��Ϣ�������


BOOL CalculateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_Progress.SetRange(0,100);
	//m_Animate.Open(IDR_AVI1);
	//m_stWeb.SetURL("http://blog.csdn.net/cbnotes");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CalculateDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (MessageBox(_T("ȷ��Ҫ����ֹ����ǰ�Ĳ�����?"),_T("������ʾ��Ϣ"),MB_OKCANCEL|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
	{
		//ֹ֪ͣͨ���˴���Ƶ���Ϣ��ͬ���ͷ�������ʱ������
		OnCancel();
	}

	//CDialog::OnCancel();
}


void CalculateDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	DestroyWindow();
	//CDialog::OnCancel();
}


void CalculateDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::PostNcDestroy();
	delete this;
}


void CalculateDlg::UpdateTime(float fTime)
{
	CString szStr;
	if (fTime >= 60)
	{
		szStr.Format(_T("%.2f /��"),fTime/60.0);
	}
	else
	{
		szStr.Format(_T("%.1f /��"),fTime);
	}
	m_stTime.SetWindowText(szStr);
}
