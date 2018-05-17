#pragma once


// LdcTool dialog

class LdcTool : public CDialog
{
	DECLARE_DYNAMIC(LdcTool)

public:
	LdcTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~LdcTool();

// Dialog Data
	enum { IDD = IDD_LDC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// //2nd radial distortio coefficient 
	double m_dbCoef;
};
