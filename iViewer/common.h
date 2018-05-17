#pragma once

// FEC definitions
typedef enum _MountMode{
	MountMode_WALL = 0,
	MountMode_CEILING = 1,
	MountMode_TABLE = 2
}MountMode;
typedef enum _LensType{
	LensType_0,		//no correction
	LensType_1,		//tan(angle/2)
	LensType_2 ,	//sin(angle/2)
	LensType_3,		//angle
	LensType_4,		//sin(angle)
	LensType_5		//user defined curve
}LensType;
typedef struct _FecParam{
	SIZE	szInput;	//input image size
	POINT ptCenter; //symmetry center of image
	SIZE	szRadius;	//radius of fisheye lens, in pixels
	float	pitch;//pitch angle in radiun
	float	yaw; //rotate with respect to y-axis
	float	roll; //rotate with respect to z-axis
	SIZE	szOutput; //desired output image size
	MountMode	mm;
	LensType	lt;
}FecParam;