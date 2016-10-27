#include "HSChamp.h"

void HSChamp::initialize() {

	champKeys = LEDController::getInstance()->getAllKeysByRect(CLK_Insert, CLK_KeypadEnter);
	RECT keyDim = LEDController::getInstance()->getKeyBoardDimensions(champKeys);

	champKeyWidth = keyDim.right;
	champKeyHeight = keyDim.bottom;

	srand((int)time(0));

	// Mask for UI
	int cW = getCaptureWidth(true);
	int cH = getCaptureHeight(true);
	int radius = cW - champRadius;// min(cW, cH);

	if (ImageFilterMat::isValidRect(cv::Rect(0, 0, cW, cH))) {
		mask = new Mat(cW, cH, CV_8UC1);
		cv::rectangle(*mask, cv::Point(0, 0), cv::Point(cW, cH), Scalar(0), CV_FILLED, 8, 0);
		cv::circle(*mask, cv::Point((int)floor(cW / 2) + champOffsetX, (int)floor(cH / 2) + champOffsetY), (int)floor(radius / 2), Scalar(255), CV_FILLED, CV_AA, 0);
	}
}

HSChamp::HSChamp()
{
	

}

HSChamp::~HSChamp()
{
}

int HSChamp::getType() {
	return ScreenHotSpot::TYPE_CHAMP;
}

void HSChamp::filterMat() {

	int maxDiff = 2;

	int diffL = (int)((double)rand() / (RAND_MAX + 1) * maxDiff);
	int diffT = (int)((double)rand() / (RAND_MAX + 1) * maxDiff);


	if (mask == nullptr) {
		return;
	}

	ImageFilterMat::addAlphaMask(getOriginalMatRespectBorders(), mask);

	champImageColorTable = Mat4b(getCaptureWidth(true), getCaptureHeight(true), CV_8UC4);
	getOriginalMatRespectBorders()->copyTo(champImageColorTable);

	cv::Rect curRect(champRadius + diffL, champRadius + diffT, champImageColorTable.cols - champRadius * 2 + diffL, champImageColorTable.rows - champRadius * 2 + diffT);

	if (!ImageFilterMat::isValidRect(&champImageColorTable, curRect)) {
		return;
	}
	champImageColorTable = Mat4b(champImageColorTable, curRect);

	ImageFilterMat::saturation(champImageColorTable, 0, 255, 1);
	resize(champImageColorTable, champImageColorTable, cv::Size(champKeyWidth, champKeyHeight), 0, 0, INTER_CUBIC);
	
	

	// ImageFilterMat::killGrayPixel(champImageColorTable, 25);
}

Vec4b HSChamp::getCurrentColor(int index) {
	Vec4b color(0, 0, 0, 255);
	return color;
}

void HSChamp::updateKey() {

	/*
	for (vector<CorsairLedPosition>::iterator it = champKeys.begin(); it != champKeys.end(); ++it) {
	

		cv::Rect testRect((int)it->left, (int)it->top, (int)it->width, (int)it->height);
		Mat4b testMat = Mat(champImageColorTable, testRect);
		resize(testMat, testMat, cv::Size(1, 1), 0, 0, INTER_CUBIC);
		
		Vec4b color = testMat.at<Vec4b>(0, 0);
		LEDController::getInstance()->setKey(it->ledId, color[2], color[1], color[0]);
	}
	*/

}

int HSChamp::getUpdateFrame() {
	return 20;
}

int HSChamp::getMaxTick() {
	return 1;
}

