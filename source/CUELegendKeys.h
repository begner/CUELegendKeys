#pragma once

// #include "stdafx.h"
#include "Resource.h"
#include <windows.h>
#include "UIMessage.h"
#include "NuLogger.h"
#include "ProcessList.h"
#include "FPIdle.h"
#include "FPSelectClient.h"
#include "FPGameClient.h"
#include "LEDController.h"
#include "Settings.h"

#include "HotSpotLearn.h"

class CUELegendKeys
{
	
private:
	static CUELegendKeys * theInstance;
	CUELegendKeys();
	~CUELegendKeys();

	FPIdle* fpIdle;
	FPSelectClient* fpSelectClient;
	FPGameClient* fpGameClient;

	bool forceInGameClient = false;
	
	bool appStarted = false;
	bool running = true;
	bool forceNextDraw = false;
	
	FrameProcessing* frameProcessing;
	INT currentClientType = 0;
	HWND currentProcessHWND = NULL;
	
	static const int CLIENT_TYPE_NONE = 0;
	static const int CLIENT_TYPE_LOL_SELECT = 1;
	static const int CLIENT_TYPE_LOL_INGAME = 2;

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
	void learnHotSpots();
	void quit();
};

