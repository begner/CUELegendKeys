#pragma once
#include "ScreenHotSpot.h"
class HSBar :
	public ScreenHotSpot
{
private: 
	int determinePercentage();
	Mat4b colorBarMat;
public:
	HSBar();
	~HSBar();
	int getType();
	void filterMat();
	Vec4b getCurrentColor(int index = 0);
	int getMaxTick();
	void updateKey();
};

