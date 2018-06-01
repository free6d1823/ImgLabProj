// FecTool.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "resource.h"
#include "FecTool.h"
#include "FecView.h"
#include "ImgFile.h"
#include "common.h"


//////////////////////////////

FecWrap ::FecWrap():ProcessWrap()
{
	//lut buffer is allocated in ProcessWrap()
};
FecWrap ::~FecWrap()
{
}

void FecWrap ::SetParam( void* pParam)
{
	FecParam* pData = (FecParam*) pParam;
	memcpy(&param, pData, sizeof(FecParam));
	MakeMapTable();
}

/*!
 * Position mapping according to the parameters with normalized image dimension. (which means the width and height is 1.0x1.0
 * \param[in] (u, v) destination coordinates, [0~1.0]
 * \paramout] (x,y)  source coordinates	[0~1.0]
 */
void FecWrap::PosMapNormalize(double u, double v, double &x, double &y)
{
	double uu, vv, u1, v1,u2,v2; 
	//transform uu = M x u
	
	u1 = u-0.5;
	v1 = v-0.5;
	//apply rotation
	//spin - z-axis
	u2 = cos(param.roll)* u1 - sin(param.roll)*v1;
	v2 = sin(param.roll)* u1 + cos(param.roll)*v1;
	//yaw
	u1 = cos(param.yaw)* u2/(1.0+sin(param.yaw)*u2);
	//pitch
	v1 = cos(param.pitch)* v2/(1.0+sin(param.pitch)*v2);

	//projection to rectlinear, apply lenstype
	double phi;
	double theda;
	switch(param.lt) {
		case LensType_1://tan(angle/2)
			uu = tan(u1*PI/2.0);
			vv = tan(v1*PI/2.0);
			break;
		case LensType_2: //sin(angle/2)
			uu = sin(u1*PI/2.0)*1.414;
			vv = sin(v1*PI/2.0)*1.414;
			break;
		case LensType_4: //sin(angle)
			uu = sin(u1*PI);
			vv = sin(v1*PI);
			break;
		case LensType_3: //angle
			uu = u1*2; vv = v1*2; //-1~1
			break;
		default: //	LensType_0 or 5
			x = u1*(double)param.szRadius.cx*2.0/(double)param.szInput.cx  + (double)param.ptCenter.x/(double)param.szInput.cx;
			y=  v1*(double)param.szRadius.cx*2.0/(double)param.szInput.cy + (double)param.ptCenter.y/(double)param.szInput.cy;
			return;
	}

	//rectlinear to sphere
	phi = vv*PI_2;
	theda = uu*PI_2;
	//big sphere to small sphere
	//phi = asin(sin(phi)/1.2);
	//theda = asin(sin(theda)/1.2);
	
	//sphere to fisheye
	x = ((double)param.szRadius.cx/(double)param.szInput.cx)* cos(phi) * sin(theda)*(1+param.curveV*vv*vv) + (double)param.ptCenter.x/(double)param.szInput.cx;
	y = ((double)param.szRadius.cy/(double)param.szInput.cy)* sin(phi)*(1+param.curveH*uu*uu) + (double)param.ptCenter.y/(double)param.szInput.cy;
}
void FecWrap::MakeMapTable()
{
	int i,j;
	int dx = cxLut-1;//interval numbers of width
	int dy = cyLut-1;//interval numbers of height

	for(i=0;i<= dy; i++ ) {
		for(j=0;j<= dx; j++) {
			double u,v;
			PosMapNormalize((double)j/(double)dx, (double)i/(double)dy, u,v);
			SetLutDataX(j,i, u);
			SetLutDataY(j,i, v);
		}
	}
}

