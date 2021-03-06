#include "StdAfx.h"
#include "Dib2.h"



Dib::Dib ()
	: m_pDibBits ( NULL ), m_pDibInfo ( NULL )
{
	
}
	
Dib::Dib (LPBITMAPINFO pDibInfo, void* pDibBits)
{
	ASSERT(pDibInfo);
		
	m_pDibInfo = pDibInfo; //header and color table
	m_pDibBits = pDibBits;
	
}

void Dib::AttachDibBits(LPBITMAPINFO pDibInfo, void* pDibBits)
{
	if(m_pDibInfo)
		free(m_pDibInfo);
	if(m_pDibBits)
		free(m_pDibBits);

	//return pDib back
	m_pDibBits = pDibBits;
	m_pDibInfo = pDibInfo;
}

void Dib::DetachDibBits()
{
	//return pDib back
	m_pDibBits = NULL;
	m_pDibInfo = NULL;
}

Dib::~Dib ( )
{
	if(m_pDibInfo) free(m_pDibInfo);
	if(m_pDibBits) free(m_pDibBits);


}

BOOL	Dib::Create( int width, int height, int depth, int colorUsed )
{
	BITMAPINFOHEADER *bmheader;
	int PaletteSize = 0;
	int cbHeaderSize;

	if ( colorUsed != 0 ) 
		PaletteSize = colorUsed;
	else if ( depth < 16 )
		PaletteSize = ( 1 << depth );

	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize * sizeof( RGBQUAD );
	if ( m_pDibInfo ) free(m_pDibInfo);
	m_pDibInfo	= (BITMAPINFO*) malloc(cbHeaderSize );
	memset( m_pDibInfo, 0, cbHeaderSize );

	bmheader	= &( m_pDibInfo->bmiHeader );
	bmheader->biSize			= sizeof( BITMAPINFOHEADER ); 
	bmheader->biWidth			= width; 
	bmheader->biHeight			= height; 
	bmheader->biPlanes			= 1; 
	bmheader->biBitCount		= depth; 
	bmheader->biCompression		= BI_RGB; 
	bmheader->biSizeImage		= 0; 
	bmheader->biXPelsPerMeter	= 10000; 
	bmheader->biYPelsPerMeter	= 10000; 
	bmheader->biClrUsed			= PaletteSize; 
	bmheader->biClrImportant	= PaletteSize; 

	int	size = BytesPerLine() * height;
	if(m_pDibBits) free(m_pDibBits);
	m_pDibBits = (void*) malloc( size);
	memset( m_pDibBits, 0, size );
	bmheader->biSizeImage		= size; 

	//create gray pallete
	RGBQUAD* pPal = (RGBQUAD*) ((LPBYTE)m_pDibInfo + sizeof(BITMAPINFOHEADER));
	for(int i=0;i<PaletteSize;i++)
	{
		pPal->rgbBlue = pPal ->rgbGreen = pPal->rgbRed= i;
		pPal ++;

	}
	return TRUE;
}

CPalette* Dib::GetPalette()
{
	int i;
	CPalette *palette;
	LOGPALETTE *log;

	if ( !GetPaletteCount() ) return NULL;

	log = (LOGPALETTE*) new char [ sizeof(WORD)*2 + sizeof(PALETTEENTRY)*GetPaletteCount() ];
	log->palVersion = 0x300;
	log->palNumEntries = WORD(GetPaletteCount());

	for ( i = 0 ; i < log->palNumEntries ; i ++ )
	{
		log->palPalEntry[i].peRed = m_pDibInfo->bmiColors[i].rgbRed;
		log->palPalEntry[i].peGreen = m_pDibInfo->bmiColors[i].rgbGreen;
		log->palPalEntry[i].peBlue = m_pDibInfo->bmiColors[i].rgbBlue;
		log->palPalEntry[i].peFlags = NULL;
	}

	palette = new CPalette;
	palette->CreatePalette( log );
	delete[] log;
	return palette;
}

BOOL	Dib::GetColor( int index, COLORREF &color )
{
	int PaletteSize = 0;

	if ( m_pDibInfo->bmiHeader.biClrUsed != 0 ) 
		PaletteSize = m_pDibInfo->bmiHeader.biClrUsed;
	else if ( m_pDibInfo->bmiHeader.biBitCount < 16 )
		PaletteSize = ( 1 << m_pDibInfo->bmiHeader.biBitCount );

	if ( index < 0 || index >= PaletteSize )
		return FALSE;
	else {
		color = RGB( m_pDibInfo->bmiColors[ index].rgbRed,
					 m_pDibInfo->bmiColors[ index].rgbGreen,
					 m_pDibInfo->bmiColors[ index].rgbBlue );
		return TRUE;
		}
}
BOOL		Dib::GetPixelColor(POINT pt, COLORREF& color)
{
	if(	m_pDibInfo->bmiHeader.biBitCount != 24) {
		TRACE("Not support GetPixelColor for this color format !\n");
		return FALSE;
	}
	if (pt.x >= (int)Width() || pt.y >= (int)Height())
		return FALSE;
	BYTE* offset =  (BYTE*) GetBits() +  ((Height() - pt.y-1)* BytesPerLine() + pt.x * 3);
	color = RGB(offset[2],offset[1], offset[0]);

	return TRUE;
}

