#include "CUELegendKeys.h"

CUELegendKeys* CUELegendKeys::theInstance = NULL;

CUELegendKeys* CUELegendKeys::getInstance()
{
	if (theInstance == NULL)
		theInstance = new CUELegendKeys();
	return theInstance;
}

void CUELegendKeys::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}


CUELegendKeys::CUELegendKeys()
{
	procL = ProcessList::getInstance();
	
}

CUELegendKeys::~CUELegendKeys()
{
}

bool CUELegendKeys::AppStartupCheck() {
	
	// UIWrapper::getInstance()->DisplayErrorAndQuit("TestH", "TestT");

	NuLogger::getInstance()->log("-------------------------------------------------------");
	NuLogger::getInstance()->log("AppStartupCheck");
	
	procL->makeSnapshot();
	
	// detect active corsair CUE
	if (!procL->ProcessExists("CorsairHID.exe") && 
		!procL->ProcessExists("CUE.exe")) {
		NuLogger::getInstance()->log("Process i can see: \n" + procL->getList(" - ", GetCurrentProcessId()));
		UIMessage::getInstance()->DisplayErrorAndQuit("CUELegendKeys", "Corsair Cue not running!");
		return FALSE;
	};
	
	// detects itself... :(
	if (procL->ProcessExists("CUELegendKeys.exe", GetCurrentProcessId())) {
		NuLogger::getInstance()->log("Process i can see: \n" + procL->getList(" - ", GetCurrentProcessId()));
		UIMessage::getInstance()->DisplayErrorAndQuit("CUELegendKeys", "CUELegendKeys is already running!");
		return FALSE;
	};
	
	NuLogger::getInstance()->log("App started...");

	// detect cue hardware

	if (!LEDController::getInstance()->checkCompatibleDevice()) {
		
		UIMessage::getInstance()->DisplayErrorAndQuit("CUELegendKeys", "Can't detect compatible Hardware!");
		return FALSE;
	}

	if (!Settings::getInstance()->checkSettings()) {
		UIMessage::getInstance()->DisplayErrorAndQuit("CUELegendKeys", "Cant create init File");
		return FALSE;
	}
	;

	// log hardware model...
	CorsairDeviceInfo * di = LEDController::getInstance()->getDeviceInfo();
	const char * model = di->model;
	NuLogger::getInstance()->log("Model Found: %s", di->model);

	vector<CorsairLedPosition> LedPositions = LEDController::getInstance()->getKeyPositions();
	NuLogger::getInstance()->log("Number of Leds found:" + LedPositions.size());

	NuLogger::getInstance()->log("AppStartupCheck Done!");
	NuLogger::getInstance()->log("-------------------------------------------------------");
	
	

	return true;
}

void CUELegendKeys::AppInit()
{
	bool writeLog = (Settings::getInstance()->getValue("main", "WriteLog", (int)0) == 1);
	if (!writeLog) {
		NuLogger::getInstance()->log("Stop Logging cause of ini or default value...\nzzz....");
	}
	NuLogger::getInstance()->writeLogFile(writeLog);

	NuLogger::getInstance()->log("-------------------------------------------------------");
	NuLogger::getInstance()->log("AppInit");
	
	fpIdle = new FPIdle(UIMainDialog::getInstance()->getHandle());
	fpSelectClient = new FPSelectClient(UIMainDialog::getInstance()->getHandle());
	fpGameClient = new FPGameClient(UIMainDialog::getInstance()->getHandle());
	fpPatchClient = new FPPatchClient(UIMainDialog::getInstance()->getHandle());

	NuLogger::getInstance()->log("Load Settings");

	setScreenMirrorOnIdleMode(Settings::getInstance()->getValue("IdleMode", "screenMirror", (int)0) == 1);
	setLimitFPS(Settings::getInstance()->getValue("Main", "limitFPS", (int)0) == 1);
	setShowFilteredMat(Settings::getInstance()->getValue("GameClientMode", "showFilteredMat", (int)0) == 1);
	setForceInGameClient(Settings::getInstance()->getValue("Main", "forceInGameClient", (int)0) == 1);

	NuLogger::getInstance()->log("AppInit Done!");
	NuLogger::getInstance()->log("-------------------------------------------------------");


}

void CUELegendKeys::AppStart() {

	NuLogger::getInstance()->log("-------------------------------------------------------");
	NuLogger::getInstance()->log("AppStart");
	UIMainDialog::getInstance()->Show(true);
	fpIdle->process();
	
	appStarted = true;
	
	while (running) {
		processFrame();
	}
}

void CUELegendKeys::AppStop() {
	NuLogger::getInstance()->log("AppStop");
	running = false;
}

void CUELegendKeys::forceRefresh() {
	forceNextDraw = true;
}