BOOL	FecWrap::PosMap(int u, int v, float&x, float&y)
{

	double fu = (double)(u*  (cxLut-1))/(double)(param.szOutput.cx);//u= 0~FEC_PARAM().szOutput.cx, 0~TABLE_SIZE
	double fv = (double)(v*  (cyLut-1))/(double)(param.szOutput.cy);
	int u0 = (int) fu;
	int u1 = u0+1;
	int v0 = (int) fv;
	int v1 = v0+1;
	double dfu = fu - (double) u0;
	double dfv = fv - (double) v0;
	double x0 = GetLutDataX(u0,v0) + dfu * (GetLutDataX(u1,v0) - GetLutDataX(u0,v0) );//normalized
	double x1 = GetLutDataX(u0,v1) + dfu * (GetLutDataX(u1,v1) - GetLutDataX(u0,v1) );//normalized
	x = (float) (x0 + dfv*(x1-x0)) *  (float) param.szInput.cx; 

	double y0 = GetLutDataY(u0,v0) + dfv * (GetLutDataY(u0,v1) - GetLutDataY(u0,v0) );//normalized
	double y1 = GetLutDataY(u1,v0) + dfv * (GetLutDataY(u1,v1) - GetLutDataY(u1,v0) );//normalized
	y = (float) (y0 + dfu* (y1-y0)) *  (float) param.szInput.cy; 

	return TRUE;

}
BOOL FecWrap::SaveParameter(LPCTSTR filename)
{
	FILE* fp = NULL;
	fopen_s(&fp, filename, "wt");
	if(fp == NULL)
		return FALSE;
	TCHAR buff[256];
	sprintf_s(buff, sizeof(buff), "FecTool; //line 1 \tTool name\n");
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 2\tptCenter.x\n", param.ptCenter.x);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 3\tptCenter.y\n", param.ptCenter.y);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 4\tszRadius.cx\n", param.szRadius.cx);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 5\tszRadius.cy\n", param.szRadius.cy);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 6\tszInput.cx\n", param.szInput.cx);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 7\tszInput.cy\n", param.szInput.cy);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 8\tszOutput.cx\n", param.szOutput.cx);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 9\tszOutput.cy\n", param.szOutput.cy);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%f;\t//line 10\tpitch\n", param.pitch);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%f;\t//line 11\tyaw\n", param.yaw);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%f;\t//line 12\troll\n", param.roll);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 13\tlens type\n", param.lt);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 14\tmount mode\n", param.mm);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 15\tcurvature H.\n", param.curveH);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 16\tcurvature V.\n", param.curveV);
	fputs(buff, fp);

	sprintf_s(buff, sizeof(buff), "LUT_%dx%d X{ \t//line 17\tLook up table X value\n", cxLut, cyLut);
	fputs(buff, fp);
	for (int i=0; i < this->cyLut; i++) {
		fprintf(fp, "\t");
		for(int j=0;j< cxLut; j++) {
			sprintf_s(buff, sizeof(buff), "%11.9f, ", GetLutDataX(i, j));
			fwrite(buff, 1, 12, fp);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "}\n");
	sprintf_s(buff, sizeof(buff), "LUT_%dx%d Y{ \t//line %d\tLook up table Y value\n", cxLut, cyLut, 17+cyLut+1);
	fputs(buff, fp);
	for (int i=0; i< cyLut; i++) {
		fprintf(fp, "\t");
		for(int j=0;j< cxLut; j++) {
			sprintf_s(buff, sizeof(buff), "%11.9f, ", GetLutDataY(i, j));
			fwrite(buff, 1, 12, fp);
		}
		fprintf(fp, "\n");
	}
	
	fprintf(fp, "}\n-- end of parameters --\n");
	fclose(fp);
	return TRUE;	
}
BOOL FecWrap::LoadParameter(LPCTSTR filename)
{

	FILE* fp = NULL;
	fopen_s(&fp, filename, "rt");
	if(fp == NULL)
		return FALSE;
	TCHAR buff[1024];
	BOOL bOK = FALSE;
	do {
		if(NULL == fgets(buff, sizeof(buff), fp))
			break;
		if(NULL ==strstr(buff, "FecTool;")) {
			AfxMessageBox("This is not a FecTool prameters file! Please select the other file.");
			break;
		}
		if(NULL == fgets(buff, sizeof(buff), fp))break;
		param.ptCenter.x = atoi(buff);
		if(NULL == fgets(buff, sizeof(buff), fp))break;
		param.ptCenter.y = atoi(buff);

		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szRadius.cx = atoi(buff);
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szRadius.cy = atoi(buff);

		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szInput.cx = atoi(buff);
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szInput.cy = atoi(buff);

		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szOutput.cx = atoi(buff);
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szOutput.cy = atoi(buff);

 		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.pitch = (float) atof(buff);
 		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.yaw = (float) atof(buff);
 		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.roll = (float) atof(buff);

		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.lt = (LensType)atoi(buff);
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.mm = (MountMode) atoi(buff);

		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.curveH = (float)atoi(buff);
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.curveV = (float) atoi(buff);


		//load LUT
		ResetLutData();
		int cx, cy;
		fscanf_s(fp, "LUT_%dx%d\n", &cx, &cy);
		if(cx != cxLut || cy != cyLut) {
			AfxMessageBox("The Lookup table ize is not consistent. LUT is not loaded!");
			bOK = TRUE;
			break;
		}
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		TRACE("line = %s\n", buff);
		char* pContext;
		for(int i=0;i<= cy; i++) {
			char* p;
			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			p = strtok_s(buff, ",", &pContext);
			if (p) SetLutDataX(i,0, atof(p));
			for(int j=1; j<= cx; j++) {
				p = strtok_s(NULL, ",", &pContext);
				if (!p) break;
				SetLutDataX(i,j, atof(p));
			}
		}

		if(NULL == fgets(buff, sizeof(buff), fp)) break; //}

		fscanf_s(fp, "LUT_%dx%d\n", &cx, &cy);
		if(cx != cxLut || cy != cyLut) {
			AfxMessageBox("The Lookup table ize is not consistent. LUT is not loaded!");
			bOK = TRUE;
			break;
		}
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		TRACE("line = %s\n", buff);
		for(int i=0;i<= cy; i++) {
			char* p;

			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			p = strtok_s(buff, ",", &pContext);
			if (p) SetLutDataY(i,0, atof(p));
			for(int j=1; j<= cx; j++) {
				p = strtok_s(NULL, ",", &pContext);
				if (!p) break;
				SetLutDataY(i,j, atof(p));
			}
		}

		bOK = TRUE;
	} while(0);
	fclose(fp);
	return bOK;
}