BOOL	Dib::SetColor( int index, COLORREF color )
{
	int PaletteSize = 0;

	if ( m_pDibInfo->bmiHeader.biClrUsed != 0 ) 
		PaletteSize = m_pDibInfo->bmiHeader.biClrUsed;
	else if ( m_pDibInfo->bmiHeader.biBitCount < 16 )
		PaletteSize = ( 1 << m_pDibInfo->bmiHeader.biBitCount );

	if ( index < 0 || index >= PaletteSize )
		return FALSE;
	else {
		m_pDibInfo->bmiColors[ index].rgbBlue		= GetBValue( color );
		m_pDibInfo->bmiColors[ index].rgbGreen		= GetGValue( color );
		m_pDibInfo->bmiColors[ index].rgbRed		= GetRValue( color );
		m_pDibInfo->bmiColors[ index].rgbReserved	= 0;
		return TRUE;
		}
}

BOOL Dib::CopyTo ( CBitmap& ddb )
{
	BOOL ret;
	if ( !m_pDibInfo ) return FALSE;
	// copy DIB to DDB //
	// generate DC of desktop window //
	CWnd DesktopWnd;
	DesktopWnd.Attach ( ::GetDesktopWindow() );
	CWindowDC dtpDC ( &DesktopWnd );
	// use SetDIBits to convert DIB to DDB //

	ret = ::SetDIBits( dtpDC.m_hDC,	// handle of device context
						HBITMAP (ddb),	// handle of bitmap
						0,	// starting scan line
						m_pDibInfo->bmiHeader.biHeight,	// number of scan lines
						m_pDibBits,	// array of bitmap bits
						m_pDibInfo,	// address of structure with bitmap data
						DIB_RGB_COLORS	// type of color indices to use
				 );
	DesktopWnd.Detach();
    return ret;
}

int Dib::SetToDC ( CDC& dc, CRect& src, CPoint& dst )
{
	if ( !m_pDibInfo ) return FALSE;

	return ::SetDIBitsToDevice(
				dc.m_hDC,	// handle of device context
				dst.x,	// x-coordinate of upper-left corner of dest. rect. 
				dst.y,	// y-coordinate of upper-left corner of dest. rect. 
				src.Width(),	// source rectangle width 
				src.Height(),	// source rectangle height 
				src.left,	// x-coordinate of lower-left corner of source rect. 
				src.top,	// y-coordinate of lower-left corner of source rect. 
				0,	// first scan line in array 
				m_pDibInfo->bmiHeader.biHeight,	// number of scan lines 
				m_pDibBits,	// address of array with DIB bits 
				m_pDibInfo,	// address of structure with bitmap info. 
				DIB_RGB_COLORS	// RGB or palette indices 
			);	
}

int Dib::StretchToDC ( CDC& dc, RECT& src, RECT& dst, DWORD rop )
{
	if ( !m_pDibInfo ) return FALSE;
	dc.SetStretchBltMode(STRETCH_DELETESCANS);
	return ::StretchDIBits (
				dc.m_hDC,	// handle of device context
				dst.left,	// x-coordinate of upper-left corner of dest. rect. 
				dst.top,	// y-coordinate of upper-left corner of dest. rect. 
				dst.right - dst.left,	// width of destination rectangle 
				dst.bottom - dst.top,	// height of destination rectangle 
				src.left,	// x-coordinate of lower-left corner of source rect. 
				Height() - src.bottom,	// y-coordinate of lower-left corner of source rect. 
				src.right - src.left,	// source rectangle width 
				src.bottom - src.top,	// source rectangle height 
				m_pDibBits,	// address of array with DIB bits 
				m_pDibInfo,	// address of structure with bitmap info. 
				DIB_RGB_COLORS,	// RGB or palette indices 
				rop		// raster operation code
			);	
}

int Dib::GetPaletteCount ()
{
	int PaletteSize = 0;
	if (!m_pDibInfo) return 0;
	switch ( m_pDibInfo->bmiHeader.biBitCount )
	{
		case 1:
			PaletteSize = 2;
			break;
		case 4:
			PaletteSize = 16;
			break;
		case 8:
			PaletteSize = 256;
			break;
	}
	return PaletteSize;
}


