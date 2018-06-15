@mainpage Introduction
@section Purpose
This application is designed as a lens correction tool.

@section arch Application Architecture
@dot
  digraph mfc {
		edge [fontname="Helvetica",fontsize="10",labelfontname="Helvetica",labelfontsize="10"];
		node [fontname="Helvetica",color="black", height=0.2,width=0.4,fontsize="10",shape=record];
		CMainApp [ label="CMainApp",color="red", fillcolor="grey75", style="filled", fontcolor="black"];
		CMainFrame [ label="CMainFrame", fillcolor="grey75", style="filled", fontcolor="black"];
			
		CMainView [ label="CMainView", fillcolor="grey75", style="filled",tooltip="main display window, including ruler and image viewer"];	  
		
		CMainDoc [ label="CMainDoc", fillcolor="grey75", style="filled",tooltip="This class encapsulate the source image file."];	 
		ImgFile [ label="ImgFile",fillcolor="lightblue", style="filled", fontcolor="black", tooltip="source image"];	   
		WorkView [ label="WorkView",URL="$class_work_view.html",tooltip="WorkView is the window displays the original image."];	 	   
		ProcessTool [ label="ProcessTool", tooltip="The process related stuff"];
		
		CMainApp -> CMainFrame [ arrowhead="open", style="dashed", label="m_pMainWnd" ];
		CMainApp -> CMainView [ arrowhead="open", style="dotted" ];
		CMainApp -> CMainDoc [ arrowhead="open", style="dotted" ];	

		
		CMainDoc -> ImgFile [ arrowhead="open", style="dashed", label="m_pImg" ];		   
		CMainView -> WorkView [ arrowhead="open", style="solid", label = "m_view" ];	
		CMainView -> CMainDoc [ arrowhead="open", style="dashed", label = "m_pDocument" ];	
		WorkView -> ProcessTool [color="midnightblue",fontsize="10",style="dashed", label="m_pProcessTool"];	

		CStatusBar [ label="CStatusBar", fillcolor="grey75", style="filled"];
		CToolBar [ label="CToolBar", fillcolor="grey75", style="filled"];
		CPaletteBar [ label="CPaletteBar", fillcolor="grey75", style="filled"];
		RulerBar[ label="RulerBar", fillcolor="grey75", style="filled"];
		Gride [ label="Gride", fillcolor="yellow", style="filled"];
		Mesh [ label="Mesh", fillcolor="yellow", style="filled"];
		CMainFrame -> CStatusBar;
		CMainFrame -> CToolBar;
		CMainFrame -> CPaletteBar[label="m_wndToolBox" ];   
		CMainView -> RulerBar;
		WorkView -> Gride[label="m_gride"];
		WorkView -> Mesh[label="m_mesh"];
		WorkView -> ImgFile [ arrowhead="open", style="dashed", label="m_pImage" ];
   }
@enddot 
		
@section process ProcessTool Hierarchy
@dot
   digraph processtool {
		edge [fontname="Helvetica",fontsize="10",labelfontname="Helvetica",labelfontsize="10"];   
		node [fontname="Helvetica",color="black", height=0.2,width=0.4,fontsize="10",shape=record];
		ProcessTool [ label="ProcessTool", URL="class_process_tool.html"];
		ProcessView [ label="ProcessView", URL="class_process_view.html"];
		ProcessWrap [ label="ProcessWrap", URL="class_process_cap.html"];	
		ProcessParam [ label="ProcessParam\nLUT",height=0.2,width=0.4, fillcolor="lightblue", style="filled", tooltip="process dependent parameters"];	 
		ImgFile [ label="ImgFile",height=0.2,width=0.4,fillcolor="lightblue", style="filled", fontcolor="black", tooltip="processed image"];	   

		ProcessTool -> ProcessView [ arrowhead="open", style="dashed", fontsize="10",label="m_pBasicView" ];
		ProcessTool -> ProcessWrap [ arrowhead="open", style="dashed",fontsize="10", label="m_pParCap" ];	   
		ProcessView -> ProcessWrap [ arrowhead="open", style="dotted",fontsize="10", label="m_pParamRef" ];	
		ProcessWrap -> ProcessParam [ arrowhead="open", style="solid",fontsize="10", label = "param, lut" ];	 
		ProcessView -> ImgFile [ arrowhead="open", fontsize="10",fontname="Helvetica", label="m_pImg", style="solid" ];	
	  
   }
@enddot
	
@section rendering Rendering Flow
@dot
	digraph rendering {
		edge [fontname="Helvetica",fontsize="10",labelfontname="Helvetica",labelfontsize="10"];
		node [fontname=Helvetica, fontsize=10];
		ProcessView [shape=box label="ProcessView\n on param changed"]; /* this is a comment */
		MakeMapTable [label="MakeMapTable"];
		OnPreview [shape=box];
		PosMap [label="PosMap"];
		GetColor [label="GetColor"];
		NextFrame [shape=box];
		ProcessView -> MakeMapTable [weight=8];
		MakeMapTable -> OnPreview[style=dashed]; 
		OnPreview -> PosMap[label="for each pixel\non destination", style="solid" ]; 
		PosMap -> GetColor;
		GetColor -> NextFrame;
		NextFrame -> OnPreview;

	}

@enddot