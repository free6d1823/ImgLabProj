
// MainDoc.cpp : implementation of the CMainDoc class
//

#include "stdafx.h"
#include "iStitch.h"

#include "MainDoc.h"
#include "ProjSettingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CALCULATE_BOUND(b, pos, size, scale) {						\
				b.left = pos.x; b.top = pos.y;												\
				b.right = b.left + (LONG) ((float) size.cx *scale);						\
				b.bottom = b.top+ (LONG) ((float) size.cy *scale);	}		\
// CMainDoc

IMPLEMENT_DYNCREATE(CMainDoc, CDocument)

BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
		ON_COMMAND(ID_FILE_PROPERTIES, &CMainDoc::OnFileProperties)
		ON_UPDATE_COMMAND_UI(ID_FILE_PROPERTIES, &CMainDoc::OnUpdateFileProperties)
		ON_COMMAND(ID_FILE_EXPORT, &CMainDoc::OnFileExport)
		ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT, &CMainDoc::OnUpdateFileExport)
END_MESSAGE_MAP()


// CMainDoc construction/destruction

CMainDoc::CMainDoc()
{
	memset(&m_ProjectSetting, 0, sizeof(m_ProjectSetting));
	m_pImg = NULL;
	m_pImgLab = ImgLab::Create(TRUE);
}

CMainDoc::~CMainDoc()
{
		FreePropertySetting();
		if (m_pImg){
			delete m_pImg;
			m_pImg = NULL;
		}
		if(m_pImgLab) {
			delete m_pImgLab;
			m_pImgLab = NULL;
		}
}

BOOL CMainDoc::OnNewDocument()
{
		CWnd* p1 = AfxGetMainWnd();
		if (!CDocument::OnNewDocument())
				return FALSE;
		CWnd* p2 = AfxGetMainWnd();
		strcpy_s(m_ProjectSetting.title, sizeof(m_ProjectSetting.title), "New Project");
		ProjSettingDlg dlg;
		ProjectSetting ps;
		memset(&ps, 0, sizeof(ps));
		strcpy_s(ps.title, sizeof(ps.title), this->GetTitle());
		ps.nImages = 4;
		ps.canvas.cx = 1024;
		ps.canvas.cy = 480;
		for(int i=0; i< ps.nImages; i++ ) {
				ps.ip[i].pos.x = i* (ps.canvas.cx/ps.nImages);
				ps.ip[i].pos.y = 0;
				ps.ip[i].scale = 1;
				ps.ip[i].rotate = 0;
		}
		dlg.SetProperty(&ps);
		if( dlg.DoModal() == IDOK) {
				UpdatePropertySetting(dlg.GetProperty());
		}
		//keep created
		this->SetTitle("Empty Project");
		return TRUE;
}

BOOL CMainDoc::UpdatePropertySetting(PProjectSetting pPs)
{
	if(!pPs || pPs->nImages == 0)
		return FALSE;
	FreePropertySetting();
	m_ProjectSetting.nImages = pPs->nImages;
	m_ProjectSetting.canvas = pPs->canvas;
	strcpy_s(m_ProjectSetting.title,sizeof(m_ProjectSetting.title), pPs->title);
	for (int i=0; i< m_ProjectSetting.nImages; i++) {
		m_ProjectSetting.ip[i].pos = pPs->ip[i].pos;
		m_ProjectSetting.ip[i].size = pPs->ip[i].size;
		m_ProjectSetting.ip[i].scale = pPs->ip[i].scale;
		m_ProjectSetting.ip[i].rotate = pPs->ip[i].rotate;
		CALCULATE_BOUND(m_ProjectSetting.ip[i].rcBound, pPs->ip[i].pos, pPs->ip[i].size, pPs->ip[i].scale);

		if( pPs->ip[i].path[0] != 0) {
			strcpy_s(m_ProjectSetting.ip[i].path,MAX_PATH, pPs->ip[i].path);
			
			ImgFile* pImg = new ImgFile();
			if(Img_OK != pImg->Load(m_ProjectSetting.ip[i].path)) {
					delete pImg;
					pImg = NULL;
					CString szMessage;
					szMessage.Format("Failed to open image file %s!", m_ProjectSetting.ip[i].path);
					AfxMessageBox(szMessage);
			}
			m_ProjectSetting.ip[i].pImg = pImg;
		}
	}
	//
	if (m_pImg) delete m_pImg;
	m_pImg = new ImgFile;
	m_pImg->Create(m_ProjectSetting.canvas.cx, m_ProjectSetting.canvas.cy, 24);
	DoStitching(0,0);
	return TRUE;
}
void CMainDoc::FreePropertySetting()
{
		for (int i=0; i<m_ProjectSetting.nImages; i++) {
				if (m_ProjectSetting.ip[i].pImg) delete m_ProjectSetting.ip[i].pImg;
		}
		memset(&m_ProjectSetting, 0, sizeof(m_ProjectSetting));
}

