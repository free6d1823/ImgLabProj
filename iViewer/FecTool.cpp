// FecTool.cpp : implementation file
//

#include "stdafx.h"
#include "iViewer.h"
#include "FecTool.h"
#include "FecView.h"
#include "ImgFile.h"

#define PI		3.1415967f
// FecTool dialog

IMPLEMENT_DYNAMIC(FecTool, ProcessTool)

#define DEGREE_TO_RADIUN(d)	(d*PI/180)
#define GRADIUN_TO_DEGREE(g) (g*180/PI)

FecTool::FecTool(CWnd* pParent /*=NULL*/)
	: ProcessTool(FecTool::IDD, pParent)
{
	memset(&m_param, 0, sizeof(m_param));
}

FecTool::~FecTool()
{

}

void FecTool::UpdateData(BOOL bSaveAndValidate)
{
		if(bSaveAndValidate) //UI to variables
		{
				m_param.ptCenter.x = GetDlgItemInt(IDC_CENTER_X);
				m_param.ptCenter.y = GetDlgItemInt(IDC_CENTER_Y);
				m_param.szRadius.cx = GetDlgItemInt(IDC_RADIUS_X );
				m_param.szRadius.cy = GetDlgItemInt(IDC_RADIUS_Y );
				m_param.pitch = DEGREE_TO_RADIUN(GetDlgItemFloat(IDC_PITCH));
				m_param.yaw = DEGREE_TO_RADIUN(GetDlgItemFloat(IDC_YAW));			
				m_param.roll = DEGREE_TO_RADIUN(GetDlgItemFloat(IDC_ROLL));
				int x = GetCheckedRadioButton(IDC_RADIO0, IDC_RADIO4);
				m_param.lt = (LensType) (x - IDC_RADIO0);
				m_param.szOutput.cx= GetDlgItemInt(IDC_OUT_CX);
				m_param.szOutput.cy= GetDlgItemInt(IDC_OUT_CY);
				
		} else { //m_param to UI
				SetDlgItemInt(IDC_CENTER_X, m_param.ptCenter.x);
				SetDlgItemInt(IDC_CENTER_Y, m_param.ptCenter.y);
				SetDlgItemInt(IDC_RADIUS_X, m_param.szRadius.cx);
				SetDlgItemInt(IDC_RADIUS_Y, m_param.szRadius.cy);
				SetDlgItemFloat(IDC_PITCH, GRADIUN_TO_DEGREE(m_param.pitch));
				SetDlgItemFloat(IDC_YAW, GRADIUN_TO_DEGREE(m_param.yaw));
				SetDlgItemFloat(IDC_ROLL, GRADIUN_TO_DEGREE(m_param.roll));
				CheckRadioButton(IDC_RADIO0, IDC_RADIO4, IDC_RADIO0+m_param.lt);
				SetDlgItemInt(IDC_OUT_CX, m_param.szOutput.cx);
				SetDlgItemInt(IDC_OUT_CY, m_param.szOutput.cy);
		}
}



BEGIN_MESSAGE_MAP(FecTool, ProcessTool)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PITCH_SPIN, OnDeltaposPitchSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_YAW_SPIN, OnDeltaposYawSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ROLL_SPIN, OnDeltaposRollSpin)
	ON_BN_CLICKED(IDCANCEL, &FecTool::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &FecTool::OnBnClickedOk)
END_MESSAGE_MAP()
void FecTool::SetParam(void* pParam)
{
		memcpy(&m_param, pParam, sizeof(FecParam));
		UpdateData(FALSE);
}
void* FecTool::GetParam()
{
		UpdateData(TRUE);
		return (void*) &m_param;
}

void FecTool::SetImgFile(ImgFile* pImage)
{
	if(!m_pBasicView) {
		m_pBasicView = new FecView();
		m_pBasicView->Create("FEC", GetParent(), TRUE);
		m_pBasicView->ShowWindow(SW_SHOW);
	}
	m_pBasicView->SetImgFile(pImage);
	if(pImage) {
		m_param.ptCenter.x = pImage->Width()/2;
		m_param.ptCenter.y = pImage->Height()/2;
		m_param.szRadius.cx = m_param.ptCenter.x;
		m_param.szRadius.cy = m_param.ptCenter.y;
		m_param.szInput.cx = pImage->Width();
		m_param.szInput.cy = pImage->Height();
		m_param.szOutput.cx = m_param.szInput.cx;
		m_param.szOutput.cy =  m_param.szInput.cy;
		m_param.pitch = 0;//pitch angle in radiun
		m_param.yaw = 0; //rotate with respect to y-axis
		m_param.roll = 0; //rotate with respect to z-axis
		m_param.lt = LensType_0;
		m_param.mm = MountMode_WALL;
		UpdateData(FALSE);
		m_pBasicView->SetParam(&m_param);

	}
}
// FecTool message handlers
#define MAX_ANGLE_DEGREE		30
#define MIN_ANGLE_DEGREE			-30
void FecTool::OnDeltaposPitchSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	float value = GetDlgItemFloat(IDC_PITCH);
	float value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 0.1f; if(value < MIN_ANGLE_DEGREE) value = MIN_ANGLE_DEGREE;
	} else {
			value += 0.1f; if(value > MAX_ANGLE_DEGREE) value = MAX_ANGLE_DEGREE;
	}
	if(value != value2)
		SetDlgItemFloat(IDC_PITCH, value);
	*pResult = 0;
}
void FecTool::OnDeltaposYawSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	float value = GetDlgItemFloat(IDC_YAW);
	float value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 0.1f; if(value < MIN_ANGLE_DEGREE) value = MIN_ANGLE_DEGREE;
	} else {
			value += 0.1f; if(value > MAX_ANGLE_DEGREE) value = MAX_ANGLE_DEGREE;
	}
	if(value != value2)
		SetDlgItemFloat(IDC_YAW, value);
	*pResult = 0;
}
void FecTool::OnDeltaposRollSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	float value = GetDlgItemFloat(IDC_ROLL);
	float value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 0.1f; if(value < MIN_ANGLE_DEGREE) value = MIN_ANGLE_DEGREE;
	} else {
			value += 0.1f; if(value > MAX_ANGLE_DEGREE) value = MAX_ANGLE_DEGREE;
	}
	if(value != value2)
		SetDlgItemFloat(IDC_ROLL, value);
	*pResult = 0;
}

void FecTool::OnBnClickedOk()
{
	//inform parent to read data
	GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);	
	m_pBasicView->SetParam(&m_param);
	m_pBasicView->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);
}
