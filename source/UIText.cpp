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

void UIText::setFontFace(int face) {
	fontFace = face;
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

		vector<string> lines = {};


		if (getWidth() - mySize.width > 0) {
			lines.push_back(label);
		}
		else {
			// multiline!
			stringstream ss(label);
			istream_iterator<string> begin(ss);
			istream_iterator<string> end;
			vector<string> vstrings(begin, end);

			string checkLine, line = "";
			cv::Size lineSize;
			bool lastWord = false;

			for (vector<string>::iterator it = vstrings.begin(); it != vstrings.end(); ++it) {
				
				checkLine = line + *it;
				lineSize = getTextSize(line, fontFace, fontSize, fontWeight, &baseLine);

				if (next(it) == vstrings.end()) {
					line = checkLine;
					lastWord = true;
				}

				if (lineSize.width > getWidth() || lastWord) {
					lines.push_back(line);
					line = "";
				}
				else {
					line = line + *it + " ";
				}
			}
		}
		
		string currentLine = "";
		int lineNr = 0;
		int lineY = 0;
		for (vector<string>::iterator it = lines.begin(); it != lines.end(); ++it) {
			currentLine = *it;
			lineNr = (int)(it - lines.begin());
			lineY = (int)(labelY + (lineNr * (mySize.height + baseLine)));

			putText(*drawUI, currentLine, cv::Point(labelX + shadowOffset, lineY + shadowOffset), fontFace, fontSize, Scalar(0, 0, 0, 128), fontWeight, fontLineType);
			putText(*drawUI, currentLine, cv::Point(labelX, lineY), fontFace, fontSize, fontColor, fontWeight, fontLineType);
		}
		

		/*

		for (i, line in enumerate(text.split('\n')) :
			y = y0 + i*dy
			cv2.putText(img, line, (50, y), cv2.FONT_HERSHEY_SIMPLEX, 1, 2)
			*/
	

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

