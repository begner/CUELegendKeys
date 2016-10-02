#include "FPGameClient.h"



FPGameClient::FPGameClient(HWND uiHWND) : FrameProcessing(uiHWND)
{

	// ***********************************************************
	// Creating HotSpotGroup... :)
	// ***********************************************************
 	hotSpotGroup = new HotSpotGroup();	
	hotSpotGroup->setReferenceX(1920);
	hotSpotGroup->setReferenceY(1080);
	hotSpotGroup->setLolUIScale(100);

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
	hsAbilityQ.setCaptureCoordinates(195, 22, 60, 61);
	hsAbilityQ.setUiCoordinates(18 + iconPositionX * xSpace, iconPositionY);
	hsAbilityQ.setBorder(4);
	hsAbilityQ.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityQ);
	
	iconPositionX++;
	hsAbilityW.addKey(CLK_W);
	hsAbilityW.setCaptureCoordinates(262, 22, 60, 61);
	hsAbilityW.setUiCoordinates(18 + iconPositionX * xSpace, iconPositionY);
	hsAbilityW.setBorder(4);
	hsAbilityW.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityW);
	
	iconPositionX++;
	hsAbilityE.addKey(CLK_E);
	hsAbilityE.setCaptureCoordinates(328, 22, 60, 61);
	hsAbilityE.setUiCoordinates(18 + iconPositionX * xSpace, iconPositionY);
	hsAbilityE.setBorder(4);
	hsAbilityE.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityE);
	
	iconPositionX++;
	hsAbilityR.addKey(CLK_R);
	hsAbilityR.setCaptureCoordinates(395, 22, 60, 61);
	hsAbilityR.setUiCoordinates(18 + iconPositionX * xSpace, iconPositionY);
	hsAbilityR.setBorder(4);
	hsAbilityR.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityR);

	// Summoner Spells D, F, B
	// ***********************************************************
	
	iconPositionX = 0;
	iconPositionY = 403;
	xSpace = 80;
	cPreviewPositionY = 479;
	
	hsAbilityD.addKey(CLK_D);
	hsAbilityD.setCaptureCoordinates(470, 22, 46, 46);
	hsAbilityD.setUiCoordinates(24 + iconPositionX * xSpace, iconPositionY);
	hsAbilityD.setBorder(4);
	hsAbilityD.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityD);

	iconPositionX++;
	hsAbilityF.addKey(CLK_F);
	hsAbilityF.setCaptureCoordinates(520, 22, 46, 46);
	hsAbilityF.setUiCoordinates(25 + iconPositionX * xSpace, iconPositionY);
	hsAbilityF.setBorder(4);
	hsAbilityF.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsAbilityF);
	
	iconPositionX++;
	hsAbilityB.addKey(CLK_B);
	hsAbilityB.setCaptureCoordinates(745, 74, 37, 37);
	hsAbilityB.setUiCoordinates(28 + iconPositionX * xSpace, 4+iconPositionY);
	hsAbilityB.addPreviewColorCoordinates(46 + iconPositionX * xSpace, cPreviewPositionY);
	hsAbilityB.setBackportDetectionCaptureCoordinates(1920 / 2 - 204 - 1, 1080 - 287, 419, 16);
	hsAbilityB.setBorder(2);
	hsAbilityB.deleteFilteredMatRect(0, 32, 5, 5);

	hotSpotGroup->addHotSpot(&hsAbilityB);
	

	// Items 1, 2, 3, 4, 5, 6, 7
	// ***********************************************************
	iconPositionX = 0;
	iconPositionY = 182;
	xSpace = 60;
	cPreviewPositionY = 249;
	
	hsItem1.addKey(CLK_1);
	hsItem1.setCaptureCoordinates(596, 21, 43, 43);
	hsItem1.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY);
	hsItem1.setBorder(4);
	hsItem1.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem1);
	
	iconPositionX++;
	hsItem2.addKey(CLK_2);
	hsItem2.setCaptureCoordinates(645, 21, 43, 43);
	hsItem2.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY);
	hsItem2.setBorder(4);
	hsItem2.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem2);
	
	iconPositionX++;
	hsItem3.addKey(CLK_3);
	hsItem3.setCaptureCoordinates(694, 21, 43, 43);
	hsItem3.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY);
	hsItem3.setBorder(4);
	hsItem3.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem3);
	
	iconPositionX++;
	hsItem4.addKey(CLK_4);
	hsItem4.setCaptureCoordinates(744, 25, 37, 37);
	hsItem4.setUiCoordinates(371 + iconPositionX * xSpace, iconPositionY+6);
	hsItem4.setBorder(4);
	hsItem4.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem4);

	iconPositionX++;
	hsItem5.addKey(CLK_5);
	hsItem5.setCaptureCoordinates(595, 68, 43, 43);
	hsItem5.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY);
	hsItem5.setBorder(4);
	hsItem5.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem5);

	iconPositionX++;
	hsItem6.addKey(CLK_6);
	hsItem6.setCaptureCoordinates(645, 68, 43, 43);
	hsItem6.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY);
	hsItem6.setBorder(4);
	hsItem6.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem6);

	iconPositionX++;
	hsItem7.addKey(CLK_7);
	hsItem7.setCaptureCoordinates(693, 68, 43, 43);
	hsItem7.setUiCoordinates(368 + iconPositionX * xSpace, iconPositionY);
	hsItem7.setBorder(4);
	hsItem7.addPreviewColorCoordinates(384 + iconPositionX * xSpace, cPreviewPositionY);
	hotSpotGroup->addHotSpot(&hsItem7);
	
	// Resource Bars
	// ***********************************************************

	int barMinPosX = 348;
	int barMaxPosX = barMinPosX + 414;
	hsHealBar.setCaptureCoordinates(148, 105, 414, 16);
	hsHealBar.setUiCoordinates(348, 371);
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
	
	hsManaBar.setCaptureCoordinates(148, 125, 414, 16);
	hsManaBar.setUiCoordinates(348, 463);
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
	
	hsChamp.setCaptureCoordinates(0, 15, 123, 123);
	hsChamp.setUiCoordinates(8, 12);
	hotSpotGroup->addHotSpot(&hsChamp);
	
	// Passive
	// ***********************************************************

	hsPassive.addKey(CLK_Enter);
	hsPassive.setCaptureCoordinates(146, 23, 44, 44);
	hsPassive.setUiCoordinates(150, 12);
	hsPassive.setBorder(4);
	hsPassive.addPreviewColorCoordinates(150, 70);
	hotSpotGroup->addHotSpot(&hsPassive);


	loadingDetectionTemplate = ImageFilterMat::loadResourceAsMat(IDB_LOADING_DETECTION_TEMPLATE);

}


