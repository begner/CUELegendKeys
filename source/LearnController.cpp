#include "LearnController.h"

LearnController* LearnController::theInstance = NULL;

LearnController* LearnController::getInstance()
{
	if (theInstance == NULL)
		theInstance = new LearnController();
	return theInstance;
}

LearnController::LearnController()
{
	screenShotRaw = Mat(1, 1, CV_8UC3);
	wideMask = ImageFilterMat::loadResourceAsMat(IDB_UI_LEARNING_WIDE_MASK);
	



	allSkills.push_back(new HSLSkill("Passive Icon", "Passive", IDB_AUTO_LEARN_PASSIVE));

	allSkills.push_back(new HSLSkill("Ability Q", "AbilityQ", IDB_AUTO_LEARN_SKILL_Q));
	allSkills.push_back(new HSLSkill("Ability W", "AbilityW", IDB_AUTO_LEARN_SKILL_W));
	allSkills.push_back(new HSLSkill("Ability E", "AbilityE", IDB_AUTO_LEARN_SKILL_E));
	allSkills.push_back(new HSLSkill("Ability R", "AbilityR", IDB_AUTO_LEARN_SKILL_R));

	allSkills.push_back(new HSLSkill("Summoner Spell D", "SummonerD", IDB_AUTO_LEARN_SKILL_D));
	allSkills.push_back(new HSLSkill("Summoner Spell F", "SummonerF", IDB_AUTO_LEARN_SKILL_F));
	allSkills.push_back(new HSLItem("Summoner Spell B", "SummonerB", IDB_AUTO_LEARN_SKILL_B));


	allSkills.push_back(new HSLItem("Item 1", "Item1", IDB_AUTO_LEARN_ITEM_1));
	allSkills.push_back(new HSLItem("Item 2", "Item2", IDB_AUTO_LEARN_ITEM_2));
		
	allSkills.push_back(new HSLRelativeItem("Item 3", "Item3", IDB_AUTO_LEARN_ITEM_3, 3));
	allSkills.push_back(new HSLRelativeItem("Item 4", "Item4", IDB_AUTO_LEARN_ITEM_4, 4));
	allSkills.push_back(new HSLRelativeItem("Item 5", "Item5", IDB_AUTO_LEARN_ITEM_5, 5));
	allSkills.push_back(new HSLRelativeItem("Item 6", "Item6", IDB_AUTO_LEARN_ITEM_6, 6));
	allSkills.push_back(new HSLRelativeItem("Item 7", "Item7", IDB_AUTO_LEARN_ITEM_7, 7));
	
	allSkills.push_back(new HSLBar("Heal Bar", "BarHeal", IDB_AUTO_LEARN_BAR_HEAL, 120, 130));
	allSkills.push_back(new HSLBar("Mana Bar", "BarMana", IDB_AUTO_LEARN_BAR_MANA, 208, 215));

}


LearnController::~LearnController()
{
}


void LearnController::setScreenShot(Mat* screenShot) {
	screenShot->copyTo(screenShotRaw);
}

Mat* LearnController::getScreenshot() {
	return &screenShotRaw;
}


bool LearnController::areThereSavedChanges() {
	bool oldState = thereAreSavedChanges;
	thereAreSavedChanges = false;
	return oldState;
}

void LearnController::save() {

	cv::Rect resultRect = *currentLocation;
	int coordX = resultRect.x + borderPosL-overScan;
	int coordY = resultRect.y + borderPosT - overScan;
	int width = borderPosR - borderPosL;
	int height = borderPosB - borderPosT;
	int borderWidth = borderSize;
	if (!currentHsl->needsBorder()) {
		borderWidth = 0;
	}

	string saveId = currentHsl->getSaveId();

	Settings::getInstance()->setValue("HotSpots", saveId + "PosX", coordX);
	Settings::getInstance()->setValue("HotSpots", saveId + "PosY", coordY);
	Settings::getInstance()->setValue("HotSpots", saveId + "Width", width);
	Settings::getInstance()->setValue("HotSpots", saveId + "Height", height);
	Settings::getInstance()->setValue("HotSpots", saveId + "Border", borderWidth);

	thereAreSavedChanges = true;
	dataChanged = false;
}


