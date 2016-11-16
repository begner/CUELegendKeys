#include "UILearn.h"

UILearn* UILearn::theInstance = NULL;

// extern CUELegendKeys cueLegendKeys;


UILearn* UILearn::getInstance()
{
	if (theInstance == NULL)
		theInstance = new UILearn();
	return theInstance;
}

void UILearn::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

UILearn::UILearn()
{
	NuLogger::getInstance()->log("here");

}

UILearn::~UILearn()
{
}

LearnController* UILearn::getLearnController() {
	return LearnController::getInstance();
}




UIButton* UILearn::createKeyButton(string label, int onClickId, int posX, int posY, int iconResource) {
	UIButton* keyBarButton;
	keyBarButton = new UIButton(IDB_UI_KEY_N, IDB_UI_KEY_NH);
	keyBarButton->addState(UIButton::BUTTON_STATE_EXTRA, IDB_UI_KEY_E, IDB_UI_KEY_EH);
	keyBarButton->addState(UIButton::BUTTON_STATE_DISABLED, IDB_UI_KEY_D, IDB_UI_KEY_D);
	keyBarButton->addState(UIButton::BUTTON_STATE_ACTIVE, IDB_UI_KEY_A, IDB_UI_KEY_AH);
	keyBarButton->setLabel(label);
	keyBarButton->setFontSize(0.35);
	keyBarButton->setOnClickId(onClickId);
	keyBarButton->setPos(posX, posY);
	if (iconResource > -1) {
		keyBarButton->addIcon(iconResource, 10, 10);
	}
	keyBar.push_back(keyBarButton);
	addElement(keyBarButton);
	return keyBarButton;
}


