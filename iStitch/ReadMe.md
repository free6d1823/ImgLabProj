@mainpage Introduction
@section Purpose
This application is designed as a tool for images stitching

@section arch Application Architecture
@dot
   digraph mfc {
		edge [fontname="Helvetica",fontsize="10",labelfontname="Helvetica",labelfontsize="10"];
		node [fontname="Helvetica",color="black", height=0.2,width=0.4,fontsize="10",shape=record];
		CMainApp [ label="CMainApp",color="red", fillcolor="grey75", style="filled", fontcolor="black"];
		CMainFrame [ label="CMainFrame", fillcolor="grey75", style="filled", fontcolor="black"];
		CMainView [ label="CMainView", fillcolor="grey75", style="filled",tooltip="main display window, including ruler and image viewer"];	  
		CMainDoc [ label="CMainDoc", fillcolor="grey75", style="filled",tooltip="This class encapsulate the source image file."];	 
		ImageProperty [ label="ImageProperty[]",fillcolor="lightblue", style="filled", fontcolor="black", tooltip="source images"];		
		WorkView [ label="WorkView",URL="$class_work_view.html",tooltip="WorkView is the window displays the original image."];	 	   
		Image  [ label="ImgFile",fillcolor="yellow", style="filled", fontcolor="black", tooltip="canvas image"];		
		ControlBar [ label="ControlBar", tooltip="Stitching control bar - pan, tilt, zoom"];
		ProjSettingDlg [ label="ProjSettingDlg", tooltip="images settings dialog box"];
		ProjectSetting [ label="ProjectSetting", fillcolor="yellow", style="filled", fontcolor="black", tooltip="images settings and source buffer"];		
		RulerBar [label="RulerBar"];
		Gride [label="Gride"];		
		CMainApp -> CMainFrame [ arrowhead="open", style="dashed", label="m_pMainWnd" ];
		CMainApp -> CMainView [ arrowhead="open", style="dotted" ];
		CMainApp -> CMainDoc [ arrowhead="open", style="dotted" ];
		CMainFrame -> CToolBar[label = "m_wndToolBar" ];
		CMainFrame -> CStatusBar [label = "m_wndStatusBar" ];
		CMainDoc -> ProjectSetting [label = "m_ProjectSetting"];
		ProjSettingDlg ->ProjectSetting {style="dashed"};
		CMainView -> RulerBar [ arrowhead="open", style="solid"];		
		CMainView -> WorkView [ arrowhead="open", style="solid", label = "m_view" ];	
		WorkView -> Gride [color="midnightblue",fontsize="10",style="dashed"];			
		WorkView -> ControlBar [color="midnightblue",fontsize="10",style="shed", label="m_ControlBar"];	  
   		WorkView -> Image [label = "m_pImage"];	   
		ControlBar -> Image [style="dotted"];
		ControlBar -> ProjectSetting[style="dotted"];
		ProjectSetting -> Image [style="dotted"];
		ProjectSetting -> ImageProperty [ arrowhead="open", style="solid", label="ip"];				
   }
@enddot