bool LearnController::loadDataExists(string saveId) {
	cv::Rect loadRect = getSavedLocation(saveId);
	int b = getBorderSize(saveId);
	return (loadRect.x > -1 && loadRect.y > -1 && loadRect.width > -1 && loadRect.height > -1 && b > -1);
}

int LearnController::getBorderSize(string saveId) {
	return Settings::getInstance()->getValue("HotSpots", saveId + "Border", -1);
}

cv::Rect LearnController::getSavedLocation(string saveId) {
	int x = Settings::getInstance()->getValue("HotSpots", saveId + "PosX", -1);
	int y = Settings::getInstance()->getValue("HotSpots", saveId + "PosY", -1);
	int w = Settings::getInstance()->getValue("HotSpots", saveId + "Width", -1);
	int h = Settings::getInstance()->getValue("HotSpots", saveId + "Height", -1);
	
	return cv::Rect(x, y, w, h);
}

void LearnController::load() {
	if (currentHsl) {
		currentHsl->releaseData();
	}
	
	currentHsl = getHSLbyIndex(currentSkillIDX);
	
	if (loadDataExists(currentHsl->getSaveId())) {
		Mat* screenShot = getScreenshot();
		if (screenShot) {
			
			currentHsl->setOriginalImage(*screenShot);
			currentHsl->setHaystack(*screenShot);

			currentLocation = new cv::Rect(getSavedLocation(currentHsl->getSaveId()));

			borderSize = getBorderSize(currentHsl->getSaveId());
			borderPosB = currentLocation->height + overScan;
			borderPosT = overScan;
			borderPosL = overScan;
			borderPosR = currentLocation->width + overScan;
			
			dataChanged = false;

		}
		currentHSLfoundIDX = 0;
		
	}
	else {
		currentLocation = nullptr;
	}
	
	

}


void LearnController::nextSkill() {
	currentSkillIDX++;
	if (currentSkillIDX > allSkills.size() - 1) {
		currentSkillIDX = 0;
	}
	load();
}

void LearnController::gotoSkill(int idx) {
	currentSkillIDX = idx;
	load();
}

void LearnController::prevSkill() {
	currentSkillIDX--;
	if (currentSkillIDX < 0) {
		currentSkillIDX = 0;
	}
	load();
}

bool LearnController::threadLearn() {
	LearnController* mySelf = LearnController::getInstance();

	mySelf->currentHsl->findLocations();
	if ((int)mySelf->currentHsl->getFoundLocations().size() > 1) {
		mySelf->currentHsl->orderLocations();
	}
	mySelf->currentHSLfoundIDX = 0;
	if ((int)mySelf->currentHsl->getFoundLocations().size() > 0) {
		mySelf->currentLocation = new cv::Rect(mySelf->currentHsl->getLocationRectByLocationIndex(mySelf->currentHSLfoundIDX));
		mySelf->resetBorders();
		mySelf->dataChanged = true;
	}
	
	mySelf->jobInProgress = false;
	return true;
}

bool LearnController::isLearningInProgress() {
	return jobInProgress;
}

void LearnController::learn() {
	if (jobInProgress) {
		return;
	}

	Mat* screenShot = getScreenshot();
	if (screenShot) {
		if (currentHsl) {
			currentHsl->releaseData();
		}
		currentHsl = getHSLbyIndex(currentSkillIDX);
		currentHsl->setOriginalImage(*screenShot);
		currentHsl->setHaystack(*screenShot);

		HSLRelativeItem* currentHslDynamicItem = dynamic_cast<HSLRelativeItem*>(currentHsl);
		if (currentHslDynamicItem) {
			currentHslDynamicItem->setItem1(getSavedLocation("Item1"));
			currentHslDynamicItem->setItem2(getSavedLocation("Item2"));
		}


		HSLBar* currentHslDynamicBar = dynamic_cast<HSLBar*>(currentHsl);
		if (currentHslDynamicBar) {
			currentHslDynamicBar->setPassive(getSavedLocation("Passive"));
			currentHslDynamicBar->setAbilityQ(getSavedLocation("AbilityQ"));
			currentHslDynamicBar->setAbilityF(getSavedLocation("SummonerF"));
		}

		jobInProgress = true;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LearnController::threadLearn, NULL, NULL, NULL);

	}
}