// FecTool dialog

IMPLEMENT_DYNAMIC(FecTool, ProcessTool)

#define DEGREE_TO_RADIUN(d)	(d*PI/180)
#define GRADIUN_TO_DEGREE(g) (g*180/PI)


#define FEC_PARAM() ((FecWrap*)m_pProcessWrap)->param
FecTool::FecTool(CWnd* pParent /*=NULL*/)
	: ProcessTool(FecTool::IDD, pParent)
{
	m_pProcessWrap = new FecWrap();
}

FecTool::~FecTool()
{
	if(m_pProcessWrap) {
		delete m_pProcessWrap;
		m_pProcessWrap = NULL;
	}
	//will call ProcessTool destruct then 
}


void FecTool::UpdateData(BOOL bSaveAndValidate)
{
	if(bSaveAndValidate) //UI to variables
	{
		FEC_PARAM().ptCenter.x = GetDlgItemInt(IDC_CENTER_X);
		FEC_PARAM().ptCenter.y = GetDlgItemInt(IDC_CENTER_Y);
		FEC_PARAM().szRadius.cx = GetDlgItemInt(IDC_RADIUS_X );
		FEC_PARAM().szRadius.cy = GetDlgItemInt(IDC_RADIUS_Y );
		FEC_PARAM().pitch = DEGREE_TO_RADIUN(GetDlgItemFloat(IDC_PITCH));
		FEC_PARAM().yaw = DEGREE_TO_RADIUN(GetDlgItemFloat(IDC_YAW));			
		FEC_PARAM().roll = DEGREE_TO_RADIUN(GetDlgItemFloat(IDC_ROLL));
		int x = GetCheckedRadioButton(IDC_RADIO0, IDC_RADIO4);
		FEC_PARAM().lt = (LensType) (x - IDC_RADIO0);
		FEC_PARAM().szOutput.cx= GetDlgItemInt(IDC_OUT_CX);
		FEC_PARAM().szOutput.cy= GetDlgItemInt(IDC_OUT_CY);
		FEC_PARAM().curveH = GetDlgItemFloat(IDC_CURVE_H);
		FEC_PARAM().curveV = GetDlgItemFloat(IDC_CURVE_V);
			
	} else { //FEC_PARAM() to UI
		SetDlgItemInt(IDC_CENTER_X, FEC_PARAM().ptCenter.x);
		SetDlgItemInt(IDC_CENTER_Y, FEC_PARAM().ptCenter.y);
		SetDlgItemInt(IDC_RADIUS_X, FEC_PARAM().szRadius.cx);
		SetDlgItemInt(IDC_RADIUS_Y, FEC_PARAM().szRadius.cy);
		SetDlgItemFloat(IDC_PITCH, GRADIUN_TO_DEGREE(FEC_PARAM().pitch));
		SetDlgItemFloat(IDC_YAW, GRADIUN_TO_DEGREE(FEC_PARAM().yaw));
		SetDlgItemFloat(IDC_ROLL, GRADIUN_TO_DEGREE(FEC_PARAM().roll));
		CheckRadioButton(IDC_RADIO0, IDC_RADIO4, IDC_RADIO0+FEC_PARAM().lt);
		SetDlgItemInt(IDC_OUT_CX, FEC_PARAM().szOutput.cx);
		SetDlgItemInt(IDC_OUT_CY, FEC_PARAM().szOutput.cy);
		SetDlgItemFloat(IDC_CURVE_H, FEC_PARAM().curveH);
		SetDlgItemFloat(IDC_CURVE_V, FEC_PARAM().curveV);
	}
}


