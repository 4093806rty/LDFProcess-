// CalculateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "CalculateDlg.h"
#include "afxdialogex.h"


// CalculateDlg 对话框

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


// CalculateDlg 消息处理程序


BOOL CalculateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    m_Progress.SetRange(0,100);
	//m_Animate.Open(IDR_AVI1);
	//m_stWeb.SetURL("http://blog.csdn.net/cbnotes");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CalculateDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	if (MessageBox(_T("确定要【中止】当前的操作吗?"),_T("操作提示信息"),MB_OKCANCEL|MB_DEFBUTTON2|MB_ICONQUESTION) == IDYES)
	{
		//停止通知，此处设计到信息的同步和反馈，暂时不考虑
		OnCancel();
	}

	//CDialog::OnCancel();
}


void CalculateDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	DestroyWindow();
	//CDialog::OnCancel();
}


void CalculateDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::PostNcDestroy();
	delete this;
}


void CalculateDlg::UpdateTime(float fTime)
{
	CString szStr;
	if (fTime >= 60)
	{
		szStr.Format(_T("%.2f /分"),fTime/60.0);
	}
	else
	{
		szStr.Format(_T("%.1f /秒"),fTime);
	}
	m_stTime.SetWindowText(szStr);
}
