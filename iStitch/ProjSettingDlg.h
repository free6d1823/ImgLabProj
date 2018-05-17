#pragma once

#include "MainDoc.h"
#include "afxwin.h"
#include "afxcmn.h"

// ProjSettingDlg dialog

class ProjSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(ProjSettingDlg)

public:
	ProjSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProjSettingDlg();
	void SetProperty(ProjectSetting* pSetting);
//gets reference pointer to setting
	ProjectSetting* GetProperty(){return &m_Setting;};	

// Dialog Data
	enum { IDD = IDD_PROJECT_PROPERTY };

protected:
	ProjectSetting m_Setting;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL UpdateData(BOOL bReadFromUI);
	void OnEditItem(int nID, int nSub);
protected:
		void UpdateListCtrl();
	virtual void OnOK();
public:
	// image property list
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	// image property list
	CListCtrl* m_pListImage;
	int				m_nCurrentCounts; 
	DWORD		m_dwSelectedCell;
	afx_msg void OnHdnItemdblclickListimage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeImageCounts();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnKillfocusEdit1();
};
