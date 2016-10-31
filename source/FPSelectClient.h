#pragma once
#include "Resource.h"
#include "FPSMeter.h"
#include "FPScreenMirror.h"
#include "Settings.h"

class FPSelectClient :
	public FPScreenMirror
{
private:
	int mode = 0;


	
	FPScreenMirror* fpMirror;
	HWND desktopWindow;

public:
	explicit FPSelectClient(HWND uiHWND); 
	~FPSelectClient();
	
	bool process();
	int getWindowBackgroundResource() override;
	// void setMode(int setMode);
	void setCaptureWindow(HWND currentProcess) override;
};

