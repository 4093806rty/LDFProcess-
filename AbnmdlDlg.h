#if !defined(AFX_ABNMDLDLG_H__5254925E_8A35_411B_B929_189180BBF199__INCLUDED_)
#define AFX_ABNMDLDLG_H__5254925E_8A35_411B_B929_189180BBF199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AbnmdlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAbnmdlDlg dialog

class CAbnmdlDlg : public CDialog
{
// Construction
public:
	CAbnmdlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAbnmdlDlg)
	enum { IDD = IDD_DIALOG_abnormalmodel };
	int		m_algorithm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbnmdlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAbnmdlDlg)
	afx_msg void OnRADIOKValue();
	afx_msg void OnRadioRst();
	afx_msg void OnRADIOWavelet();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABNMDLDLG_H__5254925E_8A35_411B_B929_189180BBF199__INCLUDED_)
