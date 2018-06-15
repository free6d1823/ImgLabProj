#include "stdafx.h"
#include <vector>
#include <math.h>
#include "ImgLab.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_FRAME_SIZE 1024*1024*3

#define PRINTF(x) {if(bEnableLog)fprintf(fp, x);}
#define PRINTF2(x,y) {if(bEnableLog)fprintf(fp, x,y);}
#define KERNEL(...)#__VA_ARGS__

typedef struct CL_STRUCT
{
	cl_program program;
	cl_command_queue queue;
	cl_context context;
	cl_mem cl_in;
	cl_mem cl_out;
}*PCL_STRUCT;


const char* source = KERNEL(

__kernel void clAlphaBlending(__global const unsigned char* in,
						  const int in_stride, 
						  __global unsigned char* out,
						  const int out_stride, 
						 float alpha)
{
    int x = get_global_id(0); 
	int y = get_global_id(1);
	int out_width = get_global_size(0);
	int out_height = get_global_size(1);
	float p0, p1;
	// B
	int offset_in = in_stride *y + x*3;
	int offset_out= out_stride*y+x*3;
	p0 = in[offset_in];
	p1 = out[offset_out];
	p1 = p0* alpha + p1*(1-alpha);
	out[offset_out] = p1; 

	// G
	p0 = in[offset_in+1];
	p1 = out[offset_out+1];
	p1 = p0* alpha + p1*(1-alpha);
	out[offset_out+1] = p1; 
	//R
	p0 = in[offset_in+2];
	p1 = out[offset_out+2];
	p1 = p0* alpha + p1*(1-alpha);
	out[offset_out+2] = p1;  
}

__kernel void clLinearBlending(__global const unsigned char* in,
						  const int in_stride, 
						  __global unsigned char* out,
						  const int out_stride)
{
    int x = get_global_id(0); 
	int y = get_global_id(1);
	int out_width = get_global_size(0);
	int out_height = get_global_size(1);
	float p0, p1;
	// B
	int offset_in = in_stride *y + x*3;
	int offset_out= out_stride*y+x*3;
	float alpha = (float)x /(float)out_width;
	p0 = in[offset_in];
	p1 = out[offset_out];
	p1 = p0* alpha + p1*(1-alpha);
	out[offset_out] = p1; 

	// G
	p0 = in[offset_in+1];
	p1 = out[offset_out+1];
	p1 = p0* alpha + p1*(1-alpha);
	out[offset_out+1] = p1; 
	//R
	p0 = in[offset_in+2];
	p1 = out[offset_out+2];
	p1 = p0* alpha + p1*(1-alpha);
	out[offset_out+2] = p1;  
}

__kernel void clStretchImage(__global const unsigned char* in,
						  const int in_stride,  const int in_width,  const int in_height,
						  __global unsigned char* out,
						  const int out_stride
						  )
{
	/* out coordinates */
    int x = get_global_id(0); 
	int y = get_global_id(1);

	int out_width = get_global_size(0);
	int out_height = get_global_size(1);
	/* in coordinates */
	float u = (float)x * in_width/ (float)out_width;
	float v = (float)y * in_height/ (float)out_height;
	
	int u0 = (int) u;
	int u1 = u0+1;
	int v0 = (int) v;
	int v1 = v0+1;
	float fu = u - (float) u0;
	float fv = v - (float) v0;

	float p0,p1,p2,p3,p01,p23;
	// B
	p0 = in[3*(v0 * in_width + u0)];
	p1 = in[3*(v0 * in_width + u1)];
	p2 = in[3*(v1 * in_width + u0)];
	p3 = in[3*(v1 * in_width + u1)];
	p01 = p0 + fu*(p1-p0);
	p23 = p2 + fu*(p3-p2);
	out[3*(y*out_width + x)] = (p01 + fv*(p23-p01));
	// G
	p0 = in[3*(v0 * in_width + u0)+1];
	p1 = in[3*(v0 * in_width + u1)+1];
	p2 = in[3*(v1 * in_width + u0)+1];
	p3 = in[3*(v1 * in_width + u1)+1];
	p01 = p0 + fu*(p1-p0);
	p23 = p2 + fu*(p3-p2);
	out[3*(y*out_width + x)+1] = (p01 + fv*(p23-p01));
	// R
	p0 = in[3*(v0 * in_width + u0)+2];
	p1 = in[3*(v0 * in_width + u1)+2];
	p2 = in[3*(v1 * in_width + u0)+2];
	p3 = in[3*(v1 * in_width + u1)+2];
	p01 = p0 + fu*(p1-p0);
	p23 = p2 + fu*(p3-p2);
	out[3*(y*out_width + x)+2] = (p01 + fv*(p23-p01));


}

); 


