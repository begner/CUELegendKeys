#include "HSPassive.h"

void HSPassive::initialize() {
	champKeys = LEDController::getInstance()->getAllKeysByRect(CLK_Insert, CLK_KeypadEnter);
	RECT keyDim = LEDController::getInstance()->getKeyBoardDimensions(champKeys);

	
	champKeyWidth = keyDim.right;
	champKeyHeight = keyDim.bottom;

	srand((uint)time(0));
	reinitAnimation();

}

HSPassive::HSPassive()
{
	
}

HSPassive::~HSPassive()
{
}

int HSPassive::getType() {
	return ScreenHotSpot::TYPE_PASSIVE;
}

int HSPassive::calcAnimationStep(int oldTarget, int newTarget) {
	int newPos = 0;
	
	float myTick = (float)getCurrentTick();
	float maxTicks = (float)getMaxTick();
	
	float step = (float)(newTarget - oldTarget) * myTick / maxTicks;
	int diff = (int)step;
	newPos = oldTarget + diff;
	return newPos;
}

void HSPassive::filterMat() {
	reinitAnimation();
	int diffX = calcAnimationStep(lastTargetDiffX, targetDiffX);
	int diffY = calcAnimationStep(lastTargetDiffY, targetDiffY);
	
	// remainingTicks--;


	champImageColorTable = Mat(getCaptureWidth(true), getCaptureHeight(true), CV_8UC3);
	getOriginalMatRespectBorders()->copyTo(champImageColorTable);

	ImageFilterMat::brightnessContrast(champImageColorTable, &champImageColorTable, 2, 0.25);
	ImageFilterMat::killDarkPixel(champImageColorTable, 60);
	ImageFilterMat::killGrayPixel(champImageColorTable, 60);
	
	blur(champImageColorTable, champImageColorTable, cv::Size(3, 3), cv::Point(-1, -1));
	
	cv::Size newSize = cv::Size(champKeyWidth + maxDiff, champKeyHeight + maxDiff);
	if (!ImageFilterMat::isValidRect(&champImageColorTable, cv::Rect(0, 0, newSize.width, newSize.height))) {
		return;
	}
	resize(champImageColorTable, champImageColorTable, newSize, 0, 0, INTER_CUBIC);

	// ImageFilterMat::saturation(champImageColorTable, 0, 255, 1);

	cv::Rect curRect(maxDiff - diffX, maxDiff - diffY, champImageColorTable.cols - maxDiff, champImageColorTable.rows - maxDiff);
	if (!ImageFilterMat::isValidRect(&champImageColorTable, curRect)) {
		return;
	}
	champImageColorTable = Mat(champImageColorTable, curRect);
	

	Mat* filterd = getFilteredMat();

	champImageColorTable.copyTo(*filterd);

	for (vector<CorsairLedPosition>::iterator it = champKeys.begin(); it != champKeys.end(); ++it) {
		Scalar colorBorder(0, 0, 0, 128);
		cv::rectangle(*filterd, cv::Rect((int)it->left, (int)it->top, (int)it->width, (int)it->height), colorBorder, 1, 8, 0);
	}



	// ImageFilterMat::killGrayPixel(champImageColorTable, 25);
}

Vec4b HSPassive::getCurrentColor(int index) {
	Vec4b color(0, 0, 0, 255);
	return color;
}

void HSPassive::reinitAnimation() {
	int curTick = getCurrentTick();
	if (curTick == 0) {
		lastTargetDiffX = targetDiffX;
		lastTargetDiffY = targetDiffY;
		remainingTicks = 10 + (int)((double)rand() / (RAND_MAX + 1) * 10);
		targetDiffX = (int)((double)rand() / (RAND_MAX + 1) * maxDiff);
		targetDiffY = (int)((double)rand() / (RAND_MAX + 1) * maxDiff);
	}
}

int HSPassive::getMaxTick() {
	return remainingTicks;
}

void HSPassive::updateKey() {

	for (vector<CorsairLedPosition>::iterator it = champKeys.begin(); it != champKeys.end(); ++it) {

		cv::Rect testRect((int)it->left, (int)it->top, (int)it->width, (int)it->height);
		if (!ImageFilterMat::isValidRect(&champImageColorTable, testRect)) {
			return;
		}
		
		Mat4b testMat = Mat(champImageColorTable, testRect);


		resize(testMat, testMat, cv::Size(1, 1), 0, 0, INTER_CUBIC);

		Vec4b color = testMat.at<Vec4b>(0, 0);
		LEDController::getInstance()->setKey(it->ledId, color[2], color[1], color[0]);
	}
	

}

