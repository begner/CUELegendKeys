#pragma once

#include "Constants.h"
#include <Windows.h>

#include "ImageFilterMat.h"

class FPBase
{
protected:
	HDC drawHDC;
private:
	HDC windowHDC;
	HWND windowHWND;
	int uiWidth = 0;
	int uiHeight = 0;

	map<int, Mat4b> windowBackgrounds;

public:
	FPBase(HWND uiHWND);
	~FPBase();
	virtual int getWindowBackgroundResource() = 0;
	Mat4b* getBackgroundMat();
	void drawToWindow(Mat* image);
};

