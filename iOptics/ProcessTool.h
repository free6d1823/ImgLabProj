/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file ProcessTool.h declarations of class ProcessTool and ProcessWrap
 */
#pragma once
#include "afxwin.h"

class ProcessView;
class ImgFile;
class WorkView;
/**
 * \class ProcessWrap 
 * \brief wrap process parameters 
 * \brief The data of process parameters are depedent on individual process tool. Each tool will produce LUT data, which is maintained by this class.
 * The ProcessWrap keeps process's parameters and allocate memory space for LUT. The struct of a Process's parameter is defined by each Process and decalred in common.h
 * \sa ProcessView ProcessWrap
  */
class ProcessWrap
{
public:
	ProcessWrap();
	virtual ~ProcessWrap();
	/*!
	 * Sets processing prameters, defined in its derived class, into the capasolutor 
	 * \public \memberof ProcessWrap
	 */
	virtual void SetParam(void* pParam){};

	/*!
	 * Create the LUT data according its processing parameters
	 */
	virtual void MakeMapTable()=0;

	/*! Position mapping from processed image to ideal position of original image
	* \param[in] u [0~szOutput.cx) in rocessed image
	* \param[in] v [0~szOutput.cy) in rocessed image
	* \param[out] x [0~szInput.cx) in rocessed image
	* \param[out] y [0~szInput.cy) in rocessed image
	*/	
	virtual BOOL	PosMap(int u, int v, float&x, float&y)=0;

	/*!
	 * Gets the start address of LUT buffer
	 * \public \memberof ProcessWrap
	 */
	PointDouble* GetLutTable(){return lut;};
	/*!
	 * Gets the numbers of column of the LUT, including bondary
	 * \public \memberof ProcessWrap
	 */
	int	LutColumns(){return cxLut;} 
	/*!
	 * Gets the numbers of rows of the LUT, including bondary
	 * \public \memberof ProcessWrap
	 */
	int LutRows(){return cyLut;}
	/*!
	 * Cleans the LUT buffer
	 * \public \memberof ProcessWrap
	 */
	void	ResetLutData(){if(lut) memset(lut, 0, cxLut*cyLut*sizeof(double));}
	/*!
	 * Get normalized X coordinate the LUT in the position (X,Y) which is on processed image
	 * \public \memberof ProcessWrap
	 */
	double GetLutDataX(int x, int y);
	/*!
	 * Get normalized Y coordinate the LUT in the position (X,Y) which is on processed image
	 * \public \memberof ProcessWrap
	 */
	double GetLutDataY(int x, int y);
	/*!
	 * Set normalized X coordinate the LUT in the position (X,Y) which is on processed image
	 * \public \memberof ProcessWrap
	 */
	void	SetLutDataX(int x, int y, double value);
	/*!
	 * Set normalized Y coordinate the LUT in the position (X,Y) which is on processed image
	 * \public \memberof ProcessWrap
	 */
	void	SetLutDataY(int x, int y, double value);

	/*!
	 * Save process specified parameters in a file. The format of data can be defined dependently in this function.
	 * \public \memberof ProcessWrap
	 */
	virtual BOOL SaveParameter(LPCTSTR filename)=0;
	/*!
	 * Read process specified parameters from a file. The format of data can be defined dependently by the correspondent ProcessWrap class.
	 * \public \memberof ProcessWrap
	 */
	virtual BOOL LoadParameter(LPCTSTR filename)=0;
protected:
	int cxLut;	/*!< numbers of column of the LUT */
	int cyLut;	/*!<  numbers of row of the LUT */
	PointDouble* lut; /*!< stat address of the LUT */
};
 
inline	double ProcessWrap::GetLutDataX(int x, int y) {return lut[y*cxLut + x].x;};
inline	double ProcessWrap::GetLutDataY(int x, int y) {return lut[y*cxLut + x].y;};
inline	void	ProcessWrap::SetLutDataX(int x, int y, double value){lut[y*cxLut + x].x = value;};
inline	void	ProcessWrap::SetLutDataY(int x, int y, double value){lut[y*cxLut + x].y = value;};

/**
 * \class ProcessTool
 * \brief ProcessTool is the abstract class which is used to control the settings of a process with UI in a dialog box.
 * A ProcessTool class consistent of a ProcessView member to display the processed image and a ProcessWrap class member 
 * to aintain the process related parameters and LUT data.
 * ProcessTool is the bridge class between WorkView and the Process. The application use ProcessTool to access the specified process stuff.
 *
 */
class ProcessTool : public CDialog
{
	DECLARE_DYNAMIC(ProcessTool)

public:
	/*!
	 * Create the derrived class of the specified process typr
	 * \public \memberof ProcessTool
	 * \param pOwner	parent window of the ProcessTool, the WorkView class
	 * \param nType	type of ProcessTool
	 *			nType = IDD_FEC	FEC process tool
	 *			nType = IDD_LDC LDC process tool
	 * \param pImage the original image to be processed from
	 * \return the derrived ProcessTool class pointer of the specified process.
	 */
static ProcessTool* CreateProcessTool(WorkView* pOwner,int nType, ImgFile* pImage);
	/*!
	 * Gets the ProcessTool ID.
	 * \public \memberof ProcessTool
	 */
int GetProcessToolID(){ return IDD;}
	enum { IDD = 0 };
	ProcessTool(UINT nIDTemplate, WorkView* pParent );   // standard constructor
	virtual ~ProcessTool();

	/*!
	 * Copys contents of process parameters into related ProcessWrap.
	 * \public \memberof ProcessTool
	 */
	virtual void SetParam(ProcessWrap* pParam){};
	/*!
	 * Gets the ProcessWrap class pointer of the process tool.
	 * \public \memberof ProcessTool
	 */
	virtual ProcessWrap* GetParam(){return m_pProcessWrap;};

	/*!
	 * Sets the image to be processed from. The processed imaged won't replace this original image.
	 * \public \memberof ProcessTool
	 */
	virtual void SetImgFile(ImgFile* pImage);
	/*!
	 * Gets the processed image in ProcessView of this ProcessTool.
	 * \public \memberof ProcessTool
	 */
	virtual ImgFile* GetImgFile();

// Dialog Data
protected:
	double GetDlgItemDouble(int nID);
	void SetDlgItemFloat(int nID, float fValue);
	void SetDlgItemDouble(int nID, double dValue);
	virtual void UpdateData(BOOL bSaveAndValidate = TRUE){};		
	virtual void PostNcDestroy();
	WorkView*			m_pOwner;
	ProcessWrap*		m_pProcessWrap;		/*!< pointer to parameter capsulator class*/
	ProcessView*	m_pBasicView;			/*!< pointer to processed image viewer */
	void OnSaveParameter();/*!< UI request to save process parameters */
	void OnLoadParameter();/*!< UI request to load process parameters */
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnUpdateToolbox(CCmdUI *pCmdUI);
	afx_msg void OnCommandToolbox(UINT nCmd);
	afx_msg void OnDestroy();
protected:

};

	