// CMainDoc diagnostics

#ifdef _DEBUG
void CMainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/*!
 *  Apply stitching parameters and make the image
 * \param nTopID index of the image to be shown on top
 * \param mode	0 no transparent, 1 50% alpha blending, 2= linear blending
 */
int CMainDoc::DoStitching(int nTopID, int mode)
{
	ASSERT(m_pImg);
	m_pImg->FillColor(0xff);
	
	Image24 desImg;
	desImg.data = (BYTE*) m_pImg->GetBits();
	desImg.width = m_pImg->Width();
	desImg.stride = m_pImg->BytesPerLine();
	desImg.height = m_pImg->Height();
	for (int i=0;i<m_ProjectSetting.nImages; i++) {
		if(nTopID == i)
			continue;
		if(m_ProjectSetting.ip[i].pImg) {
			Image24 srcImg;
			srcImg.data = (BYTE*) m_ProjectSetting.ip[i].pImg->GetBits();
			srcImg.stride = m_ProjectSetting.ip[i].pImg->BytesPerLine();
			srcImg.width = m_ProjectSetting.ip[i].pImg->Width();
			srcImg.height = m_ProjectSetting.ip[i].pImg->Height();
			CRect rcSourc = CRect(0,0,srcImg.width, srcImg.height );
			if(m_pImgLab){
				m_pImgLab->StretchImage(&srcImg,  rcSourc, &desImg, m_ProjectSetting.ip[i].rcBound);
			}
		}
	}
	if(nTopID >=0 && nTopID <m_ProjectSetting.nImages && m_ProjectSetting.ip[nTopID].pImg) {
			Image24 srcImg;
			srcImg.data = (BYTE*) m_ProjectSetting.ip[nTopID].pImg->GetBits();
			srcImg.stride = m_ProjectSetting.ip[nTopID].pImg->BytesPerLine();
			srcImg.width = m_ProjectSetting.ip[nTopID].pImg->Width();
			srcImg.height = m_ProjectSetting.ip[nTopID].pImg->Height();
			CRect rcSourc = CRect(0,0,srcImg.width, srcImg.height );
			if(m_pImgLab){
				m_pImgLab->StretchImage(&srcImg,  rcSourc, &desImg, m_ProjectSetting.ip[nTopID].rcBound);
			}
	}
	return 0;
}
// CMainDoc commands

