#include "UIBaseElement.h"



UIBaseElement::UIBaseElement()
{
}


UIBaseElement::~UIBaseElement()
{
}

void UIBaseElement::setOnClickId(int id) {
	onClickId = id;
}

int UIBaseElement::getOnClickId() {
	return onClickId;
}

void UIBaseElement::setPos(int x, int y) {
	posX = x;
	posY = y;
}

int UIBaseElement::getX() {
	return posX;
}

int UIBaseElement::getY() {
	return posY;
}

bool UIBaseElement::onMouseOn(int x, int y) {
	return false;
}

bool UIBaseElement::onMouseOut() {
	return false;
}

bool UIBaseElement::needsUpdate() {
	bool ret = needUpdate;
	needUpdate = false;
	return ret;
}
void UIBaseElement::needsUpdate(bool state) {
	needUpdate = state;
}