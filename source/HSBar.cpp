#include "HSBar.h"



HSBar::HSBar()
{
}


HSBar::~HSBar()
{
}

int HSBar::getType() {
	return ScreenHotSpot::TYPE_BAR;
}

void HSBar::filterMat() {
	if (!getFilteredMat()->empty()) {
		ImageFilterMat::whiteToDarkPixel(*getFilteredMat(), 150);
		ImageFilterMat::killDarkPixel(*getFilteredMat(), 40);
		ImageFilterMat::saturation(*getFilteredMat(), 50, 255, 2);

		cv::Rect colorDetectionRect(0, (int)floor(getOriginalMatRespectBorders()->rows / 2), (int)getKeys()->size(), 1);

		if (!ImageFilterMat::isValidRect(getOriginalMatRespectBorders(), colorDetectionRect)) {
			return;
		}
		colorBarMat = Mat4b(*getOriginalMatRespectBorders(), colorDetectionRect);
		ImageFilterMat::saturation(colorBarMat, 0, 255, 1);
	}
}

Vec4b HSBar::getCurrentColor(int index) {
	int percent = determinePercentage();

	Vec4b color(0, 0, 0, 255);
	if (colorBarMat.cols < 1) {
		return color;
	}


	if (colorBarMat.cols >= index) {
		if (index * (int)(100 / colorBarMat.cols) < percent) {
			color = colorBarMat.at<Vec4b>(0, index);
		}
	}

	return color;
}

int HSBar::getMaxTick() {
	return 1;
}

int HSBar::determinePercentage() {
	int percent = ImageFilterMat::getBarPercentage(*getFilteredMat());
	return percent;
}


void HSBar::updateKey() {
	int percent = determinePercentage();
	
	vector<CorsairLedId>* allKeys = getKeys();
	size_t keyCount = allKeys->size();

	int index = 0;
	for (vector<CorsairLedId>::iterator currentKey = allKeys->begin(); currentKey != allKeys->end(); ++currentKey, ++index) {
		
		Vec4b color = getCurrentColor(index);
		LEDController::getInstance()->setKey((CorsairLedId)*currentKey, color[2], color[1], color[0]);
		
	}
}