ImgLab::ImgLab()
{

}
ImgLab::~ImgLab()
{
	Free();
}
ImgLab* ImgLab::Create(BOOL bDebug)
{
	ImgLab* pClass = new ImgLab();
	if (pClass->Init(bDebug) != CL_SUCCESS)
	{
		delete pClass;
		pClass = NULL;
	}
	return pClass;
}

void GetErrorLog(cl_program program, cl_device_id device_id, FILE *f)
{
	char* programLog;
	cl_build_status status;
	size_t logSize;
	cl_int err = CL_SUCCESS;
 
	err = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_STATUS,
		sizeof(cl_build_status), &status, NULL);
	if (err != CL_SUCCESS) {
		fprintf(f, "clGetProgramBuildInfo  CL_PROGRAM_BUILD_STATUS faile, error=%d\n", err);
		return;
	}
	// check build log
	err = clGetProgramBuildInfo(program, device_id,
		CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
	if (err != CL_SUCCESS) {
		fprintf(f, "clGetProgramBuildInfo  CL_PROGRAM_BUILD_LOG faile, error=%d\n", err);
		return;
	}
	programLog = (char*)malloc((logSize + 1) * sizeof(char));
	clGetProgramBuildInfo(program, device_id,
		CL_PROGRAM_BUILD_LOG, logSize + 1, programLog, NULL);
	fprintf(f, "Build Info:\nstatus = %d\n%s\n",
		status, programLog);
	free(programLog);
 
}

int ImgLab::Init(BOOL bEnableLog)
{
	cl_int err = CL_SUCCESS;
	cl_uint num;

	m_handle =malloc(sizeof(CL_STRUCT));

	PCL_STRUCT pCL = (PCL_STRUCT) m_handle;
	memset(pCL, 0, sizeof(CL_STRUCT));

	FILE* fp = NULL;
	if (bEnableLog) {
		err = fopen_s(&fp, "CL_init_log.txt", "w");
		if (err != 0) {
			bEnableLog = false;
		}
		fprintf(fp, "OpenCL_Init ---\n");
	}

	do {
		err = clGetPlatformIDs(0, 0, &num);
		if (err != CL_SUCCESS) {
			PRINTF("OpenCL err in clGetPlatformIDs\n");
			break;
		}
		std::vector<cl_platform_id> platforms(num);
		err = clGetPlatformIDs(num, &platforms[0], &num);
		if (err != CL_SUCCESS) {

			PRINTF2("err in clGetPlatformIDs - %d\n", num);
			break;
		}

		cl_context_properties prop[] = { CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platforms[num - 1]), 0 };
		pCL->context = clCreateContextFromType(prop, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
		if (pCL->context == 0) {
			PRINTF("OpenCL error in clCreateContextFromType\n");
			break;
		}

		size_t cb;
		clGetContextInfo(pCL->context, CL_CONTEXT_DEVICES, 0, NULL, &cb);
		std::vector<cl_device_id> devices(cb / sizeof(cl_device_id));
		clGetContextInfo(pCL->context, CL_CONTEXT_DEVICES, cb, &devices[0], 0);

		clGetDeviceInfo(devices[0], CL_DEVICE_NAME, 0, NULL, &cb);
		std::string devname;
		devname.resize(cb);
		clGetDeviceInfo(devices[0], CL_DEVICE_NAME, cb, &devname[0], 0);
		PRINTF2("OpenCL device[0] %s found\n", devname.c_str());

		if ((pCL->cl_in = clCreateBuffer(pCL->context, CL_MEM_READ_ONLY, MAX_FRAME_SIZE, 0, &err)) == 0)
		{
			PRINTF("clCreateBuffer input buffer err\n");
			break;
		}
		if ((pCL->cl_out = clCreateBuffer(pCL->context, CL_MEM_WRITE_ONLY, MAX_FRAME_SIZE, 0, &err)) == 0)
		{
			PRINTF("clCreateBuffer output buffer err\n");
			break;
		}


		pCL->queue = clCreateCommandQueue(pCL->context, devices[0], 0, &err);
		if (pCL->queue == 0) {
			PRINTF("clCreateCommandQueue error\n");
			break;
		}

		pCL->program = clCreateProgramWithSource(pCL->context, 1, &source, 0, &err);
		if (pCL->program == 0) {
			PRINTF("clCreateProgramWithSource error\n");
			break;
		}

		err = clBuildProgram(pCL->program, 0,	0, 0, 0, 0);
		GetErrorLog(pCL->program, devices[0], fp);
		if (err != CL_SUCCESS) {
			PRINTF("clBuildProgram failed!");
			break;
		}
	}while(0);

	if(err == CL_SUCCESS) {
		if (fp) PRINTF("OpenCL_Init successfully\n");
	} else {
		if(fp) PRINTF2("OpenCL_Init error code = %d. (see cl.h)\n", err);
		Free();
	}
	if (fp)
		fclose(fp);
	return err;
}
void ImgLab::Free()
{
	PCL_STRUCT pCL = (PCL_STRUCT) m_handle;
	if(pCL) {
		if(pCL->program) clReleaseProgram(pCL->program);
		if(pCL->context) clReleaseContext(pCL->context);
		if (pCL->queue) clReleaseCommandQueue(pCL->queue);
		if(pCL->cl_in) clReleaseMemObject(pCL->cl_in);
		if(pCL->cl_out) clReleaseMemObject(pCL->cl_out);
		free(pCL);
		m_handle = NULL;
	}
}

