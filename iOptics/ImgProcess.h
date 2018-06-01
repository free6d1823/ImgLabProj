// ImgProcess.h: interface for the ImgProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGPROCESS_H__1020ABC3_6A82_484F_A0B6_BE0D0B3943A0__INCLUDED_)
#define AFX_IMGPROCESS_H__1020ABC3_6A82_484F_A0B6_BE0D0B3943A0__INCLUDED_

//convert DIB RGB24 data pSrc to pDest grayscale 8-bit data
BOOL  RGB24ToGray(LPBYTE pDest, LPBYTE pSrc, int width, int height, int nStride);
BOOL  LaPlacian(short * pBuffer, LPBYTE pSrc, int width, int height);
BOOL  ZeroCrossing(LPBYTE pBin, short * pBuffer, int width, int height, int nTH);
BOOL  GrayToRGB24(LPBYTE pGray, int width, int height, int nStride, LPBYTE pBin);
BOOL  Gray16ToRGB24(LPBYTE pGray, int width, int height, int nStride, short* pData);

#endif // !defined(AFX_IMGPROCESS_H__1020ABC3_6A82_484F_A0B6_BE0D0B3943A0__INCLUDED_)
