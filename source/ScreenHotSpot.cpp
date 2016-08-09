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


/*
ScreenHotSpot::ScreenHotSpot()
{
	
	for (int i = 0; i < 3; i++) {
		primaryColor[i] = 0;
		secondaryColor[i] = 0;
		currentColor[i] = 0;
	}
	
}


void ScreenHotSpot::setPercent(int p) {
	percent = p;
}

void ScreenHotSpot::UpdateKey() {
	if (t == ScreenHotSpot::TYPE_CASTABLE) {
		Vec3b myColor(currentColor[0], currentColor[1], currentColor[2]);
		LEDController::getInstance()->setKey(lid, currentColor[2], currentColor[1], currentColor[0]);
	}
	else if (t == ScreenHotSpot::TYPE_BAR) {
		for (int i = 0; i < getBarColorAmount(); i++) {

			Vec3b color(0, 0, 0);

			if (i * (int)(100 / getBarColorAmount()) < percent) {
				color = hsmat->at<Vec3b>(0, i);
			}

			Vec6i data = barColorCoordinates[i];

			LEDController::getInstance()->setKey((CorsairLedId)data[0], color[2], color[1], color[0]);
		}
		

		for (vector<Vec6i>::iterator it = barColorCoordinates.begin(); it != barColorCoordinates.end(); ++it) {
			
			for (int i = 0; i < hs->getBarColorAmount(); i++) {
				Vec3b myColor(0, 0, 0);
				if (i * (int)(100 / hs->getBarColorAmount()) < percent) {
					myColor = activeColor;
				}
				hs->setBarColor(i, myColor);
				KeyboardPreview::drawKeyColor(myColor, hs->getBarColorX(i), hs->getBarColorY(i));
			}

		

			auto ledColor = CorsairLedColor{ (CorsairLedId)data[0], color[2], color[1], color[0] };
			CorsairSetLedsColors(1, &ledColor);
		}
		
	}
}

void ScreenHotSpot::calculateCurrentColor() {
	int r = secondaryColor[2];
	int g = secondaryColor[1];
	int b = secondaryColor[0];

	// todo some animation here :)

	// setCurrentColor(r, g, b);
}


HBITMAP ScreenHotSpot::getBitmap() {
	return image;
}


void ScreenHotSpot::setBitmap(HBITMAP hbitmap) {
	image = hbitmap;
}



void ScreenHotSpot::setBitmapX(int x) {
	pbx = x;
}

void ScreenHotSpot::setBitmapY(int y) {
	pby = y;
}

int ScreenHotSpot::getBitmapX() {
	return pbx;
}

int ScreenHotSpot::getBitmapY() {
	return pby;
}


void ScreenHotSpot::setPrimaryColorCoordinates(int x, int y) {
	pColorX = x;
	pColorY = y;
}
void ScreenHotSpot::setSecondaryColorCoordinates(int x, int y) {
	sColorX = x;
	sColorY = y;
}

void ScreenHotSpot::setCurrentColorCoordinates(int x, int y) {
	cColorX = x;
	cColorY = y;
}

int ScreenHotSpot::getPrimaryColorX() {
	return pColorX;
}
int ScreenHotSpot::getPrimaryColorY() {
	return pColorY;
}
int ScreenHotSpot::getSecondaryColorX() {
	return sColorX;
}
int ScreenHotSpot::getSecondaryColorY() {
	return sColorY;
}
int ScreenHotSpot::getCurrentColorX() {
	return cColorX;
}
int ScreenHotSpot::getCurrentColorY() {
	return cColorY;
}

void ScreenHotSpot::setType(int type) {
	t = type;
}
void ScreenHotSpot::setCaptureCoordinates(int x, int y, int w, int h) {
	px = x;
	py = y;
	pw = w;
	ph = h;
}



void ScreenHotSpot::setSecondaryColor(Vec3b color) {
	setSecondaryColor(color[2], color[1], color[0]);
}

void ScreenHotSpot::setSecondaryColor(int r, int g, int b) {
	secondaryColor[2] = r;
	secondaryColor[1] = g;
	secondaryColor[0] = b;
	calculateCurrentColor();
}

void ScreenHotSpot::setPrimaryColor(Vec3b color) {
	setPrimaryColor(color[2], color[1], color[0]);
}

void ScreenHotSpot::setPrimaryColor(int r, int g, int b) {
	primaryColor[2] = r;
	primaryColor[1] = g;
	primaryColor[0] = b;
	calculateCurrentColor();
}

void ScreenHotSpot::setCurrentColor(Vec3b color) {
	currentColor = color;
}
void ScreenHotSpot::setCurrentColor(int r, int g, int b) {
	currentColor[2] = r;
	currentColor[1] = g;
	currentColor[0] = b;
}

Vec3b ScreenHotSpot::getCurrentColor() {
	return currentColor;
}
 
void ScreenHotSpot::addBarColorCoordinates(CorsairLedId key, int x, int y) {
	Vec6i pair(key, x, y, 0, 0, 0);
	barColorCoordinates.push_back(pair);
}

size_t ScreenHotSpot::getBarColorAmount() {
	return barColorCoordinates.size();
}

int ScreenHotSpot::getBarColorX(int count) {
	return barColorCoordinates[count][1];
}

int ScreenHotSpot::getBarColorY(int count) {
	return barColorCoordinates[count][2];
}

void ScreenHotSpot::setKey(CorsairLedId ledId) {
	lid = ledId;
}

void ScreenHotSpot::setBarColor(int count, Vec3b color) {
	barColorCoordinates[count][3] = color[2];
	barColorCoordinates[count][4] = color[1];
	barColorCoordinates[count][5] = color[0];
}


Vec3b ScreenHotSpot::getColorByTick() {
	int x = (animationTick % (hsmat->cols));
	int y = floor(animationTick / (hsmat->cols)); 

	

	Vec3b color = hsmat->at<Vec3b>(y,x);
	return color;
}

Vec3b ScreenHotSpot::getInternalAnimationColor() {

	int count = 0;
	int max = (hsmat->cols * hsmat->rows);
	Vec3b color;

	while (count < max) {
		color = getColorByTick();
		int r = color[2];
		int g = color[1];
		int b = color[0];

		if (r < 100 && g < 100 && b < 100) {
			tick();
		}
		else {
			break;
		}
	}
	

	return color;
}

Vec3b ScreenHotSpot::getAnimationColor() {
	Vec3b color = getInternalAnimationColor();
	return color;
}
*/