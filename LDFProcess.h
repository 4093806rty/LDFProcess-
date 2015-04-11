// LDFProcess.h : main header file for the LDFPROCESS application
//

#if !defined(AFX_LDFPROCESS_H__E14DDD8C_F6DB_4A1F_A92A_DD27C6A23063__INCLUDED_)
#define AFX_LDFPROCESS_H__E14DDD8C_F6DB_4A1F_A92A_DD27C6A23063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLDFProcessApp:
// See LDFProcess.cpp for the implementation of this class
//

class CLDFProcessApp : public CWinApp
{
public:
	CLDFProcessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLDFProcessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLDFProcessApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDFPROCESS_H__E14DDD8C_F6DB_4A1F_A92A_DD27C6A23063__INCLUDED_)
