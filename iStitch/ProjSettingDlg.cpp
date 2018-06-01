// ProjSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iStitch.h"
#include "ProjSettingDlg.h"
#include <string.h>

#define ID_NO				0
#define ID_X				1
#define ID_Y				2
#define ID_W				3
#define ID_H				4
#define ID_ROTATE	5
#define ID_SCALE		6
#define ID_PATH			7
#define MAX_COLUMN	8

#define 	DONT_UPDATE_VARIABLE		0x00010000 //when EDIT1 UI is updated, dn't save UI to variabl
// ProjSettingDlg dialog

IMPLEMENT_DYNAMIC(ProjSettingDlg, CDialog)

ProjSettingDlg::ProjSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ProjSettingDlg::IDD, pParent)
{
	memset(&m_Setting, 0, sizeof(m_Setting));
	m_pListImage = NULL;
	m_nCurrentCounts = 0;
	m_dwSelectedCell = 0;
}

ProjSettingDlg::~ProjSettingDlg()
{
}
void ProjSettingDlg::SetProperty(ProjectSetting* pSetting)
{
	memset(&m_Setting, 0, sizeof(m_Setting));
	m_Setting.nImages = pSetting->nImages;
	strcpy_s(m_Setting.title, sizeof(m_Setting.title), pSetting->title);
	m_Setting.canvas = pSetting->canvas;
	for (int i=0;i<m_Setting.nImages; i++) {
		strcpy_s(m_Setting.ip[i].path, sizeof(m_Setting.ip[i].path), pSetting->ip[i].path);
		m_Setting.ip[i].pos = pSetting->ip[i].pos;
		m_Setting.ip[i].size = pSetting->ip[i].size;
		m_Setting.ip[i].scale = pSetting->ip[i].scale;
		m_Setting.ip[i].rotate = pSetting->ip[i].rotate;
	}
}


