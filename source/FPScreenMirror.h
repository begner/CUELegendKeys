#pragma once

#include "Resource.h"
#include "FrameProcessing.h"
#include <algorithm>

using namespace std;

class FPScreenMirror :
	public FrameProcessing
{
private :
	
	// keyboard margins to screen/window size
	int offsetTop = 10;
	int offsetLeft = 10;
	int offsetBottom = 10;
	int offsetRight = 10;
	float offsetScaleX = 1.0;
	float offsetScaleY = 1.0;
	
	int targetWidth = 320;
	int targetHeight = 180;
	int targetX = 100;
	int targetY = 100;

	int keyboardWidth = 0;
	int keyboardHeight = 0;
	
	int overscan = 7;

	boolean initialized = false;


	float keyboardZoomFactor;
	float uiZoom;
	vector<CorsairLedPosition> allKeys;

	HWND currentProcessHWND = NULL;
	HDC captureWindowHDC = NULL;

	Mat4b* mask;
	Mat4b* keyboardFx;
public:
	FPScreenMirror(HWND uiHWND);
	virtual ~FPScreenMirror();
	int getWindowBackgroundResource();
	void reinitialize();
	bool process();
	void setCaptureWindow(HWND currentProcess);
	void setOffset(RECT offset);
	void setUIDimensions(int x, int y, int width, int height);
	void setOffsetScale(float x, float y);
};

