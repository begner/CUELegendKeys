#include "FPScreenMirror.h"



FPScreenMirror::FPScreenMirror(HWND uiHWND, CorsairLedId keyTL, CorsairLedId keyBR) : FPSMeter(), FPBase(uiHWND)
{
	NuLogger::getInstance()->log("Setup FrameProcessing ScreenMirror");
	// get keyboard keys
	allKeys = LEDController::getInstance()->getAllKeysByRect(keyTL, keyBR);
	reinitialize();
}

void FPScreenMirror::reinitialize() {

	RECT keyDim = LEDController::getInstance()->getKeyBoardDimensions(allKeys);

	keyboardWidth = keyDim.right;
	keyboardHeight = keyDim.bottom;

	if (!captureWindowHDC || !currentProcessHWND) {
		return;
	}

	RECT targetRect;
	GetWindowRect(currentProcessHWND, &targetRect);

	int clientWidth = targetRect.right - targetRect.left; // -offsetLeft - offsetRight;
	int clientHeight = targetRect.bottom - targetRect.top; // -offsetTop - offsetBottom;


	// targetWidth = uiWidth - marginLeft * 2;
	// float aspectRatio = (float)keyboardHeight / (float)keyboardWidth;
	// targetHeight = (int)floor(targetWidth * aspectRatio);
	keyboardZoomFactor = (float)(clientWidth - offsetLeft - offsetRight) / (float)keyboardWidth;
	uiZoom = (float)targetWidth / (float)clientWidth;

	// create Alpha Screenhot Mask
	int cB = 80;
	if (mask) {
		delete mask;
	}
	mask = new Mat4b(targetHeight, targetWidth, CV_8UC4);
	for (int i = 0; i < cB; i++) {
		int c = (int)floor((float)i / (float)cB * (float)192);
		Scalar color(c, c, c, 255);
		cv::rectangle(*mask, cv::Point((int)floor(i), i), cv::Point(targetWidth - (int)floor(i), targetHeight - i), color, CV_FILLED, 8, 0);
	}

	// keyboard graphics
	int keyShadowSize = 4;

	if (keyboardFx) {
		delete keyboardFx;
	}
	keyboardFx = new Mat4b(targetHeight, targetWidth, Vec4b(0, 0, 0, 0));
	
	vector<CorsairLedPosition>::iterator it;
	it = allKeys.begin();
	for (; it != allKeys.end(); ) {
		cv::Rect keyRect(
			(int)floor((float)(it->left*keyboardZoomFactor + offsetLeft)*uiZoom * offsetScaleX),
			(int)floor((float)(it->top*keyboardZoomFactor + offsetTop)*uiZoom* offsetScaleY),
			(int)floor((float)it->width*keyboardZoomFactor*uiZoom * offsetScaleX),
			(int)floor((float)it->height*keyboardZoomFactor*uiZoom* offsetScaleY));

		for (int i = keyShadowSize; i > 0; i--) {
			int c = (int)floor((float)(keyShadowSize - i) / (float)keyShadowSize * (float)192);
			Scalar color(0, 0, 0, c);
			cv::rectangle(*keyboardFx, cv::Rect(keyRect.x - i, keyRect.y - (int)floor((float)i / 2), keyRect.width + i * 2, keyRect.height + i * 2), color, CV_FILLED, 8, 0);
		}
		++it;
	}

	initialized = true;
}


FPScreenMirror::~FPScreenMirror()
{
}

int FPScreenMirror::getWindowBackgroundResource() {
	return IDB_WINDOW_BACKGROUND_SELECT;
}

void FPScreenMirror::setCaptureWindow(HWND currentProcess) {
	if (currentProcess != currentProcessHWND) {
		ReleaseDC(currentProcessHWND, captureWindowHDC);
		if (captureWindowHDC != NULL) {
			DeleteObject(captureWindowHDC);
		}
		currentProcessHWND = currentProcess;
		captureWindowHDC = GetDC(currentProcessHWND);
	}
}


