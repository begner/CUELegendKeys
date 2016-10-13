#include "FPLearnMode.h"



FPLearnMode::FPLearnMode(HWND uiHWND) : FPIdle::FPIdle(uiHWND)
{
}


FPLearnMode::~FPLearnMode()
{
}


int FPLearnMode::getWindowBackgroundResource() {
	return IDB_WINDOW_BACKGROUND_LEARNMODE;
}

bool FPLearnMode::process() {

	// IDLE Mode does not need to be limitless
	enableFpsLimit(true);
	setFpsLimit(Settings::getInstance()->getValue("LearnMode", "FPSLimit", (int)60));

	PerformanceStart();

	// copy background to UI
	windowBackground.copyTo(drawUI);

	PerformanceDraw();

	// draw the UI
	ImageFilterMat::DrawToHDC(drawHDC, drawUI, 0, 0, windowBackground.cols, windowBackground.rows);


	// double buffer write
	BitBlt(windowHDC, 0, 0, uiWidth, uiHeight, drawHDC, 0, 0, SRCCOPY);

	PerformanceStop();
	return true;

}
