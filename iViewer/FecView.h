#pragma once

#include "ProcessView.h"
#include "FecTool.h"


class FecView : public ProcessView
{
	DECLARE_DYNAMIC(FecView)

public:
	FecView();
	virtual ~FecView();
	virtual	void SetParam(void* pParam);
protected:
	//(u, v) destination coordinates
	//(x,y)  source coordinates
	virtual BOOL		PosMap(int u, int v, float&x, float&y);
	//interpolate the color in position x,y in source coordinates
	virtual COLORREF GetColor(BYTE* pSrc, int stride, float x, float y);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FecView)
public:

	//}}AFX_VIRTUAL
	FecParam m_param;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPreview();

protected:

};


