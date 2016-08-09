#include "ImageFilterMat.h"

ImageFilterMat::ImageFilterMat()
{
}


ImageFilterMat::~ImageFilterMat()
{
}

bool ImageFilterMat::hasPixelBorder(Mat &image, int top, int right, int bot, int left, Vec3b allowedColor1, Vec3b allowedColor2) {

	Mat_<Vec4b> _I = image;

	for (int i = 0; i < image.rows; ++i)
		for (int j = 0; j < image.cols; ++j)
		{
			Vec4b currentColor(_I(i, j));
			Vec3b currentColor3(currentColor[0], currentColor[1], currentColor[2]);
			if (
				((i == top || i == image.rows - 1 - bot) && (j >= left && j <= image.cols - 1 - right)) ||
				((j == left || j == image.cols - 1 - right)  && (i >= top && i <= image.rows - 1 - bot))
				) 
			{
				if (currentColor3 != allowedColor1 && currentColor3 != allowedColor2) {
					return false;
				}
			}
		}

	return true;
}

// checked: Works & no memory leaks!
void ImageFilterMat::colorReduce(Mat &image, int div, bool ignoreAlpha)
{
	Mat_<Vec4b> _I = image;

	for (int j = image.cols - 1; j > -1; --j) {
		bool completeFilledCol = true;

		for (int i = 0; i < image.rows; ++i) {
			_I(i, j)[0] = _I(i, j)[0] / div * div + div / 2;
			_I(i, j)[1] = _I(i, j)[1] / div * div + div / 2;
			_I(i, j)[2] = _I(i, j)[2] / div * div + div / 2;
		}
	}

	/*
	int nl = image.rows;                    // number of lines
	int nc = image.cols*image.channels();					// number of elements per line

	for (int j = 0; j < nl; j++)
	{
		// get the address of row j
		uchar* data = image.ptr<uchar>(j);
		
		for (int i = 0; i < nc; i++)
		{
			if (ignoreAlpha && image.channels() == 4 && i-1 % 4 != 0) {
				data[i] = data[i] / div * div + div / 2;
			}
			
		}
	}
	*/
}

int ImageFilterMat::getBarPercentage(Mat srcImage) {
	Mat_<Vec4b> _I = srcImage;
	int jPositionFound = -1;
	int percentageInt = 100;

	for (int j = srcImage.cols-1; j >= 0; --j) {
		bool completeFilledCol = true;
		
		for (int i = srcImage.rows-1; i >= 0; --i) {
			
			int cr = _I(i, j)[0];
			int cg = _I(i, j)[1];
			int cb = _I(i, j)[2];
			
			
			if (cr == 0 && cg == 0 && cb == 0) {

			}
			else {
				completeFilledCol = false;
				break;
			}
			
		}
		
		if (completeFilledCol) {
			jPositionFound = j;
		}
		
	}
		
	if (jPositionFound > -1) {
		percentageInt = (int)(float(100) / float(srcImage.cols) * float(jPositionFound));
	}
	
	return percentageInt;
}

void ImageFilterMat::whiteToDarkPixel(Mat srcImage, int threshold) {
	Mat_<Vec4b> _I = srcImage;

	for (int i = 0; i < srcImage.rows; ++i)
		for (int j = 0; j < srcImage.cols; ++j)
		{
			if (_I(i, j)[0] > threshold && _I(i, j)[1] > threshold && _I(i, j)[2] > threshold) {
				_I(i, j)[0] = 0;
				_I(i, j)[1] = 0;
				_I(i, j)[2] = 0;
			}
		}
}

void ImageFilterMat::killGrayPixel(Mat srcImage, int threshold) {
	Mat_<Vec4b> _I = srcImage;
	int diffRG, diffRB, diffGB;
	for (int i = 0; i < srcImage.rows; ++i)
		for (int j = 0; j < srcImage.cols; ++j)
		{
			diffRG = abs(_I(i, j)[2] - _I(i, j)[1]);
			diffRB = abs(_I(i, j)[2] - _I(i, j)[0]);
			diffGB = abs(_I(i, j)[1] - _I(i, j)[0]);

			if (diffRG < threshold && diffRB < threshold && diffGB < threshold) {
				_I(i, j)[0] = 0;
				_I(i, j)[1] = 0;
				_I(i, j)[2] = 0;
			}
		}
}


