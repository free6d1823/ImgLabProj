 #include "stdafx.h"
#include "ImgFile.h"
#include <stdio.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//
#include <setjmp.h>     /* jmp_buf, setjmp, longjmp */
#include "libjpeg/include/jpeglib.h"
////
inline long long PerformanceCounter() 
{
    LARGE_INTEGER li;
    ::QueryPerformanceCounter(&li);
    return li.QuadPart;
}

inline long long PerformanceFrequency() 
{
    LARGE_INTEGER li;
    ::QueryPerformanceFrequency(&li);
    return li.QuadPart;
}


/////////////////////
struct my_error_mgr {
	struct jpeg_error_mgr pub;    /* "public" fields */
	jmp_buf setjmp_buffer;        /* for return to caller */
};
typedef struct my_error_mgr *my_error_ptr;

static void my_error_exit(j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

void ImgFile::ReadJpg(FILE* pf)
{
	LPBITMAPINFO pDibInfo = NULL;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr2;
	int row_stride;							/* physical row width in output buffer */

	//test performance
	long long  freq = PerformanceFrequency();
	long long tmStart = PerformanceCounter();
	//===================================
	/* Step 1: allocate and initialize JPEG decompression object */
 	memset(&cinfo,0,sizeof(struct jpeg_decompress_struct));
	cinfo.err = jpeg_std_error(&jerr2.pub);
	 jerr2.pub.error_exit = my_error_exit;
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr2.setjmp_buffer)) {
		/* If we get here, the JPEG code has signaled an error.
		* We need to clean up the JPEG object, close the input file, and return.
		*/
		jpeg_destroy_decompress(&cinfo);
		throw Img_DllNotFound;
		return;
	 }
	/* Now we can initialize the JPEG decompression object. */

	jpeg_CreateDecompress(&cinfo, JPEG_LIB_VERSION, (size_t)sizeof(struct jpeg_decompress_struct) );

	/* Step 2: specify data source (eg, a file) */

	jpeg_stdio_src(&cinfo, pf);

		/* Step 3: read file parameters with jpeg_read_header() */
		(void)jpeg_read_header(&cinfo, TRUE);
		/* We can ignore the return value from jpeg_read_header since
		*   (a) suspension is not possible with the stdio data source, and
		*   (b) we passed TRUE to reject a tables-only JPEG file as an error.
		* See libjpeg.txt for more info.
		*/
		/* Step 4: set parameters for decompression */
		/* Step 5: Start decompressor */
		cinfo.out_color_space = JCS_BGR;
		
		(void)jpeg_start_decompress(&cinfo);
		/* We can ignore the return value since suspension is not possible
		* with the stdio data source.
		*/

		/* JSAMPLEs per row in output buffer */
		row_stride = cinfo.output_width * cinfo.output_components;
		/* Make a one-row-high sample array that will go away when done with image */
//		buffer = (*cinfo.mem->alloc_sarray)
//                ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);


		int	size = row_stride *  cinfo.output_height;

	BYTE* imageData = (BYTE*) malloc( size);
//	BYTE* pDest = imageData + (cinfo.output_height-1)*row_stride;
JSAMPROW pDest = (JSAMPROW) (imageData + (cinfo.output_height-1)*row_stride);
	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */
	while (cinfo.output_scanline < cinfo.output_height) {
	/* jpeg_read_scanlines expects an array of pointers to scanlines.
	 * Here the array is only one element long, but you could ask for
	 * more than one scanline at a time if that's more convenient.
	 */

//			(void)jpeg_read_scanlines(&cinfo, buffer, 1);
			/* Assume put_scanline_someplace wants a pointer and sample count. */
//			memcpy(pDest,  buffer[0], row_stride);

			(void)jpeg_read_scanlines(&cinfo, &pDest, 1);

			pDest -= row_stride;
			//put_scanline_someplace(buffer[0], row_stride);
	}

	/* Step 7: Finish decompression */

	(void)jpeg_finish_decompress(&cinfo);

	/* Step 8: Release JPEG decompression object */
	jpeg_destroy_decompress(&cinfo);

		//
		pDibInfo = (LPBITMAPINFO)malloc(sizeof(BITMAPINFO));
	    BITMAPINFOHEADER& bih = pDibInfo->bmiHeader;

		memset( &bih, 0, sizeof( BITMAPINFOHEADER ) );
		bih.biSize        = sizeof( BITMAPINFOHEADER );
		bih.biWidth       = cinfo.output_width;
		bih.biHeight      = cinfo.output_height;
		bih.biCompression = BI_RGB;
		bih.biPlanes      = 1;

		switch(cinfo.output_components)
		{
		case 3:
		  bih.biBitCount = 24;
		  break;

		case 4:
		  bih.biBitCount = 32;
		  break;

		default:
		  TRACE("Unsupported number of channels!\n");
		  break;
		}

		AttachDibBits(pDibInfo,imageData);

		long long tmEnd = PerformanceCounter();
		double elapsedMilliseconds = ((tmEnd - tmStart) * 1000.0) / freq;

		TRACE("--- Take %f ms \n", elapsedMilliseconds);
}
