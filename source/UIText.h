#pragma once

#include "UIBaseElement.h"

class UIText :
	public UIBaseElement
{
private:
	int fontFace = FONT_HERSHEY_DUPLEX;
	double fontSize = 0.6;
	Scalar fontColor = Scalar(255, 255, 255, 255);
	int fontWeight = 1;
	int fontLineType = CV_AA;
	int shadowOffset = 2;
	string label = "";
	int align;
	int textWidth = -1; // -1 = auto
public:
	static CONST int ALIGN_LEFT = 0;
	static CONST int ALIGN_CENTER = 1;
	

	UIText(string text);
	~UIText();
	void alignCenter();
	void alignLeft();
	void setWidth(int width);
	void setFontSize(double size);
	void setFontColor(Scalar color);
	void setFontColor(int r, int g, int b, int a = 255);
	void setLabel(string text, ...);
	void processUI(Mat4b* drawUI);
	int getWidth();
	int getHeight();
};


