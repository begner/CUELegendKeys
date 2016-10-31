#pragma once
#include "Resource.h"
#include "FPSMeter.h"
#include "FPScreenMirror.h"
#include "Settings.h"

class FPIdle :
	public FPScreenMirror
{
private:
	int mode = 0;


	
	FPScreenMirror* fpMirror;
	HWND desktopWindow;

public:
	explicit FPIdle(HWND uiHWND);
	~FPIdle();

	static CONST int FP_IDLE_MODE_OFF = 0;
	static CONST int FP_IDLE_MODE_SCREEN_MIRROR = 1;
	
	int getWindowBackgroundResource() override;
	bool process() override;
	void setMode(int setMode);
	void setCaptureWindow(HWND currentProcess) override;
};

