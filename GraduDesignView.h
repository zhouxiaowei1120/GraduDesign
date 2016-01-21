// GraduDesignView.h : interface of the CGraduDesignView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADUDESIGNVIEW_H__ED5E4979_D947_42A4_A3FC_F50503EA6D9A__INCLUDED_)
#define AFX_GRADUDESIGNVIEW_H__ED5E4979_D947_42A4_A3FC_F50503EA6D9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGraduDesignView : public CView
{
protected: // create from serialization only
	CGraduDesignView();
	DECLARE_DYNCREATE(CGraduDesignView)

// Attributes
public:
	CGraduDesignDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraduDesignView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraduDesignView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraduDesignView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GraduDesignView.cpp
inline CGraduDesignDoc* CGraduDesignView::GetDocument()
   { return (CGraduDesignDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADUDESIGNVIEW_H__ED5E4979_D947_42A4_A3FC_F50503EA6D9A__INCLUDED_)