void ImageFilterMat::killDarkPixel(Mat srcImage, int threshold) {
	Mat_<Vec4b> _I = srcImage;

	for (int i = 0; i < srcImage.rows; ++i)
		for (int j = 0; j < srcImage.cols; ++j)
		{
			if (_I(i, j)[0] < threshold && _I(i, j)[1]  < threshold && _I(i, j)[2] < threshold) {
				_I(i, j)[0] = 0;
				_I(i, j)[1] = 0;
				_I(i, j)[2] = 0;
			}
		}
}

bool ImageFilterMat::colorIsPresent(Mat srcImage, int r, int g, int b) {
	Mat_<Vec4b> _I = srcImage;

	for (int i = 0; i < srcImage.rows; ++i)
		for (int j = 0; j < srcImage.cols; ++j)
		{
			if (_I(i, j)[0] == r && _I(i, j)[1] == g && _I(i, j)[2] == b) {
				return true;
			}
		}
	return false;
}


map<Vec4b, int, lessVec4b> ImageFilterMat::getPalette(Mat srcImage)
{
	map<Vec4b, int, lessVec4b> palette;

	Mat_<Vec4b> _I = srcImage;

	for (int i = 0; i < srcImage.rows; ++i)
		for (int j = 0; j < srcImage.cols; ++j)
		{
			
			
			// slip non color values!
			int cr = _I(i, j)[0];
			int cg = _I(i, j)[1];
			int cb = _I(i, j)[2];

			if (cr == cg && cr == cb && cg == cb) {
			}
			else {
				Vec4b color(_I(i, j)[0], _I(i, j)[1], _I(i, j)[2]);
				if (palette.count(color) == 0)
				{
					palette[color] = 1;
				}
				else
				{
					palette[color] = palette[color] + 1;
				}
			}
		}

	return palette;
}


// i think this one isnt working anymore...
void ImageFilterMat::saturation(Mat srcImage, int trashhold, int satValue, float brightnessFactor) {

	Mat satImage(srcImage);

	cvtColor(satImage, satImage, CV_RGBA2RGB);
	cvtColor(satImage, satImage, CV_RGB2HSV);

	Mat_<Vec3b> _I = satImage;

	for (int i = 0; i < satImage.rows; ++i)
		for (int j = 0; j < satImage.cols; ++j)
		{
			int h, s, v;
			h = _I(i, j)[0];
			s = _I(i, j)[1];
			v = _I(i, j)[2];

			if (s >= trashhold) {
				s = satValue;
			}
			
			v = (int)floor(v * brightnessFactor);
			if (v > 255) v = 255;

			_I(i, j)[0] = h;
			_I(i, j)[1] = s;
			_I(i, j)[2] = v;

		}
	satImage = _I;


	cvtColor(satImage, satImage, CV_HSV2RGB);
	cvtColor(satImage, srcImage, CV_RGB2RGBA);

}

void ImageFilterMat::incSaturation(Mat srcImage, int incValue, float brightnessFactor) {
	Mat satImage(srcImage);

	cvtColor(satImage, satImage, CV_RGBA2RGB);
	cvtColor(satImage, satImage, CV_RGB2HSV);

	Mat_<Vec3b> _I = satImage;

	for (int i = 0; i < satImage.rows; ++i)
		for (int j = 0; j < satImage.cols; ++j)
		{
			int h, s, v;
			h = _I(i, j)[0];
			s = _I(i, j)[1];
			v = _I(i, j)[2];

			s = s + incValue;
			if (s > 255) s = 255;

			v = (int)floor((float)v * brightnessFactor);
			if (v > 255) v = 255;

			_I(i, j)[0] = h;
			_I(i, j)[1] = s;
			_I(i, j)[2] = v;

		}
	satImage = _I;


	cvtColor(satImage, satImage, CV_HSV2RGB);
	cvtColor(satImage, srcImage, CV_RGB2RGBA);

}


