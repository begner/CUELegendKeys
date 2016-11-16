#include "HSChamp.h"

void HSChamp::initialize() {

	createMask();
}

HSChamp::HSChamp()
{
	

}

HSChamp::~HSChamp()
{
}

void HSChamp::createMask() {
	// create mask
	int cW = getCaptureWidth(true);
	int cH = getCaptureHeight(true);
	if (cW != mask.cols || cH != mask.rows) { 
		int radius = cW - 8;
		mask = Mat(cH, cW, CV_8UC4);
		cv::rectangle(mask, cv::Point(0, 0), cv::Point(cW, cH), Scalar(0, 0, 0, 255), CV_FILLED, 8, 0);
		cv::circle(mask, cv::Point((int)floor(cW / 2), (int)floor(cH / 2)), (int)floor(radius / 2), Scalar(255, 255, 255, 255), CV_FILLED, CV_AA, 0);
	}
}

int HSChamp::getType() {
	return ScreenHotSpot::TYPE_CHAMP;
}

void HSChamp::filterMat() {

	champImageColorTable = Mat4b(getCaptureWidth(true), getCaptureHeight(true), CV_8UC4);
	if (!champImageColorTable.empty()) {
		getOriginalMatRespectBorders()->copyTo(champImageColorTable);

		ImageFilterMat::saturation(champImageColorTable, 0, 255, 150);

		resize(champImageColorTable, champImageColorTable, cv::Size(getUiWidth(), getUiHeight()), 0, 0, INTER_CUBIC);
	}
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


Mat HSChamp::getFilterdMatForUI() {
	createMask();
	Mat ret;
	ret = ScreenHotSpot::getFilterdMatForUI();
	if (!ret.empty()) {
		ImageFilterMat::addAlphaMask(&ret, &mask);
	}
	return ret;
}

Mat HSChamp::getOriginalMatForUI() {
	createMask();
	Mat ret;
	ret = ScreenHotSpot::getOriginalMatForUI();
	if (!ret.empty()) {
		ImageFilterMat::addAlphaMask(&ret, &mask);
	}
	return ret;
}

