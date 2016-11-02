#include "FPGameClient.h"
#include "UILearn.h"

FPGameClient::FPGameClient(HWND uiHWND) : FPSMeter(), FPBase(uiHWND)
{
	NuLogger::getInstance()->log("Setup FrameProcessing GameClient");

	// ***********************************************************
	// Creating HotSpotGroup... :)
	// ***********************************************************
 	hotSpotGroup = new HotSpotGroup();	

	// setting sub UI Bar
	uiBarWidth = 850;
	uiBarHeight = 155;
	uiBarX = 1920 / 2 - uiBarWidth / 2;
	uiBarY = 1080 - 155;

	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	// Abilities Q, W, E, R
	// ***********************************************************
	int iconPositionX = 0;
	int iconPositionY = 181;
	int xSpace = 80;
	int cPreviewPositionY = 275;
	

	hsAbilityQ.addKey(CLK_Q);
	hsAbilityQ.setUiCoordinates(18 + iconPositionX * xSpace, iconPositionY, 52, 53);
	hsAbilityQ.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityQ);
	
	iconPositionX++;
	hsAbilityW.addKey(CLK_W);
	hsAbilityW.setUiCoordinates(18 + iconPositionX * xSpace, iconPositionY, 52, 53);
	hsAbilityW.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityW);
	
	iconPositionX++;
	hsAbilityE.addKey(CLK_E);
	hsAbilityE.setUiCoordinates(18 + iconPositionX * xSpace, iconPositionY, 52, 53);
	hsAbilityE.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityE);
	
	iconPositionX++;
	hsAbilityR.addKey(CLK_R);
	hsAbilityR.setUiCoordinates(18 + iconPositionX * xSpace, iconPositionY, 52, 53);
	hsAbilityR.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityR);

	// Summoner Spells D, F, B
	// ***********************************************************
	
	iconPositionX = 0;
	iconPositionY = 403;
	xSpace = 80;
	cPreviewPositionY = 479;
	
	hsAbilityD.addKey(CLK_D);
	hsAbilityD.setUiCoordinates(24 + iconPositionX * xSpace, iconPositionY, 38, 38);
	hsAbilityD.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityD);

	iconPositionX++;
	hsAbilityF.addKey(CLK_F);
	hsAbilityF.setUiCoordinates(25 + iconPositionX * xSpace, iconPositionY, 38, 38);
	hsAbilityF.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityF);
	
	iconPositionX++;
	hsAbilityB.addKey(CLK_B);
	hsAbilityB.setUiCoordinates(28 + iconPositionX * xSpace, 4+iconPositionY, 33, 33);
	hsAbilityB.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hsAbilityB.setBackportDetectionCaptureCoordinates(1920 / 2 - 204 - 1, 1080 - 287, 419, 16);
	hsAbilityB.deleteFilteredMatRect(0, 32, 5, 5);

	hotSpotGroup->addHotSpot(&hsAbilityB);
	

	// Items 1, 2, 3, 4, 5, 6, 7
	// ***********************************************************
	iconPositionX = 0;
	iconPositionY = 182;
	xSpace = 60;
	cPreviewPositionY = 249;
	
	hsItem1.addKey(CLK_1);
	hsItem1.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY, 35, 35);
	hsItem1.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem1);
	
	iconPositionX++;
	hsItem2.addKey(CLK_2);
	hsItem2.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY, 35, 35);
	hsItem2.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem2);
	
	iconPositionX++;
	hsItem3.addKey(CLK_3);
	hsItem3.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY, 35, 35);
	hsItem3.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem3);
	
	iconPositionX++;
	hsItem4.addKey(CLK_4);
	hsItem4.setUiCoordinates(371 + iconPositionX * xSpace, iconPositionY+6, 29, 29);
	hsItem4.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem4);

	iconPositionX++;
	hsItem5.addKey(CLK_5);
	hsItem5.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY, 35, 35);
	hsItem5.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem5);

	iconPositionX++;
	hsItem6.addKey(CLK_6);
	hsItem6.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY, 35, 35);
	hsItem6.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem6);

	iconPositionX++;
	hsItem7.addKey(CLK_7);
	hsItem7.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY, 35, 35);
	hsItem7.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem7);
	
	// Resource Bars
	// ***********************************************************

	int barMinPosX = 348;
	int barMaxPosX = barMinPosX + 414;
	hsHealBar.setUiCoordinates(348, 371, 414, 16);
	hsHealBar.addKey(CLK_F1);
	hsHealBar.addKey(CLK_F2);
	hsHealBar.addKey(CLK_F3);
	hsHealBar.addKey(CLK_F4);
	hsHealBar.addKey(CLK_F5);
	hsHealBar.addKey(CLK_F6);
	hsHealBar.addKey(CLK_F7);
	hsHealBar.addKey(CLK_F8);
	for (int i = 0; i < hsHealBar.getKeys()->size(); i++) {
		hsHealBar.addPreviewColorCoordinates(barMinPosX + static_cast<int>(floor((barMaxPosX - barMinPosX) / hsHealBar.getKeys()->size() * (i+0.5))), 418);
	}
	hotSpotGroup->addHotSpot(&hsHealBar);
	
	hsManaBar.setUiCoordinates(348, 463, 414, 16);
	hsManaBar.addKey(CLK_F9);
	hsManaBar.addKey(CLK_F10);
	hsManaBar.addKey(CLK_F11);
	hsManaBar.addKey(CLK_F12);
	hsManaBar.addKey(CLK_PrintScreen);
	hsManaBar.addKey(CLK_ScrollLock);
	hsManaBar.addKey(CLK_PauseBreak);
	for (int i = 0; i < hsManaBar.getKeys()->size(); i++) {
		hsManaBar.addPreviewColorCoordinates(barMinPosX + static_cast<int>(floor((barMaxPosX - barMinPosX) / hsManaBar.getKeys()->size() * (i + 0.5))), 510);
	}
	hotSpotGroup->addHotSpot(&hsManaBar);
	
	// Champ
	// ***********************************************************
	hsChamp.setUiCoordinates(8, 12, 123, 123);
	hotSpotGroup->addHotSpot(&hsChamp);
	
	// Passive
	// ***********************************************************

	hsPassive.addKey(CLK_Enter);
	hsPassive.setUiCoordinates(150, 12, 44, 44);
	hsPassive.addPreviewColorCoordinates(150, 70);
	hotSpotGroup->addHotSpot(&hsPassive);




