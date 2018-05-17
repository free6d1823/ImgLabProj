// ImgDoc.cpp : implementation of the CMainDoc class
//

#include "stdafx.h"
#include "iViewer.h"

#include "MainDoc.h"
#include "ImgProcess.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDoc
#define CLIP(x)     min(255, max(0,x))

IMPLEMENT_DYNCREATE(CMainDoc, CDocument)

BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
	//{{AFX_MSG_MAP(CMainDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDoc construction/destruction

CMainDoc::CMainDoc()
{
	// TODO: add one-time construction code here
	m_pImg = NULL;
    m_pImgEdge = NULL;
    m_pImgOut = NULL;
}

CMainDoc::~CMainDoc()
{
	if(m_pImg)
		delete m_pImg;
	m_pImg = NULL;
	if(m_pImgEdge)
		delete m_pImgEdge;
    m_pImgEdge = NULL;
	if(m_pImgOut)
		delete m_pImgOut;
    m_pImgOut = NULL;
}

BOOL CMainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMainDoc serialization

void CMainDoc::Serialize(CArchive& ar)
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
// CMainDoc diagnostics

#ifdef _DEBUG
void CMainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainDoc commands

void CMainDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pImg)
		delete m_pImg;
	m_pImg = NULL;
	CDocument::DeleteContents();
}

BOOL CMainDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	
	// TODO: Add your specialized creation code here
	ImgFile* pImg = new ImgFile;
	ASSERT(pImg);
	if(0!=pImg->Load(lpszPathName))
	{
		delete pImg;
		AfxMessageBox("Unable to open the file!");
		return FALSE;
	}
	if(m_pImg)
		delete m_pImg;
	m_pImg = pImg;

	m_szImage.cy = m_pImg->Height();
	m_szImage.cx = m_pImg->Width();

//CWinApp will call CView::OnInitialUpdate again
	UpdateAllViews(NULL);

	return TRUE;
}

void CMainDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	
}

void CMainDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pImg!=NULL);
	
}

void CMainDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	
}

void CMainDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pImg!=NULL);
	
}

BOOL CMainDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::OnSaveDocument(lpszPathName);
}