FPGameClient::~FPGameClient()
{
}


int FPGameClient::getWindowBackgroundResource() {
	return IDB_WINDOW_BACKGROUND_INGAME;
}


void FPGameClient::setCaptureWindow(HWND currentProcess) {
	if (currentProcess != currentProcessHWND) {
		currentProcessHWND = currentProcess;
		if (gameClientHDC != nullptr) {
			DeleteObject(gameClientHDC);
		}
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

	Mat* screenshotRaw = ImageFilterMat::hdc2mat(gameClientHDC, 0, 0, clientWidth, clientHeight);
	if (screenshotRaw == nullptr) {
		return false;
	}
	/*
	Mat* screenshotRaw = ImageFilterMat::hdc2mat(desktopHDC, 0, 0, nScreenWidth, nScreenHeight);
	if (screenshotRaw == NULL) {
		return false;
	}
	*/
	Mat4b screenshotMat = static_cast<Mat4b>(*screenshotRaw);
	if (screenshotMat.cols < nScreenWidth && screenshotMat.rows < nScreenHeight) {
		delete screenshotRaw;
		return false;
	}

	
	// copy background to UI
	windowBackground.copyTo(drawUI);

	PerformanceDraw();
	bool isLoading = false;


	bool success = false;
	if (isLoading) {
		success = processLoading(screenshotMat);
	}
	else {
		success = processInGame(screenshotMat);
	}
	

	// draw the UI
	ImageFilterMat::DrawToHDC(drawHDC, drawUI, 0, 0, windowBackground.cols, windowBackground.rows);


	// double buffer write
	BitBlt(windowHDC, 0, 0, uiWidth, uiHeight, drawHDC, 0, 0, SRCCOPY);

	// throwing away pointer, so opencv releases memory
	delete screenshotRaw;

	PerformanceStop();
	return success;
}


// ReSharper disable once CppMemberFunctionMayBeStatic
bool FPGameClient::processLoading(Mat4b screenshotMat) {
	
	return true;
}

bool FPGameClient::processInGame(Mat4b screenshotMat) {
	

	// Grab UIBar from Screenshot
	cv::Rect uiBarRect(uiBarX, uiBarY, uiBarWidth, uiBarHeight);
	Mat4b uiBarMat = Mat(screenshotMat, uiBarRect);

	// iterate all hotSpots	
	vector<ScreenHotSpot*> hsg = hotSpotGroup->getHotSpots();


	// vector<HANDLE> multiThreadHandle;

	bool exclusiveMode = false;


	// add current screenshot data to hotSpots
	// and execute exclusive Effects
	for (vector<ScreenHotSpot*>::iterator it = hsg.begin(); it != hsg.end(); ++it) {
		ScreenHotSpot* hs = (*it);

		hs->setUIMat(&uiBarMat);
		hs->setScreenshotMat(&screenshotMat);
		hs->initializeFrame();

		if (!exclusiveMode && hs->hasExclusiveEffect()) {
			hs->doExclusiveEffect();
			exclusiveMode = true;
		}
	}



	if (!exclusiveMode) {

		for (vector<ScreenHotSpot*>::iterator it = hsg.begin(); it != hsg.end(); ++it) {

			ScreenHotSpot* hs = (*it);

			if (showFilteredMat) {
				ImageFilterMat::overlayImage(&drawUI, hs->getFilteredMat(), cv::Point(hs->getUiX(), hs->getUiY()));
			}
			else {
				ImageFilterMat::overlayImage(&drawUI, hs->getOriginalMatRespectBorders(), cv::Point(hs->getUiX(), hs->getUiY()));
			}
			
			

			vector<cv::Rect>* previewColors = hs->getPreviewColors();
			int index = 0;
			for (vector<cv::Rect>::iterator pC = previewColors->begin(); pC != previewColors->end(); ++pC, ++index) {
				ImageFilterMat::overlayImage(&drawUI, hs->getPreviewColorMat(index), cv::Point(pC->x, pC->y));
			}

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


