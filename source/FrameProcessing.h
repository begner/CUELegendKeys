#pragma once

#include "Constants.h"

#include <Windows.h>
#include <vector>

#include "CUESDK.h"
#include "Settings.h"

#include "ImageFilterMat.h"



class FrameProcessing
{
protected:
	// ui draw
	HDC windowHDC;
	HDC drawHDC;
	int uiWidth = 0;
	int uiHeight = 0;

	Mat4b drawUI;

	void PerformanceStart();
	void PerformanceDraw();
	void PerformanceStop();
	
	Mat4b windowBackground;
	
	
private:
	// fps / performance
	LONGLONG g_Frequency, g_CurentCount, g_LastCount = 0;
	LONGLONG g_CurrentFPS, g_LastFPS = 0;
	int currentFrames = 0;
	int realFramesLastSecond = 0;
	double lastFrameRenderDuration = 0;

	int fpsLimit = 0;
	bool useFpsLimit = true;

	// CUE
	vector<CorsairLedPosition> LedPositions;

public:
	FrameProcessing(HWND uiHWND);
	virtual ~FrameProcessing();
	virtual int getWindowBackgroundResource() = 0;
	virtual void setCaptureWindow(HWND currentProcess) = 0;
	virtual bool process() = 0;

	double getElapsedTime();
	void setFpsLimit(int fps);
	void enableFpsLimit(bool state);
	

	void loadResources();
};

