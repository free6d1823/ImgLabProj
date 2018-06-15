#include "StdAfx.h"
#include "resource.h"
#include "common.h"

#include "HomoTool.h"
#include "HomoView.h"
#include "ImgFile.h"
#include "WorkView.h"
#include "Mat.h"

#define HOMO_PARAM() ((HomoWrap*)m_pProcessWrap)->param
HomoWrap ::HomoWrap():ProcessWrap()
{
	//lut buffer is allocated in ProcessWrap()
};
HomoWrap ::~HomoWrap()
{
}

void HomoWrap ::SetParam( void* pParam)
{
	HomoParam* pData = (HomoParam*) pParam;
	memcpy(&param, pData, sizeof(HomoParam));
	MakeMapTable();	 
}

/*!
 * Position mapping according to Homography Matrix Q(x,y) =HP(u,v)
the coordinates are normalized, which means the width and height of P is 1.0x1.0
 * \param[in] (u, v) destination coordinates, [0~1.0]
 * \paramout] (x,y)  source coordinates	[0~1.0]
 */
void HomoWrap::PosMapNormalize(double u, double v, double &x, double &y)
{
	x = param.h[0][0] * u + param.h[0][1]*v + param.h[0][2];
	y = param.h[1][0] * u + param.h[1][1]*v + param.h[1][2];
	double s = param.h[2][0] * u + param.h[2][1]*v + 1;
	ASSERT(s == 0);
	x = x/s;
	y = y/s;
}
void HomoWrap::MakeMapTable()
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

BOOL	HomoWrap::PosMap(int u, int v, float&x, float&y)
{
	//if bottom-down
	v = param.szOutput.cy - v;
	//end of bottom-down
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

//if bottom-down
	y = (float) param.szInput.cy - y;
//end of bottom-down
	return TRUE;

}


BOOL HomoWrap::SaveParameter(LPCTSTR filename)
{
	FILE* fp = NULL;
	fopen_s(&fp, filename, "wt");
	if(fp == NULL)
		return FALSE;
	int lineno;
	TCHAR buff[256];
	sprintf_s(buff, sizeof(buff), "HomoTool; //line 1 \tTool name\n");
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 2\tszInput.cx\n", param.szInput.cx);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 3\tszInput.cy\n", param.szInput.cy);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 4\tszOutput.cx\n", param.szOutput.cx);
	fputs(buff, fp);
	sprintf_s(buff, sizeof(buff), "%d;\t//line 5\tszOutput.cy\n", param.szOutput.cy);
	fputs(buff, fp);
	lineno = 6;
	for (int i=0; i< 4; i++ ) {
		sprintf_s(buff, sizeof(buff), "%d;\t//line %d\tp[%d].x\n", param.p[i].x, lineno++, i);
		fputs(buff, fp);
		sprintf_s(buff, sizeof(buff), "%d;\t//line %d\tp[%d].x\n", param.p[i].y, lineno++, i);
		fputs(buff, fp);
		sprintf_s(buff, sizeof(buff), "%d;\t//line %d\tq[%d].x\n", param.q[i].x, lineno++, i);
		fputs(buff, fp);
		sprintf_s(buff, sizeof(buff), "%d;\t//line %d\tq[%d].x\n", param.q[i].y, lineno++, i);
		fputs(buff, fp);
	}
	for (int i=0; i< 3; i++) {
		sprintf_s(buff, sizeof(buff), "%f;\t//line %d\th[0][%d]\n", param.h[0][i], lineno++, i);
		fputs(buff, fp);
		sprintf_s(buff, sizeof(buff), "%f;\t//line %d\th[1][%d]\n", param.h[1][i], lineno++, i);
		fputs(buff, fp);
		sprintf_s(buff, sizeof(buff), "%f;\t//line %d\th[2][%d]\n", param.h[2][i], lineno++, i);
		fputs(buff, fp);
	}
	sprintf_s(buff, sizeof(buff), "LUT_%dx%d X{ \t//line %d\tLook up table X value\n", cxLut, cyLut, lineno++);
	fputs(buff, fp);
	for (int i=0; i < this->cyLut; i++) {
		fprintf(fp, "\t");
		for(int j=0;j< cxLut; j++) {
			sprintf_s(buff, sizeof(buff), "%11.9f, ", GetLutDataX(i, j));
			fwrite(buff, 1, 12, fp);
		}
		fprintf(fp, "\n");
		lineno++;
	}
	fprintf(fp, "}\n");lineno++;
	sprintf_s(buff, sizeof(buff), "LUT_%dx%d Y{ \t//line %d\tLook up table Y value\n", cxLut, cyLut, lineno++);
	fputs(buff, fp);
	for (int i=0; i< cyLut; i++) {
		fprintf(fp, "\t");
		for(int j=0;j< cxLut; j++) {
			sprintf_s(buff, sizeof(buff), "%11.9f, ", GetLutDataY(i, j));
			fwrite(buff, 1, 12, fp);
		}
		fprintf(fp, "\n");lineno++;
	}
	
	fprintf(fp, "}\tline %d\n", lineno++);
	fclose(fp);
	return TRUE;	
}
BOOL HomoWrap::LoadParameter(LPCTSTR filename)
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
		if(NULL ==strstr(buff, "HomoTool;")) {
			AfxMessageBox("This is not a FecTool prameters file! Please select the other file.");
			break;
		}
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szInput.cx = atoi(buff);
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szInput.cy = atoi(buff);

		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szOutput.cx = atoi(buff);
		if(NULL == fgets(buff, sizeof(buff), fp)) break;
		param.szOutput.cy = atoi(buff);

		for (int i=0; i< 4; i++ ) {
			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			param.p[i].x = atoi(buff);
			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			param.p[i].y = atoi(buff);
			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			param.q[i].x = atoi(buff);
			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			param.q[i].y = atoi(buff);			
		}
		for (int i=0; i< 3; i++) {
			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			param.h[0][i] = (float) atof(buff);
			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			param.h[1][i] = (float) atof(buff);
			if(NULL == fgets(buff, sizeof(buff), fp)) break;
			param.h[2][i] = (float) atof(buff);
		}

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