void UILearn::createWindow() {
	HWND hwnd;

	hwnd = CreateDialog(getHInstance(), MAKEINTRESOURCE(IDD_LEARN), UIMainWindow::getInstance()->getHandle(), UILearn::MessageHandler);
	setHandle(hwnd);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(getHInstance(), MAKEINTRESOURCE(IDI_APP_ICON)));

	// setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR);
	initHost();

	setAlwaysOnTop();

	
	// #############################
	// References
	// #############################
	int keyBarY = 60;
	int keyBarCurrentX = 50;
	int keyBarXSpace = 30;
	int keyBarXDivider = 10;


	/*
#define IDI_UI_KEY_PASSIVE					2250
#define IDI_UI_KEY_CHAMP					2251
#define IDI_UI_KEY_HEAL						2252
#define IDI_UI_KEY_MANA						2253
	*/
	createKeyButton("", IDE_CLICK_KEY_PASSIVE, keyBarCurrentX, keyBarY, IDI_UI_KEY_PASSIVE); keyBarCurrentX += keyBarXSpace + keyBarXDivider;
	createKeyButton("Q", IDE_CLICK_KEY_Q, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace;
	createKeyButton("W", IDE_CLICK_KEY_W, keyBarCurrentX, keyBarY);	keyBarCurrentX += keyBarXSpace;
	createKeyButton("E", IDE_CLICK_KEY_E, keyBarCurrentX, keyBarY);	keyBarCurrentX += keyBarXSpace;
	createKeyButton("R", IDE_CLICK_KEY_R, keyBarCurrentX, keyBarY);	keyBarCurrentX += keyBarXSpace + keyBarXDivider;
	createKeyButton("D", IDE_CLICK_KEY_D, keyBarCurrentX, keyBarY);	keyBarCurrentX += keyBarXSpace;
	createKeyButton("F", IDE_CLICK_KEY_F, keyBarCurrentX, keyBarY);	keyBarCurrentX += keyBarXSpace + keyBarXDivider;
	createKeyButton("B", IDE_CLICK_KEY_B, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace + keyBarXDivider;
	createKeyButton("1", IDE_CLICK_KEY_1, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace;
	createKeyButton("2", IDE_CLICK_KEY_2, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace;
	createKeyButton("3", IDE_CLICK_KEY_3, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace;
	createKeyButton("4", IDE_CLICK_KEY_4, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace;
	createKeyButton("5", IDE_CLICK_KEY_5, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace;
	createKeyButton("6", IDE_CLICK_KEY_6, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace;
	createKeyButton("7", IDE_CLICK_KEY_7, keyBarCurrentX, keyBarY); keyBarCurrentX += keyBarXSpace + keyBarXDivider;
	createKeyButton("", IDE_CLICK_KEY_CHAMP, keyBarCurrentX, keyBarY, IDI_UI_KEY_CHAMP); keyBarCurrentX += keyBarXSpace + keyBarXDivider;
	createKeyButton("", IDE_CLICK_KEY_HEAL, keyBarCurrentX, keyBarY, IDI_UI_KEY_HEAL); keyBarCurrentX += keyBarXSpace;
	createKeyButton("", IDE_CLICK_KEY_MANA, keyBarCurrentX, keyBarY, IDI_UI_KEY_MANA); keyBarCurrentX += keyBarXSpace;

	



	// #############################
	// SkillInfo
	// #############################

	needlePreviewUI = new UIImage();
	needlePreviewUI->setPos(needleX, needleY);
	addElement(needlePreviewUI);

	skillNameText = new UIText("");  // current Skill name
	skillNameText->setFontSize(0.5);
	skillNameText->setFontColor(215, 196, 164);
	skillNameText->setPos(10, 165);
	addElement(skillNameText);

	// #############################
	// Learn & References
	// #############################


	eLearnButton = new UIButton(IDB_UI_BUTTON_TEXT_N, IDB_UI_BUTTON_TEXT_NH);
	eLearnButton->addState(UIButton::BUTTON_STATE_DISABLED, IDB_UI_BUTTON_TEXT_D, IDB_UI_BUTTON_TEXT_D);
	eLearnButton->addState(UIButton::BUTTON_STATE_ACTIVE, IDB_UI_BUTTON_TEXT_A, IDB_UI_BUTTON_TEXT_AH);
	eLearnButton->setWidth(100);
	eLearnButton->set9ScaleSize(7);
	eLearnButton->setPos(10, 190);
	eLearnButton->setState(UIButton::BUTTON_STATE_DISABLED);
	eLearnButton->setLabel("LEARN");
	eLearnButton->setOnClickId(IDE_CLICK_LEARN);
	addElement(eLearnButton);

	eLearnButtonRequireText = new UIText("");
	eLearnButtonRequireText->setFontSize(0.35);
	eLearnButtonRequireText->setFontColor(215, 196, 164);
	eLearnButtonRequireText->setPos(12, 240);
	eLearnButtonRequireText->setWidth(200);
	eLearnButtonRequireText->setFontFace(CV_FONT_HERSHEY_SIMPLEX);
	addElement(eLearnButtonRequireText);

	ePrevReference = new UIButton(IDB_UI_BUTTON_ARROW_L_NORMAL, IDB_UI_BUTTON_ARROW_L_OVER, IDB_UI_BUTTON_ARROW_L_MASK);
	ePrevReference->setPos(150, 190);
	ePrevReference->setOnClickId(IDE_CLICK_PREV_REFERENCE);
	addElement(ePrevReference);
	referenceControl.push_back(ePrevReference);

	eNextReference = new UIButton(IDB_UI_BUTTON_ARROW_R_NORMAL, IDB_UI_BUTTON_ARROW_R_OVER, IDB_UI_BUTTON_ARROW_R_MASK);
	eNextReference->setPos(230, 190);
	eNextReference->setOnClickId(IDE_CLICK_NEXT_REFERENCE);
	addElement(eNextReference);
	referenceControl.push_back(eNextReference);
	
	referenceNumberText = new UIText(""); // x/y
	referenceNumberText->setWidth(60);
	referenceNumberText->alignCenter();
	referenceNumberText->setFontSize(0.5);
	referenceNumberText->setFontColor(215, 196, 164);
	referenceNumberText->setPos(175, 190+7);
	addElement(referenceNumberText);
	referenceControl.push_back(referenceNumberText);

	// #############################
	// Size Controll
	// ############ä################


	int borderPosControlX = 10;
	int borderPosControlY = 330;

	UIImage* uiBackgroundBorderPos = new UIImage(IDB_UI_SIZE_POS_BACKGROUND, IDB_UI_SIZE_POS_BACKGROUND_MASK);
	uiBackgroundBorderPos->setPos(borderPosControlX+23, borderPosControlY+23);
	addElement(uiBackgroundBorderPos);
	borderPosControl.push_back(uiBackgroundBorderPos);

	// Top
	UIButton* uiButton = new UIButton(IDB_UI_BUTTON_ARROW_T_NORMAL, IDB_UI_BUTTON_ARROW_T_OVER, IDB_UI_BUTTON_ARROW_T_MASK );
	uiButton->setPos(borderPosControlX + 50, borderPosControlY);
	uiButton->setOnClickId(IDE_CLICK_BORDER_POS_T_DEC);
	addElement(uiButton);
	borderPosControl.push_back(uiButton);

	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_B_NORMAL, IDB_UI_BUTTON_ARROW_B_OVER, IDB_UI_BUTTON_ARROW_B_MASK);
	uiButton->setPos(borderPosControlX + 50, borderPosControlY + 20);
	uiButton->setOnClickId(IDE_CLICK_BORDER_POS_T_INC);
	addElement(uiButton);
	borderPosControl.push_back(uiButton);

	// Right
	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_L_NORMAL, IDB_UI_BUTTON_ARROW_L_OVER, IDB_UI_BUTTON_ARROW_L_MASK);
	uiButton->setPos(borderPosControlX + 80, borderPosControlY + 50);
	uiButton->setOnClickId(IDE_CLICK_BORDER_POS_R_DEC);
	addElement(uiButton);
	borderPosControl.push_back(uiButton);

	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_R_NORMAL, IDB_UI_BUTTON_ARROW_R_OVER, IDB_UI_BUTTON_ARROW_R_MASK);
	uiButton->setPos(borderPosControlX + 100, borderPosControlY + 50);
	uiButton->setOnClickId(IDE_CLICK_BORDER_POS_R_INC);
	addElement(uiButton);
	borderPosControl.push_back(uiButton);

	// Bottom
	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_T_NORMAL, IDB_UI_BUTTON_ARROW_T_OVER, IDB_UI_BUTTON_ARROW_T_MASK);
	uiButton->setPos(borderPosControlX + 50, borderPosControlY + 80);
	uiButton->setOnClickId(IDE_CLICK_BORDER_POS_B_DEC);
	addElement(uiButton);
	borderPosControl.push_back(uiButton);

	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_B_NORMAL, IDB_UI_BUTTON_ARROW_B_OVER, IDB_UI_BUTTON_ARROW_B_MASK);
	uiButton->setPos(borderPosControlX + 50, borderPosControlY + 100);
	uiButton->setOnClickId(IDE_CLICK_BORDER_POS_B_INC);
	addElement(uiButton);
	borderPosControl.push_back(uiButton);

	// Left
	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_L_NORMAL, IDB_UI_BUTTON_ARROW_L_OVER, IDB_UI_BUTTON_ARROW_L_MASK);
	uiButton->setPos(borderPosControlX, borderPosControlY + 50);
	uiButton->setOnClickId(IDE_CLICK_BORDER_POS_L_DEC);
	addElement(uiButton);
	borderPosControl.push_back(uiButton);

	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_R_NORMAL, IDB_UI_BUTTON_ARROW_R_OVER, IDB_UI_BUTTON_ARROW_R_MASK);
	uiButton->setPos(borderPosControlX + 20, borderPosControlY + 50);
	uiButton->setOnClickId(IDE_CLICK_BORDER_POS_L_INC);
	addElement(uiButton);
	borderPosControl.push_back(uiButton);


	// #############################
	// Border
	// #############################

	int borderSizeControlX = 170;
	int borderSizeControlY = 330;

	UIImage* uiBackgroundBorderSize = new UIImage(IDB_UI_SIZE_BORDER_BACKGROUND, IDB_UI_SIZE_BORDER_BACKGROUND_MASK);
	uiBackgroundBorderSize->setPos(borderSizeControlX + 23, borderSizeControlY + 23);
	addElement(uiBackgroundBorderSize);
	borderSizeControl.push_back(uiBackgroundBorderSize);

	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_L_NORMAL, IDB_UI_BUTTON_ARROW_L_OVER, IDB_UI_BUTTON_ARROW_L_MASK);
	uiButton->setPos(borderSizeControlX, borderPosControlY + 50);
	uiButton->setOnClickId(IDE_CLICK_BORDER_SIZE_DEC);
	addElement(uiButton);
	borderSizeControl.push_back(uiButton);

	uiButton = new UIButton(IDB_UI_BUTTON_ARROW_R_NORMAL, IDB_UI_BUTTON_ARROW_R_OVER, IDB_UI_BUTTON_ARROW_R_MASK);
	uiButton->setPos(borderSizeControlX+24, borderPosControlY + 50);
	uiButton->setOnClickId(IDE_CLICK_BORDER_SIZE_INC);
	addElement(uiButton);
	borderSizeControl.push_back(uiButton);




	zoomedPreviewUI = new UIImage();
	zoomedPreviewUI->setPos(zpmX, zpmY);
	addElement(zoomedPreviewUI);

	referenceInfoText = new UIText("0x0 / 0x0");
	referenceInfoText->setFontSize(0.3);
	referenceInfoText->setFontColor(215, 196, 164);
	referenceInfoText->setPos(390, 540);
	addElement(referenceInfoText);


	eSaveButton = new UIButton(IDB_UI_BUTTON_TEXT_N, IDB_UI_BUTTON_TEXT_NH);
	eSaveButton->addState(UIButton::BUTTON_STATE_DISABLED, IDB_UI_BUTTON_TEXT_D, IDB_UI_BUTTON_TEXT_D);
	eSaveButton->addState(UIButton::BUTTON_STATE_ACTIVE, IDB_UI_BUTTON_TEXT_A, IDB_UI_BUTTON_TEXT_AH);
	eSaveButton->setPos(10, 500);
	eSaveButton->setLabel("SAVE");
	eSaveButton->setState(UIButton::BUTTON_STATE_DISABLED);
	eSaveButton->setOnClickId(IDE_CLICK_SAVE);
	addElement(eSaveButton);


	uiLearningProgressBlocker = new UIImage(IDB_UI_LEARNING_BLOCKER);
	uiLearningProgressBlocker->setPos(0, 0);
	uiLearningProgressBlocker->hide();
	uiLearningProgressBlocker->setCaptureMouseEvents(true);
	// Todo: Capture over's and links...
	addElement(uiLearningProgressBlocker);

	// Preload Backgrounds
	setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR_NEEDS_GAME_CLIENT);
	setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR_SQUARE);
	setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR_WIDE);
	setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR);

	
}

