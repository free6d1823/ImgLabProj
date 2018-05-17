#pragma once
#include "afxwin.h"

class ProcessView;
class ImgFile;
class ProcessTool : public CDialog
{
	DECLARE_DYNAMIC(ProcessTool)

public:

	ProcessTool(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~ProcessTool();
	virtual void SetParam(void* pParam)=0;
	virtual void* GetParam()=0;
	virtual void SetImgFile(ImgFile* pImage)=0;
// Dialog Data
protected:
	float GetDlgItemFloat(int nID);
	void SetDlgItemFloat(int nID, float fValue);
	virtual void UpdateData(BOOL bSaveAndValidate = TRUE){};		
	virtual void PostNcDestroy();
	ProcessView*	m_pBasicView;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnUpdateToolbox(CCmdUI *pCmdUI);
	afx_msg void OnCommandToolbox(UINT nCmd);
protected:

};

