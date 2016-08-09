#pragma once
#include "Resource.h"
#include "FrameProcessing.h"
#include "FPScreenMirror.h"

class FPIdle :
	public FPScreenMirror
{
private:
	int mode = 0;


	
	FPScreenMirror* fpMirror;
	HWND desktopWindow;

public:
	FPIdle(HWND uiHWND);
	~FPIdle();

	static CONST int FP_IDLE_MODE_OFF = 0;
	static CONST int FP_IDLE_MODE_SCREEN_MIRROR = 1;


	int getWindowBackgroundResource();
	bool process();
	void setMode(int setMode);
	void setCaptureWindow(HWND currentProcess);
};