void UILearn::setGroupOffset(vector<UIBaseElement*> elementList, int offsetX, int offsetY) {
	for (int idx = 0; idx < elementList.size(); idx++) {
		elementList[idx]->setOffset(offsetX, offsetY);
	}
}

void UILearn::setGroupVisibility(vector<UIBaseElement*> elementList, bool state) {
	for (int idx = 0; idx < elementList.size(); idx++) {
		if (state) {
			elementList[idx]->show();
		}
		else {
			elementList[idx]->hide();
		}
	}
}

void UILearn::setGroupVisibility(vector<UIButton*> elementList, bool state) {
	for (int idx = 0; idx < elementList.size(); idx++) {
		if (state) {
			elementList[idx]->show();
		}
		else {
			elementList[idx]->hide();
		}
	}
}

void UILearn::onBeforeShow() {
	


	int x = Settings::getInstance()->getValue("Main", "LearnWindowX", -1);
	int y = Settings::getInstance()->getValue("Main", "LearnWindowY", -1);
	if (x > -1 && y > -1) {
		SetWindowPos(hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}


	getLearnController()->unloadCurrentHSL();
	forceRefresh();
	updateLcUI();
	UIBaseWindowGUIHost::processUI();
}

void UILearn::onShow() {
	forceRefresh();
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UILearn::threadWorker, NULL, NULL, NULL);
}



