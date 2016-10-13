#pragma once

#include "UIBaseElement.h"

class UIButton :
	public UIBaseElement
{
private:
	Mat imageNormal;
	Mat imageOver;
	Mat imageMask;
	Mat* currentImage;
	int fontFace = FONT_HERSHEY_DUPLEX;
	double fontSize = 0.6;
	int fontWeight = 1;
	int fontLineType = CV_AA;
	string label = "";
	int shadowOffset = 2;
	double scale = 1;
public:
	UIButton(int resourceNormal, int resourceOver, int resourceMask, double scale = 1);
	~UIButton();
	void processUI(Mat4b* drawUI);
	bool onMouseOut();
	bool onMouseOn(int uiX, int uiY);
	void setLabel(string text);
	int getWidth();
	int getHeight();
};