int ImgLab:: AlphaBlending3(void* pIn, int in_stride, void* pOut, int out_stride, int width, int height, float alpha)
{
	PCL_STRUCT pCL = (PCL_STRUCT) m_handle;
	int in_size = in_stride * height;

	//check parameters
	ASSERT (alpha < 1.0 && alpha >= 0);
	ASSERT(out_stride >= width * 3 && in_stride >= width*3);
	ASSERT(pIn != NULL && pOut != NULL);

	if (in_size > MAX_FRAME_SIZE)
		return IMGLAB_ERROR_FRAME_BUFFER_NOT_ENOUGH;
	
	size_t in_offset =0;
	size_t out_offset =0;
	size_t cl_line_length = sizeof(cl_uchar) *3*width; //size per line in OpenCL buffer
	cl_int err;
	for(int i=0; i<height; i++) {
		err = clEnqueueWriteBuffer(pCL->queue, pCL->cl_in, CL_TRUE, cl_line_length*i, 
			cl_line_length, (unsigned char*)pIn+in_offset, 0, 0, 0);

		err = clEnqueueWriteBuffer(pCL->queue, pCL->cl_out, CL_TRUE, cl_line_length*i, 
			cl_line_length, (unsigned char*)pOut+out_offset, 0, 0, 0);
		out_offset += out_stride;
		in_offset += in_stride;

	}

	cl_kernel nKernel = clCreateKernel(pCL->program, "clAlphaBlending", &err);
	if(nKernel == 0) {
		return IMGLAB_OPENCL_CREATE_KERNEL_ERROR;
	}
	//
	clSetKernelArg(nKernel, 0, sizeof(cl_mem), &(pCL->cl_in));
	clSetKernelArg(nKernel, 1, sizeof(cl_int), &(in_stride));
	clSetKernelArg(nKernel, 2, sizeof(cl_mem), &(pCL->cl_out));
	clSetKernelArg(nKernel, 3, sizeof(cl_int), &(out_stride));
	clSetKernelArg(nKernel, 4, sizeof(cl_float), &(alpha));

	size_t global_size[] = {(size_t)width, (size_t)height};
	err = clEnqueueNDRangeKernel(pCL->queue, nKernel, 2, 0, global_size, 0, 0, 0, 0);

	if(err == CL_SUCCESS) {
		out_offset =0;
		for(int i=0; i<height; i++) {
			err = clEnqueueReadBuffer(pCL->queue, pCL->cl_out, CL_TRUE, i*cl_line_length, cl_line_length, 
					(unsigned char*)pOut+out_offset, 0, 0, 0);
			out_offset += out_stride;
		}
	}

	clReleaseKernel(nKernel);


	return IMGLAB_OK;
}
int ImgLab:: LinearBlending3(void* pIn, int in_stride, void* pOut, int out_stride, int width, int height)
{
	PCL_STRUCT pCL = (PCL_STRUCT) m_handle;
	int in_size = in_stride * height;

	//check parameters
	ASSERT(out_stride >= width * 3 && in_stride >= width*3);
	ASSERT(pIn != NULL && pOut != NULL);

	if (in_size > MAX_FRAME_SIZE)
		return IMGLAB_ERROR_FRAME_BUFFER_NOT_ENOUGH;
	
	size_t in_offset =0;
	size_t out_offset =0;
	size_t cl_line_length = sizeof(cl_uchar) *3*width; //size per line in OpenCL buffer
	cl_int err;
	for(int i=0; i<height; i++) {
		err = clEnqueueWriteBuffer(pCL->queue, pCL->cl_in, CL_TRUE, cl_line_length*i, 
			cl_line_length, (unsigned char*)pIn+in_offset, 0, 0, 0);

		err = clEnqueueWriteBuffer(pCL->queue, pCL->cl_out, CL_TRUE, cl_line_length*i, 
			cl_line_length, (unsigned char*)pOut+out_offset, 0, 0, 0);
		out_offset += out_stride;
		in_offset += in_stride;

	}

	cl_kernel nKernel = clCreateKernel(pCL->program, "clLinearBlending", &err);
	if(nKernel == 0) {
		return  IMGLAB_OPENCL_CREATE_KERNEL_ERROR;
	}
	//
	clSetKernelArg(nKernel, 0, sizeof(cl_mem), &(pCL->cl_in));
	clSetKernelArg(nKernel, 1, sizeof(cl_int), &(in_stride));
	clSetKernelArg(nKernel, 2, sizeof(cl_mem), &(pCL->cl_out));
	clSetKernelArg(nKernel, 3, sizeof(cl_int), &(out_stride));

	size_t global_size[] = {(size_t)width, (size_t)height};
	err = clEnqueueNDRangeKernel(pCL->queue, nKernel, 2, 0, global_size, 0, 0, 0, 0);

	if(err == CL_SUCCESS) {
		out_offset =0;
		for(int i=0; i<height; i++) {
			err = clEnqueueReadBuffer(pCL->queue, pCL->cl_out, CL_TRUE, i*cl_line_length, cl_line_length, 
					(unsigned char*)pOut+out_offset, 0, 0, 0);
			out_offset += out_stride;
		}
	}

	clReleaseKernel(nKernel);


	return IMGLAB_OK;
}

	/*! Stretch image source on destination
	* \param pSource source image
	* \param rcSrc	area on source image
	* \param pDest	destination image
	* \param rcDest area on destination image
	* \return IMGLAB_OK if successful.
	*/	