void UILearn::setThreadStarted() {
	workerIsRunning = true;
}

void UILearn::threadWorker() {
	UILearn* mySelf = UILearn::getInstance();
	mySelf->setThreadStarted();
	while (mySelf->workerIsRunning) {
		mySelf->processUI();
	}
}

void UILearn::onHide() {
	freeBackgroundResources();
	workerIsRunning = false;
	
}

void UILearn::processUI() {
	if (isVisible()) {
		if (getLearnController()->isLearningInProgress()) {
			if (oldLearningState == false) {
				oldLearningState = true;
				
				uiLearningProgressBlocker->show();
			}
		}
		else {
			if (oldLearningState == true) {
				oldLearningState = false;
				uiLearningProgressBlocker->hide();
				oldLearningState = getLearnController()->isLearningInProgress();
			}
		}
		if (goingToDrawUpdate()) {
			updateLcUI();
		}
		UIBaseWindowGUIHost::processUI();
	}
}


void UILearn::updateLcUI() {
	
	bool inGameMode = CUELegendKeys::getInstance()->isIngameMode();
	HSLBase* cHSL = getLearnController()->getCurrentHSL();
	
	// if no item selected or not gamemode - hide almost everything!
	if (!inGameMode || cHSL == nullptr) {
		eSaveButton->hide();
		eLearnButton->hide();
		eLearnButtonRequireText->hide();
		ePrevReference->hide();
		eNextReference->hide();
		referenceNumberText->hide();
		referenceInfoText->hide();
		skillNameText->hide();
		needlePreviewUI->hide();
		zoomedPreviewUI->hide();
		setGroupVisibility(borderPosControl, false);
		setGroupVisibility(borderSizeControl, false);
		setGroupVisibility(referenceControl, false);
	}

	// in not gamemode - keybar is also hidden
	if (!inGameMode) {
		setGroupVisibility(keyBar, false);
		setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR_NEEDS_GAME_CLIENT);
		return;
	}

	// from here - keybar is visible
	setGroupVisibility(keyBar, true);
	for (int keyIdx = 0; keyIdx < keyBar.size(); keyIdx++) {
		if (getLearnController()->loadDataExists(getLearnController()->getHSLbyIndex(keyIdx)->getSaveId())) {
			keyBar[keyIdx]->setState(UIButton::BUTTON_STATE_NORMAL);
		}
		else {
			keyBar[keyIdx]->setState(UIButton::BUTTON_STATE_EXTRA);
		}
	}

	
	// if there is NO item selected
	if (cHSL == nullptr) {
		setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR);
 	}
	else {
		skillNameText->show();
		needlePreviewUI->show();
		
		zoomedPreviewUI->show();

		setGroupVisibility(borderPosControl, true);
		setGroupVisibility(borderSizeControl, cHSL->needsBorder());

		keyBar[getLearnController()->getCurrentSkillIDX()]->setState(UIButton::BUTTON_STATE_ACTIVE);


		cv::Rect* bl = getLearnController()->getCurrentLocation();
		if (bl != nullptr) {
			referenceInfoText->setLabel("%ix%i / %ix%i", bl->x, bl->y, bl->width, bl->height);
			referenceInfoText->show();
		}
		else{
			referenceInfoText->hide();
		}
		

		eLearnButton->show();
		if (cHSL->isLearningPossible()) {
			eLearnButton->setState(UIButton::BUTTON_STATE_NORMAL);
			eLearnButtonRequireText->hide();
		}
		else {
			eLearnButton->setState(UIButton::BUTTON_STATE_DISABLED);
			eLearnButtonRequireText->setLabel(cHSL->getLearningRequiredText());
			eLearnButtonRequireText->show();
		}
		
		eSaveButton->show();
		if (getLearnController()->hasDataChanged()) {
			eSaveButton->setState(UIButton::BUTTON_STATE_NORMAL);
		}
		else {
			eSaveButton->setState(UIButton::BUTTON_STATE_DISABLED);
		}

		if (getLearnController()->getCurrentHSL()->getFoundLocations().size() > 1) {
			setGroupVisibility(referenceControl, true);
		}
		else {
			setGroupVisibility(referenceControl, false);
		}


		if (cHSL->isWide()) {
			setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR_WIDE);
			
			zoomedPreviewUI->setImage(&(getLearnController()->getUIPreview(zpmWideWidth, zpmWideHeight, true)));
			zoomedPreviewUI->setPos(zpmWideX, zpmWideY);
			
			needlePreviewUI->setImage(&(getLearnController()->getUINeedle(needleWideWidth, needleWideHeight)));
			needlePreviewUI->setPos(needleWideX, needleWideY);

			eSaveButton->setOffset(eSaveButtonWideOffsetX, eSaveButtonWideOffsetY);
			eLearnButton->setOffset(eLearnButtonWideOffsetX, eLearnButtonWideOffsetY);
			setGroupOffset(borderPosControl, borderPosControlWideOffsetX, borderPosControlWideOffsetY);
			setGroupOffset(borderSizeControl, borderSizeControlWideOffsetX, borderSizeControlWideOffsetY);
			setGroupOffset(referenceControl, referenceControlWideOffsetX, referenceControlWideOffsetY);
			
			// referenceInfoText
		}
		else {
			setBackgroundResource(IDB_WINDOW_BACKGROUND_LEARNEDITOR_SQUARE);
			
			zoomedPreviewUI->setImage(&(getLearnController()->getUIPreview(zpmWidth, zpmHeight)));
			zoomedPreviewUI->setPos(zpmX, zpmY);

			needlePreviewUI->setImage(&(getLearnController()->getUINeedle(needleWidth, needleHeight)));
			needlePreviewUI->setPos(needleX, needleY);

			eSaveButton->setOffset(eSaveButtonOffsetX, eSaveButtonOffsetY);
			eLearnButton->setOffset(eLearnButtonOffsetX, eLearnButtonOffsetY);
			setGroupOffset(borderPosControl, borderPosControlOffsetX, borderPosControlOffsetY);
			setGroupOffset(borderSizeControl, borderSizeControlOffsetX, borderSizeControlOffsetY);
			setGroupOffset(referenceControl, referenceControlOffsetX, referenceControlOffsetY);
			
		}


		
		skillNameText->setLabel(cHSL->getTitle());
		
	

		
		int availableItems = getLearnController()->getFoundSkillReferences();

		if (availableItems > 0) {
			ePrevReference->show();
			eNextReference->show();
			referenceNumberText->show();
			
			Rect foundLocation = cHSL->getLocationRectByLocationIndex(getLearnController()->getCurrentHSLfoundIDX());
			int currentItem = getLearnController()->getCurrentHSLfoundIDX() + 1;

			referenceNumberText->setLabel("%i/%i", currentItem, availableItems);
			referenceInfoText->setLabel("%ix%i / %ix%i", foundLocation.x, foundLocation.y, foundLocation.width, foundLocation.height);
		}
	}
	
	
}

