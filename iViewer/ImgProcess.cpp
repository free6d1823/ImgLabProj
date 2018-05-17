// ImgProcess.cpp: implementation of the ImgProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iViewer.h"
#include "ImgProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImgProcess::ImgProcess()
{

}

ImgProcess::~ImgProcess()
{

}
#define CLIP(x)     min(255, max(0,x))

//convert DIB RGB24 data pSrc to pDest grayscale 8-bit data
BOOL  RGB24ToGray(LPBYTE pDest, LPBYTE pSrc, int width, int height, int nStride)
{
#define GRAY(b,g,r) ( b + g + (g<<2) + (r<<1) )>>3
    int i,j;
    int value;
    LPBYTE pS1;
    LPBYTE pT1; 
    pT1 = pDest;
    for(i = 0; i< height; i++)
    {
        pS1 = pSrc;
        for(j=0;j<width;j++)
        {
            value = GRAY((*pS1), (*(pS1+1)), (*(pS1+2)) );
            *pT1++ = CLIP(value);
            pS1 += 3;
        }
        pSrc += nStride;
    }
    return TRUE;
}
BOOL  LaPlacian(short * pBuffer, LPBYTE pSrc, int width, int height)
{
    int i,j;
//    short * pBuffer = (short *)malloc(width*height*sizeof(short));
    short * pDest = pBuffer;

    LPBYTE pS1;
    LPBYTE pS2;
    LPBYTE pS3;
    //copy first line
    memset(pDest, 0, width*sizeof(short));
    pDest += width;

    for(i = 2; i< height; i++)
    {
        pS1 = pSrc+1;
        pS2 = pS1 + width;
        pS3 = pS2 + width;
        *pDest++ = 0;   //first pixel
        for(j=2;j<width;j++)
        {
            *pDest ++ = (*(pS2)*4) - *(pS2-1) - *(pS2+1) -*(pS1) -*(pS3);
            pS1 ++; pS2++; pS3++;
        }
        *pDest++ = 0; //last pixel
        pSrc += width; 
    }
    //copy last line
    memset(pDest, 0, width*sizeof(short));
    
//    free(pBuffer);
    return TRUE;

}

//set 0 to zero-crossing on pBin, elsewhere set 0xff
//pBin = target binary size is (w*h)
//pBuffer = LaPlacian buffer (w*h*sizeof(short))
BOOL  ZeroCrossing(LPBYTE pBin, short * pBuffer, int width, int height, int nTH)
{
#define IsInverseSign(x,y)      ((x)*(y) < (nTH*nTH*-1))
    int i,j;
//    short * pBuffer = (short *)malloc(width*height*sizeof(short));
    LPBYTE pDest = pBin;
    short * pS1;
    short * pS2;
    short * pS3;

    //fill first line with bkgnd
    memset(pDest, 0xff, width);
    pDest += width;

    for(i = 2; i< height; i++)
    {
        //first pixel
        *pDest ++ = 0xff;

        pS1 = pBuffer+1;
        pS2 = pS1 + width;
        pS3 = pS2 + width;

        for(j=2;j<width;j++)
        {
            *pDest = 0xff;

            if( IsInverseSign( *(pS2-1), *(pS2+1))) *pDest = 0x0;             
            if( IsInverseSign( *(pS1), *(pS3))    )*pDest = 0x0;
//            if(*pS2 > nTH || *pS2 < -nTH)
//                *pDest = 0;
            pDest ++; pS1++; pS2++; pS3++;
        }

        *pDest ++ = 0xff;
        pBuffer += width;
    }
    memset(pDest, 0xff, width);
    return TRUE;
}

//convert 8-bit grayscale to 24-bit RGB dib data
BOOL  GrayToRGB24(LPBYTE pGray, int width, int height, int nStride, LPBYTE pBin)
{
    int i,j;
    LPBYTE pLine = pGray;
    LPBYTE pS = pBin;
    LPBYTE pT;
    for(i = 0; i< height; i++)
    {
        pT = pLine;
        for(j=0;j<width;j++)
        {
            *pT ++ = *pS;
            *pT ++ = *pS;
            *pT ++ = *pS;
            pS++;
        }
        pLine += nStride;
    }
    return TRUE;
}
//convert 16 bit to RGB24, 0=128,
BOOL  Gray16ToRGB24(LPBYTE pGray, int width, int height, int nStride, short* pData)
{
    int i,j;
    LPBYTE pLine = pGray;
    BYTE value;
    LPBYTE pT;
    for(i = 0; i< height; i++)
    {
        pT = pLine;
        for(j=0;j<width;j++)
        {
            value = CLIP(*pData + 128) ;

            *pT ++ = (value>128)?0:value;
            *pT ++ = value;
            *pT ++ = (value<128)? 0:value; //r

            pData++;
        }
        pLine += nStride;
    }
    return TRUE;
}