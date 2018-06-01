// LdcTool.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "LdcTool.h"
#include "LdcView.h"


LdcWrap ::LdcWrap():ProcessWrap()
{
	//lut buffer is allocated in ProcessWrap()
};
LdcWrap ::~LdcWrap()
{
}

void LdcWrap ::SetParam( void* pParam)
{
	m_dbCoef = *(double*) pParam;
	//LdcTool call FecView later to update lut data
}


// LdcTool dialog

IMPLEMENT_DYNAMIC(LdcTool, ProcessTool)

LdcTool::LdcTool(CWnd* pParent /*=NULL*/)
	: ProcessTool(LdcTool::IDD, pParent)
{
	m_pProcessWrap = new LdcWrap();
}

LdcTool::~LdcTool()
{
	if(m_pProcessWrap) delete m_pProcessWrap;
}
void LdcTool::SetImgFile(ImgFile* pImage)
{
	if(!m_pBasicView) {
		m_pBasicView = new LdcView();
		m_pBasicView->Create("LdcView", GetParent(), TRUE);
		m_pBasicView->ShowWindow(SW_SHOW);
	}
	m_pBasicView->SetImgFile(pImage);

	//post-process the BasicView window
	ProcessTool::SetImgFile(pImage);

}

void LdcTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LdcTool, ProcessTool)
END_MESSAGE_MAP()


// LdcTool message handlers
