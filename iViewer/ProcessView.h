#pragma once
#include "basicview.h"
#include "Gride.h"


class ProcessView : public BasicView
{
	DECLARE_DYNAMIC(ProcessView)

public:
	ProcessView();
	virtual ~ProcessView();
	virtual	void SetImgFile(ImgFile* pImg);
	virtual	void SetParam(void* pParam);
protected:
	//(u, v) destination coordinates
	//(x,y)  source coordinates
	virtual BOOL		PosMap(int u, int v, float&x, float&y);
	//interpolate the color in position x,y in source coordinates
	virtual COLORREF GetColor(BYTE* pSrc, int stride, float x, float y);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BasicView)
public:
	BOOL SetScrollPos(int nX, int nY);
	BOOL			m_bShowGride;
	Gride m_gride;
	//}}AFX_VIRTUAL
	ImgFile*	m_pOriginalImg;	//a reference pointer to original image

	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnPreview();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewGride();
	afx_msg void OnUpdateViewGride(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolbox(CCmdUI *pCmdUI);
	afx_msg void OnCommandToolbox(UINT nCmd);
protected:

};
