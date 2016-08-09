#include "ImageFilter.h"

ImageFilter::ImageFilter()
{
}


ImageFilter::~ImageFilter()
{
}



void ImageFilter::colorReduce(IplImage* image, int div)
{
	for (int y = 0; y < image->height; y++)
	{
		uchar* ptr = (uchar*)(image->imageData + y * image->widthStep);

		for (int x = 0; x < image->width; x++) {
			{
				// process each pixel
				ptr[3 * x+0] = ptr[3 * x+0] / div * div + div / 2; // b
				ptr[3 * x+1] = ptr[3 * x+1] / div * div + div / 2; // g
				ptr[3 * x+2] = ptr[3 * x+2] / div * div + div / 2; // r
			}
		}
	}
}

map<Vec3b, int, lessVec3b> ImageFilter::getPalette(IplImage* image)
{
	map<Vec3b, int, lessVec3b> palette;
	
	for (int y = 0; y < image->height; y++)
	{
		uchar* ptr = (uchar*)(image->imageData + y * image->widthStep);

		for (int x = 0; x < image->width; x++) {
			{
				uchar cr = ptr[3 * x + 2];
				uchar cg = ptr[3 * x + 1];
				uchar cb = ptr[3 * x + 0];


				Vec3b color(ptr[3 * x + 0], ptr[3 * x + 1], ptr[3 * x + 2]);
				
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
	}

	return palette;
}



IplImage* ImageFilter::hBitmap2Ipl(HBITMAP hbmp, HDC hdc)
{
	IplImage* cvImage = NULL;
	bool mustrelease = false;
	BITMAPINFO bmi;
	BITMAPINFOHEADER* bmih = &(bmi.bmiHeader);

	if (hdc == NULL) {
		hdc = GetDC(NULL);
		mustrelease = true;
	}

	ZeroMemory(bmih, sizeof(BITMAPINFOHEADER));
	bmih->biSize = sizeof(BITMAPINFOHEADER);

	if (GetDIBits(hdc, hbmp, 0, 0, NULL, &bmi, DIB_RGB_COLORS)) {
		int height = (bmih->biHeight > 0) ? bmih->biHeight : -bmih->biHeight;
		
		
		
		cvImage = cvCreateImage(cvSize(bmih->biWidth, height), IPL_DEPTH_8U, 3);
		cvImage->origin = (bmih->biHeight > 0);
		bmih->biBitCount = 24;
		bmih->biCompression = BI_RGB;
		GetDIBits(hdc, hbmp, 0, height, cvImage->imageData, &bmi,	DIB_RGB_COLORS);
	}

	if (mustrelease)
		ReleaseDC(NULL, hdc);

	return cvImage;
}


void ImageFilter::drawToHDC(IplImage* pImage, HDC hdc, int x, int y)
{
	RECT rect;
	rect.left = 0 + x;
	rect.top = 0 + y;
	rect.right = x + pImage->width;
	rect.bottom = y + pImage->height;

	CvvImage cimg;
	cimg.CopyOf(pImage);
	cimg.DrawToHDC(hdc, &rect);
}
