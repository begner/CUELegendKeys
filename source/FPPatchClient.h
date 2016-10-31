#pragma once
#include "Resource.h"
#include "FPSMeter.h"
#include "FPScreenMirror.h"
#include "Settings.h"

class FPPatchClient :
	public FPScreenMirror
{
private:
	int mode = 0;



	FPScreenMirror* fpMirror;
	HWND desktopWindow;

public:
	explicit FPPatchClient(HWND uiHWND);
	~FPPatchClient();

	bool process();
	int getWindowBackgroundResource() override;
	// void setMode(int setMode);
	void setCaptureWindow(HWND currentProcess) override;
};

