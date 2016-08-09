#pragma once
#include <opencv/cv.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2\opencv.hpp>
#include <opencv2\photo.hpp>
#include <Windows.h>
#include <iostream>
#include "CvvImage.h"
#include <map>

using namespace cv;
using namespace std;

struct lessVec3b
{
	bool operator()(const Vec3b& lhs, const Vec3b& rhs) const {
		return (lhs[0] != rhs[0]) ? (lhs[0] < rhs[0]) : ((lhs[1] != rhs[1]) ? (lhs[1] < rhs[1]) : (lhs[2] < rhs[2]));
	}
};

class ImageFilter
{
public:
	ImageFilter();
	~ImageFilter();

	static IplImage* hBitmap2Ipl(HBITMAP hbmp, HDC hdc = NULL);
	static void drawToHDC(IplImage* pImage, HDC hdc, int x, int y);
	static void colorReduce(IplImage* image, int div);
	static map<Vec3b, int, lessVec3b> getPalette(IplImage* image);
	
	


};


