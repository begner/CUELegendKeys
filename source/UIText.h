#pragma once

#include "UIBaseElement.h"

class UIText :
	public UIBaseElement
{
private:
	int fontFace = FONT_HERSHEY_DUPLEX;
	double fontSize = 0.6;
	int fontWeight = 1;
	int fontLineType = CV_AA;
	int shadowOffset = 2;
	string label = "";
public:
	UIText(string text);
	~UIText();
	void setLabel(string text);

	void processUI(Mat4b* drawUI);
	int getWidth();
	int getHeight();
};


