#pragma once

// #include "stdafx.h"
// #include "Resource.h"
#include <windows.h>
#include "UIMessage.h"
#include "NuLogger.h"
#include "ProcessList.h"
#include "FPIdle.h"
#include "FPLearnMode.h"
#include "FPSelectClient.h"
#include "FPPatchClient.h"
#include "FPGameClient.h"
#include "LEDController.h"
#include "Settings.h"


class CUELegendKeys
{
	
private:
	static CUELegendKeys * theInstance;
	CUELegendKeys();
	~CUELegendKeys();

	FPIdle* fpIdle;
	FPLearnMode* fpLearn;
	FPSelectClient* fpSelectClient;
	FPPatchClient* fpPatchClient;

	FPGameClient* fpGameClient;
	
	bool forceInGameClient = false;
	bool screenMirrorOnIdleMode = false;
	bool limitFPS = false;

	bool appStarted = false;
	bool running = true;
	bool forceNextDraw = false;
	
	FrameProcessing* frameProcessing;
	INT currentClientType = 0;
	HWND currentProcessHWND = NULL;
	
	static const int CLIENT_TYPE_NONE = 0;
	static const int CLIENT_TYPE_LOL_SELECT = 1;
	static const int CLIENT_TYPE_LOL_INGAME = 2;

	ProcessList* procL;

public:
	static CUELegendKeys * getInstance();
	static void release();
	
	bool AppStartupCheck();

	void AppInit();

	void AppStart();
	void AppStop();

	void processFrame(bool forceRecheckProcess = false);
	void forceRefresh();
	
	bool getForceInGameClient();
	void setForceInGameClient(bool state);

	bool getScreenMirrorOnIdleMode();
	void setScreenMirrorOnIdleMode(bool state);

	bool getLimitFPS();
	void setLimitFPS(bool state);

	void setShowFilteredMat(bool state);
	bool getShowFilteredMat();

	void openMapView();

	void quit();
};

