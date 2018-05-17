#pragma once


// RulerBar

class RulerBar : public CWnd
{
	DECLARE_DYNAMIC(RulerBar)

public:
	RulerBar();
	virtual ~RulerBar();
	BOOL	Create(CWnd* pParent, BOOL bTypeHoriz = TRUE);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	BOOL	m_bTypeHoriz;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};
class RulerCorner : public CWnd
{
	DECLARE_DYNAMIC(RulerCorner)

public:
	RulerCorner();
	virtual ~RulerCorner();
	BOOL	Create(CWnd* pParent);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


