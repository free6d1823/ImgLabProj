#pragma once

#include "ProcessView.h"
#include "LdcTool.h"

// LdcView

class LdcView : public ProcessView
{
	DECLARE_DYNAMIC(LdcView)

public:
	LdcView();
	virtual ~LdcView();

protected:
	LdcTool	m_tool;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


