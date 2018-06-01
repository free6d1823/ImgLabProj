// ImgDoc.cpp : implementation of the CMainDoc class
//

#include "stdafx.h"
#include "resource.h"

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

BOOL	CMainDoc::UpdateImage(ImgFile* pImg)
{
	if(!pImg) {
		AfxMessageBox("Failed to update image!");
		return FALSE;
	}
	ImgFile* pNew = new ImgFile();
	if(Img_OK != pNew->CopyFrom(pImg)) {
		delete pNew;
		AfxMessageBox("Failed to duplicate image!");
		return FALSE;
	}
	if(m_pImg) delete m_pImg;
	m_pImg = pNew;
	this->SetModifiedFlag();
	UpdateAllViews(NULL);
	return TRUE;
}

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

	//UI will query image size frequently
	m_szImage.cy = m_pImg->Height();
	m_szImage.cx = m_pImg->Width();

//CWinApp will call CView::OnInitialUpdate again
	UpdateAllViews(NULL);

	return TRUE;
}

void CMainDoc::OnFileSaveAs() 
{
	CDocument::OnFileSaveAs();
	
}

void CMainDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pImg!=NULL);
	
}

void CMainDoc::OnFileSave() 
{
	CDocument::OnFileSave();
	
}

void CMainDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(this->IsModified());	
}

BOOL CMainDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	TCHAR path[MAX_PATH]; 
	strcpy_s(path, MAX_PATH, lpszPathName);
	TCHAR* p = strrchr(path, _T('.'));
	if (p == NULL) {
		//append ".
		strcat_s(path, MAX_PATH, _T(".bmp"));
	} else {
		if( _stricmp(p, ".bmp") != 0) {
			int ret= AfxMessageBox("We support save BMP file only. Do you want to continue?", MB_YESNO);
			if (ret != IDYES)
				return FALSE;
		}
		strcpy_s(p, 32, ".bmp");
	}
	TRACE("Save file %s\n", path);
	return m_pImg->WriteBmp(path);
	
}
