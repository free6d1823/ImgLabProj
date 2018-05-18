   // ImgFile.h: interface for the ImgFile class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "DIB2.H"

typedef enum tagIMGFORMAT
{
	Img_OK = 0,
	Img_OutOfMemory = -1,
	Img_InvalideImageFile,
	Img_UnsupportedFormat,
	Img_DllNotFound,
	Img_WrongDll,
	Img_DecodeError,
	Img_FileCrupt,
	Img_UnknownError,
}	IMGERRORS;
// for progress func
typedef void (*ImgProgressProc)(int nOpCode, int nParam);

// for abort func
typedef BOOL (*ImgAbortProc)(void);

typedef enum _tagIMGFORMAT
{
	Image_Format_Memory,
	Image_Format_DIB,
	Image_Format_BMP,
	Image_Format_TIF,
	Image_Format_PCX,
	Image_Format_GIF,
	Image_Format_TGA,
	Image_Format_JPG,
	Image_Format_J6I,
	Image_Format_PCD,
	Image_Format_AVI,
	Image_Format_WMF,
	Image_Format_Unknown

}	IMG_FORMAT;

typedef struct _SaveAs{
	LPSTR	pFileName;
	IMG_FORMAT	eFormat;
	int			nQuality;
	LPSTR		pComment;
}SAVEAS_PARAM;

class ImgFile  : public Dib
{
public:
IMGLAB_API	ImgFile();
IMGLAB_API	virtual ~ImgFile();

IMGLAB_API	int Load(LPCTSTR szFile, ImgProgressProc pfnProgress = NULL, ImgAbortProc pfnAbort = NULL);
IMGLAB_API	int Save(ImgProgressProc pfnProgress = NULL, ImgAbortProc pfnAbort = NULL);
IMGLAB_API	int SaveAs(SAVEAS_PARAM& saveas, ImgProgressProc pfnProgress = NULL, ImgAbortProc pfnAbort = NULL);
IMGLAB_API	int CopyFrom(ImgFile* pSource);

IMGLAB_API	static	IMG_FORMAT ReadFileFormat(FILE* pFile);
private:
	void FreePrivateData();
	void ReadBmp(FILE* pf);
	void ReadJpg(FILE* pf);
	void SaveBmp(SAVEAS_PARAM& saveas);
	void SaveJpg(SAVEAS_PARAM& saveas);
	void	FreeParams();
	SAVEAS_PARAM	m_FileParam;

};