BOOL CMainDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
		/*don't use Serialize if (!CDocument::OnOpenDocument(lpszPathName)) */
		FreePropertySetting();
		ProjectSetting ps;
		memset(&ps, 0, sizeof(ProjectSetting));
		CString section;
		TCHAR value[32];
		BOOL bOK = FALSE;
		while(lpszPathName) {
				bOK = GetPrivateProfileString("project", "title", "untitled", ps.title, sizeof(ps.title), lpszPathName);
				if (!bOK)break;
				bOK = GetPrivateProfileString("project", "images", "0", value, sizeof(value), lpszPathName);
				if (!bOK)break;
				ps.nImages = atoi(value);
				GetPrivateProfileString("project", "canvas_width", "0", value, sizeof(value), lpszPathName);
				ps.canvas.cx = atoi(value);
				GetPrivateProfileString("project", "canvas_height", "0", value, sizeof(value), lpszPathName);
				ps.canvas.cy = atoi(value);

				for (int i=0;i<ps.nImages; i++) {
						section.Format("image%d", i+1);
						GetPrivateProfileString(section, "pos_x", "0", value, sizeof(value), lpszPathName);
						ps.ip[i].pos.x = atoi(value);
						GetPrivateProfileString(section, "pos_y", "0", value, sizeof(value), lpszPathName);
						ps.ip[i].pos.y= atoi(value);
						GetPrivateProfileString(section, "width", "0", value, sizeof(value), lpszPathName);
						ps.ip[i].size.cx = atoi(value);
						GetPrivateProfileString(section, "height", "0", value, sizeof(value), lpszPathName);
						ps.ip[i].size.cy = atoi(value);
						GetPrivateProfileString(section, "scale", "1", value, sizeof(value), lpszPathName);
						ps.ip[i].scale= (float) atof(value);
						GetPrivateProfileString(section, "rotate", "0", value, sizeof(value), lpszPathName);
						ps.ip[i].rotate= (float) atof(value);
						//calculate rcBound
						CALCULATE_BOUND(m_ProjectSetting.ip[i].rcBound, m_ProjectSetting.ip[i].pos, 
								m_ProjectSetting.ip[i].size, m_ProjectSetting.ip[i].scale);

						GetPrivateProfileString(section, "path", "", ps.ip[i].path, MAX_PATH, lpszPathName);
				}	
				break;

		}

		return UpdatePropertySetting(&ps);
}

BOOL CMainDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	BOOL bOK = FALSE;
	while(lpszPathName) {
		CString value, section;
		bOK = WritePrivateProfileString("project", "title", m_ProjectSetting.title, lpszPathName);
		if(!bOK) break;
		value.Format("%d", m_ProjectSetting.nImages);
		bOK = WritePrivateProfileString("project", "images", value, lpszPathName);
		if(!bOK) break;
		value.Format("%d", m_ProjectSetting.canvas.cx);
		bOK = WritePrivateProfileString("project", "canvas_width", value, lpszPathName);
		if(!bOK) break;
		value.Format("%d", m_ProjectSetting.canvas.cy);
		bOK = WritePrivateProfileString("project", "canvas_height", value, lpszPathName);
		if(!bOK) break;
		for (int i=0;i<m_ProjectSetting.nImages; i++) {
			section.Format("image%d", i+1);
			value.Format("%d", m_ProjectSetting.ip[i].pos.x);
			WritePrivateProfileString(section, "pos_x", value, lpszPathName);
			value.Format("%d", m_ProjectSetting.ip[i].pos.y);
			WritePrivateProfileString(section, "pos_y", value, lpszPathName);
			value.Format("%d", m_ProjectSetting.ip[i].size.cx);
			WritePrivateProfileString(section, "width", value, lpszPathName);
			value.Format("%d", m_ProjectSetting.ip[i].size.cy);
			WritePrivateProfileString(section, "height", value, lpszPathName);
			value.Format("%9.4f", m_ProjectSetting.ip[i].scale);
			WritePrivateProfileString(section, "scale", value, lpszPathName);
			value.Format("%9.4f", m_ProjectSetting.ip[i].rotate);
			WritePrivateProfileString(section, "rotate", value, lpszPathName);
			WritePrivateProfileString(section, "path", m_ProjectSetting.ip[i].path, lpszPathName);
		}
		break;
	}
	return bOK; //don't use Serialize;
}
void CMainDoc::IncreaseImagePosX(int id, int x)
{
		ASSERT(id >= 0 && id < m_ProjectSetting.nImages);
		m_ProjectSetting.ip[id].pos.x += x;
		CALCULATE_BOUND(m_ProjectSetting.ip[id].rcBound,m_ProjectSetting.ip[id].pos,
				m_ProjectSetting.ip[id].size, m_ProjectSetting.ip[id].scale);
		DoStitching(id, 1);
}
void CMainDoc::IncreaseImagePosY(int id, int y)
{
		ASSERT(id >= 0 && id < m_ProjectSetting.nImages);
		m_ProjectSetting.ip[id].pos.y += y;
		CALCULATE_BOUND(m_ProjectSetting.ip[id].rcBound,m_ProjectSetting.ip[id].pos,
				m_ProjectSetting.ip[id].size, m_ProjectSetting.ip[id].scale);
		DoStitching(id, 1);
}
void CMainDoc::IncreaseImageScale(int id, float scale)
{
		ASSERT(id >= 0 && id < m_ProjectSetting.nImages);
		m_ProjectSetting.ip[id].scale += scale;
		if (m_ProjectSetting.ip[id].scale <0.1f) 
				m_ProjectSetting.ip[id].scale = 0.1f;
		CALCULATE_BOUND(m_ProjectSetting.ip[id].rcBound,m_ProjectSetting.ip[id].pos,
				m_ProjectSetting.ip[id].size, m_ProjectSetting.ip[id].scale);
		DoStitching(id, 1);

}
void CMainDoc::GetImagesBoundary(CRect& rc)
{
		rc = CRect(1000,1000,0,0);
		for (int i=0; i < m_ProjectSetting.nImages; i++) {
				if (rc.top > m_ProjectSetting.ip[i].rcBound.top) rc.top = m_ProjectSetting.ip[i].rcBound.top;
				if (rc.bottom < m_ProjectSetting.ip[i].rcBound.bottom) rc.bottom = m_ProjectSetting.ip[i].rcBound.bottom;
				if (rc.left > m_ProjectSetting.ip[i].rcBound.left) rc.left = m_ProjectSetting.ip[i].rcBound.left;
				if (rc.right < m_ProjectSetting.ip[i].rcBound.right) rc.right = m_ProjectSetting.ip[i].rcBound.right;
		}
}