void ProjSettingDlg::DoDataExchange(CDataExchange* pDX)
{
//	CDialog::DoDataExchange(pDX);
//	DDV_MinMaxInt(pDX, m_Setting.nImages, 0, MAX_IMAGES);

}
BOOL ProjSettingDlg::UpdateData(BOOL bReadFromUI)
{
	if( bReadFromUI) {//read UI
		m_Setting.canvas.cx = GetDlgItemInt(IDC_WIDTH);
		m_Setting.canvas.cy = GetDlgItemInt(IDC_HEIGHT);
		GetDlgItemText(IDC_PROJECT_NAME, m_Setting.title, sizeof(m_Setting.title));
	}else {
		SetDlgItemInt(IDC_WIDTH, m_Setting.canvas.cx );
		SetDlgItemInt(IDC_HEIGHT, m_Setting.canvas.cy );
		SetDlgItemText(IDC_PROJECT_NAME, m_Setting.title);
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(ProjSettingDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &ProjSettingDlg::OnDeltaposSpin1)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTIMAGE, &ProjSettingDlg::OnHdnItemdblclickListimage)
	ON_EN_CHANGE(IDC_IMAGE_COUNTS, &ProjSettingDlg::OnEnChangeImageCounts)
	ON_EN_CHANGE(IDC_EDIT1, &ProjSettingDlg::OnEnChangeEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &ProjSettingDlg::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// ProjSettingDlg message handlers

BOOL ProjSettingDlg::OnInitDialog()
{
		//CDialog::OnInitDialog();//called UpdateData(FALSE); 
		m_pListImage = (CListCtrl* ) GetDlgItem(IDC_LISTIMAGE);
		CRect rc;
		m_pListImage->GetClientRect(&rc);
		m_pListImage->SetExtendedStyle(LVS_EX_FULLROWSELECT);
		m_pListImage->InsertColumn(ID_NO, "No.",LVCFMT_LEFT, rc.right * 5/100);
		m_pListImage->InsertColumn(ID_X, "left",LVCFMT_RIGHT, rc.right * 10/100);
		m_pListImage->InsertColumn(ID_Y, "top",LVCFMT_RIGHT, rc.right * 10/100);
		m_pListImage->InsertColumn(ID_W, "width",LVCFMT_RIGHT, rc.right * 10/100);
		m_pListImage->InsertColumn(ID_H, "height",LVCFMT_RIGHT, rc.right * 10/100);
		m_pListImage->InsertColumn(ID_ROTATE, "rotate",LVCFMT_RIGHT, rc.right *10/100);
		m_pListImage->InsertColumn(ID_SCALE, "scale",LVCFMT_RIGHT, rc.right * 10/100);
		m_pListImage->InsertColumn(ID_PATH, "path",LVCFMT_RIGHT, rc.right * 36/100);

		LVITEM lvItem;

		for (int i=0; i<MAX_IMAGES; i++) {
				char buff[256];
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = i;
				lvItem.iSubItem = 0;
				_itoa_s((i+1), buff, sizeof(buff), 10);
				lvItem.pszText = buff;
				int nItem = m_pListImage->InsertItem(&lvItem);
		}
		UpdateData(FALSE);//write _Settings to UI
		m_nCurrentCounts = m_Setting.nImages;		
		SetDlgItemInt(IDC_IMAGE_COUNTS, m_Setting.nImages ); //this line should be last, because it will trigger CEdit change event
		//triger UpdateListCtrl() when IDC_IMAGE_COUNTS updated
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ProjSettingDlg::OnOK()
{
	UpdateData(TRUE);
	//write list to settings
	TRACE("OnOK count = %d\n", m_Setting.nImages );
	
	CDialog::OnOK();
}

void ProjSettingDlg::UpdateListCtrl()
{
		UpdateData(TRUE);//read from UI
		char buff[256];
		TRACE("New UpdateListCtrl = %d, cnt = %d\n", m_pListImage->GetItemCount(), m_Setting.nImages);
		for (int i=0; i< MAX_IMAGES; i++ ) {
				if (i >=  m_Setting.nImages)  //clear the items
				{
						m_pListImage->SetItemText(i, ID_X, "");
						m_pListImage->SetItemText(i, ID_Y, "");
						m_pListImage->SetItemText(i, ID_W, "");
						m_pListImage->SetItemText(i, ID_H, "");
						m_pListImage->SetItemText(i, ID_ROTATE, "");
						m_pListImage->SetItemText(i, ID_SCALE, "");
						m_pListImage->SetItemText(i, ID_PATH, "");
				} else if(i <  m_Setting.nImages && i >= m_nCurrentCounts) {		//add new item 
						memset(&m_Setting.ip[i], 0, sizeof(ImageProperty));
						m_Setting.ip[i].pos.x = i * m_Setting.canvas.cx / m_Setting.nImages;
						_itoa_s(m_Setting.ip[i].pos.x, buff, sizeof(buff), 10);
						m_pListImage->SetItemText(i,ID_X, buff);
						m_pListImage->SetItemText(i, ID_Y, "0");
						m_pListImage->SetItemText(i, ID_W, "0");
						m_pListImage->SetItemText(i, ID_H, "0");
						m_pListImage->SetItemText(i, ID_ROTATE, "0");
						m_pListImage->SetItemText(i, ID_SCALE, "1.0");
						m_pListImage->SetItemText(i, ID_PATH, "NULL");
				}else {

						_itoa_s(m_Setting.ip[i].pos.x, buff, sizeof(buff), 10);
						m_pListImage->SetItemText(i, ID_X, buff);
						_itoa_s(m_Setting.ip[i].pos.y, buff, sizeof(buff), 10);
						m_pListImage->SetItemText(i, ID_Y, buff);
						_itoa_s(m_Setting.ip[i].size.cx, buff, sizeof(buff), 10);
						m_pListImage->SetItemText(i, ID_W, buff);
						_itoa_s(m_Setting.ip[i].size.cy, buff, sizeof(buff), 10);
						m_pListImage->SetItemText(i, ID_H, buff);
						sprintf_s(buff, sizeof(buff), "%4.2f", m_Setting.ip[i].scale);
						m_pListImage->SetItemText(i, ID_SCALE, buff);
						sprintf_s(buff, sizeof(buff), "%5.1f", m_Setting.ip[i].rotate * 180/3.1415967);
						m_pListImage->SetItemText(i, ID_ROTATE, buff);
						if(m_Setting.ip[i].path[0] != 0) {
								char* p = strrchr(m_Setting.ip[i].path, '\\');
								if (p) 
										strcpy_s(buff, sizeof(buff), p+1);
								else
										strcpy_s(buff, sizeof(buff), m_Setting.ip[i].path);
						} else {
								strcpy_s(buff, sizeof(buff), "NULL");
						}
						m_pListImage->SetItemText(i, ID_PATH, buff);
				}
		}
		m_nCurrentCounts = m_Setting.nImages; //now m_Setting.nImages sync to m_Setting.nImages
}


void ProjSettingDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	pNMUpDown->iDelta *= -1; //change direction
	if(pNMUpDown->iPos == MAX_IMAGES && pNMUpDown->iDelta > 0)
		pNMUpDown->iDelta = 0; 

	if(pNMUpDown->iDelta != 0) 
			UpdateListCtrl();
	*pResult = 0;
}
void ProjSettingDlg::OnEditItem(int nID, int nSub)
{
		switch(nSub) {
				case ID_PATH: {
					
								CString strFileName = m_Setting.ip[nID].path;
								CString strFilter = "*.bmp|*.bmp|*.jpg|*.jpg||";
								CFileDialog aFileDialog( TRUE, NULL, strFileName,
										 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
										 strFilter);
								if( IDOK == aFileDialog.DoModal())
								{
										strFileName = aFileDialog.GetPathName();
										//check image
										ImgFile* pImg= new ImgFile();
										if (pImg->Load(strFileName)== Img_OK) {
												//m_pListImage->SetItemText(pNMLV->iItem, 5, strFileName);
												strcpy_s(m_Setting.ip[nID].path, sizeof(m_Setting.ip[nID].path), strFileName);
												m_Setting.ip[nID].size.cx = pImg->Width();
												m_Setting.ip[nID].size.cy = pImg->Height();
												UpdateListCtrl();
										} else {
												CString szText;
												szText.Format("%s is not recognized image file\n", strFileName);
												AfxMessageBox(szText);
										}
										delete pImg;
								}

						}
						break;
				case ID_X:
				case ID_Y: {
								CRect rc, rc1;

								m_pListImage->GetSubItemRect(nID, nSub, LVIR_LABEL,  rc);//relative to child
								m_pListImage->GetWindowRect(&rc1); //screen
								ScreenToClient(&rc1);
								rc.OffsetRect(rc1.left, rc1.top);

								CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT1);
								pEdit->MoveWindow(&rc);
								pEdit->ShowWindow(SW_SHOW);
								pEdit->SetFocus();
								//this event is initial CEdit UI data, don't save back to variable later
								m_dwSelectedCell = ((nID <<8) | nSub | DONT_UPDATE_VARIABLE);
								if (nSub == ID_X)
										SetDlgItemInt(IDC_EDIT1, m_Setting.ip[nID].pos.x);
								else 
										SetDlgItemInt(IDC_EDIT1, m_Setting.ip[nID].pos.y);
						   }
						break;
		}
}
void ProjSettingDlg::OnHdnItemdblclickListimage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nID = pNMLV->iItem; //-1 if out side of items
	if (pNMLV->iItem >=0 && pNMLV->iItem < m_Setting.nImages) {
			OnEditItem(pNMLV->iItem, pNMLV->iSubItem);
	}

	*pResult = 0;
}