loadHSSettings();

// loadingDetectionTemplate = ImageFilterMat::loadResourceAsMat(IDB_LOADING_DETECTION_TEMPLATE);

}


FPGameClient::~FPGameClient()
{
}

int FPGameClient::getWindowBackgroundResource() {
	return IDB_WINDOW_BACKGROUND_INGAME;
}


void FPGameClient::setCaptureWindow(HWND currentProcess) {
	if (currentProcess != currentProcessHWND) {
		ReleaseDC(currentProcessHWND, gameClientHDC);
		if (gameClientHDC != nullptr) {
			DeleteObject(gameClientHDC);
		}

		currentProcessHWND = currentProcess;
		gameClientHDC = GetDC(currentProcessHWND);
	}
}


bool FPGameClient::process() {


	if (!gameClientHDC || !currentProcessHWND) {
		return false;
	}
	if (nScreenWidth < 1 && nScreenHeight < 1) {
		return false;
	}

	setFpsLimit(Settings::getInstance()->getValue("GameClientMode", "FPSLimit", (int)15));
	PerformanceStart();

	LEDController::getInstance()->initializeFrame();

	RECT targetRect;
	GetWindowRect(currentProcessHWND, &targetRect);

	int clientWidth = targetRect.right - targetRect.left;
	int clientHeight = targetRect.bottom - targetRect.top;

	Mat4b* screenshotRaw = static_cast<Mat4b*>(ImageFilterMat::hdc2mat(gameClientHDC, 0, 0, clientWidth, clientHeight));
	if (screenshotRaw == nullptr) {
		return false;
	}

	// copy background to UI
	getBackgroundMat()->copyTo(drawUI);

	PerformanceDraw(getBackgroundMat()->cols - 130, 20);
	bool isLoading = false;


	bool success = false;
	if (isLoading) {
		success = processLoading(screenshotRaw);
	}
	else {
		success = processInGame(screenshotRaw);
	}


	drawToWindow(&drawUI);
	
	if (UILearn::getInstance()->isVisible()) {
		UILearn::getInstance()->getLearnController()->setScreenShot(screenshotRaw);

		if (UILearn::getInstance()->getLearnController()->areThereSavedChanges()) {
			loadHSSettings();
		}

//		UILearn::getInstance()->updateLcUI();
	}


	// throwing away pointer, so opencv releases memory
	delete screenshotRaw;

	PerformanceStop();
	return success;
}

void FPGameClient::loadHSSettings() {

	map<string, ScreenHotSpot*> loadMap;
	loadMap["Passive"] = &hsPassive;
	loadMap["AbilityQ"] = &hsAbilityQ;
	loadMap["AbilityW"] = &hsAbilityW;
	loadMap["AbilityE"] = &hsAbilityE;
	loadMap["AbilityR"] = &hsAbilityR;
	loadMap["SummonerD"] = &hsAbilityD;
	loadMap["SummonerF"] = &hsAbilityF;
	loadMap["SummonerB"] = &hsAbilityB;
	loadMap["Item1"] = &hsItem1;
	loadMap["Item2"] = &hsItem2;
	loadMap["Item3"] = &hsItem3;
	loadMap["Item4"] = &hsItem4;
	loadMap["Item5"] = &hsItem5;
	loadMap["Item6"] = &hsItem6;
	loadMap["Item7"] = &hsItem7;
	// loadMap["Champ"] = &hsChamp;
	loadMap["BarHeal"] = &hsHealBar;
	loadMap["BarMana"] = &hsManaBar;

	

	LearnController* lc = UILearn::getInstance()->getLearnController();

	for (auto iterator = loadMap.begin(); iterator != loadMap.end(); iterator++) {
		string saveId = iterator->first;
		ScreenHotSpot* currentSkill = iterator->second;
		
		if (lc->loadDataExists(saveId)) {
			cv::Rect loadRect = lc->getSavedLocation(saveId);
			currentSkill->setCaptureCoordinates(loadRect.x, loadRect.y, loadRect.width, loadRect.height);
			currentSkill->setBorder(lc->getBorderSize(saveId));
		}
	}
}



