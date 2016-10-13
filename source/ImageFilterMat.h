#pragma once


// #include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <Windows.h>
#include <iostream>
#include <string>
#include <map>
#include "NuLogger.h"




using namespace std;
using namespace cv;

struct lessVec4b
{
	bool operator()(const Vec4b& lhs, const Vec4b& rhs) const {
		return (lhs[0] != rhs[0]) ? (lhs[0] < rhs[0]) : ((lhs[1] != rhs[1]) ? (lhs[1] < rhs[1]) : (lhs[2] < rhs[2]));
	}
};

class ImageFilterMat
{
private:
	// static BYTE* convertBMPToRGBBuffer(BYTE* Buffer, int width, int height);
	static int getFirstBorder(Mat *hsvImage, int direction, int upperVThreshold, int lowerSThreshold);
public:
	ImageFilterMat();
	~ImageFilterMat();
	static int getBarPercentage(Mat hotspotMat);
	static void whiteToDarkPixel(Mat srcImage, int threshold);
	static void killDarkPixel(Mat srcImage, int threshold);
	static void killGrayPixel(Mat srcImage, int threshold);
	static void saturation(Mat srcImage, int trashhold, int satValue, float brightnessFactor);
	static void incSaturation(Mat srcImage, int incValue, float brightnessFactor);
	static bool DrawToHDC(HDC drawHDC, Mat mat, int x, int y, int width, int height);
	static void colorReduce(Mat &image, int div = 64, bool ignoreAlpha = true);
	static HBITMAP createBitmap(HDC drawHDC, Mat mat);
	static Mat* hdc2mat( HDC drawHDC, int x, int y, int height, int width);
	static map<Vec4b, int, lessVec4b> getPalette(Mat srcImage);
	static bool colorIsPresent(Mat srcImage, int r, int g, int b);
	static bool hasPixelBorder(Mat &image, int top, int right, int bot, int left, Vec3b allowedColor1, Vec3b allowedColor2);
	static void overlayImage(Mat* src, Mat* overlay, const cv::Point& location);
	static void addAlphaMask(Mat* src, Mat* mask);
	
	static Mat4b loadResourceAsMat(int RESOURCEID);
	static Mat4b* MatToMat4b(Mat* inputMat);
	static void deepCopyPixel(Mat *src, Mat4b *dest);
	static void deepCopyPixel(Mat *src, Mat3b *dest);
	static cv::Rect getCropRectBorder(Mat *src, int upperVThreshold = 70, int lowerSThreshold = 20);
	static void cannyThreshold(Mat src, Mat* dest, int lowThreshold = 50, int kernel_size = 3, int ratio = 3, int blur_size = 3);
	static boolean getMatchingRect(int match_method, Mat* haystackImage, Mat* needleImage, cv::Rect* foundRect);
};

