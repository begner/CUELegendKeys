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
}

int ScreenHotSpot::getCaptureX(bool borderCorrect)
{
	if (borderCorrect)
		return captureX + border;
	return captureX;
}

int ScreenHotSpot::getCaptureY(bool borderCorrect)
{
	if (borderCorrect)
		return captureY + border;
	return captureY;
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

void ScreenHotSpot::setUiCoordinates(int x, int y) {
	uiX = x;
	uiY = y;
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
}

void ScreenHotSpot::copyMats() {

	Mat4b uiMatCopy1(getCaptureWidth(), getCaptureHeight(), CV_8UC4);
	uiMatP->copyTo(uiMatCopy1);
	cv::Rect captureRect(getCaptureX(), getCaptureY(), getCaptureWidth(), getCaptureHeight());
	originalMat = Mat4b(uiMatCopy1, captureRect);

	Mat4b uiMatCopy2(getCaptureWidth(), getCaptureHeight(), CV_8UC4);
	uiMatP->copyTo(uiMatCopy2);
	cv::Rect rect(getCaptureX(true), getCaptureY(true), getCaptureWidth(true), getCaptureHeight(true));
	originalMatRespectBorders = Mat4b(uiMatCopy2, rect);

	originalMatRespectBorders.copyTo(filteredMat);

	cv::Rect deletreRect(deleteFMX, deleteFMY, deleteFMW, deleteFMH);
	cv::rectangle(filteredMat, deletreRect, Scalar(0, 0, 0, 255), CV_FILLED, 8, 0);
	
}

void ScreenHotSpot::setScreenshotMat(Mat4b* screenshot) {
	screenshotMat = screenshot;
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
}

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