void HomoWrap::CalculateMatrix()
{
	PointDouble p[4] = {0};
	PointDouble q[4] = {0};
	int i;
	//normalize
	for (i=0;i<4;i++) {
		p[i].x = (double) param.p[i].x /(double)param.szOutput.cx;
		p[i].y =(double)param.p[i].y /(double)param.szOutput.cy;
		q[i].x = (double) param.q[i].x /(double)param.szInput.cx;
		q[i].y =(double)param.q[i].y /(double)param.szInput.cy;
	}
	//Ah=b
	Mat A(8,8);
	Mat b(1,8);
	int j=4;
	for (i=0;i<4;i++) {
		A.Set(0,i, p[i].x); A.Set(1, i, p[i].y); A.Set(2,i,1); A.Set(3,i, 0); A.Set(4,i, 0); A.Set(5,i, 0); 
		A.Set(6,i, -p[i].x*q[i].x); A.Set(7, i, -p[i].y*q[i].x);
		b.Set(0,i, q[i].x);

		A.Set(0,j, 0); A.Set(1,j, 0); A.Set(2,j, 0);
		A.Set(3,j, p[i].x); A.Set(4, j, p[i].y); A.Set(5,j,1); 
		A.Set(6,j, -p[i].x*q[i].y); A.Set(7, j, -p[i].y*q[i].y);
		b.Set(0,j, q[i].y);
		j++;
	}
	TRACE("Matrix A=\n");
	A.Print();
	TRACE("Vector b=\n");
	b.Print();

	Mat h (1,8);
	Mat AI(8,8);
	if(A.FindInverse(AI)) {
			h.Multiply(AI, b);
			for(int i=0;i<8;i++)
				param.h[i/3][i%3] = 	h.Get(0,i);
			param.h[2][2] = 1;
//verify
	TRACE("derived Matrix A-1=\n");
	AI.Print();
	TRACE("Find Vector h=\n");
	h.Print();

Mat c(1,8);
c.Multiply(A, h);
TRACE("Verify A.h = b'\n");
c.Print();

	}else {
			for(int i=0;i<8;i++)
				param.h[i/3][i%3] = -1;
	}
}
// HomoTool dialog

