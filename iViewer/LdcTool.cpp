// LdcTool.cpp : implementation file
//

#include "stdafx.h"
#include "iViewer.h"
#include "LdcTool.h"


// LdcTool dialog

IMPLEMENT_DYNAMIC(LdcTool, CDialog)

LdcTool::LdcTool(CWnd* pParent /*=NULL*/)
	: CDialog(LdcTool::IDD, pParent)
	, m_dbCoef(0)
{

}

LdcTool::~LdcTool()
{
}

void LdcTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LdcTool, CDialog)
END_MESSAGE_MAP()


// LdcTool message handlers