void CMainDoc::IncreaseImageRotate(int id, float radiun)
{
		TRACE("No support rotation!!\n");
		DoStitching(id, 1);
}
void CMainDoc::OnFileProperties()
{
		ProjSettingDlg dlg;
		dlg.SetProperty(&m_ProjectSetting);
		int i;
		if( dlg.DoModal() == IDOK) {
				//is modified?
				BOOL bModified = FALSE;
				ProjectSetting* ps = dlg.GetProperty();
				do {
						if(ps->nImages != m_ProjectSetting.nImages) {
								bModified = TRUE;
								break;
						}
						if(ps->canvas.cx != m_ProjectSetting.canvas.cx || ps->canvas.cy != m_ProjectSetting.canvas.cy) {
								bModified = TRUE;
								break;
						}
						if(strcmp(ps->title, m_ProjectSetting.title)!=0) {
								strcpy_s(m_ProjectSetting.title, sizeof(m_ProjectSetting.title), ps->title);
						}
						for( i=0; i<m_ProjectSetting.nImages;i++) {
								if(strcmp(ps->ip[i].path, m_ProjectSetting.ip[i].path)!=0) {
										strcpy_s(m_ProjectSetting.title, sizeof(m_ProjectSetting.title), ps->title);
										bModified = TRUE;
										break;
								}
								if((ps->ip[i].pos.x!= m_ProjectSetting.ip[i].pos.x)||
												(ps->ip[i].pos.y!= m_ProjectSetting.ip[i].pos.y)) {
										strcpy_s(m_ProjectSetting.title, sizeof(m_ProjectSetting.title), ps->title);
										bModified = TRUE;
										break;
								}
						}
				}while(0);

				if(bModified){
						UpdatePropertySetting(dlg.GetProperty());

						this->UpdateAllViews(NULL);
				} 
		}
}

void CMainDoc::OnUpdateFileProperties(CCmdUI *pCmdUI)
{
		pCmdUI->Enable(m_ProjectSetting.nImages > 0);
}

void CMainDoc::OnFileExport()
{
		AfxMessageBox("To be implemented soon!");
}

void CMainDoc::OnUpdateFileExport(CCmdUI *pCmdUI)
{
		pCmdUI->Enable(m_ProjectSetting.nImages > 0);
}
