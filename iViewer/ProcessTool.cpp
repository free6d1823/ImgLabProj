#include "stdafx.h"
#include "resource.h"
#include "ProcessView.h"
#include "ProcessTool.h"

IMPLEMENT_DYNAMIC(ProcessTool, CDialog)

ProcessTool::ProcessTool(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_pBasicView = NULL;
}

ProcessTool::~ProcessTool()
{
}
void ProcessTool::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	if(m_pBasicView){
		//parent wnd will destroy the window, don't delete it
		m_pBasicView = NULL;
	}

	delete this;
}
BEGIN_MESSAGE_MAP(ProcessTool, CDialog)
	ON_BN_CLICKED(IDCANCEL, &ProcessTool::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &ProcessTool::OnBnClickedOk)
		ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLBOX_BASE, ID_TOOLBOX_LAST, OnUpdateToolbox)
		ON_COMMAND_RANGE(ID_TOOLBOX_BASE, ID_TOOLBOX_LAST,  OnCommandToolbox) 
END_MESSAGE_MAP()

float ProcessTool::GetDlgItemFloat(int nID)
{
		float value = 0;
		char buff[256];
		if(GetDlgItemText(nID, buff, sizeof(buff))) {
				value = (float) atof(buff);
		}
		return value;
}
void ProcessTool::SetDlgItemFloat(int nID, float fValue)
{
		char buff[256];
		sprintf_s(buff, sizeof(buff), "%6.2f", fValue);
		SetDlgItemText(nID, buff);
}
void ProcessTool::OnBnClickedCancel()
{
	GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_EXIT,0);
	 DestroyWindow();
}

void ProcessTool::OnBnClickedOk()
{
	//inform parent to read data
	GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);
	//apply new image to m_pBasicView
}
void ProcessTool::OnUpdateToolbox(CCmdUI *pCmdUI)
{
		if(m_pBasicView)
				m_pBasicView->OnUpdateToolbox(pCmdUI);

		TRACE("ProcessTool  -- OnUpdateToolbox %d - %d\n", pCmdUI->m_nID, pCmdUI->m_nIndex);
}
void ProcessTool::OnCommandToolbox(UINT nCmd)
{
		if(m_pBasicView)
				m_pBasicView->OnCommandToolbox(nCmd);
		TRACE("ProcessTool  --- cmd = %d\n", nCmd);
}