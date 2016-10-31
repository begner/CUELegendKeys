#pragma once

#include "ImageFilterMat.h"



class FPSMeter
{
protected:
	// ui draw
	Mat4b drawUI;

	void PerformanceStart();
	void PerformanceDraw(int x, int y);
	void PerformanceStop();
	
	
private:
	LONGLONG g_Frequency, g_CurentCount, g_LastCount = 0;
	LONGLONG g_CurrentFPS, g_LastFPS = 0;
	int currentFrames = 0;
	int realFramesLastSecond = 0;
	double lastFrameRenderDuration = 0;

	int fpsLimit = 0;
	bool useFpsLimit = true;

public:
	FPSMeter();
	virtual ~FPSMeter();

	virtual void setCaptureWindow(HWND currentProcess) = 0;
	virtual bool process() = 0;

	double getElapsedTime();
	void setFpsLimit(int fps);
	void enableFpsLimit(bool state);
	
	void loadResources();
};

