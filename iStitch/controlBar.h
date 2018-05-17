#pragma once
#include "MainDoc.h"
#include "afxwin.h"
// ControlBar dialog

class WorkView;

class ControlBar : public CDialog
{
	DECLARE_DYNCREATE(ControlBar)

public:
	ControlBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~ControlBar();
// Overrides
		PProjectSetting	GetProjectSetting();
		CMainDoc* GetDocument();
		//bReset = TRUE to re-init image list
		//                FALSE only update current selected content
		void				UpdateUI(int nCurSel=0, BOOL bReset=TRUE);
// Dialog Data
	enum { IDD = IDD_CONTROLLER };

protected:
		WorkView*	m_pParent;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
		CComboBox m_ImageList;

		CEdit m_PosX;
		CEdit m_PosY;
		CEdit m_Scale;
		CEdit m_Rotate;
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
		virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
		afx_msg void OnCbnSelchangeCombo1();
};
