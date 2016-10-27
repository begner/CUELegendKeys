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
	if (mode == FP_IDLE_MODE_OFF) {
		return IDB_WINDOW_BACKGROUND_IDLE;
	}
	else {
		return IDB_WINDOW_BACKGROUND_IDLE_MIRROR;
	}
	
	
}

void FPIdle::setCaptureWindow(HWND currentProcess) {
	
}

bool FPIdle::process() {
	
	if (mode == FP_IDLE_MODE_OFF) {
		
		// IDLE Mode does not need to be limitless
		enableFpsLimit(true);
		setFpsLimit(Settings::getInstance()->getValue("IdleMode", "IdleFPSLimit", (int)2));

		PerformanceStart();

		// copy background to UI
		getBackgroundMat()->copyTo(drawUI);
		
		PerformanceDraw();

		// draw the UI
		ImageFilterMat::DrawToHDC(drawHDC, drawUI, 0, 0, getBackgroundMat()->cols, getBackgroundMat()->rows);
	
		
		// double buffer write
		BitBlt(windowHDC, 0, 0, uiWidth, uiHeight, drawHDC, 0, 0, SRCCOPY);

		

		PerformanceStop();
		

		return true;
	}
	else {
		
		setFpsLimit(Settings::getInstance()->getValue("IdleMode", "ScreenMirrorFPSLimit", (int)15));
		
		return FPScreenMirror::process();
	}
}

void FPIdle::setMode(int setMode) {
	mode = setMode;
}