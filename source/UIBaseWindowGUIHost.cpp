#include "UIBaseWindowGUIHost.h"
#include "UIBaseElement.h"

UIBaseWindowGUIHost::UIBaseWindowGUIHost()
{
	
}


UIBaseWindowGUIHost::~UIBaseWindowGUIHost()
{
}

void UIBaseWindowGUIHost::addElement(UIBaseElement* element) {
	uiElements.push_back(element);
	doRedraw = true;
}

int UIBaseWindowGUIHost::onClick(int mouseX, int mouseY) {

	vector<UIBaseElement*>::iterator it = uiElements.end();
	while (it != uiElements.begin())
	{
		--it;
		
		UIBaseElement* element = *it;
		int elementMinX = element->getX();
		int elementMaxX = elementMinX + element->getWidth();
		int elementMinY = element->getY();
		int elementMaxY = elementMinY + element->getHeight();

		if (elementMinX <= mouseX && elementMaxX >= mouseX &&
			elementMinY <= mouseY && elementMaxY >= mouseY) {
			if (element->getVisibility() && !element->isDisabled() && element->getOnClickId() != -1) {
				return element->getOnClickId();
			}
		}
	}
	return -1;
}

void UIBaseWindowGUIHost::onMouseMove(int mouseX, int mouseY) {

	for (vector<UIBaseElement*>::iterator it = uiElements.begin(); it != uiElements.end(); ++it) {
		UIBaseElement* element = *it;
		int elementMinX = element->getX();
		int elementMaxX = elementMinX + element->getWidth();
		int elementMinY = element->getY();
		int elementMaxY = elementMinY + element->getHeight();

		if (element->getVisibility() && 
			elementMinX <= mouseX && elementMaxX >= mouseX &&
			elementMinY <= mouseY && elementMaxY >= mouseY) {
			element->onMouseOn(mouseX - elementMinX, mouseY - elementMinY);
		}
		else {
			element->onMouseOut();
		}
	}
}

bool UIBaseWindowGUIHost::elementNeedsUpdate() {
	for (vector<UIBaseElement*>::iterator it = uiElements.begin(); it != uiElements.end(); ++it) {
		UIBaseElement* element = *it;
		if (element->needsUpdate()) {
			return true;
		}
	}
	return false;
}

void UIBaseWindowGUIHost::initHost() {
	RECT uiDimension;
	windowHDC = GetDC(UIBaseWindow::getHandle());
	GetClientRect(hwnd, &uiDimension);

	uiWidth = uiDimension.right - uiDimension.left;
	uiHeight = uiDimension.bottom - uiDimension.top;

	drawHDC = CreateCompatibleDC(windowHDC);
	HBITMAP drawHBitmap = CreateCompatibleBitmap(windowHDC, uiWidth, uiHeight);
	SelectObject(drawHDC, drawHBitmap);
}

int UIBaseWindowGUIHost::getCurrentBackgroundResource() {
	return currentBackgroundRes;
}

bool UIBaseWindowGUIHost::setBackgroundResource(int resource) {
	if (currentBackgroundRes != resource) {
		if (windowBackgrounds.find(resource) == windowBackgrounds.end()) {
			windowBackgrounds[resource] = ImageFilterMat::loadResourceAsMat(resource);
		}
		
		currentBackgroundRes = resource;
		doRedraw = true;
		return true;
	}
	return false;
}

void UIBaseWindowGUIHost::forceRefresh() {
	doRedraw = true;
}

bool UIBaseWindowGUIHost::goingToDrawUpdate() {
	return doRedraw;
}

bool UIBaseWindowGUIHost::processUI(bool forceDraw) {
	if ((doRedraw || elementNeedsUpdate()) || forceDraw) {
		doRedraw = false;
		// NuLogger::getInstance()->log("Redraw!");
		

		Mat4b drawUI = Mat4b(uiHeight, uiWidth, Scalar(0, 0, 0, 255));

		if (currentBackgroundRes > -1) {
			if (windowBackgrounds[currentBackgroundRes].cols > 0 && windowBackgrounds[currentBackgroundRes].rows > 0) {
				ImageFilterMat::overlayImage(&drawUI, &windowBackgrounds[currentBackgroundRes], cv::Point(0, 0));
			}
		}

		for (vector<UIBaseElement*>::iterator it = uiElements.begin(); it != uiElements.end(); ++it) {
			UIBaseElement* element = *it;

			if (element->getVisibility()) {
				element->processUI(&drawUI);
			}
			element->needsUpdate(false);
		}



		// draw the UI
		ImageFilterMat::DrawToHDC(drawHDC, drawUI, 0, 0, windowBackgrounds[currentBackgroundRes].cols, windowBackgrounds[currentBackgroundRes].rows);

		// double buffer write
		BitBlt(windowHDC, 0, 0, uiWidth, uiHeight, drawHDC, 0, 0, SRCCOPY);
		
		return true;
	}
	return false;
}

