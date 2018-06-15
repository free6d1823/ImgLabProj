/****************************************************************************
 * Copyright (C) 2018 nFore Technology Co. Inc.,                            *
 *                                                                          *
 ****************************************************************************/
/**
 * \file common.h 
 * \brief This file declars process-related struc and data type
 */
#pragma once

#define CLIP(x)     min(255, max(0,x))
#define  PI		3.14159265358979323846f /* pi */ 
#define  PI_2   1.57079632679489661923f /* pi/2 */ 
	
/**
 * \struct _PointDouble
 * \brief double precision types of position coordinates used in LUT
 */
typedef struct _PointDouble{
	double x;	/*!< x-coordinates */
	double y;	/*!< y-coordinates */
}PointDouble;

/**
 * \enum _MountMode
 * \brief camera mounting position
 */
typedef enum _MountMode{
	MountMode_WALL = 0,			/*!< on the wall, only support this */
	MountMode_CEILING = 1,		/*!< on celling, not support yet */
	MountMode_TABLE = 2			/*!< on table, not support yet */
}MountMode;

/**
 * \enum _LensType
 * \brief defines lens distortion model
 */
typedef enum _LensType{
	LensType_0,		/*!< no correction */
	LensType_1,		/*!< tan(angle/2) */
	LensType_2 ,	/*!< sin(angle/2) */
	LensType_3,		/*!< angle */
	LensType_4,		/*!< sin(angle) */
	LensType_5		/*!< user defined curve */
}LensType;			


/**
 * \struct _FecParam
 * \brief FEC processing parameters.
 */
typedef struct _FecParam{
	SIZE	szInput;		/*!< input image size */
	POINT ptCenter;			/*!< symmetry center of image */
	SIZE	szRadius;		/*!< radius of fisheye lens, in pixels */
	double	pitch;			/*!< pitch angle in radiun */
	double	yaw;				/*!< rotate with respect to y-axis */
	double	roll;			/*!< rotate with respect to z-axis */
	double	k1;			/*!< radical coefficient 1 */
	double	k2;			/*!< radical coefficient 2 */
	double	fov;			/*!< hoizontal FOV factor, 0 to PI*/
	double aspect;		/*!< aspect ratio of fov, width/height*/
	SIZE	szOutput;		/*!< desired output image size */
	MountMode	mm;			/*!< mount type */
	LensType	lt;			/*!< lens type */
}FecParam;


/**
 * \struct _HomoParam
 * \brief Homography Matrix parameters. Q(u,v) = [H] * P(x,y), where P is the vector of original pixels, Q is the vector of transformed pixels.
	(x,y) is normalized to [0~1]
 <table>
<caption id="multi_row">Coefficents</caption>
<tr><th>Q                   <th> [H]   <th> P
<tr><td >u<td>a,b,c<td>x
<tr><td >v<td>d,e,f<td>y
<tr><td >s<td>g,h,1<td>1
</table>

 */
typedef struct _HomoParam{
	SIZE	szInput;		/*!< input image size */
	SIZE	szOutput;		/*!< desired output image size */
	double h[3][3]; /*!< Homography Matrix coef.  */
	POINT	p[4]; /*!< 4 points on source image*/
	PointDouble q[4];		/*!< expected 4 points location after transformation */
}HomoParam;