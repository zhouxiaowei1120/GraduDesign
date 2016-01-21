// GraduDesignView.cpp : implementation of the CGraduDesignView class
//

#include "stdafx.h"
#include "GraduDesign.h"

#include "GraduDesignDoc.h"
#include "GraduDesignView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignView

IMPLEMENT_DYNCREATE(CGraduDesignView, CView)

BEGIN_MESSAGE_MAP(CGraduDesignView, CView)
	//{{AFX_MSG_MAP(CGraduDesignView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignView construction/destruction

CGraduDesignView::CGraduDesignView()
{
	// TODO: add construction code here

}

CGraduDesignView::~CGraduDesignView()
{
}

BOOL CGraduDesignView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignView drawing

void CGraduDesignView::OnDraw(CDC* pDC)
{
	CGraduDesignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignView diagnostics

#ifdef _DEBUG
void CGraduDesignView::AssertValid() const
{
	CView::AssertValid();
}

void CGraduDesignView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraduDesignDoc* CGraduDesignView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraduDesignDoc)));
	return (CGraduDesignDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignView message handlers
