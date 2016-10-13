#include "UIImage.h"



UIImage::UIImage()
{

}

UIImage::~UIImage()
{
}

void UIImage::processUI(Mat4b* drawUI) {
	if (getWidth() > 0 && getHeight() > 0) {
		ImageFilterMat::overlayImage(drawUI, &currentImage, cv::Point(getX(), getY()));
	}
}

void UIImage::setImage(Mat src) {
	currentImage = src;
	needsUpdate(true);

}

int UIImage::getWidth() {
	return currentImage.cols;
}

int UIImage::getHeight() {
	return currentImage.rows;
}