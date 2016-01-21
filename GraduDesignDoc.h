// GraduDesignDoc.h : interface of the CGraduDesignDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADUDESIGNDOC_H__D6E42A9C_3206_44F8_B7CE_F8DA65707E2B__INCLUDED_)
#define AFX_GRADUDESIGNDOC_H__D6E42A9C_3206_44F8_B7CE_F8DA65707E2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGraduDesignDoc : public CDocument
{
protected: // create from serialization only
	CGraduDesignDoc();
	DECLARE_DYNCREATE(CGraduDesignDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraduDesignDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraduDesignDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraduDesignDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADUDESIGNDOC_H__D6E42A9C_3206_44F8_B7CE_F8DA65707E2B__INCLUDED_)