void CUELegendKeys::processFrame(bool forceRecheckProcess) {
	if (!appStarted) {
		return;
	}

	procL->makeSnapshot();

	// Game Client
	DWORD gameClientPID = procL->getPIDofProcess("League of Legends.exe");
	HWND gameClientHWND = NULL;
	BOOL gameClientVisibility = false;
	if (gameClientPID) {
		gameClientHWND = procL->getProcessWindowHandle(gameClientPID, "League of Legends (TM) Client");
		if (!IsWindow(gameClientHWND)) {
			gameClientHWND = NULL;
		}
		gameClientVisibility = IsWindowVisible(gameClientHWND);
	}

	HWND selectClientHWND = NULL;
	BOOL selectClientVisibility = false;
	if (!gameClientVisibility) {
		// selectClient
		DWORD selectClientPID = procL->getPIDofProcess("LolClient.exe");
		
		if (selectClientPID) {
			selectClientHWND = procL->getProcessWindowHandle(selectClientPID, "PVP.net Client");
			if (!selectClientHWND) {
				selectClientHWND = procL->getProcessWindowHandle(selectClientPID, "PvP.net-Client");
			}
			if (!IsWindow(selectClientHWND)) {
				selectClientHWND = NULL;
			}
			selectClientVisibility = IsWindowVisible(selectClientHWND);
		}
	}

	HWND patchClientHWND = NULL;
	BOOL patchClientVisibility = false;
	if (!gameClientVisibility && !selectClientVisibility) {
		// patchClient
		DWORD patchClientPID = procL->getPIDofProcess("LoLPatcherUx.exe");
		
		if (patchClientPID) {
			patchClientHWND = procL->getProcessWindowHandle(patchClientPID, "LoL Patcher");
			if (!IsWindow(patchClientHWND)) {
				patchClientHWND = NULL;
			}
			patchClientVisibility = IsWindowVisible(patchClientHWND);
		}
	}




	if (forceInGameClient) {
		gameClientHWND = GetDesktopWindow();
		gameClientVisibility = true;
	}

	bool drawStatus = false;
	bool showIdle = true;

	
	// if game client
	if (gameClientHWND != NULL && gameClientVisibility) {
		currentMode = CLIENT_TYPE_LOL_INGAME;
		fpGameClient->enableFpsLimit(limitFPS);
		fpGameClient->setCaptureWindow(gameClientHWND);
		drawStatus = fpGameClient->process();
		showIdle = false;
	}

	// if selectClient
	else if (selectClientHWND != NULL && selectClientVisibility) {
		currentMode = CLIENT_TYPE_LOL_SELECT;
		fpSelectClient->enableFpsLimit(limitFPS);
		fpSelectClient->setCaptureWindow(selectClientHWND);
		drawStatus = fpSelectClient->process();
		if (drawStatus) showIdle = false;
	}
	// if patchClient
	else if (patchClientHWND != NULL && patchClientVisibility) {
		currentMode = CLIENT_TYPE_LOL_PATCH;
		fpPatchClient->enableFpsLimit(limitFPS);
		fpPatchClient->setCaptureWindow(patchClientHWND);
		drawStatus = fpPatchClient->process();
		if (drawStatus) showIdle = false;
	}

	if (showIdle) {
		currentMode = CLIENT_TYPE_NONE;
		if (screenMirrorOnIdleMode) {
			fpIdle->enableFpsLimit(limitFPS);
			fpIdle->setMode(FPIdle::FP_IDLE_MODE_SCREEN_MIRROR);
		}
		else {
			fpIdle->setMode(FPIdle::FP_IDLE_MODE_OFF);
		}

		fpIdle->process();
	}
}

bool CUELegendKeys::getForceInGameClient() {
	return forceInGameClient;
}

void CUELegendKeys::setForceInGameClient(bool state) {
	NuLogger::getInstance()->log("setForceInGameClient: %d", state);
	Settings::getInstance()->setValue("Main", "forceInGameClient", (int)state);
	forceInGameClient = state;
}


bool CUELegendKeys::getScreenMirrorOnIdleMode() {
	return screenMirrorOnIdleMode;
}

void CUELegendKeys::setScreenMirrorOnIdleMode(bool state) {
	NuLogger::getInstance()->log("setScreenMirrorOnIdleMode: %d", state);
	Settings::getInstance()->setValue("IdleMode", "screenMirror", (int)state);
	screenMirrorOnIdleMode = state;
}

bool CUELegendKeys::getLimitFPS() {
	return limitFPS;
}

void CUELegendKeys::setLimitFPS(bool state) {
	NuLogger::getInstance()->log("setLimitFPS: %d", state);
	Settings::getInstance()->setValue("Main", "limitFPS", (int)state);
	limitFPS = state;
}

bool CUELegendKeys::getShowFilteredMat() {
	return fpGameClient->getShowFilteredMat();
}

void CUELegendKeys::setShowFilteredMat(bool state) {
	NuLogger::getInstance()->log("setShowFilteredMat: %d", state);
	Settings::getInstance()->setValue("GameClientMode", "showFilteredMat", (int)state);
	fpGameClient->setShowFilteredMat(state);
}


void CUELegendKeys::quit() {
	UIMainWindow::getInstance()->destroy();
	PostQuitMessage(0);
}


bool CUELegendKeys::isIngameMode() {
	return (currentMode == CLIENT_TYPE_LOL_INGAME || getForceInGameClient());
}


