#pragma once
#include "UIBaseWindow.h"
#include "UIBaseElement.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"
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

	int currentBackgroundRes = -1;
	map<int, Mat4b> windowBackgrounds;

	bool elementNeedsUpdate();

public:
	UIBaseWindowGUIHost();
	~UIBaseWindowGUIHost();
	void addElement(UIBaseElement* element);
	void processUI(bool forceDraw = false);
	void initHost();
	int getCurrentBackgroundResource();
	bool setBackgroundResource(int resouce);
	void forceRefresh();
	void onMouseMove(int mouseX, int mouseY);
	int onClick(int mouseX, int mouseY);
};

