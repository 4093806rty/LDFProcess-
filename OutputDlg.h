#if !defined(AFX_OUTPUTDLG_H__63E29822_671C_4870_B9D9_2B761053CC4B__INCLUDED_)
#define AFX_OUTPUTDLG_H__63E29822_671C_4870_B9D9_2B761053CC4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutputDlg dialog

class COutputDlg : public CDialog
{
// Construction
public:
	COutputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COutputDlg)
	enum { IDD = IDD_DIALOG_OUTPUT };
	CString	m_outputfolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COutputDlg)
	afx_msg void OnButOutsel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_selectoutputstyle;
	afx_msg void OnBnClickedRadiobackground();
	afx_msg void OnBnClickedRadioabnormaldetection();
	BOOL m_CHEK_Clddetection2;
	BOOL m_CHEK_Pseudocolor;
	BOOL m_CHEK_Kpseudocolor;
	BOOL m_CHEK_Statistics;
	BOOL m_CHEK_Compound;
	afx_msg void OnBnClickedChekPseudocolor();
	afx_msg void OnBnClickedChekKpseudocolor();
	afx_msg void OnBnClickedButSelbkgnd();
	CString m_bkgndfile;
	afx_msg void OnBnClickedChekStatistics();
	afx_msg void OnBnClickedChekCompound();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTDLG_H__63E29822_671C_4870_B9D9_2B761053CC4B__INCLUDED_)