BEGIN_MESSAGE_MAP(FecTool, ProcessTool)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CENTERX_SPIN, OnDeltaposCenterXSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CENTERY_SPIN, OnDeltaposCenterYSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RADIUSX_SPIN, OnDeltaposRadiusXSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RADIUSY_SPIN, OnDeltaposRadiusYSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CURVEH_SPIN, OnDeltaposCurveHSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CURVEV_SPIN, OnDeltaposCurveVSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PITCH_SPIN, OnDeltaposPitchSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_YAW_SPIN, OnDeltaposYawSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ROLL_SPIN, OnDeltaposRollSpin)
	ON_BN_CLICKED(IDCANCEL, &FecTool::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &FecTool::OnBnClickedOk)
END_MESSAGE_MAP()
//called by ? when first time init the dialog box
void FecTool::SetParam(ProcessWrap* pParam)
{
	m_pProcessWrap->SetParam( &((FecWrap*)pParam)->param);
	UpdateData(FALSE);
}
ProcessWrap* FecTool::GetParam()
{
	UpdateData(TRUE);
	//call FecView to process the LUT table
	m_pProcessWrap->MakeMapTable();
	m_pBasicView->SetParam(m_pProcessWrap);

	return m_pProcessWrap;
}

void FecTool::SetImgFile(ImgFile* pImage)
{
	if(!m_pBasicView) {
		m_pBasicView = new FecView();
		m_pBasicView->Create("FEC", GetParent(), TRUE);
	}
	m_pBasicView->SetImgFile(pImage);
	if(pImage) {
		FecParam param;
		memset(&param, 0, sizeof(param));
		param.szInput.cx = pImage->Width();
		param.szInput.cy = pImage->Height();
		param.ptCenter.x = param.szInput.cx/2;
		param.ptCenter.y = param.szInput.cy/2;
		param.szRadius.cx = param.ptCenter.x;
		param.szRadius.cy = param.ptCenter.y;
		param.szOutput.cx = param.szInput.cx;
		param.szOutput.cy =  param.szInput.cy;
/*
		FEC_PARAM().ptCenter.x = pImage->Width()/2;
		FEC_PARAM().ptCenter.y = pImage->Height()/2;
		FEC_PARAM().szRadius.cx = FEC_PARAM().ptCenter.x;
		FEC_PARAM().szRadius.cy = FEC_PARAM().ptCenter.y;
		FEC_PARAM().szInput.cx = pImage->Width();
		FEC_PARAM().szInput.cy = pImage->Height();
		FEC_PARAM().szOutput.cx = FEC_PARAM().szInput.cx;
		FEC_PARAM().szOutput.cy =  FEC_PARAM().szInput.cy;
		FEC_PARAM().pitch = 0;//pitch angle in radiun
		FEC_PARAM().yaw = 0; //rotate with respect to y-axis
		FEC_PARAM().roll = 0; //rotate with respect to z-axis
		FEC_PARAM().lt = LensType_0;
		FEC_PARAM().mm = MountMode_WALL;
		FEC_PARAM().curveH = FEC_PARAM().curveV = 0;
*/
		m_pProcessWrap->SetParam(&param);
		UpdateData(FALSE);
		m_pBasicView->SetParam(m_pProcessWrap);

	}
	//post-process the BasicView window
	ProcessTool::SetImgFile(pImage);
}
	
