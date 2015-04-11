#if !defined(AFX_INPUTDLG_H__414083DB_83A7_4E50_9D3B_6D349ACB8A7E__INCLUDED_)
#define AFX_INPUTDLG_H__414083DB_83A7_4E50_9D3B_6D349ACB8A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog

class CInputDlg : public CDialog
{
// Construction
public:
	CRect rectSmall;
	CRect rectLarge;
	CMapStringToString m_repeatcheck;
	CInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDlg)
	enum { IDD = IDD_DIALOG_INPUT };
	CListBox	m_listFile;
	CString	m_inputfolder;
	CString	m_strLastDir_InputFiles;//上次打开的文件夹（选择文件组时打开的）
	CString	m_minLon;
	CString	m_maxLon;
	CString	m_resLon;
	CString	m_minLat;
	CString	m_maxLat;
	CString	m_resLat;
	int		m_selectinputstyle;
	BOOL	m_InputWindow_Extend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDlg)
	afx_msg void OnButSel();
	afx_msg void OnRADIOFolder();
	afx_msg void OnRADIOFiles();
	afx_msg void OnBUTAdd();
	afx_msg void OnSelchangeListFile();
	afx_msg void OnBUTDel();
	afx_msg void OnBUTClear();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTExtend();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void ShowFilesNum();//显示选择的文件数目
	afx_msg void OnEnKillfocusEditInput();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__414083DB_83A7_4E50_9D3B_6D349ACB8A7E__INCLUDED_)
