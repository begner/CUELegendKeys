#include "FrameProcessing.h"



FrameProcessing::FrameProcessing(HWND uiHWND)
{

	// Performance measure
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency))
		std::cout << "Performance Counter nicht vorhanden" << std::endl;

	QueryPerformanceCounter((LARGE_INTEGER*)&g_CurrentFPS);


	// gdi UI setup
	// FYI: draw always to drawHDC! 
	// -> at end copy via BitBlt to windowHDC
	RECT uiDimension;
	

	windowHDC = GetDC(uiHWND);
	GetClientRect(uiHWND, &uiDimension);

	uiWidth = uiDimension.right - uiDimension.left;
	uiHeight = uiDimension.bottom - uiDimension.top;

	drawHDC = CreateCompatibleDC(windowHDC);
	HBITMAP drawHBitmap = CreateCompatibleBitmap(windowHDC, uiWidth, uiHeight);
	SelectObject(drawHDC, drawHBitmap);

}


FrameProcessing::~FrameProcessing()
{
	NuLogger::getInstance()->log("FrameProcessing Destructed!");
}

void FrameProcessing::PerformanceStart() {
	QueryPerformanceCounter((LARGE_INTEGER*)&g_CurentCount);
}

void FrameProcessing::loadResources() {
	int backgroundRes = this->getWindowBackgroundResource();
	windowBackground = ImageFilterMat::loadResourceAsMat(backgroundRes);
}

void FrameProcessing::PerformanceDraw() {

	// add performance to UI
	char fpsShowBuffer[2048];

	if (lastFrameRenderDuration > 0) {
		sprintf_s(fpsShowBuffer, "FPS: %06.2f (%03i)", (lastFrameRenderDuration > 0 ? 1000 / lastFrameRenderDuration : 0), realFramesLastSecond);
		putText(drawUI, fpsShowBuffer, cv::Point(windowBackground.cols - 130, 20), FONT_HERSHEY_PLAIN, 0.8, Scalar(164, 196, 215, 255));
	}

	sprintf_s(fpsShowBuffer, "FRAME: %06.2f ms", lastFrameRenderDuration);
	putText(drawUI, fpsShowBuffer, cv::Point(windowBackground.cols - 130, 40), FONT_HERSHEY_PLAIN, 0.8, Scalar(164, 196, 215, 255));


}

double FrameProcessing::getElapsedTime() {
	// meassure frame render performance
	QueryPerformanceCounter((LARGE_INTEGER*)&g_LastCount);
	double dTimeDiff = (((double)(g_LastCount - g_CurentCount)) / ((double)g_Frequency));
	return dTimeDiff;
}

void FrameProcessing::setFpsLimit(int limit) {
	fpsLimit = limit;
}


void FrameProcessing::enableFpsLimit(bool state) {
	useFpsLimit = state;
}


void FrameProcessing::PerformanceStop() {

	if (useFpsLimit) {

		// FPS Limiter
		if (fpsLimit < 1) {
			return;
		}

		double waitTime = 1000 / fpsLimit - getElapsedTime() * 1000;

		if (waitTime < 0) {
			return;
		}

		Sleep(DWORD(waitTime));
	}


	// NuLogger::getInstance()->log("Render Frame: %f ms", floor(1000*dTimeDiff));
	lastFrameRenderDuration = 1000 * getElapsedTime();

	// counting real frames
	currentFrames++;

	// reset real frames when a second passed
	QueryPerformanceCounter((LARGE_INTEGER*)&g_LastFPS);
	double dTimeDiffFPS = (((double)(g_LastFPS - g_CurrentFPS)) / ((double)g_Frequency));
	if (1000 * dTimeDiffFPS > 1000) {
		// NuLogger::getInstance()->log("FPS: %i", currentFrames);
		realFramesLastSecond = currentFrames;
		currentFrames = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&g_CurrentFPS);
	}

	

}

