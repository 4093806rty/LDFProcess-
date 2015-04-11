#pragma once

#include "CalculateDlg.h"

// CprocessUIThread
//文件复制模块

class CprocessUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CprocessUIThread)

protected:
	CprocessUIThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CprocessUIThread();
	CString m_inputFile;

public:
	CalculateDlg *m_pProgressDlg;//进度界面
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	afx_msg void OnThreadInfo(WPARAM wParam,LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
};


