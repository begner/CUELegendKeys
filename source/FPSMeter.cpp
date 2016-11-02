#include "FPSMeter.h"



FPSMeter::FPSMeter()
{

	// Performance measure
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency)) {
		NuLogger::getInstance()->log("Performance Counter nicht vorhanden");
	}

	QueryPerformanceCounter((LARGE_INTEGER*)&g_CurrentFPS);
}

FPSMeter::~FPSMeter()
{
	
}

void FPSMeter::PerformanceStart() {
	QueryPerformanceCounter((LARGE_INTEGER*)&g_CurentCount);
}

void FPSMeter::PerformanceDraw(int x, int y) {

	// add performance to UI
	char fpsShowBuffer[2048];

	double fps = (lastFrameRenderDuration > 0 ? 1000 / lastFrameRenderDuration : 0);

	if (lastFrameRenderDuration > 0) {
		sprintf_s(fpsShowBuffer, "FPS: %06.2f (%03i)", fps, realFramesLastSecond);
		putText(drawUI, fpsShowBuffer, cv::Point(x, y), FONT_HERSHEY_PLAIN, 0.8, Scalar(164, 196, 215, 255));
	}

	sprintf_s(fpsShowBuffer, "FRAME: %06.2f ms", lastFrameRenderDuration);
	putText(drawUI, fpsShowBuffer, cv::Point(x, y + 20), FONT_HERSHEY_PLAIN, 0.8, Scalar(164, 196, 215, 255));
}


void FPSMeter::PerformanceStop() {
	
	// counting real frames
	currentFrames++;
	lastFrameRenderDuration = 1000 * getElapsedTime();
	
	if (useFpsLimit) {

		// FPS Limiter
		if (fpsLimit < 1) {
			return;
		}

		double waitTime = 1000 / fpsLimit - lastFrameRenderDuration;

		if (waitTime< 0) {
			return;
		}


		Sleep(DWORD(floor(waitTime)));
		lastFrameRenderDuration = lastFrameRenderDuration + 1000 * getElapsedTime();
	}
	

	double fps = (lastFrameRenderDuration > 0 ? 1000 / lastFrameRenderDuration : 0);


	// reset real frames when a second passed
	QueryPerformanceCounter((LARGE_INTEGER*)&g_LastFPS);
	double dTimeDiffFPS = 1000 * (((double)(g_LastFPS - g_CurrentFPS)) / ((double)g_Frequency));
	if (dTimeDiffFPS > 1000) {
		// NuLogger::getInstance()->log("FPS: %i", currentFrames);
		QueryPerformanceCounter((LARGE_INTEGER*)&g_CurrentFPS);
		realFramesLastSecond = currentFrames;
		currentFrames = 0;
	}


}


double FPSMeter::getElapsedTime() {
	// meassure frame render performance
	QueryPerformanceCounter((LARGE_INTEGER*)&g_LastCount);
	double dTimeDiff = (((double)(g_LastCount - g_CurentCount)) / ((double)g_Frequency));
	return dTimeDiff;
}

void FPSMeter::setFpsLimit(int limit) {
	fpsLimit = limit;
}


void FPSMeter::enableFpsLimit(bool state) {
	useFpsLimit = state;
}



