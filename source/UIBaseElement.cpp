#include "UIBaseElement.h"



UIBaseElement::UIBaseElement()
{
}


UIBaseElement::~UIBaseElement()
{
}


bool UIBaseElement::isDisabled() {
	return false;
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

void UIBaseElement::setOffset(int x, int y) {
	offsetX = x;
	offsetY = y;
}

void UIBaseElement::show() {
	setVisibility(true);
}

void UIBaseElement::hide() {
	setVisibility(false);
}

void UIBaseElement::setVisibility(bool state) {
	if (getVisibility() != state) {
		visibility = state;
		needsUpdate(true);
	}
}

bool UIBaseElement::getVisibility() {
	return visibility;
}

int UIBaseElement::getX() {
	return posX + offsetX;
}

int UIBaseElement::getY() {
	return posY + offsetY;
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