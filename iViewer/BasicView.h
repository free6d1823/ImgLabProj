#pragma once


// BasicView
#define MAX_ZOOM		10
#define DEFAULT_ZOOM_FACTOR  	5
class ImgFile;
class BasicView : public CWnd
{
	DECLARE_DYNAMIC(BasicView)

public:
	BasicView();
	virtual ~BasicView();
	virtual	void SetImgFile(ImgFile* pImg);
	ImgFile* GetImgFile(){return m_pImage;}
static int ZOOM_FACTOR[MAX_ZOOM];
	void DrawImage(CDC* pDC);
	virtual BOOL Create(LPCTSTR szName, CWnd* pParentWnd, BOOL bPopup);
	virtual void UpdateScrollbarPosition();
	virtual void SetParam(void* pParam) {}//setup class dependent parameters
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
public:
	//}}AFX_VIRTUAL
protected:
	virtual void PostNcDestroy();
	void DocToViewPort(SIZE& size);
	void DocToViewPort(RECT& rect);
	void ViewPortToDoc(SIZE& size);
	void ViewPortToDoc(RECT& rect);
	void CalculateRects();
	int m_nZoomFactor;
	RECT	m_rcPort;			//顯示試窗對應到image的作標, top-left is scroll poition
											//Shif t 到top-left 即為client Window 
	SIZE	m_szCanvas;	//size of total area, the scroll area
	RECT	m_rcImage;		//location of zoomed image on canvas

	ImgFile*	m_pImage;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnViewZoomin();
	afx_msg void OnUpdateViewZoomin(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomout();
	afx_msg void OnUpdateViewZoomout(CCmdUI* pCmdUI);
	afx_msg void OnClose();
};
