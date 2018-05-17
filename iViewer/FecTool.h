#pragma once

#include "common.h"
#include "ProcessTool.h"

class ImgFile;

// FecTool dialog

class FecTool : public ProcessTool
{
	DECLARE_DYNAMIC(FecTool)

public:
	FecTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~FecTool();
	virtual void SetParam(void* pParam);
	virtual void* GetParam();
	virtual void SetImgFile(ImgFile* pImage);
// Dialog Data
	enum { IDD = IDD_FEC };

protected:
	FecParam m_param;
	void UpdateData(BOOL bSaveAndValidate = TRUE);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposPitchSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposYawSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRollSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
