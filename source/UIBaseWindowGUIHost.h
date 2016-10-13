#pragma once
#include "UIBaseWindow.h"
#include "UIBaseElement.h"
#include "UIButton.h"
#include "UIImage.h"
#include <vector>
#include "ImageFilterMat.h"

class UIBaseWindowGUIHost :
	public UIBaseWindow
{
private:
	vector<UIBaseElement*> uiElements;
	
	bool doRedraw = false;

	HDC windowHDC;
	int uiWidth;
	int uiHeight;

	HDC drawHDC;

	Mat4b windowBackground;
	bool elementNeedsUpdate();

public:
	UIBaseWindowGUIHost();
	~UIBaseWindowGUIHost();
	void addElement(UIBaseElement* element);
	void processUI();
	void initHost();
	void setBackgroundResource(int resouce);
	void forceRefresh();
	void onMouseMove(int mouseX, int mouseY);
	int onClick(int mouseX, int mouseY);
};

