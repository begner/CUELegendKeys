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
	FPSelectClient(HWND uiHWND);
	~FPSelectClient();

	int getWindowBackgroundResource();
	void setMode(int setMode);
	void setCaptureWindow(HWND currentProcess);
};

