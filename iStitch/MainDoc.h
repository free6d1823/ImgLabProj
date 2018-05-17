
// MainDoc.h : interface of the CMainDoc class
//


#pragma once

#include "ImgFile.h"

#define MAX_IMAGES	4
typedef struct _ImageProperty {
	POINT	pos; //position on the canvas
	SIZE		size;	//size of the scaled image on canvas
	float		rotate; //rotation radian of the image
	float		scale;
	TCHAR		path[MAX_PATH];	//file path of the image
/* Intermediate data */
	RECT	rcBound; //boundary of  image after transfomation
	ImgFile*	pImg;		//original Imag data

}ImageProperty;

typedef struct _ProjectSetting{
	TCHAR		title[64];	
	int nImages;	//number of pictures
	SIZE canvas;
	ImageProperty	ip[MAX_IMAGES];
}ProjectSetting, * PProjectSetting;

class CMainDoc : public CDocument
{
protected: // create from serialization only
	CMainDoc();
	DECLARE_DYNCREATE(CMainDoc)

	PProjectSetting GetProjectSetting() { return &m_ProjectSetting;};
// Attributes
public:
// Operations
		BOOL InitPropertySetting(PProjectSetting pPs);
		void FreePropertySetting();
public:
		//modify image parameters
		//use these functions to modify partial parameters
		void IncreaseImagePosX(int id, int x); 
		void IncreaseImagePosY(int id, int y);
		void IncreaseImageScale(int id, float scale);
		void IncreaseImageRotate(int id, float radiun);
		void GetImagesBoundary(CRect& rc); 
// Overrides
public:
	virtual BOOL OnNewDocument();

// Implementation
public:
	virtual ~CMainDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ProjectSetting	m_ProjectSetting;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileProperties();
	afx_msg void OnUpdateFileProperties(CCmdUI *pCmdUI);
	afx_msg void OnFileExport();
	afx_msg void OnUpdateFileExport(CCmdUI *pCmdUI);
};


