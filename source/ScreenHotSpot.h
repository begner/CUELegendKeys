#pragma once

#include <Windows.h>
#include <vector>
#include "LEDController.h"
#include "CUESDK.h"
#include "ImageFilterMat.h"
#include "opencv2/core/core.hpp"

class ScreenHotSpot
{
private:
	int captureX = 0;
	int captureY = 0;
	int captureW = 0;
	int captureH = 0;

	int captureOffsetX = 0;
	int captureOffsetY = 0;

	int uiX = 0; // pbx
	int uiY = 0; // pby
	int uiW = 0; // pbx
	int uiH = 0; // pby
	int border = 0; // brd
	
	int deleteFMX = 0;
	int deleteFMY = 0;
	int deleteFMW = 0;
	int deleteFMH = 0;

	int currentColorCoordX = -1;
	int currentColorCoordY = -1;

	bool cacheIsDirty = true;

	vector<cv::Rect> previewColors;
	vector<Mat4b> previewColorsMat;

	vector<CorsairLedId> keys;
	
	int animationTick = 0;

	Mat4b* screenshotMat;
	Mat4b* uiMatP;
	Mat4b originalMat;
	Mat4b originalMatRespectBorders;
	Mat4b filteredMat;
	
	virtual void filterMat();
	// static unsigned int __stdcall doInitializeFrame(void *p_this);
		
public:
	ScreenHotSpot();
	virtual ~ScreenHotSpot();
	
	static const int TYPE_UNKNOWN = 0;
	static const int TYPE_CASTABLE = 1;
	static const int TYPE_BAR = 2;
	static const int TYPE_BACKPORT = 3;
	static const int TYPE_CHAMP = 4;
	static const int TYPE_PASSIVE = 5;

	virtual int getType();
	void setUiMatOffset(int x, int y);
	void setCaptureCoordinates(int x, int y, int w, int h);
	int getCaptureX(bool borderCorrect = false, bool rawMode = false);
	int getCaptureY(bool borderCorrect = false, bool rawMode = false);
	int getCaptureWidth(bool borderCorrect = false);
	int getCaptureHeight(bool borderCorrect = false);
	int getUiX();
	int getUiY();
	int getUiWidth();
	int getUiHeight();
	int getBorder();
	void setBorder(int b);

	void addPreviewColorCoordinates(int x, int y, int width = 10, int height = 10);
	vector<cv::Rect>* getPreviewColors();

	void setUiCoordinates(int x, int y, int w, int h);
	void deleteFilteredMatRect(int x, int y, int width, int height);

	void setUIMat(Mat4b* uiMat);
	void setScreenshotMat(Mat4b* screenShot);

	Mat4b* getScreenshotMat();
	Mat4b* getOriginalMat();
	Mat4b* getOriginalMatRespectBorders();
	Mat4b* getFilteredMat();
	Mat4b* getPreviewColorMat(int index = 0);

	void addKey(CorsairLedId ledId);
	CorsairLedId getKey(int index = 0);
	vector<CorsairLedId>* getKeys();

	virtual Vec4b getCurrentColor(int index = 0) = 0;
	virtual int getMaxTick() = 0;
	virtual void resetResources();

	void tick();
	int getCurrentTick();
	virtual void updateKey();

	virtual int getUpdateFrame();
	void copyMats();
	virtual void initialize();
	virtual void initializeFrame();
	// HANDLE initializeFrameInThread();

	virtual bool hasExclusiveEffect();
	virtual void doExclusiveEffect();

	bool isCacheDirty();
	void setCacheDirtyState(bool state);

	void setCurrentColorCoord(int x, int y);

	Mat getFilterdMatForUI();

	Mat getOriginalMatForUI();

};

