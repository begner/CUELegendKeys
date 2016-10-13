#pragma once
#include "FPIdle.h"
class FPLearnMode :
	public FPIdle
{
public:
	explicit FPLearnMode(HWND uiHWND);
	~FPLearnMode();

	int getWindowBackgroundResource() override;
	bool process() override;
};

