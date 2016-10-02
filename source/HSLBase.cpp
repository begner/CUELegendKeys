#include "HSLBase.h"

HSLBase::HSLBase()
{
}

HSLBase::~HSLBase()
{
}

void HSLBase::setTargetRect(cv::Rect* tRect) {
	targetRect = tRect;
}

cv::Rect* HSLBase::getTargetRect() {
	return targetRect;
}

void HSLBase::setTemplateResourceId(int rid) {
	templateResourceId = rid;
	setNeedle(ImageFilterMat::loadResourceAsMat(templateResourceId));
}

void HSLBase::setHaystack(Mat hs) {
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


