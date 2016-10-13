#include "LearnController.h"



LearnController::LearnController()
{
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_Q, "Skill Q"));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_W, "Skill W"));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_E, "Skill E"));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_R, "Skill R"));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_D, "Skill D"));
	allSkills.push_back(make_pair(IDB_AUTO_LEARN_SKILL_F, "Skill F"));
}


LearnController::~LearnController()
{
	
}



Mat* LearnController::getScreenshot() {

	HWND desktop = GetDesktopWindow();
	HDC desktopDC = GetDC(desktop);

	RECT targetRect;
	GetWindowRect(desktop, &targetRect);

	int clientWidth = targetRect.right - targetRect.left;
	int clientHeight = targetRect.bottom - targetRect.top;

	Mat* screenshotRaw = ImageFilterMat::hdc2mat(desktopDC, 0, 0, clientWidth, clientHeight);

	ReleaseDC(desktop, desktopDC);

	return screenshotRaw;
}


void LearnController::start() {

	Mat* screenShot = getScreenshot();
	if (screenShot) {
		
		currentHsl = new HSLSkill();
		currentHsl->setHaystack(*screenShot);
		currentHsl->setNeedle(IDB_AUTO_LEARN_SKILL_Q);
		currentHsl->findLocations();

		currentHSLfoundIDX = 0;
		resetBorders();
	}
	else {
		// todo - errormessage!
	}
}

void LearnController::next() {
	if (!currentHsl) {
		return;
	}
	currentHSLfoundIDX++;
	if (currentHSLfoundIDX > (int)currentHsl->getFoundLocations().size() - 1) {
		currentHSLfoundIDX = 0;
	}
	resetBorders();
}

void LearnController::prev() {
	if (!currentHsl) {
		return;
	}
	currentHSLfoundIDX--;
	if (currentHSLfoundIDX < 0) {
		currentHSLfoundIDX = (int)currentHsl->getFoundLocations().size() - 1;
	}
	resetBorders();
}

Mat* LearnController::getUIPreview(int width, int height) {

	Mat* uiPreview = new Mat(cv::Size(height, width), CV_8UC4, Scalar(0, 0, 0, 255));

	// nothing so show
	if (!currentHsl) {
		return uiPreview;
	}

	// Mat uiPreview(width, height, CV_8UC4, Scalar(0, 255, 128, 255));

	Mat* previewImage;
	previewImage = currentHsl->getPreviewMatForLocationIndex(currentHSLfoundIDX);
	
	int originalWidth = previewImage->cols;
	int originalHeight = previewImage->rows;

	int scaleFactor = (int)min(floor((double)(width-1) / (double)originalWidth), floor((double)(height-1) / (double)originalHeight));

	// double xScale = (double)(height-1) / (double)(originalHeight-1);
	// double yScale = (double)(width-1) / (double)(originalWidth-1);
	resize(*previewImage, *previewImage, cv::Size(originalWidth * scaleFactor + 1, originalHeight * scaleFactor + 1), 0, 0, INTER_NEAREST);

	Scalar gridColor = Scalar(0, 0, 0, 128);
	Scalar borderColor = Scalar(0, 255, 255, 128);

	for (int x = 0; x < originalWidth + 1; x++) {
		int iAxisX = (int)round((double)(x*scaleFactor));

		Scalar lineColor;
		if (x == borderPosL || x == borderPosL + borderSize || 
			x == borderPosR || x == borderPosR - borderSize) {
			lineColor = borderColor;
		}
		else {
			lineColor = gridColor;
		}

		line(*previewImage, cv::Point(iAxisX, 0), cv::Point(iAxisX, height), lineColor, 1, 8, 0);
		
	}


	for (int y = 0; y < originalHeight+1; y++) {
		int iAxisY = (int)round((double)(y*scaleFactor));

		Scalar lineColor;
		if (y == borderPosT || y == borderPosT + borderSize ||
			y == borderPosB || y == borderPosB - borderSize) {
			lineColor = borderColor;
		}
		else {
			lineColor = gridColor;
		}

		line(*previewImage, cv::Point(0, iAxisY), cv::Point(width, iAxisY), lineColor, 1, 8, 0);
	}
	
	int applyToX = (int)((uiPreview->cols - previewImage->cols) / 2);
	int applyToY = (int)((uiPreview->rows - previewImage->rows) / 2);
		
	ImageFilterMat::overlayImage(uiPreview, previewImage, cv::Point(applyToX, applyToY));


	return uiPreview;
}


void LearnController::resetBorders() {
	Mat* previewImage;
	previewImage = currentHsl->getPreviewMatForLocationIndex(currentHSLfoundIDX);


	borderPosT = 1;
	borderPosR = previewImage->cols - 2;
	borderPosB = previewImage->rows - 2;
	borderPosL = 1;
	borderSize = 2;

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
}


void LearnController::setBorderSizeDelta(int diff) {
	borderSize = borderSize + diff;
	if (borderSize < 1) borderSize = 1;
}
