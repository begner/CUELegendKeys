#pragma once
#include "FrameProcessing.h"


#include "HSCastable.h"
#include "HSBackport.h"
#include "HSBar.h"
#include "HSBackport.h"
#include "HSChamp.h"


class FPGameClient :
	public FrameProcessing
{
private:
	Mat4b loadingDetectionTemplate;

	HWND currentProcessHWND = NULL;
	HDC gameClientHDC = NULL;
	int nScreenWidth = 0;
	int nScreenHeight = 0;


	// LOL Stuff
	HSCastable hsAbilityQ;
	HSCastable hsAbilityW;
	HSCastable hsAbilityE;
	HSCastable hsAbilityR;
	HSCastable hsAbilityD;
	HSCastable hsAbilityF;
	HSBackport hsAbilityB;

	HSCastable hsItem1;
	HSCastable hsItem2;
	HSCastable hsItem3;
	HSCastable hsItem4;
	HSCastable hsItem5;
	HSCastable hsItem6;
	HSCastable hsItem7;

	HSBar hsHealBar;
	HSBar hsManaBar;
	HSChamp hsChamp;

	HotSpotGroup *hotSpotGroup;

	int uiBarWidth = 0;
	int uiBarHeight = 0;
	int uiBarX = 0;
	int uiBarY = 0;


	bool processLoading(Mat4b screenShot);
	bool processInGame(Mat4b screenShot);
public:
	FPGameClient(HWND uiHWND);
	~FPGameClient();
	bool process();
	int getWindowBackgroundResource();
	void setCaptureWindow(HWND currentProcess);

};