// ReSharper disable once CppMemberFunctionMayBeStatic
bool FPGameClient::processLoading(Mat4b* screenshotMat) {
	
	return true;
}

cv::Rect FPGameClient::calcUiBarLocation(Mat4b* screenshotMat) {
	
	cv::Point topLeft = cv::Point(screenshotMat->cols, screenshotMat->rows);
	cv::Point bottomRight = cv::Point(0, 0);
	cv::Rect location(0, 0, 0, 0);

	vector<ScreenHotSpot*> hsg = hotSpotGroup->getHotSpots();
	for (vector<ScreenHotSpot*>::iterator it = hsg.begin(); it != hsg.end(); ++it) {
		ScreenHotSpot* hs = (*it);
		if (!ImageFilterMat::isValidRect(cv::Rect(0, 0, hs->getCaptureWidth(false), hs->getCaptureHeight(false)))) {
			continue;
		}

		topLeft.x = min(topLeft.x, hs->getCaptureX(false, true));
		topLeft.y = min(topLeft.y, hs->getCaptureY(false, true));
		
		bottomRight.x = max(bottomRight.x, hs->getCaptureX(false, true) + hs->getCaptureWidth());
		bottomRight.y = max(bottomRight.y, hs->getCaptureY(false, true) + hs->getCaptureHeight());

		#ifdef _DEBUG
			location = cv::Rect(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
			Mat4b locationTest = Mat(*screenshotMat, location);
		#endif // _DEBUG
	}

	location = cv::Rect(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, bottomRight.y - topLeft.y);
	return location;
}

bool FPGameClient::processInGame(Mat4b* screenshotMat) {
	

	// Grab UIBar from Screenshot
	cv::Rect uiBarRect = calcUiBarLocation(screenshotMat);
	if (!ImageFilterMat::isValidRect(uiBarRect)) {
		return false;
	}
	Mat4b uiBarMat = Mat(*screenshotMat, uiBarRect);


	// iterate all hotSpots	
	vector<ScreenHotSpot*> hsg = hotSpotGroup->getHotSpots();

	for (vector<ScreenHotSpot*>::iterator it = hsg.begin(); it != hsg.end(); ++it) {
		ScreenHotSpot* hs = (*it);
		hs->setUiMatOffset(uiBarRect.x, uiBarRect.y);
		hs->setUIMat(&uiBarMat);
		hs->setScreenshotMat(screenshotMat);
	}

	bool exclusiveMode = false;

	// add current screenshot data to hotSpots
	// and execute exclusive Effects
	for (vector<ScreenHotSpot*>::iterator it = hsg.begin(); it != hsg.end(); ++it) {
		ScreenHotSpot* hs = (*it);

	
		hs->initializeFrame();

		if (!exclusiveMode && hs->hasExclusiveEffect()) {
			hs->doExclusiveEffect();
			exclusiveMode = true;
		}
	}



	if (!exclusiveMode) {

		for (vector<ScreenHotSpot*>::iterator it = hsg.begin(); it != hsg.end(); ++it) {

			ScreenHotSpot* hs = (*it);

			
			Mat displayMat;

			if (showFilteredMat) {
				hs->getFilteredMat()->copyTo(displayMat);
			}
			else {
				hs->getOriginalMatRespectBorders()->copyTo(displayMat);
			}

			cv::Size uiElementSize(hs->getUiWidth(), hs->getUiHeight());

			if (displayMat.cols > 0 && displayMat.rows > 0) {
				resize(displayMat, displayMat, uiElementSize, 0, 0, INTER_CUBIC);
			}
			else {
				displayMat = Mat(hs->getUiWidth(), hs->getUiHeight(), CV_8UC3, Scalar(0, 128, 255));
			}

			ImageFilterMat::overlayImage(&drawUI, &displayMat, cv::Point(hs->getUiX(), hs->getUiY()));
			
	
			// increase tick
			hs->tick();

			// update key color
			hs->updateKey();

		}
	}


	LEDController::getInstance()->updateFrame();
	return true;
}

void FPGameClient::setShowFilteredMat(bool state) {
	showFilteredMat = state;
}

bool FPGameClient::getShowFilteredMat() {
	return showFilteredMat;
}


