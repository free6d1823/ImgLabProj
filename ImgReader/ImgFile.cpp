// ImgFile.cpp: implementation of the ImgFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImgFile.h"
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImgFile::ImgFile() : CDib()
{

	memset(&m_FileParam, 0 , sizeof(m_FileParam));
}

void	ImgFile::FreeParams()
{
	if(m_FileParam.pFileName)
		free(m_FileParam.pFileName);
	if(m_FileParam.pComment)
		free(m_FileParam.pComment);
	memset(&m_FileParam, 0 , sizeof(m_FileParam));
}
ImgFile::~ImgFile()
{
	FreeParams();
}

int ImgFile::Load(LPCTSTR szFile, ImgProgressProc pfnProgress, ImgAbortProc pfnAbort)
{

	FILE*	pFile = NULL;
	int		nErr = Img_OK;
	try{
		if((pFile = fopen( (const char*) szFile, "r+b")) == NULL)
			throw Img_InvalideImageFile;

		FreeParams();
		m_FileParam.pFileName = _strdup((const char*) szFile);

		m_FileParam.eFormat = ReadFileFormat(pFile);

		if(m_FileParam.eFormat == Image_Format_Unknown)
			throw Img_UnsupportedFormat;
		switch(m_FileParam.eFormat)
		{
		case Image_Format_BMP:
			ReadBmp(pFile);
			break;
		case Image_Format_JPG:
			ReadJpg(pFile);
			break;
		}

	}
	catch(int err)
	{
		nErr = err;
	}
	catch(...)
	{
		nErr = Img_UnknownError;
	}
	if(pFile)
		fclose(pFile);
	return nErr;
}
IMG_FORMAT ImgFile::ReadFileFormat(FILE* pFile)
{
	IMG_FORMAT	eImgFormat = Image_Format_Unknown;
    BYTE    abyID[4];
	long	pos;

	try{
	
		pos = ftell(pFile);	// Save file position
		if(sizeof(BYTE) != fread(abyID, sizeof(abyID), sizeof(BYTE), pFile))
			throw 1;

		if ((abyID[0] & abyID[1]) == 'I' || (abyID[0] & abyID[1]) == 'M')	// TIF
			eImgFormat = Image_Format_TIF;
		else if (abyID[0] == 'B' && abyID[1] == 'M')						// BMP
			eImgFormat = Image_Format_BMP;
		else if (abyID[0] == 0x0A &&										// PCX
				 (abyID[1] == 0 || abyID[1] == 2 || abyID[1] == 3 ||
				  abyID[1] == 4 || abyID[1] == 5) &&
				 abyID[2] == 1)
			eImgFormat = Image_Format_PCX;
		else if (abyID[0] == 'G' && abyID[1] == 'I' && abyID[2] == 'F')     // GIF
			eImgFormat = Image_Format_GIF;
		else if (abyID[2] == 0x01 || abyID[2] == 0x02 || abyID[2] == 0x03 ||
				 abyID[2] == 0x09 || abyID[2] == 0x0a || abyID[2] == 0x0b)  // TARGA
			eImgFormat = Image_Format_TGA;
		else if (abyID[0] == 0xFF && abyID[1] == 0xD8 && abyID[2] == 0xFF)  // JPEG
		{
			eImgFormat = Image_Format_JPG;
		}
		else
		{
			eImgFormat = Image_Format_Unknown;
		}

	}
	catch(...)
	{
		
	}
	// Restore file pointer
	fseek(pFile, pos, SEEK_SET);
    return eImgFormat;
}
void ImgFile::ReadBmp(FILE* pf)
{
	BITMAPFILEHEADER bmfileheader;
	BITMAPINFOHEADER bmheader;
	unsigned long size, headpos;
	int PaletteSize = 0;
	int cbHeaderSize;

	headpos	= ftell(pf);	// Save file position
	if(1 != fread(&bmfileheader,sizeof(BITMAPFILEHEADER),1, pf))
		throw Img_UnsupportedFormat;

	if ( bmfileheader.bfType != 0x4d42 )
		throw Img_UnsupportedFormat;

	if(1 != fread(&bmheader,sizeof(BITMAPINFOHEADER),1, pf))
		throw Img_UnsupportedFormat;

	if ( bmheader.biClrUsed != 0 ) 
		PaletteSize = bmheader.biClrUsed;
	else if ( bmheader.biBitCount < 16 )
		PaletteSize = ( 1 << bmheader.biBitCount );

	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize*sizeof ( RGBQUAD );
	BITMAPINFO* pDibInfo = (BITMAPINFO*) malloc(cbHeaderSize);
	if(!pDibInfo)
	{
		throw Img_OutOfMemory;			
	}
	pDibInfo->bmiHeader = bmheader;

	if ( PaletteSize )
	{	
		if(1  != fread(
			&(pDibInfo->bmiColors[0]),PaletteSize*sizeof ( RGBQUAD ), 1, pf))
		{
			free( pDibInfo);
			throw Img_UnsupportedFormat;			
		}
	}
	DWORD BytesPerLine = (pDibInfo->bmiHeader.biWidth * pDibInfo->bmiHeader.biBitCount+31)>>5<<2;

	size = BytesPerLine * pDibInfo->bmiHeader.biHeight;
	char * pDibBits = (char*) malloc(size);
	if(! pDibBits)
	{
		free( pDibInfo);
		throw Img_OutOfMemory;
	}
	fseek(pf, headpos + bmfileheader.bfOffBits, SEEK_SET);
	
	if(fread(pDibBits, size, 1, pf )!= 1)
	{
		free( pDibInfo);
		free(pDibBits);
		throw Img_FileCrupt;
	}

	AttachDibBits(pDibInfo,pDibBits);
}

int ImgFile::Save(ImgProgressProc pfnProgress, ImgAbortProc pfnAbort)
{

	return SaveAs(m_FileParam, pfnProgress,pfnAbort);
}
int ImgFile::SaveAs(SAVEAS_PARAM& saveas, ImgProgressProc pfnProgress, ImgAbortProc pfnAbort)
{
	int		nErr = Img_OK;
	try{
	
		switch(saveas.eFormat)
		{
			case Image_Format_BMP:
				SaveBmp(saveas);
				break;
			case Image_Format_JPG:
				SaveJpg(saveas);
				break;
			default:
				throw Img_UnsupportedFormat;
		}
	
	}
	catch(int err)
	{
		nErr = err;
	}
	return nErr;

}
int ImgFile::CopyFrom(ImgFile* pSource)
{
	if(m_pDibInfo) free(m_pDibInfo);
	m_pDibInfo = NULL;
	if(m_pDibBits) {
		free(m_pDibBits);
		m_pDibInfo = NULL;
	}
	if(!Create(pSource->Width(), pSource->Height(), 
			pSource->GetBitsPerPixel(),
			pSource->GetPaletteCount()))
		return Img_InvalideImageFile;
	BYTE* pBits = (BYTE*) GetBits();
	memcpy(pBits, pSource->GetBits(), pSource->GetBodySize());

	return Img_OK;

}

void ImgFile::SaveBmp(SAVEAS_PARAM& saveas)
{
}
void ImgFile::SaveJpg(SAVEAS_PARAM& saveas)
{
}

void ImgFile::FreePrivateData()
{

}
