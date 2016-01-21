// GraduDesign.h : main header file for the GRADUDESIGN application
//

#if !defined(AFX_GRADUDESIGN_H__5C0A101A_9028_4F5B_9853_31AC0D7FF46D__INCLUDED_)
#define AFX_GRADUDESIGN_H__5C0A101A_9028_4F5B_9853_31AC0D7FF46D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignApp:
// See GraduDesign.cpp for the implementation of this class
//

class CGraduDesignApp : public CWinApp
{
public:
	CGraduDesignApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraduDesignApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGraduDesignApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADUDESIGN_H__5C0A101A_9028_4F5B_9853_31AC0D7FF46D__INCLUDED_)