bool FPScreenMirror::process() {

	if (!initialized) {
		reinitialize();
		return false;
	}
	
	if (!captureWindowHDC || !currentProcessHWND) {
		return false;
	}
	/*
	if (keyboardWidth < 1 || keyboardHeight < 1) {
		return false;
	}
	*/

	PerformanceStart();

	
	
	// get window bitmap as Mat screenshotMat
	RECT targetRect;
	GetWindowRect(currentProcessHWND, &targetRect);

	int clientWidth = targetRect.right - targetRect.left; // -offsetLeft - offsetRight;
	int clientHeight = targetRect.bottom - targetRect.top; // -offsetTop - offsetBottom;

	if ((targetRect.left < 0 && targetRect.top < 0 && targetRect.right < 0 && targetRect.bottom < 0) ||
		clientWidth < 0 || clientHeight < 0) {
		PerformanceStop();
		return false;
	}

	

	Mat* screenshotRaw = ImageFilterMat::hdc2mat(captureWindowHDC, 0, 0, clientWidth, clientHeight);
	if (screenshotRaw == NULL) {
		PerformanceStop();
		delete screenshotRaw;
		return false;
	}

	Mat4b screenshotMat = (Mat4b)*screenshotRaw;
	
	cv::Rect cropRect(cutLeft, cutTop, screenshotMat.cols - cutLeft - cutRight, screenshotMat.rows - cutTop - cutBottom);
	screenshotMat = Mat(screenshotMat, cropRect);
	
	blur(screenshotMat, screenshotMat, cv::Size(20, 20));
	resize(screenshotMat, screenshotMat, cv::Size(targetWidth, targetHeight), 2, 2, INTER_CUBIC);
	
	// ImageFilterMat::incSaturation(screenshotMat, 50, (float)0.7);


	LEDController::getInstance()->initializeFrame();

	
	Mat4b keyboardMat = Mat4b(*keyboardFx);

	
	
	// draw Keyboard
	
	vector<CorsairLedPosition>::iterator it = allKeys.begin();
	it = allKeys.begin();
	for (; it != allKeys.end(); ) {
		cv::Rect keyRect(
			(int)floor((float)(it->left*keyboardZoomFactor + offsetLeft)*uiZoom * offsetScaleX),
			(int)floor((float)(it->top*keyboardZoomFactor + offsetTop)*uiZoom* offsetScaleY),
			(int)floor((float)it->width*keyboardZoomFactor*uiZoom * offsetScaleX),
			(int)floor((float)it->height*keyboardZoomFactor*uiZoom* offsetScaleY));

	
		int overScanX = (int)keyRect.x - overscan;
		if (overScanX < 0) overScanX = 0;
		
		int overScanY = (int)keyRect.y - overscan;
		if (overScanY < 0) overScanY = 0;
		
		int overScanW = (int)keyRect.width + overscan * 2;
		if (overScanX + overScanW > keyboardMat.cols) overScanW = keyboardMat.cols - overScanX;
		
		int overScanH = (int)keyRect.height + overscan * 2;
		if (overScanY + overScanH > keyboardMat.rows) overScanH = keyboardMat.rows - overScanY;


		cv::Rect colorDetectionRect(overScanX, overScanY, overScanW, overScanH);

		try {
			Mat4b keyMat = Mat(screenshotMat, colorDetectionRect);
			resize(keyMat, keyMat, cv::Size(1, 1), 0, 0, INTER_CUBIC);
			Vec4b color = keyMat.at<Vec4b>(0, 0);

			LEDController::getInstance()->setKey(it->ledId, color[2], color[1], color[0]);
			cv::rectangle(keyboardMat, keyRect, Scalar(color[0], color[1], color[2], 255), CV_FILLED, 8, 0);
			cv::rectangle(keyboardMat, keyRect, Scalar(255, 255, 255, 128), 1, 8, 0);
			// cv::rectangle(keyboardMat, colorDetectionRect, Scalar(0, 0, 255, 128), 1, 8, 0);
		}
		catch (...) {
			// ups...
		}
		++it;
	}
	
	LEDController::getInstance()->updateFrame();
	
	// copy background to UI
	getBackgroundMat()->copyTo(drawUI);
	
	// ImageFilterMat::addAlphaMask(&screenshotMat, mask);
	
	
	ImageFilterMat::overlayImage(&drawUI, &screenshotMat, cv::Point(targetX, targetY));
	
	resize(keyboardMat, keyboardMat, cv::Size(targetWidth, targetHeight));
	ImageFilterMat::overlayImage(&drawUI, &keyboardMat, cv::Point(targetX, targetY));


	PerformanceDraw(getBackgroundMat()->cols - 130, 20);
	drawToWindow(&drawUI);

	// throwing away pointer, so opencv releases memory
	delete screenshotRaw;
	PerformanceStop();
	
	return true;
}


void FPScreenMirror::setUIDimensions(int x, int y, int w, int h) {
	targetX = x;
	targetY = y;
	targetWidth = w;
	targetHeight = h;
	reinitialize();
}

void FPScreenMirror::setOffset(RECT offset) {
	offsetTop = offset.top;
	offsetLeft = offset.left;
	offsetBottom = offset.bottom;
	offsetRight = offset.right;
	reinitialize();
}

void FPScreenMirror::setOffsetScale(float x, float y) {
	offsetScaleX = x;
	offsetScaleY = y;
	reinitialize();
}

void FPScreenMirror::setCutRect(int l, int t, int r, int b) {
	cutTop = t;
	cutLeft = l;
	cutBottom = b;
	cutRight = r;
}