void ProjSettingDlg::OnEnChangeImageCounts()
{
		//this will be triggered when CEdit is first time created, before OnInitDialog
		if(m_pListImage) //update list control when user change the image numbers
		{
				m_Setting.nImages = GetDlgItemInt(IDC_IMAGE_COUNTS);
				UpdateListCtrl();
		}
}

void ProjSettingDlg::OnEnChangeEdit1()
{
		if (m_dwSelectedCell & DONT_UPDATE_VARIABLE) {
				//CEdit is changed because it  is inited by CEdit from m_Setting
				// no need to write to m_Setting again.
				m_dwSelectedCell &= (~DONT_UPDATE_VARIABLE);
				return;
		}
		TRACE("Changed %x\n", m_dwSelectedCell);
		//CEdit is changed by user, write back to variable
		int nItem = (m_dwSelectedCell >> 8 )& 0xff;
		int nSub = (m_dwSelectedCell & 0xff);


		if (nItem < m_Setting.nImages) {
				int value = GetDlgItemInt(IDC_EDIT1);
				char buff[32];
				_itoa_s(value, buff, sizeof(buff), 10);
				if (nSub == ID_X) {
						m_Setting.ip[nItem].pos.x = value;
						m_pListImage->SetItemText(nItem, ID_X, buff);
				}
				else if (nSub == ID_Y){
						m_Setting.ip[nItem].pos.y = value;
						m_pListImage->SetItemText(nItem, ID_Y, buff);
				}



		}

}

void ProjSettingDlg::OnEnKillfocusEdit1()
{
		// TODO: Add your control notification handler code here
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		TRACE("Killfocus %x\n", m_dwSelectedCell);
		m_dwSelectedCell = 0;
}
