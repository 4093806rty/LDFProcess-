#pragma once

#include "CalculateDlg.h"

// CprocessUIThread
//�ļ�����ģ��

class CprocessUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CprocessUIThread)

protected:
	CprocessUIThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CprocessUIThread();
	CString m_inputFile;

public:
	CalculateDlg *m_pProgressDlg;//���Ƚ���
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	afx_msg void OnThreadInfo(WPARAM wParam,LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
};


