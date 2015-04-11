#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CalculateDlg 对话框

class CalculateDlg : public CDialog
{
	DECLARE_DYNAMIC(CalculateDlg)

public:
	CalculateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CalculateDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALCULATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


private:
	//CAnimateCtrl m_Animate;
private:
	//CHyperLink m_stWeb;
public:
	//CString m_szDesPath;
private:
	//CStatic m_stSpeed;
public:
	//void UpdateSpeed(unsigned long nSpeed);

public:
	CProgressCtrl m_Progress;
	CString m_inputFile;
	CString m_Percent;
	CStatic m_stTime;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	void UpdateTime(float fTime);
};
