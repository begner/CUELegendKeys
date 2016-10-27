#include "HSLBar.h"

HSLBar::HSLBar(string title, string saveId, int needleResource, int hueRangeLow, int hueRangeHigh) : HSLBase(title, saveId, needleResource)
{
	hueRangeH = hueRangeHigh;
	hueRangeL = hueRangeLow;
}


HSLBar::~HSLBar()
{

}

void HSLBar::setPassive(cv::Rect input) {
	passiveRect = input;
}

void HSLBar::setAbilityQ(cv::Rect input) {
	abilityQRect = input;
}

void HSLBar::setAbilityF(cv::Rect input) {
	abilityFRect = input;
}

string HSLBar::getLearningRequiredText() {
	return "The passive, Ability Q and Ability F needs to be saved to autodetect this Element";
}

bool HSLBar::isLearningPossible() {
	return (ImageFilterMat::isValidRect(passiveRect) && ImageFilterMat::isValidRect(abilityQRect) && ImageFilterMat::isValidRect(abilityFRect));
}

bool HSLBar::findLocations(Mat* haystack, Mat* needle, int offsetX, int offsetY) {
	if (!isLearningPossible()) {
		NuLogger::getInstance()->log("Learning canceled: "+getLearningRequiredText());
		return false;
	}

	int paddingLR = 10;
	
	cv::Rect output(passiveRect.x - paddingLR, // x
			abilityQRect.y + abilityQRect.height, // y 
			abilityFRect.x + abilityFRect.width - passiveRect.x + paddingLR * 2,  // w
			getHaystack()->rows - 1 - (abilityQRect.y + abilityQRect.height));
	

	Mat test;
	(*getHaystack())(output).copyTo(test);
	
	ImageFilterMat::killDarkPixel(test, 70);
	ImageFilterMat::killGrayPixel(test, 20);
	
	cvtColor(test, test, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	// H = 0 - 179 ( = H° / 2)
	int iLowH = (int)floor(hueRangeL / 2);
	int iHighH = (int)floor(hueRangeH / 2);

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	Mat range;
	inRange(test, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), range); //Threshold the image
	
	cv::Rect foundBox = ImageFilterMat::findBox(range, 200);

	/*
	ImageFilterMat::brightnessContrast(test, &test, 2.5);
	
	ImageFilterMat::colorReduce(test, 128);
	*/


	output.x = output.x + foundBox.x;
	output.y = output.y + foundBox.y;
	output.width = foundBox.width;
	output.height = foundBox.height;
	addFoundLocation(output);
	return true;
}

bool HSLBar::isWide() {
	return true;
}

bool HSLBar::needsBorder() {
	return false;
}