IMPLEMENT_DYNAMIC(HomoTool, ProcessTool)

HomoTool::HomoTool(WorkView* pParent /*=NULL*/)
	: ProcessTool(HomoTool::IDD, pParent)
{
	m_pProcessWrap = new HomoWrap();
}

HomoTool::~HomoTool()
{
	if(m_pProcessWrap){
		delete m_pProcessWrap;
		m_pProcessWrap = NULL;
	}
}
void HomoTool::UpdateData(BOOL bSaveAndValidate)
{
	if(bSaveAndValidate) //UI to variables
	{
		HOMO_PARAM().szInput.cx = GetDlgItemInt(IDC_IN_CX);
		HOMO_PARAM().szInput.cy = GetDlgItemInt(IDC_IN_CY);
		HOMO_PARAM().szOutput.cx= GetDlgItemInt(IDC_OUT_CX );
		HOMO_PARAM().szOutput.cy= GetDlgItemInt(IDC_OUT_CY );
		//P
		for (int i=0;i < 4; i++) {
			HOMO_PARAM().p[i].x  = GetDlgItemInt(IDC_U0+i );
			HOMO_PARAM().p[i].y  = GetDlgItemInt(IDC_V0 +i);
			HOMO_PARAM().q[i].x  = GetDlgItemInt(IDC_X0 +i);
			HOMO_PARAM().q[i].y  = GetDlgItemInt(IDC_Y0 +i);
		}
		for (int i=0;i < 3; i++) {
			HOMO_PARAM().h[0][i]  = GetDlgItemDouble(IDC_H00+i );
			HOMO_PARAM().h[1][i]  = GetDlgItemDouble(IDC_H10+i );
			HOMO_PARAM().h[2][i]  = GetDlgItemDouble(IDC_H20+i );
		}
	} else { //FEC_PARAM() to UI
		SetDlgItemInt(IDC_IN_CX, HOMO_PARAM().szInput.cx);
		SetDlgItemInt(IDC_IN_CY, HOMO_PARAM().szInput.cy);
		SetDlgItemInt(IDC_OUT_CX, HOMO_PARAM().szOutput.cx);
		SetDlgItemInt(IDC_OUT_CY, HOMO_PARAM().szOutput.cy);
		//P,Q
		for (int i=0;i < 4; i++) {
			SetDlgItemInt(IDC_U0+i ,HOMO_PARAM().p[i].x);
			SetDlgItemInt(IDC_V0+i ,HOMO_PARAM().p[i].y);
			SetDlgItemDouble(IDC_X0+i ,(float)HOMO_PARAM().q[i].x);
			SetDlgItemDouble(IDC_Y0+i ,(float)HOMO_PARAM().q[i].y);
		}
		for (int i=0;i < 3; i++) {
			SetDlgItemDouble(IDC_H00+i , (float)HOMO_PARAM().h[0][i]);
			SetDlgItemDouble(IDC_H10+i , (float)HOMO_PARAM().h[1][i]);
			SetDlgItemDouble(IDC_H20+i , (float)HOMO_PARAM().h[2][i]);
		}		
	}
}

BEGIN_MESSAGE_MAP(HomoTool, ProcessTool)
	ON_BN_CLICKED(ID_SET_P, &HomoTool::OnBnClickedSetP)
	ON_BN_CLICKED(ID_GET_P, &HomoTool::OnBnClickedGetP)
	ON_BN_CLICKED(ID_SET_Q, &HomoTool::OnBnClickedSetQ)
	ON_BN_CLICKED(ID_GET_Q, &HomoTool::OnBnClickedGetQ)
	ON_BN_CLICKED(ID_UPDATE, &HomoTool::OnBnClickedUpdate)
	ON_BN_CLICKED(IDOK, &HomoTool::OnBnClickedOk)
