// processUIThread.cpp : 实现文件
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "processUIThread.h"


// CprocessUIThread

IMPLEMENT_DYNCREATE(CprocessUIThread, CWinThread)

CprocessUIThread::CprocessUIThread()
{
	m_pProgressDlg = NULL;
}

CprocessUIThread::~CprocessUIThread()
{
}

BOOL CprocessUIThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	
	return TRUE;
}

int CprocessUIThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CprocessUIThread, CWinThread)
	ON_THREAD_MESSAGE(WM_THREADINFO,&CprocessUIThread::OnThreadInfo)
END_MESSAGE_MAP()


// CprocessUIThread 消息处理程序

//显示消息处理函数
void CprocessUIThread::OnThreadInfo(WPARAM wParam,LPARAM lParam)
{
	if (wParam == 0)
	{//源文件路径参数
		m_inputFile.Format(_T("%s"),lParam);
		//AfxMessageBox(m_szSrcPath);
	}
	else if (wParam == 1)
	{//目标文件路径参数
		//m_szDesPath.Format("%s",lParam);
		//AfxMessageBox(m_szDesPath);
	}
	else if (wParam == 2)
	{//启动
		m_pProgressDlg = new CalculateDlg;
		m_pProgressDlg->Create(IDD_DIALOG_CALCULATE);
		m_pProgressDlg->m_inputFile = m_inputFile;
		(m_pProgressDlg->m_Percent).Format(_T("%d%%"),0);
		//m_pProgressDlg->m_szDesPath = m_szDesPath;
        m_pProgressDlg->UpdateData(FALSE);
		m_pProgressDlg->ShowWindow(TRUE);
	}
	else if (wParam == 3)
	{//进度
		m_pProgressDlg->m_Progress.SetPos(lParam);
		//百分比
		(m_pProgressDlg->m_Percent).Format(_T("%d%%"),lParam);
        m_pProgressDlg->UpdateData(FALSE);
	}
	else if (wParam == 4)
	{//显示待处理文件名
		m_inputFile.Format(_T("%s"),lParam);
		m_pProgressDlg->m_inputFile = m_inputFile;
        m_pProgressDlg->UpdateData(FALSE);
	}
	else if (wParam == 5)
	{//时间
		float *p = (float *)lParam;
		m_pProgressDlg->UpdateTime(*p);
	}
	else if (wParam == 6)
	{//显示待处理文件名
		m_inputFile.Format(_T("%s"),lParam);
		m_pProgressDlg->m_inputFile = m_inputFile;
		(m_pProgressDlg->m_Percent).Format(_T("%d%%"),0);
		//m_pProgressDlg->m_szDesPath = m_szDesPath;
	//进度
		m_pProgressDlg->m_Progress.SetPos(0);
	//百分比
		(m_pProgressDlg->m_Percent).Format(_T("%d%%"),0);
        m_pProgressDlg->UpdateData(FALSE);
	}
	else 
	{//完成
		m_pProgressDlg->OnCancel();
	}
	//return 0;
}