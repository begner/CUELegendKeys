#pragma once
#include "FrameProcessing.h"


#include "Resource.h"
#include "HotSpotGroup.h"
#include "HSCastable.h"
#include "HSBackport.h"
#include "HSBar.h"
#include "HSChamp.h"
#include "HSPassive.h"



class FPGameClient :
	public FrameProcessing
{
private:
	Mat4b loadingDetectionTemplate;


	HWND currentProcessHWND = nullptr;
	HDC gameClientHDC = nullptr;
	int nScreenWidth = 0;
	int nScreenHeight = 0;

	bool showFilteredMat = false;

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
	HSPassive hsPassive;

	HotSpotGroup *hotSpotGroup;

	int uiBarWidth = 0;
	int uiBarHeight = 0;
	int uiBarX = 0;
	int uiBarY = 0;

	cv::Rect calcUiBarLocation(Mat4b* screenshotMat);
	bool processLoading(Mat4b screenShot);
	bool processInGame(Mat4b screenShot);
	void loadHSSettings();
public:
	explicit FPGameClient(HWND uiHWND);
	~FPGameClient();
	bool process() override;
	int getWindowBackgroundResource() override;
	void setCaptureWindow(HWND currentProcess) override;
	void setShowFilteredMat(bool state);
	bool getShowFilteredMat();
};