// checked: Works & no memory leaks!
bool ImageFilterMat::DrawToHDC(HDC drawHDC, Mat mat, int x, int y, int width, int height)
{

	// create output 
	HDC hDCMem = CreateCompatibleDC(drawHDC);

	HBITMAP m_hBitmap;
	m_hBitmap = ImageFilterMat::createBitmap(hDCMem, mat);
	SelectObject(hDCMem, m_hBitmap);

	BitBlt(drawHDC, x, y, width, height, hDCMem, 0, 0, SRCCOPY);


	DeleteObject(m_hBitmap);
	DeleteDC(hDCMem);
	
	return TRUE;
}




// checked: Works & no memory leaks!
HBITMAP ImageFilterMat::createBitmap(HDC drawHDC, cv::Mat mat)
{

	unsigned char* lpBitmapBits;

	HBITMAP m_hBitmap;

	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = mat.cols;
	bi.bmiHeader.biHeight = -mat.rows;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = ((WORD)mat.elemSize() << 3);
	bi.bmiHeader.biCompression = BI_RGB;

	m_hBitmap = CreateDIBSection(drawHDC, &bi, DIB_RGB_COLORS, (VOID**)&lpBitmapBits, NULL, 0);

	#define ALIGN(x,a)              __ALIGN_MASK(x, a-1)
	#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))
		int width = mat.cols * (int)mat.elemSize();
		int pitch = ALIGN(width, 4);
	#undef __ALIGN_MASK
	#undef ALIGN

	for (int i = 0; i < mat.rows; i++)
	{
		unsigned char* data = mat.ptr<unsigned char>(i);
		memcpy(lpBitmapBits, data, width);
		lpBitmapBits += pitch;
	}

	return m_hBitmap;
}

// checked: Works & no memory leaks!
Mat* ImageFilterMat::hdc2mat(HDC drawHDC, int x, int y, int width, int height) {

	HBITMAP hBitmap; // <-- The image represented by hBitmap
	HBITMAP oldHBitmap; 


	Mat* matBitmap; // <-- The image represented by mat
	HDC hdcMem = CreateCompatibleDC(drawHDC); // Create compatible DC 

	void *ptrBitmapPixels; // <-- Pointer variable that will contain the potinter for the pixels


						   // Create hBitmap with Pointer to the pixels of the Bitmap
	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = width;
	bi.bmiHeader.biHeight = -height;  //negative so (0,0) is at top left
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;

	
	hBitmap = CreateDIBSection(drawHDC, &bi, DIB_RGB_COLORS, &ptrBitmapPixels, NULL, 0);


	// ^^ The output: hBitmap & ptrBitmapPixels

	// Set hBitmap in the hdcMem 
	oldHBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);


	// Set matBitmap to point to the pixels of the hBitmap
	BitBlt(hdcMem, 0, 0, width, height, drawHDC, x, y, SRCCOPY);

	// put data in temp bitmap (because deleteObject hdcMem will delete pixel info)
	Mat referencedBitmap = Mat(height, width, CV_8UC4, ptrBitmapPixels, 0);
	//                        ^^ note: first it is y, then it is x. very confusing
	// create output 
	matBitmap = new Mat(height, width, CV_8UC4);

	// copy data
	referencedBitmap.copyTo(*matBitmap);

	// release
	referencedBitmap.release();



	// * SETUP DONE *




	// Now update the pixels using BitBlt



	// Just to do some image processing on the pixels.. (Dont have to to this)
	// Mat matRef = matBitmap(Range(100, 200), Range(100, 200));
	//                            y1    y2          x1   x2
	// bitwise_not(matRef, matRef); // Invert the colors in this x1,x2,y1,y2
	SelectObject(hdcMem, oldHBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);


	// Display the results through Mat
	// imshow("Title", matBitmap);

	// Wait until some key is pressed
	// waitKey(0);
	return matBitmap;
	
	
	
}

