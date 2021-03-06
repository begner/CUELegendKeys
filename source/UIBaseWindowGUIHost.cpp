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
	
	int currentClickId = -1;
	int elementMinX, elementMaxX, elementMinY, elementMaxY = 0;
	UIBaseElement* element;

	while (it != uiElements.begin())
	{
		--it;
		
		element = *it;
		elementMinX = element->getX();
		elementMaxX = elementMinX + element->getWidth();
		elementMinY = element->getY();
		elementMaxY = elementMinY + element->getHeight();

		if (elementMinX <= mouseX && elementMaxX >= mouseX &&
			elementMinY <= mouseY && elementMaxY >= mouseY) {
			
			if (element->getVisibility() && element->getCaptureMouseEvents()) {
				return -1;
			}
			
			currentClickId = element->getOnClickId(mouseX - elementMinX, mouseY - elementMinY);
			if (element->getVisibility() && !element->isDisabled() && currentClickId != -1) {
				return currentClickId;
			}
			
		}
	}
	return -1;
}

void UIBaseWindowGUIHost::onMouseMove(int mouseX, int mouseY) {

	int elementMinX, elementMaxX, elementMinY, elementMaxY = 0;
	UIBaseElement* element;

	for (vector<UIBaseElement*>::iterator it = uiElements.begin(); it != uiElements.end(); ++it) {

		element = *it;
		elementMinX = element->getX();
		elementMaxX = elementMinX + element->getWidth();
		elementMinY = element->getY();
		elementMaxY = elementMinY + element->getHeight();

		if (element->getVisibility() && element->getCaptureMouseEvents()) {
			return;
		}

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

void UIBaseWindowGUIHost::freeBackgroundResources() {
	windowBackgrounds.clear();
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