void LearnController::nextReference() {
	if (!currentHsl) {
		return;
	}
	currentHSLfoundIDX++;
	if (currentHSLfoundIDX > (int)currentHsl->getFoundLocations().size() - 1) {
		currentHSLfoundIDX = 0;
	}

	currentLocation = new cv::Rect(currentHsl->getLocationRectByLocationIndex(currentHSLfoundIDX));
	resetBorders();
}

void LearnController::prevReference() {
	if (!currentHsl) {
		return;
	}
	currentHSLfoundIDX--;
	if (currentHSLfoundIDX < 0) {
		currentHSLfoundIDX = (int)currentHsl->getFoundLocations().size() - 1;
	}

	currentLocation = new cv::Rect(currentHsl->getLocationRectByLocationIndex(currentHSLfoundIDX));
	resetBorders();
	
}

Mat LearnController::getUINeedle(int width, int height) {
	Mat needleImage = Mat(cv::Size(height, width), CV_8UC4, Scalar(0, 0, 0, 255));

	if (!currentHsl) {
		return needleImage;
	}

	needleImage = Mat(*currentHsl->getNeedle());
	resize(needleImage, needleImage, cv::Size(width, height), 0, 0, INTER_CUBIC);

	return needleImage;
}

Mat LearnController::getPreviewMatForCurrentLocation() {
	Mat preview;
	preview = Mat(1, 1, CV_8UC4);

	if (currentLocation == nullptr) {
		return preview;
	}

	if (currentLocation->x < 0 || currentLocation->y < 0 || currentLocation->width < 0 || currentLocation->height < 0) {
		return preview;
	}

	Mat* screenShot = getScreenshot();
	if (!screenShot) {
		return preview;
	}

	cv::Rect overlapLocation;
	overlapLocation = *currentLocation;
	overlapLocation.x -= overScan;
	overlapLocation.y -= overScan;
	overlapLocation.width += overScan * 2;
	overlapLocation.height += overScan * 2;

	if (overlapLocation.width + overlapLocation.x > screenShot->cols) {
		overlapLocation.width = screenShot->cols - overlapLocation.x;
	}
	if (overlapLocation.height + overlapLocation.y > screenShot->rows) {
		overlapLocation.height = screenShot->rows - overlapLocation.y;
	}

	if (overlapLocation.width < 1 || overlapLocation.height < 1 || overlapLocation.x < 1 || overlapLocation.y < 1) {
		return preview;
	}


 	preview = Mat(*screenShot, overlapLocation);
	
	return preview;
}

void LearnController::moveCurrentLocation(int dir, int diff) {

}

