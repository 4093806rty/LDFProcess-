// processUIThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	
	return TRUE;
}

int CprocessUIThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CprocessUIThread, CWinThread)
	ON_THREAD_MESSAGE(WM_THREADINFO,&CprocessUIThread::OnThreadInfo)
END_MESSAGE_MAP()


// CprocessUIThread ��Ϣ�������

//��ʾ��Ϣ������
void CprocessUIThread::OnThreadInfo(WPARAM wParam,LPARAM lParam)
{
	if (wParam == 0)
	{//Դ�ļ�·������
		m_inputFile.Format(_T("%s"),lParam);
		//AfxMessageBox(m_szSrcPath);
	}
	else if (wParam == 1)
	{//Ŀ���ļ�·������
		//m_szDesPath.Format("%s",lParam);
		//AfxMessageBox(m_szDesPath);
	}
	else if (wParam == 2)
	{//����
		m_pProgressDlg = new CalculateDlg;
		m_pProgressDlg->Create(IDD_DIALOG_CALCULATE);
		m_pProgressDlg->m_inputFile = m_inputFile;
		(m_pProgressDlg->m_Percent).Format(_T("%d%%"),0);
		//m_pProgressDlg->m_szDesPath = m_szDesPath;
        m_pProgressDlg->UpdateData(FALSE);
		m_pProgressDlg->ShowWindow(TRUE);
	}
	else if (wParam == 3)
	{//����
		m_pProgressDlg->m_Progress.SetPos(lParam);
		//�ٷֱ�
		(m_pProgressDlg->m_Percent).Format(_T("%d%%"),lParam);
        m_pProgressDlg->UpdateData(FALSE);
	}
	else if (wParam == 4)
	{//��ʾ�������ļ���
		m_inputFile.Format(_T("%s"),lParam);
		m_pProgressDlg->m_inputFile = m_inputFile;
        m_pProgressDlg->UpdateData(FALSE);
	}
	else if (wParam == 5)
	{//ʱ��
		float *p = (float *)lParam;
		m_pProgressDlg->UpdateTime(*p);
	}
	else if (wParam == 6)
	{//��ʾ�������ļ���
		m_inputFile.Format(_T("%s"),lParam);
		m_pProgressDlg->m_inputFile = m_inputFile;
		(m_pProgressDlg->m_Percent).Format(_T("%d%%"),0);
		//m_pProgressDlg->m_szDesPath = m_szDesPath;
	//����
		m_pProgressDlg->m_Progress.SetPos(0);
	//�ٷֱ�
		(m_pProgressDlg->m_Percent).Format(_T("%d%%"),0);
        m_pProgressDlg->UpdateData(FALSE);
	}
	else 
	{//���
		m_pProgressDlg->OnCancel();
	}
	//return 0;
}