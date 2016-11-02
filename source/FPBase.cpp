#include "FPBase.h"



FPBase::FPBase(HWND uiHWND)
{
	// gdi UI setup
	// FYI: draw always to drawHDC! 
	// -> at end copy via BitBlt to windowHDC
	RECT uiDimension;

	windowHWND = uiHWND;
	windowHDC = GetDC(uiHWND);
	GetClientRect(uiHWND, &uiDimension);

	uiWidth = uiDimension.right - uiDimension.left;
	uiHeight = uiDimension.bottom - uiDimension.top;

	drawHDC = CreateCompatibleDC(windowHDC);
	HBITMAP drawHBitmap = CreateCompatibleBitmap(windowHDC, uiWidth, uiHeight);
	SelectObject(drawHDC, drawHBitmap);
}


FPBase::~FPBase()
{
}


Mat4b* FPBase::getBackgroundMat() {
	int backgroundRes = this->getWindowBackgroundResource();
	if (windowBackgrounds.find(backgroundRes) == windowBackgrounds.end()) {
		windowBackgrounds[backgroundRes] = ImageFilterMat::loadResourceAsMat(backgroundRes);
	}
	return &windowBackgrounds[backgroundRes];
}


void FPBase::drawToWindow(Mat* image) {

	// draw the UI
	ImageFilterMat::DrawToHDC(drawHDC, *image, 0, 0, getBackgroundMat()->cols, getBackgroundMat()->rows);

	// double buffer write
	BitBlt(windowHDC, 0, 0, uiWidth, uiHeight, drawHDC, 0, 0, SRCCOPY);

	

}