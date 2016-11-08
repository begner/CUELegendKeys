#pragma once
#include "ScreenHotSpot.h"
#include <algorithm>

#include <cstdlib> // for srand, rand
#include <ctime>  

class HSChamp :
	public ScreenHotSpot
{
private:
	int champKeyWidth = 0;
	int champKeyHeight = 0;
	vector<CorsairLedPosition> champKeys;

	int champOffsetX = 0;
	int champOffsetY = -2;

	Mat4b champImageColorTable;
	Mat mask;

public:
	HSChamp();
	~HSChamp();
	void createMask();
	int getType();
	void filterMat();
	Vec4b getCurrentColor(int index = 0);
	int getMaxTick();
	Mat getFilterdMatForUI();
	Mat getOriginalMatForUI();
	void updateKey();
	int getUpdateFrame();
	void initialize();
};

