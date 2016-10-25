#include "UIText.h"



UIText::UIText(string text)
{
	label = text;
}


UIText::~UIText()
{
}

void UIText::setFontSize(double size) {
	fontSize = size;
	needsUpdate(true);
}

void UIText::setFontColor(Scalar color) {
	fontColor = color;
	needsUpdate(true);
}

void UIText::setFontColor(int r, int g, int b, int a) {
	setFontColor(Scalar(b, g, r, a));
}

void UIText::processUI(Mat4b* drawUI) {
	
	if (label != "") {

		int baseLine;
		cv::Size mySize = getTextSize(label, fontFace, fontSize, fontWeight, &baseLine);

		int labelX = getX();
		int labelY = getY() + getHeight() - (int)((double)(getHeight() - mySize.height + baseLine) / (double)2);

		switch (align) {
			case ALIGN_CENTER:
				{
					double paddingLeft = (double)(getWidth() - mySize.width) / (double)2;
					labelX = getX() + (int)paddingLeft;
				}
				break;
			case ALIGN_LEFT:
				{

				}
				break;
		}

		putText(*drawUI, label, cv::Point(labelX + shadowOffset, labelY + shadowOffset), fontFace, fontSize, Scalar(0, 0, 0, 128), fontWeight, fontLineType);
		putText(*drawUI, label, cv::Point(labelX, labelY), fontFace, fontSize, fontColor, fontWeight, fontLineType);

		// rectangle(*drawUI, cv::Rect(getX(), getY(), getWidth(), getHeight()), Scalar(0, 0, 255));
	}
	

}

void UIText::setLabel(string text, ...) {
	char buffer[2048];
	va_list args;
	va_start(args, text);
	vsprintf_s(buffer, text.c_str(), args);
	perror(buffer);
	va_end(args);
	label = string(buffer);
	needsUpdate(true);
}



int UIText::getWidth() {
	if (textWidth == -1) {
		int baseLine;
		cv::Size mySize = getTextSize(label, fontFace, fontSize, fontWeight, &baseLine);
		return mySize.width;
	}
	else {
		return textWidth;
	}
	
}

void UIText::alignCenter() {
	align = ALIGN_CENTER;
}

void UIText::alignLeft() {
	align = ALIGN_LEFT;
}

void UIText::setWidth(int width) {
	textWidth = width;
}

int UIText::getHeight() {
	int baseLine;
	cv::Size mySize = getTextSize(label, fontFace, fontSize, fontWeight, &baseLine);
	return mySize.height;
}

