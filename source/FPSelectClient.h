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
	bool isNewClient = false;

	
	FPScreenMirror* fpMirror;
	HWND desktopWindow;

public:
	explicit FPSelectClient(HWND uiHWND); 
	~FPSelectClient();
	
	bool process();
	void setNewClient(bool state, bool forceSet = false);
	int getWindowBackgroundResource() override;
	// void setMode(int setMode);
	void setCaptureWindow(HWND currentProcess) override;
	
};

