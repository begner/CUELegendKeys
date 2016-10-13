#include "HSLBase.h"

HSLBase::HSLBase()
{
}

HSLBase::~HSLBase()
{
}

void HSLBase::setNeedle(int templateResourceId) {
	setNeedle(ImageFilterMat::loadResourceAsMat(templateResourceId));
}

void HSLBase::setHaystack(Mat hs) {
	hs.copyTo(originalImage);
	hs.copyTo(haystackImage);
}

Mat* HSLBase::getHaystack() {
	return &haystackImage;
}

void HSLBase::setNeedle(Mat n) {
	n.copyTo(needleImage);
}

Mat* HSLBase::getNeedle() {
	return &needleImage;
}

vector<cv::Rect> HSLBase::getFoundLocations() {
	return foundLocations;
}
void HSLBase::addFoundLocation(cv::Rect foundRect) {
	foundLocations.push_back(foundRect);
}

Mat* HSLBase::getPreviewMatForLocationIndex(int idx) {
	Mat* preview;
	if (idx >= 0 && idx < foundLocations.size()) {
		preview = new Mat(originalImage, foundLocations[idx]);
	}
	else {
		preview = new Mat(1, 1, CV_8UC4);
	}
	return preview;
}