int ImgLab:: StretchImage(PImage24 pSource, CRect  rcSrc, PImage24 pDest, CRect rcDest)
{
	PCL_STRUCT pCL = (PCL_STRUCT) m_handle;
	CRect rc1;		//valid area on pSource
	CRect rc2;		//valid area on pDest
	if (rcSrc.left < 0) {
		rc1.left =0;
		rc2.left = rcDest.left + (rc1.left - rcSrc.left)*rcDest.Width()/rcSrc.Width();
	} else {
		rc1.left = rcSrc.left;
		rc2.left = rcDest.left;
	}
	if (rcSrc.top < 0) {
		rc1.top =0;
		rc2.top = rcDest.top+ (rc1.top - rcSrc.top)*rcDest.Height()/rcSrc.Height();
	} else {
		rc1.top = rcSrc.top;
		rc2.top = rcDest.top;
	}
	if (rcSrc.right > pSource->width) {
		rc1.right = pSource->width;
		rc2.right = rcDest.left + (rc1.right - rcSrc.left)*rcDest.Width()/rcSrc.Width();
	} else {
		rc1.right = rcSrc.right;
		rc2.right = rcDest.right;
	}
	if (rcSrc.bottom > pSource->height) {
		rc1.bottom = pSource->height;
		rc2.bottom = rcDest.top + (rc1.bottom - rcSrc.top)*rcDest.Height()/rcSrc.Height();
	} else {
		rc1.bottom = rcSrc.bottom;
		rc2.bottom = rcDest.bottom;
	}
	//check rc2, if it out of pDest, clip and remap back to rc1
	if (rc2.left < 0) {
		rc2.left =0;
		rc1.left = rcSrc.left + (rc2.left - rcDest.left)*rcSrc.Width()/rcDest.Width();
	} 
	if (rc2.top < 0) {
		rc2.top =0;
		rc1.top = rcSrc.top + (rc2.top - rcDest.top)*rcSrc.Height()/rcDest.Height();
	} 
	if (rc2.right >  pDest->width) {
		rc2.right =  pDest->width;
		rc1.right = rcSrc.left + (rc2.right - rcDest.left)*rcSrc.Width()/rcDest.Width();
	} 
	if (rc2.bottom  >  pDest->height) {
		rc2.bottom = pDest->height;
		rc1.bottom = rcSrc.top + (rc2.bottom - rcDest.top)*rcSrc.Height()/rcDest.Height();
	} 
	cl_int err;
	size_t in_line_length = sizeof(cl_uchar) *rc1.Width()*3;
	//now, copy pIn to  pCL->cl_in
	BYTE* pIn = pSource->data + rc1.top* pSource->stride + rc1.left*3;
	for(int i=0; i<rc1.Height();  i++) {
		err = clEnqueueWriteBuffer(pCL->queue, pCL->cl_in, CL_TRUE, in_line_length*i, 
			in_line_length, (unsigned char*)pIn  + i*pSource->stride, 0, 0, 0);
	}

	BYTE* pOut = pDest->data + rc2.top * pDest->stride + rc2.left*3;
	size_t out_line_length = sizeof(cl_uchar) *3*rc2.Width();
	//now, copy pOut to  pCL->cl_out
	for(int i=0; i< rc2.Height(); i++) {
		err = clEnqueueWriteBuffer(pCL->queue, pCL->cl_out, CL_TRUE, out_line_length*i, 
			out_line_length, (unsigned char*)pOut  + i*pDest->stride, 0, 0, 0);
	}

	cl_kernel nKernel = clCreateKernel(pCL->program, "clStretchImage", &err);
	if(nKernel == 0) {
		return  IMGLAB_OPENCL_CREATE_KERNEL_ERROR;
	}
	//
	cl_int in_width = rc1.Width();
	cl_int in_height = rc1.Height();
	clSetKernelArg(nKernel, 0, sizeof(cl_mem), &(pCL->cl_in));	/*!< input image */
	clSetKernelArg(nKernel, 1, sizeof(cl_int), &(in_line_length));			/*!< stride of input */
	clSetKernelArg(nKernel, 2, sizeof(cl_int), &(in_width));			/*!< source width */
	clSetKernelArg(nKernel, 3, sizeof(cl_int), &(in_height));			/*!< source height */
	clSetKernelArg(nKernel, 4, sizeof(cl_mem), &(pCL->cl_out));
	clSetKernelArg(nKernel, 5, sizeof(cl_int), &(out_line_length));

	//make sure output range is within pDest
	size_t global_size[] = {rc2.Width(), rc2.Height()};
	err = clEnqueueNDRangeKernel(pCL->queue, nKernel, 2, 0, global_size, 0, 0, 0, 0);

	if(err == CL_SUCCESS) {
		//copy CL to Dest 
		for(int i=0; i< rc2.Height(); i++) {
			err = clEnqueueReadBuffer(pCL->queue, pCL->cl_out, CL_TRUE, out_line_length*i, 
				out_line_length, (unsigned char*)pOut  + i*pDest->stride, 0, 0, 0);
		}
	}

	clReleaseKernel(nKernel);

	return (err == CL_SUCCESS)?IMGLAB_OK:IMGLAB_OPENCL_CREATE_KERNEL_ERROR;
}
