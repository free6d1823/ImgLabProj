/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                                                                              *
 *                                                                                                                                                                       *
 ****************************************************************************/
/**
 * \file MainDoc.h : interface of the CMainDoc class
 */



#pragma once

#include "ImgFile.h"
#include "ImgLab.h"

#define MAX_IMAGES	4
/**
 * \struct  _ImageProperty 
 * \brief The source image properties and stitching parameters
  */
typedef struct _ImageProperty {
	POINT	pos;									/*!< the top-left of this image on canvas   */
	SIZE		size;									/*!< size of the original image, in pixel    */	
	float		rotate;								/*!< rotation radian of the image    */	
	float		scale;									/*!< cale factor of the image    */	
	TCHAR		path[MAX_PATH];		/*!< file path of the image    */	 
/* Intermediate data */
	RECT	rcBound; /*!< boundary of  image after transfomation, in canvas space*/	
	ImgFile*	pImg;		/*!< original Imag data*/	
}ImageProperty;

/**
 * \struct  _ProjectSetting 
 * \brief The stitching images parameters
  */
typedef struct _ProjectSetting{
	TCHAR		title[64];								/*!< name of this project settings   */
	int nImages;												/*!< number of pictures     */
	SIZE canvas;												/*!< size of stitched image */
	ImageProperty	ip[MAX_IMAGES];	/*!< number of pictures     */
}ProjectSetting, * PProjectSetting;

/**
 * \class CMainDoc 
 * \brief This class stores the images properties and stitched image
  */
class CMainDoc : public CDocument
{
protected: // create from serialization only
	CMainDoc();
	DECLARE_DYNCREATE(CMainDoc)

	PProjectSetting GetProjectSetting() { return &m_ProjectSetting;};
// Attributes
public:
// Operations
		BOOL UpdatePropertySetting(PProjectSetting pPs);
		void FreePropertySetting();
public:
/**
 * \brief use these functions to modify partial parameters
  */
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

	int DoStitching(int nTopID, int mode);									/*!< make stitched image */
	ImgFile* GetImage(){ return m_pImg;};								/*!< get the stitched image, you might need to call DoStitch before to get stitched result */
protected:
	ProjectSetting	m_ProjectSetting;		/*!< source images propert and transformation parameters*/
	ImgFile*	m_pImg;								/*!< stitched image */
	ImgLab*	m_pImgLab;
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