END_MESSAGE_MAP()


void HomoTool::SetParam(ProcessWrap* pParam)
{
	m_pProcessWrap->SetParam( &((HomoWrap*)pParam)->param);
	UpdateData(FALSE);
}

ProcessWrap* HomoTool::GetParam()
{
	UpdateData(TRUE);
	//call FecView to process the LUT table
	m_pProcessWrap->MakeMapTable();
	m_pBasicView->SetParam(m_pProcessWrap);

	return m_pProcessWrap;
}
void HomoTool::SetImgFile(ImgFile* pImage)
{
	if(!m_pBasicView) {
		m_pBasicView = new HomoView();
		m_pBasicView->Create("Homography Matrix View", GetParent(), TRUE);
		m_pBasicView->ShowWindow(SW_SHOW);
	}
	m_pBasicView->SetImgFile(pImage);
	if(pImage) {
		HomoParam param;
		memset(&param, 0, sizeof(param));
		param.szInput.cx = pImage->Width();
		param.szInput.cy = pImage->Height();
		param.szOutput.cx = param.szInput.cx;
		param.szOutput.cy =  param.szInput.cy;
		param.h[0][0] = param.h[1][1] = param.h[2][2] = 1;
		//presets a center rect
		param.p[0].x = param.szOutput.cx/4;param.p[0].y = param.szOutput.cy/4;
		param.p[1].x = param.szOutput.cx*3/4;param.p[1].y = param.szOutput.cy/4;
		param.p[2].x = param.szOutput.cx*3/4;param.p[2].y = param.szOutput.cy*3/4;
		param.p[3].x = param.szOutput.cx/4;param.p[3].y = param.szOutput.cy*3/4;
		param.q[0].x = param.szInput.cx/4;param.q[0].y = param.szInput.cy/4;
		param.q[1].x = param.szInput.cx*3/4;param.q[1].y = param.szInput.cy/4;
		param.q[2].x = param.szInput.cx*3/4;param.q[2].y = param.szInput.cy*3/4;
		param.q[3].x = param.szInput.cx/4;param.q[3].y = param.szInput.cy*3/4;


		m_pProcessWrap->SetParam(&param);
		UpdateData(FALSE);
		m_pBasicView->SetParam(m_pProcessWrap);
	}
	//post-process the BasicView window
	ProcessTool::SetImgFile(pImage);

}

/*!
 * Update 4-control points to corrected image viewer
 * \param pParam pointer to 
 */
void HomoTool::OnBnClickedSetP()
{
		UpdateData(TRUE);
		m_pBasicView->SetParam(m_pProcessWrap);
		m_pBasicView->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);
}

void HomoTool::OnBnClickedGetP()
{
	HomoView* pView = (HomoView* )m_pBasicView;
	HomoControlPoints cp;
	pView->GetControlPoints(cp);
	memcpy(&HOMO_PARAM().p, &cp.pt, sizeof(cp.pt));

	UpdateData(FALSE);
}

void HomoTool::OnBnClickedSetQ()
{
		UpdateData(TRUE);
		m_pOwner->SetToolParam((void*)m_pProcessWrap);
	GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);	
}

void HomoTool::OnBnClickedGetQ()
{
	m_pOwner->GetToolParam((void*)m_pProcessWrap);
	UpdateData(FALSE);
}


void HomoTool::OnBnClickedUpdate()
{
	UpdateData(TRUE);
	// find new Matrix to Q
	((HomoWrap*)m_pProcessWrap)->CalculateMatrix();

	UpdateData(FALSE);
	//apply to source image
	GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);		
	//apply to P image
	if(m_pBasicView){
		m_pBasicView->SetParam(m_pProcessWrap);
		m_pBasicView->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);
	}

}

void HomoTool::OnBnClickedOk()
{
	UpdateData(TRUE);
	//find M form P-Q map
	GetParent()->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);	
//	m_pBasicView->SetParam(m_pProcessWrap);
	m_pBasicView->SendMessage(WM_COMMAND, ID_PROCESS_PREVIEW,0);
}
