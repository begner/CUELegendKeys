#pragma once

#include <Windows.h>
#include <vector>

#include "NuLogger.h"
#include "CUESDK.h"

#include "ImageFilterMat.h"
#include "LEDController.h"

#include "HotSpotGroup.h"



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
	LONGLONG g_Frequency, g_CurentCount, g_LastCount;
	LONGLONG g_CurrentFPS, g_LastFPS;
	int currentFrames = 0;
	int realFramesLastSecond = 0;
	double lastFrameRenderDuration = 0;


	// gdi screengrab data
	
	




	
	

	// CUE
	vector<CorsairLedPosition> LedPositions;

public:
	FrameProcessing(HWND uiHWND);
	~FrameProcessing();
	virtual int getWindowBackgroundResource() = 0;
	virtual void setCaptureWindow(HWND currentProcess) = 0;
	virtual bool process() = 0;
	
	void loadResources();
};