Mat LearnController::getUIPreview(int width, int height, bool wideMode) {

	Mat uiPreview = Mat(cv::Size(width, height), CV_8UC4, Scalar(0, 0, 0, 255));

	if (currentLocation && currentHsl) {

		// Mat uiPreview(width, height, CV_8UC4, Scalar(0, 255, 128, 255));

		Mat previewImage;
		getPreviewMatForCurrentLocation().copyTo(previewImage);


		int originalWidth = previewImage.cols;
		int originalHeight = previewImage.rows;

		int scaleFactor;
		if (wideMode) {
			scaleFactor = (int)floor((double)(height - 1) / (double)originalHeight);
		}
		else {
			scaleFactor = (int)min(floor((double)(width - 1) / (double)originalWidth), floor((double)(height - 1) / (double)originalHeight));
		}



		// double xScale = (double)(height-1) / (double)(originalHeight-1);
		// double yScale = (double)(width-1) / (double)(originalWidth-1);
		resize(previewImage, previewImage, cv::Size(originalWidth * scaleFactor + 1, originalHeight * scaleFactor + 1), 0, 0, INTER_NEAREST);

		Scalar gridColor = Scalar(0, 0, 0, 255);
		Scalar borderColor = Scalar(0, 255, 255, 255);




		for (int x = 0; x < originalWidth + 1; x++) {
			int iAxisX = (int)round((double)(x*scaleFactor));

			Scalar lineColor;
			if (x == borderPosL || (currentHsl->needsBorder() && x == borderPosL + borderSize) ||
				x == borderPosR || (currentHsl->needsBorder() && x == borderPosR - borderSize)) {
				lineColor = borderColor;
			}
			else {
				lineColor = gridColor;
			}

			line(previewImage, cv::Point(iAxisX, 0), cv::Point(iAxisX, previewImage.rows), lineColor, 1, 8, 0);

		}


		for (int y = 0; y < originalHeight + 1; y++) {
			int iAxisY = (int)round((double)(y*scaleFactor));

			Scalar lineColor;
			if (y == borderPosT || (currentHsl->needsBorder() && y == borderPosT + borderSize) ||
				y == borderPosB || (currentHsl->needsBorder() && y == borderPosB - borderSize)) {
				lineColor = borderColor;
			}
			else {
				lineColor = gridColor;
			}

			line(previewImage, cv::Point(0, iAxisY), cv::Point(previewImage.cols, iAxisY), lineColor, 1, 8, 0);
		}



		if (wideMode) {
			
			int halfWidth = (int)floor((double)width / (double)2);
			if (previewImage.cols < halfWidth) {
				return uiPreview;
			}

			Mat leftSide;
			previewImage(cv::Rect(0, 0, halfWidth, previewImage.rows)).copyTo(leftSide);
			Mat rightSide;
			previewImage(cv::Rect(previewImage.cols - halfWidth, 0, halfWidth, previewImage.rows)).copyTo(rightSide);

			previewImage = Mat(previewImage.rows, width, CV_8UC4, Scalar(0, 0, 0, 255));
			leftSide.copyTo(previewImage(cv::Rect(0, 0, leftSide.cols, leftSide.rows)));
			rightSide.copyTo(previewImage(cv::Rect(leftSide.cols-1, 0, rightSide.cols, rightSide.rows)));
			
		}

		// center image in viewPort
		int applyToX = (int)((uiPreview.cols - previewImage.cols) / 2);
		int applyToY = (int)((uiPreview.rows - previewImage.rows) / 2);
		ImageFilterMat::overlayImage(&uiPreview, &previewImage, cv::Point(applyToX, applyToY));
		
		if (wideMode) {
			ImageFilterMat::addAlphaMask(&uiPreview, &wideMask(cv::Rect(0, 0, uiPreview.cols, uiPreview.rows)));
		}
	}
	else {
		currentLocation = nullptr;
	}

	

	return uiPreview;
}


void LearnController::resetBorders() {
	Mat previewImage = getPreviewMatForCurrentLocation();

	borderPosT = overScan;
	borderPosR = previewImage.cols - overScan * 2;
	borderPosB = previewImage.rows - overScan * 2;
	borderPosL = overScan;
	borderSize = 1;

	dataChanged = true;
}


void LearnController::setBorderPosDelta(int position, int diff) {
	int dummy = 0;
	int* posVar;
	posVar = &dummy;

	switch (position) {
		case BORDER_POS_T:
			posVar = &borderPosT;
			break;
		case BORDER_POS_R:
			posVar = &borderPosR;
			break;
		case BORDER_POS_B:
			posVar = &borderPosB;
			break;
		case BORDER_POS_L:
			posVar = &borderPosL;
			break;
	}

	*posVar = *posVar + diff;
	if (posVar < 0) {
		*posVar = 0;
	}

	dataChanged = true;
}

int LearnController::getFoundSkillReferences() {
	if (!currentHsl) {
		return 0;
	}
	return (int)currentHsl->getFoundLocations().size();
}

int LearnController::getCurrentHSLfoundIDX() {
	return currentHSLfoundIDX;
}

void LearnController::setBorderSizeDelta(int diff) {
	borderSize = borderSize + diff;
	if (borderSize < 1) borderSize = 1;
	dataChanged = true;
}


HSLBase* LearnController::getHSLbyIndex(int idx) {
	return allSkills[idx];
}

HSLBase* LearnController::getCurrentHSL() {
	return currentHsl;
}

int LearnController::getCurrentSkillIDX() {
	return currentSkillIDX;
}

bool LearnController::hasDataChanged() {
	return dataChanged;
}