void ImageFilterMat::overlayImage(Mat* src, Mat* overlay, const Point& location)
{
	for (int y = max(location.y, 0); y < src->rows; ++y)
	{
		int fY = y - location.y;

		if (fY >= overlay->rows)
			break;

		for (int x = max(location.x, 0); x < src->cols; ++x)
		{
			int fX = x - location.x;

			if (fX >= overlay->cols)
				break;

			double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

			// c<3 (dont copy alpha - only bgr channels)
			for (int c = 0; opacity > 0 && c < 3; ++c)
			{
				unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
				unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
				src->data[y * src->step + src->channels() * x + c] = (uchar)(srcPx * (1. - opacity) + overlayPx * opacity);
			}
		}
	}

}

void ImageFilterMat::addAlphaMask(Mat* src, Mat* mask) {
	
	Mat srcChannels[4];
	cv::split(*src, srcChannels);

	Mat maskChannels[4];
	cv::split(*mask, maskChannels);
	
	srcChannels[3] = maskChannels[0];
	cv::merge(srcChannels, 4, *src);
	

}

Mat4b* ImageFilterMat::MatToMat4b(Mat* inputMat) {
	Mat4b* output = new Mat4b(inputMat->cols, inputMat->rows, CV_8UC4);
	cvtColor(*inputMat, *output, CV_RGB2RGBA);
	return output;
}

Mat4b ImageFilterMat::loadResourceAsMat(int RESOURCEID) {

	HINSTANCE hInstance = GetModuleHandle(NULL);
	HBITMAP hBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(RESOURCEID), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	HDC drawHDC = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(drawHDC);

	BITMAPINFO MyBMInfo = { 0 };
	MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

	// Get the BITMAPINFO structure from the bitmap
	if (0 == GetDIBits(hdcMem, hBitmap, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS))
	{
		NuLogger::getInstance()->log("FAIL1");
	}

	// create the bitmap buffer
	BYTE* lpPixels = new BYTE[MyBMInfo.bmiHeader.biSizeImage];

	MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);
	MyBMInfo.bmiHeader.biBitCount = 24;
	MyBMInfo.bmiHeader.biCompression = BI_RGB;
	MyBMInfo.bmiHeader.biHeight = (MyBMInfo.bmiHeader.biHeight < 0) ? (-MyBMInfo.bmiHeader.biHeight) : (MyBMInfo.bmiHeader.biHeight);


	// get the actual bitmap buffer
	if (0 == GetDIBits(drawHDC, hBitmap, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixels, &MyBMInfo, DIB_RGB_COLORS))
	{
		NuLogger::getInstance()->log("FAIL2");
		//::printToDebugWindow("FAIL\n");
	}

	// put data in temp bitmap (because deleteObject hdcMem will delete pixel info)
	Mat referencedBitmap(MyBMInfo.bmiHeader.biHeight, MyBMInfo.bmiHeader.biWidth, CV_8UC3, lpPixels, 0);
	cv::flip(referencedBitmap, referencedBitmap, 0);

	// create output 
	Mat4b matBitmap(MyBMInfo.bmiHeader.biHeight, MyBMInfo.bmiHeader.biWidth, CV_8UC4);
	deepCopyPixel(&referencedBitmap, &matBitmap);


	// release
	referencedBitmap.release();
	DeleteObject(hBitmap);

	

	return matBitmap;
}

void ImageFilterMat::deepCopyPixel(Mat *src, Mat4b *dest) {
	Mat_<Vec3b> _s = *src;
	Mat_<Vec4b> _d = *dest;

	for (int i = 0; i < src->rows; ++i)
		for (int j = 0; j < src->cols; ++j)
		{
			_d(i, j)[0] = _s(i, j)[0];
			_d(i, j)[1] = _s(i, j)[1];
			_d(i, j)[2] = _s(i, j)[2];
			_d(i, j)[3] = 255;
		}
}

