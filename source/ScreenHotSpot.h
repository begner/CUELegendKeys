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
	int uiX = 0; // pbx
	int uiY = 0; // pby
	int border = 0; // brd
	
	int deleteFMX = 0;
	int deleteFMY = 0;
	int deleteFMW = 0;
	int deleteFMH = 0;


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
	static unsigned int __stdcall doInitializeFrame(void *p_this);

	
	/*
	uint animationTick = 0;
	Vec3b primaryColor;
	Vec3b secondaryColor;
	Vec3b currentColor;
	vector< Vec6i > barColorCoordinates;
	CorsairLedId lid;
	*/
	
/*
	
	int t = 0;
	int pColorX = 0;
	int pColorY = 0;
	int sColorX = 0;
	int sColorY = 0;
	int cColorX = 0;
	int cColorY = 0;
	int percent = 0;
	void setBitmapX(int x);
	void setBitmapY(int y);
	HBITMAP image;
	void calculateCurrentColor();
	Vec3b getInternalAnimationColor();
	Vec3b getColorByTick();
	*/

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
	void setCaptureCoordinates(int x, int y, int w, int h);
	int getCaptureX(bool borderCorrect = false);
	int getCaptureY(bool borderCorrect = false);
	int getCaptureWidth(bool borderCorrect = false);
	int getCaptureHeight(bool borderCorrect = false);
	int getUiX();
	int getUiY();
	int getBorder();
	void setBorder(int b);

	void addPreviewColorCoordinates(int x, int y, int width = 10, int height = 10);
	vector<cv::Rect>* getPreviewColors();

	void setUiCoordinates(int x, int y);
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

	void tick();
	int getCurrentTick();
	virtual void updateKey();

	virtual int getUpdateFrame();
	void copyMats();
	virtual void initialize();
	virtual void initializeFrame();
	HANDLE initializeFrameInThread();

	virtual bool hasExclusiveEffect();
	virtual void doExclusiveEffect();
	/*
	
	int getBitmapX();
	int getBitmapY();
	int getBorder();
	HBITMAP getBitmap();
	void setBitmap(HBITMAP image);
	ScreenHotSpot();
	virtual ~ScreenHotSpot();
	void setPrimaryColorCoordinates(int x, int y);
	void setSecondaryColorCoordinates(int x, int y);
	void setCurrentColorCoordinates(int x, int y);
	int getPrimaryColorX();
	int getPrimaryColorY();
	int getSecondaryColorX();
	int getSecondaryColorY();
	int getCurrentColorX();
	int getCurrentColorY();
	void setType(int type);
	
	void setUiCoordinates(int x, int y);
	
	void setSecondaryColor(int r, int g, int b);
	void setSecondaryColor(Vec3b color);
	void setPrimaryColor(int r, int g, int b);
	void setPrimaryColor(Vec3b color);
	void setCurrentColor(Vec3b color);
	void setCurrentColor(int r, int g, int b);
	void setKey(CorsairLedId ledId);
	void addBarColorCoordinates(CorsairLedId key, int x, int y);
	size_t getBarColorAmount();
	int getBarColorX(int count);
	int getBarColorY(int count);
	Vec3b getCurrentColor();
	void setBarColor(int count, Vec3b color);
	
	void tick();
	Vec3b getAnimationColor();
	void setPercent(int p);
	*/
};

