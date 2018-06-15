#pragma once
#include "common.h"
#include "processtool.h"


/**
 * \class HomoWrap 
 * \brief Homogrphy matrix process parameters wrapper
 * This class wrap Homography parameters and LUT table, as well as LUT generating function
 * \sa HomoView ProcessWrap
  */
class HomoWrap : public ProcessWrap
{
	friend class HomoTool;
	friend class HomoView;
	friend class HomoGride;

public:
	HomoWrap();
	virtual ~HomoWrap();
	/*!
	 * Sets Homography processing prameters, which is copied to class members and generate the LUT data.
	 * \param pParam pointer to 
	 */
	virtual void SetParam(void* pParam);

	/*!
	 * Create the LUT data according the parameter
	 */
	virtual void MakeMapTable();

	/*! Position mapping from processed image to original image
	* \param[in] u [0~szOutput.cx) in rocessed image
	* \param[in] v [0~szOutput.cy) in rocessed image
	* \param[out] x [0~szInput.cx) in rocessed image
	* \param[out] y [0~szInput.cy) in rocessed image
	*/	
	virtual BOOL	PosMap(int u, int v, float&x, float&y);
	
	/*!
	 * Save process specified parameters in a file. The format of data can be defined dependently in this function.
	 * \public \memberof ProcessWrap
	 */
	virtual BOOL SaveParameter(LPCTSTR filename);
	/*!
	 * Read process specified parameters from a file. The format of data can be defined dependently by the correspondent ProcessWrap class.
	 * \public \memberof ProcessWrap
	 */
	virtual BOOL LoadParameter(LPCTSTR filename);

	/*!
	 * Calculate Matrix based on known P and Q, so that Q=HP
	 */
	void CalculateMatrix();
	

private:
	void PosMapNormalize(double u, double v, double &x, double &y);
	HomoParam	param;
};

class HomoTool : public ProcessTool
{
	DECLARE_DYNAMIC(HomoTool)
public:
	HomoTool(WorkView* pParent);   
	virtual ~HomoTool();
	virtual void SetParam(ProcessWrap* pParam);
	virtual ProcessWrap* GetParam();
	virtual void SetImgFile(ImgFile* pImage);
// Dialog Data
	enum { IDD = IDD_HOMO };
	void UpdateData(BOOL bSaveAndValidate = TRUE);
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedSetP();
	afx_msg void OnBnClickedGetP();
	afx_msg void OnBnClickedSetQ();
	afx_msg void OnBnClickedGetQ();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedOk();
};
