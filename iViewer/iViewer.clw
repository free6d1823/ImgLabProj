; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CThresholdDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "iViewer.h"
LastPage=0

ClassCount=7
Class1=CIvApp
Class2=CImgDoc
Class3=CImgView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Class6=Navigator
Resource2=IDR_MAINFRAME
Class7=CThresholdDlg
Resource3=IDD_THRESHOLD

[CLS:CIvApp]
Type=0
HeaderFile=iViewer.h
ImplementationFile=iViewer.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=ID_FILE_OPEN

[CLS:CImgDoc]
Type=0
HeaderFile=ImgDoc.h
ImplementationFile=ImgDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CImgDoc

[CLS:CImgView]
Type=0
HeaderFile=ImgView.h
ImplementationFile=ImgView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CImgView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_MASK
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=iViewer.cpp
ImplementationFile=iViewer.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_OPEN_EDGE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_VIEW_ZOOMIN
Command18=ID_VIEW_ZOOMOUT
Command19=ID_VIEW_Navigater
Command20=ID_APP_ABOUT
Command21=ID_MASK
Command22=ID_ScaleUp
Command23=ID_ScaleUp2
Command24=ID_ScaleUp3
Command25=ID_ScaleUp4
CommandCount=25

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_VIEW_ZOOMIN
Command2=ID_VIEW_ZOOMOUT
Command3=ID_EDIT_COPY
Command4=ID_FILE_NEW
Command5=ID_FILE_OPEN
Command6=ID_FILE_PRINT
Command7=ID_FILE_SAVE
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[CLS:Navigator]
Type=0
HeaderFile=Navigator.h
ImplementationFile=Navigator.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=Navigator

[DLG:IDD_THRESHOLD]
Type=1
Class=CThresholdDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:CThresholdDlg]
Type=0
HeaderFile=ThresholdDlg.h
ImplementationFile=ThresholdDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CThresholdDlg

