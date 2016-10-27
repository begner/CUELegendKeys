#pragma once

#include "Resource.h"
#include "FrameProcessing.h"
#include "LEDController.h"

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

	int cutTop = 0;
	int cutLeft = 0;
	int cutBottom = 0;
	int cutRight = 0;



	bool initialized = false;


	float keyboardZoomFactor;
	float uiZoom;
	vector<CorsairLedPosition> allKeys;

	HWND currentProcessHWND = nullptr;
	HDC captureWindowHDC = nullptr;

	Mat4b* mask;
	Mat4b* keyboardFx;
public:
	FPScreenMirror(HWND uiHWND, CorsairLedId keyTL = CLK_Escape, CorsairLedId keyBR = CLK_RightCtrl);
	virtual ~FPScreenMirror();
	int getWindowBackgroundResource() override;
	void reinitialize();
	bool process() override;
	void setCaptureWindow(HWND currentProcess) override;
	void setOffset(RECT offset);
	void setUIDimensions(int x, int y, int width, int height);
	void setOffsetScale(float x, float y);
	void setCutRect(int x, int y, int width, int height);
};