void FecTool::OnDeltaposCenterXSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	int value = GetDlgItemInt(IDC_CENTER_X);
	int value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 1; if(value < 1) value = 1;
	} else {
			value += 1; if(value >= FEC_PARAM().szInput.cx) value = FEC_PARAM().szInput.cx-1;
	}
	if(value != value2)
		SetDlgItemInt(IDC_CENTER_X, value);
	*pResult = 0;
}

void FecTool::OnDeltaposCenterYSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	int value = GetDlgItemInt(IDC_CENTER_Y);
	int value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 1; if(value < 1) value = 1;
	} else {
			value += 1; if(value >= FEC_PARAM().szInput.cy) value = FEC_PARAM().szInput.cy-1;
	}
	if(value != value2)
		SetDlgItemInt(IDC_CENTER_Y, value);
	*pResult = 0;
}
void FecTool::OnDeltaposRadiusXSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	int value = GetDlgItemInt(IDC_RADIUS_X);
	int value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 1; if(value < 1) value = 1;
	} else {
			value += 1; if(value >= FEC_PARAM().szInput.cx) value = FEC_PARAM().szInput.cx-1;
	}
	if(value != value2)
		SetDlgItemInt(IDC_RADIUS_X, value);
	*pResult = 0;
}
void FecTool::OnDeltaposRadiusYSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	int value = GetDlgItemInt(IDC_RADIUS_Y);
	int value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 1; if(value < 1) value = 1;
	} else {
			value += 1; if(value >= FEC_PARAM().szInput.cy) value = FEC_PARAM().szInput.cy-1;
	}
	if(value != value2)
		SetDlgItemInt(IDC_RADIUS_Y, value);
	*pResult = 0;
}
void FecTool::OnDeltaposCurveHSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	float value = GetDlgItemFloat(IDC_CURVE_H);
	float value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 0.01f;
	} else {
			value += 0.01f;
	}
	if(value != value2)
		SetDlgItemFloat(IDC_CURVE_H, value);
	*pResult = 0;
}

void FecTool::OnDeltaposCurveVSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// pNMUpDown->iDelta -1 if UP; positive +1 if down
	float value = GetDlgItemFloat(IDC_CURVE_V);
	float value2 = value;
	if(pNMUpDown->iDelta > 0) {
			value -= 0.01f;
	} else {
			value += 0.01f;
	}
	if(value != value2)
		SetDlgItemFloat(IDC_CURVE_V, value);
	*pResult = 0;
}

// FecTool message handlers
#define MAX_ANGLE_DEGREE		89
#define MIN_ANGLE_DEGREE			-89
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
	m_pBasicView->SetParam(m_pProcessWrap);
	m_pBasicView->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);
}

BOOL WritePrivateProfileInt( LPCSTR lpSecName,LPCSTR lpKeyName, int value, LPCSTR lpFileName)
{
	CString szString;
	szString.Format("%d", value);
	return WritePrivateProfileString(lpSecName, lpKeyName, szString, lpFileName);
}
BOOL WritePrivateProfileFloat( LPCSTR lpSecName,LPCSTR lpKeyName, float value, LPCSTR lpFileName)
{
	CString szString;
	szString.Format("%9.4f", value);
	return WritePrivateProfileString(lpSecName, lpKeyName, szString, lpFileName);
}

float GetPrivateProfileFloat( LPCSTR lpSecName,LPCSTR lpKeyName, float fDefault, LPCSTR lpFileName)
{
	TCHAR value[64];
	if( 0== GetPrivateProfileString(lpSecName, lpKeyName, "0", value, sizeof(value), lpFileName))
		return fDefault;
	return (float) atof(value);
}

/////////////////////////////////////////////