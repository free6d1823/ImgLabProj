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
	float	pitch;			/*!< pitch angle in radiun */
	float	yaw;				/*!< rotate with respect to y-axis */
	float	roll;			/*!< rotate with respect to z-axis */
	float	ratio;			/*!< ratio of remap from small sphere to big sphere	 */
	float	curveH;			/*!< curvature coefficient of hoizontal, upper or lower */
	float	curveV;			/*!< curvature coefficient of vertical, lefter or righter */
	SIZE	szOutput;		/*!< desired output image size */
	MountMode	mm;			/*!< mount type */
	LensType	lt;			/*!< lens type */
}FecParam;