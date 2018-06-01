// ImgDoc.h : interface of the CMainDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGDOC_H__347CD220_3BC5_4FD1_8B3D_B5039380E3B9__INCLUDED_)
#define AFX_IMGDOC_H__347CD220_3BC5_4FD1_8B3D_B5039380E3B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "imgfile.h"

class CMainDoc : public CDocument
{
protected: // create from serialization only
	CMainDoc();
	DECLARE_DYNCREATE(CMainDoc)

// Attributes
public:
	ImgFile* GetImage(){return m_pImg;}
	/* clone pImg and replace the old one, which is relased automatically. */ 
	BOOL	UpdateImage(ImgFile* pImg);
	SIZE GetImageSize(){return m_szImage;}
// Operations
protected:
	ImgFile*	m_pImg;
	ImgFile*	m_pImgEdge;
	ImgFile*	m_pImgOut;
	SIZE	m_szImage;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMainDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGDOC_H__347CD220_3BC5_4FD1_8B3D_B5039380E3B9__INCLUDED_)
