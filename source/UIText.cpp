#include "UIText.h"



UIText::UIText(string text)
{
	label = text;
}


UIText::~UIText()
{
}


void UIText::processUI(Mat4b* drawUI) {
	
	if (label != "") {
		putText(*drawUI, label, cv::Point(getY() + shadowOffset, getY()+ shadowOffset), fontFace, fontSize, Scalar(0, 0, 0, 128), fontWeight, fontLineType);
		putText(*drawUI, label, cv::Point(getY(), getY()), fontFace, fontSize, Scalar(255, 255, 255, 255), fontWeight, fontLineType);
	}

}

void UIText::setLabel(string text) {
	label = text;
	needsUpdate(true);
}

int UIText::getWidth() {
	int baseLine;
	cv::Size mySize = getTextSize(label, fontFace, fontSize, fontWeight, &baseLine);
	return mySize.width;
}

int UIText::getHeight() {
	int baseLine;
	cv::Size mySize = getTextSize(label, fontFace, fontSize, fontWeight, &baseLine);
	return mySize.height;
}

