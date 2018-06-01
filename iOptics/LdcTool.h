/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file LdcTool.h declarations of class LdcTool and LdcWrap
 */
#pragma once
#include "common.h"
#include "ProcessTool.h"

/**
 * \class LdcWrap 
 * \brief LDC process parameters wrapper
 * This class wrap LDC parameters and LUT table, as well as LUT generating function
 * \sa LdcView ProcessWrap
  */
class LdcWrap : public ProcessWrap
{
public:
	LdcWrap();
	virtual ~LdcWrap();
	/*!
	 * Sets LDC processing prameters, which is copied to class members and generate the LUT data.
	 * \param pParam pointer to m_dbCoef.
	 */
	virtual void SetParam(void* pParam);

	/*!
	 * Create the LUT data according the parameter
	 */
	virtual void MakeMapTable(){};

	/*! Position mapping from processed image to original image
	* \param[in] u [0~szOutput.cx) in rocessed image
	* \param[in] v [0~szOutput.cy) in rocessed image
	* \param[out] x [0~szInput.cx) in rocessed image
	* \param[out] y [0~szInput.cy) in rocessed image
	*/	
	virtual BOOL	PosMap(int u, int v, float&x, float&y){return TRUE;};
	
	/*!
	 * Save process specified parameters in a file. The format of data can be defined dependently in this function.
	 * \public \memberof ProcessWrap
	 */
	virtual BOOL SaveParameter(LPCTSTR filename){TRACE("Not implemented yet!\n"); return FALSE;};
	/*!
	 * Read process specified parameters from a file. The format of data can be defined dependently by the correspondent ProcessWrap class.
	 * \public \memberof ProcessWrap
	 */
	virtual BOOL LoadParameter(LPCTSTR filename){TRACE("Not implemented yet!\n"); return FALSE;};

private:
	double m_dbCoef;	/*!< 2nd order radial distortio coefficient*/

};

/**
 * \class LdcTool 
 * \brief LdccTool is used to control the settings of Ldc process with UI in a dialog box.
 * This class sends the parameters to a FecView to display the resulted image. It also connects to a LdcWrap class 
 * to maintain the LDC process related parameters and LUT data.
 *
 */
class LdcTool : public ProcessTool
{
	DECLARE_DYNAMIC(LdcTool)

public:
	LdcTool(CWnd* pParent = NULL);   
	virtual ~LdcTool();
	virtual void SetParam(ProcessWrap* pParam){};
	virtual ProcessWrap* GetParam(){return m_pProcessWrap;};
	virtual void SetImgFile(ImgFile* pImage);
// Dialog Data
	enum { IDD = IDD_LDC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	
};
