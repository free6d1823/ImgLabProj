/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file ImgLab.h declarations of class ImgLab
 */

#pragma once


#ifdef IMGLAB_EXPORTS
#define IMGLAB_API __declspec(dllexport)
#else
#define IMGLAB_API __declspec(dllimport)
#endif

/**
 * \struct _Image24
 * \brief image 24 bits packet mode structure, for example BGR BGR
 *
 */
typedef struct _Image24 {
	int	stride; /*!< bytes per line in the 2-D memory */
	int width;	/*!< pixels of width */
	int height;	/*!< pixels of height	*/ 
	BYTE* data;	/*pixel data */
}Image24, *PImage24;

#define IMGLAB_OK								0
#define IMGLAB_INVALID_PARAMETERS									-1
#define IMGLAB_OUT_OF_MEMORY											-2
#define IMGLAB_OPENCL_CREATE_KERNEL_ERROR				-3
#define IMGLAB_ERROR_FRAME_BUFFER_NOT_ENOUGH	-4
/**
 * \class ImgLab
 * \brief image processing algorithms based on OpenCL.
 *
 */
class ImgLab {
public:
	/*! Create ImgLab class
	* \param bDebug	Sets TRUE to enable OpenCL initilization log and save in CL_init_log.txt file.
	* \return pointer to ImgLab class if successful.
	*/	
IMGLAB_API	static ImgLab* Create(BOOL bDebug);
IMGLAB_API	~ImgLab();
	/*! Blends destination image with source image. The result is pOut = pIn * alpha + (1-alpha)*pOut
	* \param pIn	start pointers to source image of the blending area
	* \param in_stride	bytes-per-line of source image
	* \param pOut	start pointers to destination image where the source will blend with.
	* \param out_stride	bytes-per-line of destination image
	* \param width	pixels of width of the blending area
	* \param height	pixels of height of the blending area
	* \param alpha	blending ratio (0,1)
	* \return IMGLAB_OK if successful.
	*/	
IMGLAB_API	int AlphaBlending3(void* pIn, int in_stride, void* pOut, int out_stride, int width, int height, float alpha);

	/*! Blends destination image with source image. The result is pOut = pIn * x/width + (1-x/width)*pOut
	* \param pIn	start pointers to source image of the blending area
	* \param in_stride	bytes-per-line of source image
	* \param pOut	start pointers to destination image where the source will blend with.
	* \param out_stride	bytes-per-line of destination image
	* \param width	pixels of width of the blending area
	* \param height	pixels of height of the blending area
	* \return IMGLAB_OK if successful.
	*/	
IMGLAB_API	int LinearBlending3(void* pIn, int in_stride, void* pOut, int out_stride, int width, int height);

	/*! Stretch image source on destination
	* \param pSource source image
	* \param rcSrc	area on source image
	* \param pDest	destination image
	* \param rcDest area on destination image
	* \return IMGLAB_OK if successful.
	*/	
IMGLAB_API	int StretchImage(PImage24 pSource, CRect  rcSrc, PImage24 pDest, CRect rcDest);

private:
	ImgLab();
	int Init(BOOL bDebug);
	void Free();
	void*	m_handle;
};
