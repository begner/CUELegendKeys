#include "UIButton.h"



UIButton::UIButton(int resourceNormal, int resourceOver, int resourceMask, double scaleFactor)
{
	scale = scaleFactor;

	imageMask = ImageFilterMat::loadResourceAsMat(resourceMask);
	resize(imageMask, imageMask, cv::Size(), scale, scale, INTER_CUBIC);


	imageNormal = ImageFilterMat::loadResourceAsMat(resourceNormal);
	resize(imageNormal, imageNormal, cv::Size(), scale, scale, INTER_CUBIC);
	ImageFilterMat::addAlphaMask(&imageNormal, &imageMask); 

	imageOver = ImageFilterMat::loadResourceAsMat(resourceOver);
	resize(imageOver, imageOver, cv::Size(), scale, scale, INTER_CUBIC);
	ImageFilterMat::addAlphaMask(&imageOver, &imageMask);

	currentImage = &imageNormal;
}

UIButton::~UIButton()
{
}

void UIButton::processUI(Mat4b* drawUI) {
	

	ImageFilterMat::overlayImage(drawUI, currentImage, cv::Point(getX(), getY()));
	if (label != "") {
		
		int baseLine;
		cv::Size mySize = getTextSize(label, fontFace, fontSize, fontWeight, &baseLine);

		double paddingLeft = (double)(getWidth() - mySize.width) / (double)2;

		int labelX = getX() + (int)paddingLeft;
		int labelY = getY() + getHeight() - (int)((double)(getHeight() - mySize.height)/(double)2);

		putText(*drawUI, label, cv::Point(labelX+shadowOffset, labelY+ shadowOffset), fontFace, fontSize, Scalar(0, 0, 0, 128), fontWeight, fontLineType);
		putText(*drawUI, label, cv::Point(labelX, labelY), fontFace, fontSize, Scalar(255, 255, 255, 255), fontWeight, fontLineType);
	}
	
}

void UIButton::setLabel(string text) {
	label = text;
}

bool UIButton::onMouseOut() {
	Mat* testImage = &imageOver;
	if (currentImage == testImage) {
		currentImage = &imageNormal;
		needsUpdate(true);
		return true;
	}
	return false;
}

bool UIButton::onMouseOn(int x, int y) {
	// check alpha
	if (x < 0 || x > imageMask.cols - 1 || y < 0 || y > imageMask.rows - 1) {
		return onMouseOut();
	}

	Vec4b color = imageMask.at<Vec4b>(y, x);

	// NuLogger::getInstance()->log("Mask Color at %i x %i : %i, %i, %i", x, y, color.val[0], color.val[1], color.val[2]);
	if (color.val[0] < 64) {
		return onMouseOut();
	}

	Mat* testImage = &imageNormal;
	if (currentImage == testImage) {
		currentImage = &imageOver;
		needsUpdate(true);
		return true;
	}
	return false;
}

int UIButton::getWidth() {
	return imageNormal.cols;
}

int UIButton::getHeight() {
	return imageNormal.rows;
}