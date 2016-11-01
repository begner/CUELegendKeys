#include "UIImage.h"



UIImage::UIImage()
{

}

UIImage::UIImage(int resourceNormal, int resourceMask)
{
	Mat imageMask;
	if (resourceMask > 0) {
		imageMask = ImageFilterMat::loadResourceAsMat(resourceMask);
	}
	
	Mat image = ImageFilterMat::loadResourceAsMat(resourceNormal);
	if (resourceMask > 0) {
		ImageFilterMat::addAlphaMask(&image, &imageMask);
	}
	currentImage = image;
}

UIImage::~UIImage()
{
}

void UIImage::processUI(Mat4b* drawUI) {
	if (getWidth() > 0 && getHeight() > 0) {
		/*
		if (getX() + currentImage.cols > drawUI->cols ||
			getY() + currentImage.rows > drawUI->rows) {
			needsUpdate(true);
		}
		else {
		*/
			ImageFilterMat::overlayImage(drawUI, &currentImage, cv::Point(getX(), getY()));
		// }

		
	}
}

void UIImage::setImage(Mat* src) {
	src->copyTo(currentImage);
	needsUpdate(true);
}

int UIImage::getWidth() {
	return currentImage.cols;
}

int UIImage::getHeight() {
	return currentImage.rows;
}