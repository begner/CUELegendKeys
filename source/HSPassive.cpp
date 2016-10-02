#include "HSPassive.h"

void HSPassive::initialize() {
	champKeys = LEDController::getInstance()->getAllKeysByRect(CLK_Insert, CLK_KeypadEnter);
	RECT keyDim = LEDController::getInstance()->getKeyBoardDimensions(champKeys);

	
	champKeyWidth = keyDim.right;
	champKeyHeight = keyDim.bottom;

	srand(time(NULL));
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


void HSPassive::filterMat() {
	
	int diffL = (int)((float)targetDiffX/(float)remainingTicks);
	if (diffL < 0) diffL = 0;
	int diffT = (int)((float)targetDiffY/(float)remainingTicks);
	if (diffT < 0) diffT = 0;

	remainingTicks--;
	reinitAnimation();

	champImageColorTable = Mat(getCaptureWidth(true), getCaptureHeight(true), CV_8UC3);
	getOriginalMatRespectBorders()->copyTo(champImageColorTable);

	// ImageFilterMat::saturation(champImageColorTable, 0, 255, 1);

	cv::Rect curRect(maxDiff - diffL, maxDiff - diffT, champImageColorTable.cols - maxDiff, champImageColorTable.rows - maxDiff);
	champImageColorTable = Mat(champImageColorTable, curRect);
	
	resize(champImageColorTable, champImageColorTable, cv::Size(champKeyWidth, champKeyHeight), 0, 0, INTER_CUBIC);

	Mat* filterd = getFilteredMat();
	champImageColorTable.copyTo(*filterd);



	// ImageFilterMat::killGrayPixel(champImageColorTable, 25);
}

Vec4b HSPassive::getCurrentColor(int index) {
	Vec4b color(0, 0, 0, 255);
	return color;
}

void HSPassive::reinitAnimation() {
	if (remainingTicks < 1) {
		lastTargetDiffX = targetDiffX;
		lastTargetDiffY = targetDiffY;
		remainingTicks = (double)rand() / (RAND_MAX + 1) * 30;
		targetDiffX = (double)rand() / (RAND_MAX + 1) * maxDiff;
		targetDiffY = (double)rand() / (RAND_MAX + 1) * maxDiff;
	}
}


int HSPassive::getMaxTick() {
	return remainingTicks;
}




void HSPassive::updateKey() {

	
	for (vector<CorsairLedPosition>::iterator it = champKeys.begin(); it != champKeys.end(); ++it) {

		cv::Rect testRect((int)it->left, (int)it->top, (int)it->width, (int)it->height);
		Mat4b testMat = Mat(champImageColorTable, testRect);
		resize(testMat, testMat, cv::Size(1, 1), 0, 0, INTER_CUBIC);

		Vec4b color = testMat.at<Vec4b>(0, 0);
		LEDController::getInstance()->setKey(it->ledId, color[2], color[1], color[0]);
	}
	

}