void UILearn::handleClickEvents(int xPos, int yPos) {
	bool doUpdateLcUI = true;
	int eventId = onClick(xPos, yPos);
	if (eventId == -1) {
		return;
	}
	switch (eventId) {
		case IDE_CLICK_PREV_REFERENCE:	 getLearnController()->prevReference();	break;
		case IDE_CLICK_NEXT_REFERENCE:	 getLearnController()->nextReference();	break;
		case IDE_CLICK_LEARN:			 getLearnController()->learn();	break;
		case IDE_CLICK_SAVE:			 getLearnController()->save(); getLearnController()->load();  break;
		case IDE_CLICK_BORDER_POS_T_INC:	 getLearnController()->setBorderPosDelta(LearnController::BORDER_POS_T, 1); break;
		case IDE_CLICK_BORDER_POS_T_DEC:	 getLearnController()->setBorderPosDelta(LearnController::BORDER_POS_T, -1); break;
		case IDE_CLICK_BORDER_POS_R_INC:	 getLearnController()->setBorderPosDelta(LearnController::BORDER_POS_R, 1);	break;
		case IDE_CLICK_BORDER_POS_R_DEC:	 getLearnController()->setBorderPosDelta(LearnController::BORDER_POS_R, -1); break;
		case IDE_CLICK_BORDER_POS_B_INC:	 getLearnController()->setBorderPosDelta(LearnController::BORDER_POS_B, 1); break;
		case IDE_CLICK_BORDER_POS_B_DEC:	 getLearnController()->setBorderPosDelta(LearnController::BORDER_POS_B, -1); break;
		case IDE_CLICK_BORDER_POS_L_INC:	 getLearnController()->setBorderPosDelta(LearnController::BORDER_POS_L, 1); break;
		case IDE_CLICK_BORDER_POS_L_DEC:	 getLearnController()->setBorderPosDelta(LearnController::BORDER_POS_L, -1); break;
		case IDE_CLICK_BORDER_SIZE_INC:		 getLearnController()->setBorderSizeDelta(1); break;
		case IDE_CLICK_BORDER_SIZE_DEC:		 getLearnController()->setBorderSizeDelta(-1); break;
		case IDE_CLICK_KEY_PASSIVE:			 getLearnController()->gotoSkill(0); break;
		case IDE_CLICK_KEY_Q:				 getLearnController()->gotoSkill(1); break;
		case IDE_CLICK_KEY_W:				 getLearnController()->gotoSkill(2); break;
		case IDE_CLICK_KEY_E:				 getLearnController()->gotoSkill(3); break;
		case IDE_CLICK_KEY_R:				 getLearnController()->gotoSkill(4); break;
		case IDE_CLICK_KEY_D:				 getLearnController()->gotoSkill(5); break;
		case IDE_CLICK_KEY_F:				 getLearnController()->gotoSkill(6); break;
		case IDE_CLICK_KEY_B:				 getLearnController()->gotoSkill(7); break;
		case IDE_CLICK_KEY_1:				 getLearnController()->gotoSkill(8); break;
		case IDE_CLICK_KEY_2:				 getLearnController()->gotoSkill(9); break;
		case IDE_CLICK_KEY_3:				 getLearnController()->gotoSkill(10); break;
		case IDE_CLICK_KEY_4:				 getLearnController()->gotoSkill(11); break;
		case IDE_CLICK_KEY_5:				 getLearnController()->gotoSkill(12); break;
		case IDE_CLICK_KEY_6:				 getLearnController()->gotoSkill(13); break;
		case IDE_CLICK_KEY_7:				 getLearnController()->gotoSkill(14); break;
		case IDE_CLICK_KEY_CHAMP:			 getLearnController()->gotoSkill(15); break;
		case IDE_CLICK_KEY_HEAL:			 getLearnController()->gotoSkill(16); break;
		case IDE_CLICK_KEY_MANA:			 getLearnController()->gotoSkill(17); break;
		
		default: doUpdateLcUI = false; break;
	}	

	if (doUpdateLcUI) {
		updateLcUI();
	}
}



