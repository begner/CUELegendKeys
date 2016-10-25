#pragma once

#include "UIBaseElement.h"
#include <unordered_map>

class UIButton :
	public UIBaseElement
{
private:
	int currentImage = 0;
	unordered_map<int, int[3]> imageResources;
	unordered_map<int, Mat[2]> images;

	int edgeWidth = -1;
	
	int buttonWidth;

	int fontFace = FONT_HERSHEY_DUPLEX;
	double fontSize = 0.5;
	int fontWeight = 1;
	int fontLineType = CV_AA;
	string label = "";
	int shadowOffset = 2;
	double scale = 1;

	void createImages();
	void createImage(int buttonState);
	void createImage(int buttonState, int resourceNormal, int resourceOver, int resourceMask);
	
	static const int MOUSE_STATE_OFF = 0;
	static const int MOUSE_STATE_OVER = 1;
	int currentMouseState = MOUSE_STATE_OFF;

	static const int IMAGE_STATE_NORMAL = 0;
	static const int IMAGE_STATE_MOUSE_OVER = 1;
	static const int IMAGE_STATE_MASK = 2;

public:
		
	static CONST int BUTTON_STATE_NORMAL = 0;
	static CONST int BUTTON_STATE_DISABLED = 1;
	static CONST int BUTTON_STATE_ACTIVE = 2;
	static CONST int BUTTON_STATE_SELECTED = 3;
	static CONST int BUTTON_STATE_EXTRA = 4;

	

	bool isDisabled();
	UIButton(int resourceNormal, int resourceOver, int resourceMask);
	~UIButton();
	void set9ScaleSize(int size);
	void setWidth(int width);
	void addState(int buttonState, int resourceNormal, int resourceHover, int resourceMask);
	void addState(int buttonState, int resourceNormal, int resourceMask);
	void processUI(Mat4b* drawUI);
	void setFontSize(double size);
	bool onMouseOut();
	bool onMouseOn(int uiX, int uiY);
	void setLabel(string text);
	int getWidth();
	int getHeight();
	bool setState(int buttonState);
};


