
// ndummy_clientDoc.cpp : implementation of the CDummyClientDoc class
//

#include "stdafx.h"
#include "ndummy_client.h"

#include "ndummy_clientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDummyClientDoc

IMPLEMENT_DYNCREATE(CDummyClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CDummyClientDoc, CDocument)
END_MESSAGE_MAP()


// CDummyClientDoc construction/destruction

CDummyClientDoc::CDummyClientDoc()
{
	// TODO: add one-time construction code here

}

CDummyClientDoc::~CDummyClientDoc()
{
}

BOOL CDummyClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDummyClientDoc serialization

void CDummyClientDoc::Serialize(CArchive& ar)
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


// CDummyClientDoc diagnostics

#ifdef _DEBUG
void CDummyClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDummyClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDummyClientDoc commands
