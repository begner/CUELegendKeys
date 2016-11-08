#include "ScreenHotSpot.h"
#include <process.h> 

ScreenHotSpot::ScreenHotSpot() 
{
	
}

ScreenHotSpot::~ScreenHotSpot()
{
}

int ScreenHotSpot::getType() 
{
	return ScreenHotSpot::TYPE_UNKNOWN;
}

void ScreenHotSpot::setCaptureCoordinates(int x, int y, int w, int h) {
	captureX = x;
	captureY = y;
	captureW = w;
	captureH = h;
	resetResources();
}

int ScreenHotSpot::getCaptureX(bool borderCorrect, bool rawMode)
{
	if (rawMode) {
		if (borderCorrect)
			return captureX + border;
		return captureX;
	}
	else {
		if (borderCorrect)
			return (captureX - captureOffsetX) + border;
		return (captureX - captureOffsetX);
	}
}

int ScreenHotSpot::getCaptureY(bool borderCorrect, bool rawMode)
{
	if (rawMode) {
		if (borderCorrect)
			return captureY + border;
		return captureY;
	}
	else {
		if (borderCorrect)
			return (captureY - captureOffsetY) + border;
		return (captureY - captureOffsetY);
	}
}

int ScreenHotSpot::getCaptureWidth(bool borderCorrect)
{
	if (borderCorrect)
		return captureW - border * 2;
	return captureW;
}

int ScreenHotSpot::getCaptureHeight(bool borderCorrect)
{
	if (borderCorrect)
		return captureH - border * 2;
	return captureH;
}

void  ScreenHotSpot::setUiMatOffset(int x, int y) {
	captureOffsetX = x;
	captureOffsetY = y;
}

void ScreenHotSpot::setUiCoordinates(int x, int y, int w, int h) {
	uiX = x;
	uiY = y;
	uiW = w;
	uiH = h;
}

void ScreenHotSpot::deleteFilteredMatRect(int x, int y, int width, int height) {
	deleteFMX = x;
	deleteFMY = y;
	deleteFMW = width;
	deleteFMH = height;
}

int ScreenHotSpot::getUiX()
{
	return uiX;
}

int ScreenHotSpot::getUiY()
{
	return uiY;
}

int ScreenHotSpot::getUiWidth()
{
	return uiW;
}

int ScreenHotSpot::getUiHeight()
{
	return uiH;
}

void ScreenHotSpot::setBorder(int b) {
	border = b;
}

int ScreenHotSpot::getBorder()
{
	return border;
}

void ScreenHotSpot::addPreviewColorCoordinates(int x, int y, int width, int height) {
	cv::Rect coords(x, y, width, height);
	previewColors.push_back(coords);
	previewColorsMat.push_back(Mat4b(width, height));
}

vector<cv::Rect>* ScreenHotSpot::getPreviewColors() {
	return &previewColors;
}

void ScreenHotSpot::addKey(CorsairLedId ledId) {
	keys.push_back(ledId);
}

CorsairLedId ScreenHotSpot::getKey(int index) {
	return keys[index];
}

vector<CorsairLedId>* ScreenHotSpot::getKeys() {
	return &keys;
}

void ScreenHotSpot::setUIMat(Mat4b* uiMat) {
	uiMatP = uiMat;
	setCacheDirtyState(true);
}

void ScreenHotSpot::copyMats() {

	

	Mat4b uiMatCopy1(getCaptureWidth(), getCaptureHeight(), CV_8UC4);
	uiMatP->copyTo(uiMatCopy1);
	cv::Rect captureRect(getCaptureX(), getCaptureY(), getCaptureWidth(), getCaptureHeight());

	
	if (!ImageFilterMat::isValidRect(&uiMatCopy1, captureRect)) {
		return;
	}

	originalMat = Mat4b(uiMatCopy1, captureRect);

	Mat4b uiMatCopy2(getCaptureWidth(), getCaptureHeight(), CV_8UC4);
	uiMatP->copyTo(uiMatCopy2);
	cv::Rect rect(getCaptureX(true), getCaptureY(true), getCaptureWidth(true), getCaptureHeight(true));

	if (!ImageFilterMat::isValidRect(&uiMatCopy2, rect)) {
		return;
	}
	originalMatRespectBorders = Mat4b(uiMatCopy2, rect);

	originalMatRespectBorders.copyTo(filteredMat);

	cv::Rect deletreRect(deleteFMX, deleteFMY, deleteFMW, deleteFMH);
	cv::rectangle(filteredMat, deletreRect, Scalar(0, 0, 0, 255), CV_FILLED, 8, 0);
	
}

