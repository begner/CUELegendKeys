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
			return element->getOnClickId();
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

		if (elementMinX <= mouseX && elementMaxX >= mouseX &&
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

void UIBaseWindowGUIHost::setBackgroundResource(int resouce) {
	windowBackground = ImageFilterMat::loadResourceAsMat(resouce);
	doRedraw = true;
}

void UIBaseWindowGUIHost::forceRefresh() {
	doRedraw = true;
}

void UIBaseWindowGUIHost::processUI() {

	if (doRedraw || elementNeedsUpdate()) {
		NuLogger::getInstance()->log("Redraw!");

		Mat4b drawUI = Mat4b(uiHeight, uiWidth);

		if (windowBackground.cols > 1 && windowBackground.rows > 1) {
			windowBackground.copyTo(drawUI);
		}

		for (vector<UIBaseElement*>::iterator it = uiElements.begin(); it != uiElements.end(); ++it) {
			UIBaseElement* element = *it;
			element->processUI(&drawUI);
		}



		// draw the UI
		ImageFilterMat::DrawToHDC(drawHDC, drawUI, 0, 0, windowBackground.cols, windowBackground.rows);

		// double buffer write
		BitBlt(windowHDC, 0, 0, uiWidth, uiHeight, drawHDC, 0, 0, SRCCOPY);

		doRedraw = false;
	}

}

