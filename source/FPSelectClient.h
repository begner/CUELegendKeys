#pragma once
#include "Resource.h"
#include "FrameProcessing.h"
#include "FPScreenMirror.h"

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

