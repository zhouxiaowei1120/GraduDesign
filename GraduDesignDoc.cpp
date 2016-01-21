// GraduDesignDoc.cpp : implementation of the CGraduDesignDoc class
//

#include "stdafx.h"
#include "GraduDesign.h"
#include "Floor.h"

#include "GraduDesignDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignDoc

IMPLEMENT_DYNCREATE(CGraduDesignDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraduDesignDoc, CDocument)
	//{{AFX_MSG_MAP(CGraduDesignDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignDoc construction/destruction

CGraduDesignDoc::CGraduDesignDoc()
{
	// TODO: add one-time construction code here

}

CGraduDesignDoc::~CGraduDesignDoc()
{
}

BOOL CGraduDesignDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGraduDesignDoc serialization

void CGraduDesignDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraduDesignDoc diagnostics

#ifdef _DEBUG
void CGraduDesignDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraduDesignDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CGraduDesignDoc commands
