#if !defined(AFX_CLDETCDLG_H__D0899A86_DC65_42C1_B092_12EA75491C36__INCLUDED_)
#define AFX_CLDETCDLG_H__D0899A86_DC65_42C1_B092_12EA75491C36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CldetcDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCldetcDlg dialog

class CCldetcDlg : public CDialog
{
// Construction
public:
	CCldetcDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCldetcDlg)
	enum { IDD = IDD_DIALOG_clouddetection };
	double	m_threhold_Month1;
	double	m_threhold_Month10;
	double	m_threhold_Month11;
	double	m_threhold_Month12;
	double	m_threhold_Month2;
	double	m_threhold_Month3;
	double	m_threhold_Month4;
	double	m_threhold_Month5;
	double	m_threhold_Month6;
	double	m_threhold_Month7;
	double	m_threhold_Month8;
	double	m_threhold_Month9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCldetcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCldetcDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLDETCDLG_H__D0899A86_DC65_42C1_B092_12EA75491C36__INCLUDED_)