INT_PTR CALLBACK UILearn::MessageHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_ERASEBKGND:
			{
				RECT rc;
				GetClientRect(hDlg, &rc);
				SetBkColor((HDC)wParam, 0x001f1a06); 
				ExtTextOut((HDC)wParam, 0, 0, ETO_OPAQUE, &rc, 0, 0, 0);
				return 1;
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(hDlg, &ps);
				EndPaint(hDlg, &ps);
				return (INT_PTR)TRUE;
			}
			break;
		case WM_LBUTTONUP:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				UILearn::getInstance()->handleClickEvents(xPos, yPos);
			}
			break;
		case WM_MOUSEMOVE:
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				UILearn::getInstance()->onMouseMove(xPos, yPos);
			}
			break;
		case WM_WINDOWPOSCHANGED:
			{
				LPWINDOWPOS lpwp = (LPWINDOWPOS)lParam; // points to size and position data
				if (!(lpwp->flags & SWP_NOMOVE)) {
					Settings::getInstance()->setAutoCommit(false);
					Settings::getInstance()->setValue("Main", "LearnWindowX", lpwp->x);
					Settings::getInstance()->setValue("Main", "LearnWindowY", lpwp->y);
					Settings::getInstance()->commit();
				}

			}
			break;
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				UILearn::getInstance()->Hide();
				return (INT_PTR)TRUE;
			}
		}
	}
	return (INT_PTR)FALSE;
}