void ImageFilterMat::deepCopyPixel(Mat *src, Mat3b *dest) {
	Mat_<Vec4b> _s = *src;
	Mat_<Vec3b> _d = *dest;

	for (int i = 0; i < src->rows; ++i)
		for (int j = 0; j < src->cols; ++j)
		{
			_d(i, j)[0] = _s(i, j)[0];
			_d(i, j)[1] = _s(i, j)[1];
			_d(i, j)[2] = _s(i, j)[2];
		}
}

cv::Rect ImageFilterMat::getCropRectBorder(Mat *src, int upperVThreshold, int lowerSThreshold) {
	Mat hsvImage;
	src->copyTo(hsvImage);
	
	cvtColor(hsvImage, hsvImage, CV_RGB2HSV);

	int left = getFirstBorder(&hsvImage, 1, upperVThreshold, lowerSThreshold);
	int bottom = getFirstBorder(&hsvImage, 2, upperVThreshold, lowerSThreshold);
	int right = getFirstBorder(&hsvImage, 3, upperVThreshold, lowerSThreshold);
	int top = getFirstBorder(&hsvImage, 4, upperVThreshold, lowerSThreshold);
	

	cv::Rect cropRect(left, top, src->cols - left - right, src->rows - top - bottom);
	
	return cropRect;
}


/**
* direction
* 1 = top // left
* 2 = right // bottom
* 3 = bottom // right
* 4 = left // top
*/
int ImageFilterMat::getFirstBorder(Mat *hsvImage, int direction, int upperVThreshold, int lowerSThreshold) {
	Mat_<Vec3b> _s = *hsvImage;

	if (direction == 1 || direction == 3) {
		int x = (int)floor((float)hsvImage->rows / (float)2);

		for (int i = 0; i < hsvImage->cols-1; i++) {

			int y;

			y = i;
			if (direction == 3) {
				y = hsvImage->cols - i - 1;
			}

			int h, s, v;
			h = _s(x, y)[0];
			s = _s(x, y)[1];
			v = _s(x, y)[2];
			NuLogger::getInstance()->log("Coords: %i x %i = hsv %i %i %i ", x, y, h, s, v);

			if (v >= upperVThreshold && s <= lowerSThreshold) {
				return i;
			}
		}
	}
	if (direction == 2 || direction == 4) {
		int y = (int)floor((float)hsvImage->cols / (float)2);

		for (int i = 0; i < hsvImage->rows-1; i++) {

			int x;

			x = i;
			if (direction == 2) {
				x = hsvImage->rows - i - 1;
			}

			int h, s, v;
			h = _s(x, y)[0];
			s = _s(x, y)[1];
			v = _s(x, y)[2];
			NuLogger::getInstance()->log("Coords: %i x %i = hsv %i %i %i ", x, y, h, s, v);

			if (v >= upperVThreshold && s <= lowerSThreshold) {
				return i;
			}
		}
	}
	return 0;
}


void ImageFilterMat::cannyThreshold(Mat src, Mat* dest, int lowThreshold, int kernel_size, int ratio, int blur_size)
{
	Mat src_gray;

	if (lowThreshold > 100) {
		lowThreshold = 100;
	}
	if (lowThreshold < 0) {
		lowThreshold = 0;
	}

	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Reduce noise with a kernel 3x3
	blur(src_gray, *dest, Size(blur_size, blur_size));

	/// Canny detector
	Canny(*dest, *dest, lowThreshold, lowThreshold*ratio, kernel_size);
}


boolean ImageFilterMat::getMatchingRect(int match_method, Mat* haystackImage, Mat* needleImage, cv::Rect* foundRect)
{
	Mat result;

	/// Create the result matrix
	int result_cols = haystackImage->cols - needleImage->cols + 1;
	int result_rows = haystackImage->rows - needleImage->rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(*haystackImage, *needleImage, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	/// Show me what you got
	// rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	// rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	// imshow(image_window, img_display);
	// imshow(result_window, result);

	if (matchLoc.x > 0 && matchLoc.y > 0) {
		foundRect->x = matchLoc.x;
		foundRect->y = matchLoc.y;
		foundRect->width = needleImage->cols;
		foundRect->height = needleImage->rows;
		return true;
	}

	return false;
}