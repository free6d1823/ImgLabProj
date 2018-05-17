#if !defined(AFX_THRESHOLDDLG_H__AC0090B8_8E35_4A13_88CB_4E07DCDF26B4__INCLUDED_)
#define AFX_THRESHOLDDLG_H__AC0090B8_8E35_4A13_88CB_4E07DCDF26B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThresholdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CThresholdDlg dialog

class CThresholdDlg : public CDialog
{
// Construction
public:
	CThresholdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThresholdDlg)
	enum { IDD = IDD_THRESHOLD };
	int		m_nTH;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThresholdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThresholdDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THRESHOLDDLG_H__AC0090B8_8E35_4A13_88CB_4E07DCDF26B4__INCLUDED_)