BOOL Dib::ReadBmp(LPCTSTR szFileName)
{
	BOOL ret;
	CFile fp ( szFileName, CFile::modeRead | CFile::typeBinary );
	ret = DoRead ( fp );
	fp.Close();
	return ret;
}

BOOL Dib::DoRead ( CFile& file )
{

	return TRUE;
}

BOOL Dib::WriteBmp(LPCTSTR szFileName)
{
	BOOL ret;
	CFile fp ( szFileName, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate );
	ret = DoWrite ( fp );
	fp.Close();
	return ret;
}

BOOL Dib::DoWrite( CFile& file )
{
	BITMAPFILEHEADER bmfileheader;
	long size, headpos;
	int PaletteSize = 0;
	int cbHeaderSize;

	bmfileheader.bfType			= 0x4d42;	// 'BM'
	bmfileheader.bfReserved1	= 0;
	bmfileheader.bfReserved2	= 0;

	headpos = (long) file.GetPosition();
	file.Seek( sizeof(BITMAPFILEHEADER), CFile::current );

	if ( m_pDibInfo->bmiHeader.biClrUsed != 0 ) 
		PaletteSize = m_pDibInfo->bmiHeader.biClrUsed;
	else if ( m_pDibInfo->bmiHeader.biBitCount < 16 )
		PaletteSize = ( 1 << m_pDibInfo->bmiHeader.biBitCount );

	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize * sizeof( RGBQUAD );

	file.Write( m_pDibInfo, cbHeaderSize );

	bmfileheader.bfOffBits = (long) file.GetPosition() - headpos;

	size = BytesPerLine() * m_pDibInfo->bmiHeader.biHeight;
	file.Write( m_pDibBits, size );

	bmfileheader.bfSize = cbHeaderSize + size + sizeof( bmfileheader );
	file.Seek( headpos, CFile::begin );
	file.Write( &bmfileheader, sizeof( bmfileheader ));

	return TRUE;
}

void Dib::InitDibInfo( int BitsPerPixel, int w, int h )
{
	int i;
	int PaletteSize = 0, cbHeaderSize;

	switch ( BitsPerPixel )	
	{
		case 1:
			PaletteSize = 2;
			break;
		case 4:
			PaletteSize = 16;
			break;
		case 8:
			PaletteSize = 256;
			break;
		case 15:
		case 16:
			BitsPerPixel = 16;
			break;
	}

	cbHeaderSize = sizeof(BITMAPINFOHEADER) + PaletteSize * sizeof ( RGBQUAD );
	if ( m_pDibInfo ) delete[] m_pDibInfo;
	m_pDibInfo = (BITMAPINFO*) new char [ cbHeaderSize ]; /** [6] **/
	m_pDibInfo->bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );
	m_pDibInfo->bmiHeader.biWidth = w;
	m_pDibInfo->bmiHeader.biHeight = h;
	m_pDibInfo->bmiHeader.biPlanes = 1;
	m_pDibInfo->bmiHeader.biBitCount = short(BitsPerPixel);
	m_pDibInfo->bmiHeader.biCompression = BI_RGB;
	m_pDibInfo->bmiHeader.biSizeImage = BytesPerLine() * m_pDibInfo->bmiHeader.biHeight;
	m_pDibInfo->bmiHeader.biXPelsPerMeter =
	m_pDibInfo->bmiHeader.biYPelsPerMeter = 120;
	m_pDibInfo->bmiHeader.biClrUsed = PaletteSize;
	m_pDibInfo->bmiHeader.biClrImportant = PaletteSize;

	if ( PaletteSize )
	{
		for ( i = 0 ; i < PaletteSize ; i ++ )
		{
			m_pDibInfo->bmiColors[i].rgbRed = 0;
			m_pDibInfo->bmiColors[i].rgbGreen = 0;
			m_pDibInfo->bmiColors[i].rgbBlue = 0;
			m_pDibInfo->bmiColors[i].rgbReserved = 0;
		}
	}
}


int Dib::GetHeaderSize()
{
	int PaletteSize;
	switch ( m_pDibInfo->bmiHeader.biBitCount )
	{
		case 1:
			PaletteSize = 2 * sizeof ( RGBQUAD ) + sizeof(BITMAPINFOHEADER);
			break;
		case 4:
			PaletteSize = 16 * sizeof ( RGBQUAD ) + sizeof(BITMAPINFOHEADER);
			break;
		case 8:
			PaletteSize = 256 * sizeof ( RGBQUAD ) + sizeof(BITMAPINFOHEADER);
			break;
		default:
			PaletteSize = sizeof(BITMAPINFOHEADER);
			break;
	}
	return PaletteSize;
}
