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
}

CUELegendKeys::~CUELegendKeys()
{
}

bool CUELegendKeys::AppStartupCheck() {
	
	// UIWrapper::getInstance()->DisplayErrorAndQuit("TestH", "TestT");

	NuLogger::getInstance()->log("-------------------------------------------------------");
	NuLogger::getInstance()->log("AppStartupCheck");
	
	// detect active corsair CUE
	if (!ProcessList::getInstance()->ProcessExists("CorsairHID.exe")) {
		UIMessage::getInstance()->DisplayErrorAndQuit("CUELegendKeys", "Corsair Cue not running!");
		return FALSE;
	};

	// detects itself... :(
	if (ProcessList::getInstance()->ProcessExists("CUELegendKeys.exe", GetCurrentProcessId())) {
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
		UIMessage::getInstance()->DisplayErrorAndQuit("CUELegendKeys", "Cant create registry keys for settings");
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
	
	

	return true;
}

void CUELegendKeys::AppInit()
{
	NuLogger::getInstance()->log("-------------------------------------------------------");
	NuLogger::getInstance()->log("AppInit Done");
	fpIdle = new FPIdle(UIMainDialog::getInstance()->getHandle());
	fpIdle->loadResources();
	fpSelectClient = new FPSelectClient(UIMainDialog::getInstance()->getHandle());
	fpSelectClient->loadResources();
	fpGameClient = new FPGameClient(UIMainDialog::getInstance()->getHandle());
	fpGameClient->loadResources();

	NuLogger::getInstance()->log("AppInit Done!");
	
	Settings::getInstance()->setValue("main", "appinit", "test");
	Settings::getInstance()->commit();
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
	NuLogger::getInstance()->log("AppStop.");
	running = false;
}

void CUELegendKeys::forceRefresh() {
	forceNextDraw = true;
}

void CUELegendKeys::processFrame(bool forceRecheckProcess) {
	if (!appStarted) {
		return;
	}
	
	// Select Client
	ProcessList* procL = ProcessList::getInstance();

	DWORD selectClientPID = procL->getPIDofProcess("LolClient.exe");
	HWND selectClientHWND = NULL;
	BOOL selectClientVisibility = false;
	if (selectClientPID) {
		selectClientHWND = procL->getProcessWindowHandle(selectClientPID, "PvP.net-Client");
		if (!IsWindow(selectClientHWND)) {
			selectClientHWND = NULL;
		}
		selectClientVisibility = IsWindowVisible(selectClientHWND);
	}

	// Game Client
	DWORD gameClientPID = procL->getPIDofProcess("League of Legends.exe");
	HWND gameClientHWND = NULL;
	BOOL gameClientVisibility = false;
	if (gameClientPID) {
		gameClientHWND = procL->getProcessWindowHandle(gameClientPID, "League of Legends (TM) Client");
		if (!IsWindow(gameClientHWND)) {
			gameClientHWND = NULL;
		}
		gameClientVisibility = true; // IsWindowVisible(gameClientHWND);
	}
	
	if (forceInGameClient) {
		gameClientHWND = GetDesktopWindow();
		gameClientVisibility = true;
	}
	
	bool drawStatus = false;
	bool showIdle = true;
	
	// if game client
	if (gameClientHWND != NULL && gameClientVisibility) {
		fpGameClient->setCaptureWindow(gameClientHWND);
		drawStatus = fpGameClient->process();
		if (drawStatus) showIdle = false;
	}
	// if selectClient
	else if (selectClientHWND != NULL && selectClientVisibility) {
		fpSelectClient->setCaptureWindow(selectClientHWND);
		drawStatus = fpSelectClient->process();
		if (drawStatus) showIdle = false;
	}

	// Idle
	if (showIdle) {
		fpIdle->setMode(FPIdle::FP_IDLE_MODE_SCREEN_MIRROR);
		fpIdle->process();
		
	}

}

bool CUELegendKeys::getForceInGameClient() {
	return forceInGameClient;
}

void CUELegendKeys::setForceInGameClient(bool state) {
	forceInGameClient = state;
}

void CUELegendKeys::learnHotSpots() {
	HotSpotLearn hsl;
	hsl.findAllHotSpots();
}
