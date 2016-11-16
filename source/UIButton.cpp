#include "UIButton.h"


UIButton::UIButton(int resourceNormal, int resourceHover)
{
	addState(BUTTON_STATE_NORMAL, resourceNormal, resourceHover);
	currentImage = BUTTON_STATE_NORMAL;
}

UIButton::UIButton(int resourceNormal, int resourceHover, int resourceMask) // int resourceNormal, int resourceOver, int resourceMask, double scaleFactor)
{
	addStateMask(BUTTON_STATE_NORMAL, resourceNormal, resourceHover, resourceMask);
	currentImage = BUTTON_STATE_NORMAL;
}

UIButton::~UIButton()
{
}

void UIButton::addIcon(int ICONRESOURCE, int posX, int posY) {
	iconMat = ImageFilterMat::loadResourceAsMat(ICONRESOURCE);
	iconX = posX;
	iconY = posY;
}

void UIButton::addState(int buttonState, int resourceNormal) {
	addState(buttonState, resourceNormal, resourceNormal);
}

void UIButton::addState(int buttonState, int resourceNormal, int resourceHover) {
	imageResources[buttonState][IMAGE_STATE_NORMAL] = resourceNormal;
	imageResources[buttonState][IMAGE_STATE_MOUSE_OVER] = resourceHover;
	imageResources[buttonState][IMAGE_STATE_MASK] = 0;
	createImage(buttonState);
}


void UIButton::addStateMask(int buttonState, int resourceNormal, int resourceMask) {
	addStateMask(buttonState, resourceNormal, resourceNormal, resourceMask);
}

void UIButton::addStateMask(int buttonState, int resourceNormal, int resourceHover, int resourceMask) {
	imageResources[buttonState][IMAGE_STATE_NORMAL] = resourceNormal;
	imageResources[buttonState][IMAGE_STATE_MOUSE_OVER] = resourceHover;
	imageResources[buttonState][IMAGE_STATE_MASK] = resourceMask;
	createImage(buttonState);
}

 
void UIButton::setWidth(int width) {
	buttonWidth = width;
	createImages();
	
}

void UIButton::createImages() {
	for (auto it = imageResources.begin(); it != imageResources.end(); ++it) {
		createImage(it->first);
	}
}

void UIButton::createImage(int buttonState) {

	// IMAGE_STATE_NORMAL, IMAGE_STATE_MOUSE_OVER
	for (int imageNr = 0; imageNr < 2; imageNr++) {

		Mat image = ImageFilterMat::loadResourceAsMat(imageResources[buttonState][imageNr]);

		if (imageResources[buttonState][IMAGE_STATE_MASK] > 0) {
			Mat imageMask = ImageFilterMat::loadResourceAsMat(imageResources[buttonState][IMAGE_STATE_MASK]);
			ImageFilterMat::addAlphaMask(&image, &imageMask);
		}	
		
		if (edgeWidth < 0) {
			images[buttonState][imageNr] = Mat(image);
			continue;
		}

		// if no width is set - autoWidth
		if (buttonWidth < 1) {
			buttonWidth = image.cols;
		}


		Mat targetImage(image.rows, buttonWidth, CV_8UC4, Scalar(0, 0, 0, 0));

		// left,top,width, height
		Mat TL = image(cv::Rect(0, 0, edgeWidth, edgeWidth));
		Mat T = image(cv::Rect(edgeWidth, 0, image.cols - edgeWidth*2, edgeWidth));
		Mat TR = image(cv::Rect(image.cols - edgeWidth, 0, edgeWidth, edgeWidth));
		Mat R = image(cv::Rect(image.cols - edgeWidth, edgeWidth, edgeWidth, image.rows - edgeWidth*2));
		Mat BR = image(cv::Rect(image.cols - edgeWidth, image.rows - edgeWidth, edgeWidth, edgeWidth));
		Mat B = image(cv::Rect(edgeWidth, image.rows - edgeWidth, image.cols - edgeWidth * 2, edgeWidth));
		Mat BL = image(cv::Rect(0, image.rows - edgeWidth, edgeWidth, edgeWidth));
		Mat L = image(cv::Rect(0, edgeWidth, edgeWidth, image.rows - edgeWidth * 2));
		Mat C = image(cv::Rect(edgeWidth, edgeWidth, image.cols - edgeWidth * 2, image.rows - edgeWidth * 2));

		TL.copyTo(targetImage(cv::Rect(0, 0, edgeWidth, edgeWidth)));
		TR.copyTo(targetImage(cv::Rect(targetImage.cols - edgeWidth, 0, edgeWidth, edgeWidth)));
		BR.copyTo(targetImage(cv::Rect(targetImage.cols - edgeWidth, targetImage.rows - edgeWidth, edgeWidth, edgeWidth)));
		BL.copyTo(targetImage(cv::Rect(0, targetImage.rows - edgeWidth, edgeWidth, edgeWidth)));
		
		resize(T, T, cv::Size(targetImage.cols - edgeWidth * 2, edgeWidth));
		T.copyTo(targetImage(cv::Rect(edgeWidth, 0, targetImage.cols - edgeWidth * 2, edgeWidth)));

		resize(L, L, cv::Size(edgeWidth, targetImage.rows - edgeWidth*2));
		L.copyTo(targetImage(cv::Rect(0, edgeWidth, edgeWidth, targetImage.rows - edgeWidth * 2)));

		resize(B, B, cv::Size(targetImage.cols - edgeWidth * 2, edgeWidth));
		B.copyTo(targetImage(cv::Rect(edgeWidth, targetImage.rows - edgeWidth, targetImage.cols - edgeWidth * 2, edgeWidth)));

		resize(R, R, cv::Size(edgeWidth, targetImage.rows - edgeWidth * 2));
		R.copyTo(targetImage(cv::Rect(targetImage.cols - edgeWidth, edgeWidth, edgeWidth, targetImage.rows - edgeWidth * 2)));
		
		resize(C, C, cv::Size(targetImage.cols - edgeWidth * 2, targetImage.rows - edgeWidth * 2));
		C.copyTo(targetImage(cv::Rect(edgeWidth, edgeWidth, targetImage.cols - edgeWidth * 2, targetImage.rows - edgeWidth * 2)));

		images[buttonState][imageNr] = Mat(targetImage);
	}

}