void ScreenHotSpot::setScreenshotMat(Mat4b* screenshot) {
	screenshotMat = screenshot;
	setCacheDirtyState(true);
}

Mat4b* ScreenHotSpot::getScreenshotMat() {
	return screenshotMat;
}

Mat4b* ScreenHotSpot::getOriginalMat() {
	return &originalMat;
}

Mat4b* ScreenHotSpot::getOriginalMatRespectBorders() {
	return &originalMatRespectBorders;
}

Mat4b* ScreenHotSpot::getFilteredMat() {
	return &filteredMat;
}

Mat4b* ScreenHotSpot::getPreviewColorMat(int index) {
	Vec4b color = getCurrentColor(index);
	Scalar scolor(color[0], color[1], color[2], 255);

	cv::Rect *cP = &previewColors[index];
	cv::Rect rect(0, 0, cP->width, cP->height);

	Mat4b* previewColorMat = &previewColorsMat[index];
	cv::rectangle(*previewColorMat, rect, scolor, CV_FILLED, 8, 0);
	return previewColorMat;
}

void ScreenHotSpot::filterMat() {

}


void ScreenHotSpot::tick() {
	animationTick++;
	if (animationTick > getMaxTick()) {
		animationTick = 0;
	}
}

int ScreenHotSpot::getCurrentTick() {
	return animationTick;
}

void ScreenHotSpot::updateKey() {
	Vec4b color = getCurrentColor();

	LEDController::getInstance()->setKey(getKey(), color[2], color[1], color[0]);
}

int ScreenHotSpot::getUpdateFrame() {
	return 1;
}

void ScreenHotSpot::initialize() {

}

bool ScreenHotSpot::hasExclusiveEffect() {
	return false;
}

void ScreenHotSpot::doExclusiveEffect() {

}

void ScreenHotSpot::initializeFrame() {
	copyMats();
	filterMat();
	setCacheDirtyState(true);
}

/*
HANDLE ScreenHotSpot::initializeFrameInThread() {
	HANDLE mtHandle = (HANDLE)_beginthreadex(0, 0, &ScreenHotSpot::doInitializeFrame, this, 0, 0);
	// filterMat();
	return mtHandle;
}

unsigned int __stdcall ScreenHotSpot::doInitializeFrame(void *p_this) {
	ScreenHotSpot* mySelf = static_cast<ScreenHotSpot*>(p_this);
	mySelf->initializeFrame();
	return 0;
}
*/

void ScreenHotSpot::resetResources() {
	initialize();
}


bool ScreenHotSpot::isCacheDirty() {
	return cacheIsDirty;
}

void ScreenHotSpot::setCacheDirtyState(bool state) {
	cacheIsDirty = state;
}

void ScreenHotSpot::setCurrentColorCoord(int x, int y) {
	currentColorCoordX = x;
	currentColorCoordY = y;
}

Mat ScreenHotSpot::getFilterdMatForUI() {
	Mat fMat;
	getFilteredMat()->copyTo(fMat);


	cv::Rect curRect;
	curRect = cv::Rect(currentColorCoordX - 1, currentColorCoordY - 1, 3, 3);


	cv::Vec4b cc = getCurrentColor(0);
	Scalar color = Scalar(cc[0], cc[1], cc[2], 255);
	cv::rectangle(fMat, curRect, color);
	
	curRect = cv::Rect(currentColorCoordX - 2, currentColorCoordY - 2, 5, 5);
	cv::rectangle(fMat, curRect, Scalar(255, 255, 255, 255));
	
	return fMat;
}

Mat ScreenHotSpot::getOriginalMatForUI() {
	return *getOriginalMatRespectBorders();
}
