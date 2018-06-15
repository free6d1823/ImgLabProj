#pragma once
#include "ImgLab.h"

/*!< Matrix without real allocated data */
class Matlet{
public:
//	Matlet(){};
//	~Matlet(){};
protected:
	double* data;
	int rows; //number of row
	int stride;//bytes per row
	int cols; //number of columns
};

class Mat: Matlet{
public:
IMGLAB_API Mat(int cx, int cy);
IMGLAB_API	~Mat();
IMGLAB_API	int Cols();
IMGLAB_API	int Rows();
IMGLAB_API	void Set(int x, int y, double value);
IMGLAB_API	void Set(double* mydata, int length);
IMGLAB_API	double Get(int x, int y);
IMGLAB_API	void Identity();
IMGLAB_API	void Print();
	/*<! T= transpose of THIS */
IMGLAB_API	void Transpose(Mat& t);
	/*<! THIS= S xT */
IMGLAB_API	void 	Multiply(Mat& s, Mat& t);
	/*<! T is submatrix of THIS, excluded x column and y row*/
IMGLAB_API	void GetSubmatrix(int x, int y, Mat& t);
	/*<! return the determinant of THIS matrix */
IMGLAB_API	double Determinant();
	/*<! Find inverse of THIS and save as T, return FALSE if cannot find*/
IMGLAB_API	BOOL FindInverse(Mat& t);
};