void UIButton::setFontSize(double size) {
	fontSize = size;
	needsUpdate(true);
}

bool UIButton::setState(int buttonState) {
	if (images.find(buttonState) != images.end()) {
		if (currentImage != buttonState) {
			currentImage = buttonState;
			needsUpdate(true);
			return true;
		}
		return false;
	}
	return false;
}


void UIButton::processUI(Mat4b* drawUI) {

	int currentState = IMAGE_STATE_NORMAL;

	if (currentMouseState == MOUSE_STATE_OVER) {
		currentState = IMAGE_STATE_MOUSE_OVER;
	}

	ImageFilterMat::overlayImage(drawUI, &images[currentImage][currentState], cv::Point(getX(), getY()));
	if (label != "") {
	
		int baseLine;
		cv::Size mySize = getTextSize(label, fontFace, fontSize, fontWeight, &baseLine);

		double paddingLeft = (double)(getWidth() - mySize.width) / (double)2;

		int labelX = getX() + (int)paddingLeft;
		int labelY = getY() + getHeight() - (int)((double)(getHeight() - mySize.height + baseLine*1.5)/(double)2 );

		for (int lineX = -1; lineX < 2; lineX++) {
			for (int lineY = -1; lineY < 2; lineY++) {
				putText(*drawUI, label, cv::Point(labelX + lineX, labelY + lineY), fontFace, fontSize, Scalar(80, 80, 80), fontWeight, fontLineType);
			}
		}
		
		putText(*drawUI, label, cv::Point(labelX+shadowOffset, labelY + shadowOffset), fontFace, fontSize, Scalar(0, 0, 0), fontWeight, fontLineType);
		putText(*drawUI, label, cv::Point(labelX, labelY), fontFace, fontSize, Scalar(255, 255, 255), fontWeight, fontLineType);

		
	}
	if (iconX > -1 && iconY > -1) {
		ImageFilterMat::overlayImage(drawUI, &iconMat, cv::Point(getX()+iconX, getY() + iconY));
	}
}

bool UIButton::isDisabled() {
	return (currentImage == BUTTON_STATE_DISABLED);
};

void UIButton::set9ScaleSize(int size) {
	edgeWidth = size;
	createImages();
}

void UIButton::setLabel(string text) {
	label = text;
}

bool UIButton::onMouseOut() {
	if (currentMouseState != MOUSE_STATE_OFF) {
		currentMouseState = MOUSE_STATE_OFF;
		needsUpdate(true);
		return true;
	}
	return false;
}

bool UIButton::isOverAlpha(int x, int y) {

	Mat* cI = &images[currentImage][0];

	// check alpha channel, if in visible area
	Vec4b color = cI->at<Vec4b>(cv::Point(x, y));
	if (color.val[3] < 64) {
		return false;
	}
	
	return true;
}

bool UIButton::onMouseOn(int x, int y) {
	// check alpha
	Mat* cI = &images[currentImage][0];

	if (x < 0 || x > cI->cols - 1 || y < 0 || y > cI->rows - 1) {
		return onMouseOut();
	}

	// no hover on disabled :D
	if (isDisabled()) {
		return onMouseOut();
	}

	if (currentMouseState != MOUSE_STATE_OVER) {

		// check alpha channel, if in visible area
		if (!isOverAlpha(x, y)) {
			return false;
		}

		currentMouseState = MOUSE_STATE_OVER;
		needsUpdate(true);
		return true;
	}
	return false;
}

int UIButton::getOnClickId(int mouseX, int mouseY ) {
	
	if (isDisabled()) {
		return -1;
	}

	if (!isOverAlpha(mouseX, mouseY)) {
		return -1;
	}

	return UIBaseElement::getOnClickId();
}

int UIButton::getWidth() {
	if (images.find(BUTTON_STATE_NORMAL) != images.end()) {
		return images[BUTTON_STATE_NORMAL][0].cols;
	}
	return 0;
}

int UIButton::getHeight() {
	if (images.find(BUTTON_STATE_NORMAL) != images.end()) {
		return images[BUTTON_STATE_NORMAL][0].rows;
	}
	return 0;
}