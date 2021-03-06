#pragma once
#include "ScreenHotSpot.h"
#include "HSCastable.h"
#include "Resource.h"

class HSBackport : 
	public HSCastable 
{
private:
	int backportFinishedReleaseFrames = 0;
	int backportFinishedReleaseTreshold = 30;

	cv::Point bCenterPoint;
	int iconRadius = 0;
	int backPortDetectionX = 0, backPortDetectionY = 0, backPortDetectionW = 0, backPortDetectionH = 0;
	vector<CorsairLedPosition> allKeys;
	Mat4b mask;
	Mat4b effectColorsMat;
	Mat4b effectMat;
	Mat4b backPortMat, backPortMeasureMat;
	void createMask();
public:
	HSBackport();
	~HSBackport();
	int getType();
	void filterMat();
	void initialize();
	bool isCastable();
	
	bool isBackportActive();
	int getBackportProgress();
	Mat getFilterdMatForUI();
	Mat getOriginalMatForUI();
	bool hasExclusiveEffect();
	void doExclusiveEffect();
	void setBackportDetectionCaptureCoordinates(int x, int y, int width, int height);
	
};

