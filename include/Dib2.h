#pragma once


#ifdef IMGLAB_EXPORTS
#define IMGLAB_API __declspec(dllexport)
#else
#define IMGLAB_API __declspec(dllimport)
#endif


class Dib
{
public:

IMGLAB_API	Dib (LPBITMAPINFO pDibInfo, void* pDibBits);
IMGLAB_API	Dib();
IMGLAB_API	~Dib ( );
IMGLAB_API	void DetachDibBits();
IMGLAB_API	void AttachDibBits(LPBITMAPINFO pDibInfo, void* pDibBits);

IMGLAB_API	BOOL Create( int width, int height, int depth, int colorUsed = 0 );
IMGLAB_API	BOOL ReadBmp(LPCTSTR szFileName);
IMGLAB_API	BOOL WriteBmp(LPCTSTR szFileName);

IMGLAB_API	void*		GetBits();
IMGLAB_API	inline BITMAPINFO* Dib::GetDibInfo() { return m_pDibInfo; }
IMGLAB_API	BOOL		GetColor( int index, COLORREF &color );
IMGLAB_API	BOOL		SetColor( int index, COLORREF color );
IMGLAB_API	BOOL		GetPixelColor(POINT pt, COLORREF& color);

IMGLAB_API	int			GetBitsPerPixel ( );
IMGLAB_API	BOOL		GetDimension ( CSize& size );
IMGLAB_API    DWORD		Width();
IMGLAB_API    DWORD		Height();
IMGLAB_API	int			GetPaletteCount ();
IMGLAB_API	DWORD		BytesPerLine();
IMGLAB_API	CPalette*	GetPalette();

IMGLAB_API	int			StretchToDC ( CDC& dc, RECT& src, RECT& dst, DWORD rop = SRCCOPY );
IMGLAB_API	int			SetToDC ( CDC& dc, CRect& src, CPoint& dst );
IMGLAB_API	BOOL		CopyTo ( CBitmap& );

IMGLAB_API	BOOL		DoRead ( CFile& file );
IMGLAB_API	BOOL		DoWrite( CFile& file );

IMGLAB_API	long		GetBodySize();


protected:
	int			GetHeaderSize();
	void		InitDibInfo ( int , int = 0, int = 0 );

	BITMAPINFO*	m_pDibInfo;
	void*		m_pDibBits;

};

inline int Dib::GetBitsPerPixel () { return ( !m_pDibInfo ) ? 0 : m_pDibInfo->bmiHeader.biBitCount; }
inline void* Dib::GetBits() { return m_pDibBits; }
inline DWORD Dib::Width() { return m_pDibInfo->bmiHeader.biWidth; }
inline DWORD Dib::Height() { return m_pDibInfo->bmiHeader.biHeight; }
inline long Dib::GetBodySize() { return BytesPerLine() * m_pDibInfo->bmiHeader.biHeight; }
inline DWORD Dib::BytesPerLine() 
{ 
	DWORD bytes_per_line;
	// fillup byte //
    bytes_per_line = ( m_pDibInfo->bmiHeader.biWidth * GetBitsPerPixel()+7)/8;
	// quad-byte alignment //
    bytes_per_line = ( bytes_per_line + 3 ) / 4;
    return bytes_per_line*4;
}

inline BOOL Dib::GetDimension ( CSize& size )
{
	if ( !m_pDibInfo ) return FALSE;	
	size.cx = m_pDibInfo->bmiHeader.biWidth;
	size.cy = m_pDibInfo->bmiHeader.biHeight;
	return TRUE;
}