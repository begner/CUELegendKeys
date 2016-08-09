#include "FPIdle.h"


FPIdle::FPIdle(HWND uiHWND) : FPScreenMirror(uiHWND) 
{
	
	desktopWindow = GetDesktopWindow();
	FPScreenMirror::setCaptureWindow(desktopWindow);
	FPScreenMirror::setOffset(RECT{ 100, 200, 100, 0 });
	FPScreenMirror::setUIDimensions(209, 287, 363, 204);
	FPScreenMirror::reinitialize();
}

FPIdle::~FPIdle()
{

}

int FPIdle::getWindowBackgroundResource() {
	return IDB_WINDOW_BACKGROUND_IDLE;
}

void FPIdle::setCaptureWindow(HWND currentProcess) {
	
}

bool FPIdle::process() {
	
	if (mode == FP_IDLE_MODE_OFF) {

		// copy background to UI
		windowBackground.copyTo(drawUI);
		// draw the UI
		ImageFilterMat::DrawToHDC(drawHDC, drawUI, 0, 0, windowBackground.cols, windowBackground.rows);
	
		// double buffer write
		BitBlt(windowHDC, 0, 0, uiWidth, uiHeight, drawHDC, 0, 0, SRCCOPY);

		// idle does not need more than 5 fps...
		Sleep(1000/5);
		return true;
	}
	else {
		
		
		
		return FPScreenMirror::process();
	}
}

void FPIdle::setMode(int setMode) {
	mode = setMode;
}