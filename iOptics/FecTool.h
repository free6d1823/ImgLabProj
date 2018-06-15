/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file FecTool.h declarations of class FecTool and FecWrap
 */
#pragma once

#include "common.h"
#include "ProcessTool.h"

class ImgFile;

/**
 * \class FecWrap 
 * \brief FEC process parameters wrapper
 * This class wrap FEC parameters and LUT table, as well as LUT generating function
 * \sa FecView ProcessWrap
  */
class FecWrap : public ProcessWrap
{
	friend class FecTool;
	friend class FecView;
	friend class FecGride;
public:
	FecWrap();
	virtual ~FecWrap();
	/*!
	 * Sets FEC processing prameters, which is copied to class members and generate the LUT data.
	 * \param pParam pointer to FecParam.
	 */
	virtual void SetParam(void* pParam);
	/*!
	 * Create the LUT data according the internal FecParam
	 */
	virtual void MakeMapTable();

	/*! Position mapping from processed image to original image
	* \param[in] u [0~szOutput.cx) in rocessed image
	* \param[in] v [0~szOutput.cy) in rocessed image
	* \param[out] x [0~szInput.cx) in rocessed image
	* \param[out] y [0~szInput.cy) in rocessed image
	*/	
	virtual BOOL	PosMap(int u, int v, float&x, float&y);
	
	virtual BOOL SaveParameter(LPCTSTR filename);
	virtual BOOL LoadParameter(LPCTSTR filename);
private:
	void PosMapNormalize(double u, double v, double &x, double &y);
	FecParam	param;

};

/**
 * \class FecTool 
 * \brief FecTool is used to control the settings of FEC process with UI in a dialog box.
 * This class sends the parameters to a FecView to display the resulted image. It also connects to a FecWrap class 
 * to maintain the FEC process related parameters and LUT data.
 *
 */
class FecTool : public ProcessTool
{
	DECLARE_DYNAMIC(FecTool)

public:
	FecTool(WorkView* pParent); 
	virtual ~FecTool();

	virtual void SetParam(ProcessWrap* pParam);
	virtual ProcessWrap* GetParam();

	virtual void SetImgFile(ImgFile* pImage);
// Dialog Data
	enum { IDD = IDD_FEC };

protected:
	void UpdateData(BOOL bSaveAndValidate = TRUE);
	BOOL CheckParameters(FecParam* pParam);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposCenterXSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposCenterYSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRadiusXSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRadiusYSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPitchSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposYawSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRollSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
