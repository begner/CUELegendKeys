#pragma once
#include "HSCastable.h"
#include <algorithm>

#include <cstdlib> // for srand, rand
#include <ctime>  

class HSPassive :
	public HSCastable
{
private:
	int champKeyWidth = 0;
	int champKeyHeight = 0;
	vector<CorsairLedPosition> champKeys;

	int maxDiff = 6;
	int lastTargetDiffX = maxDiff / 2;
	int lastTargetDiffY = maxDiff / 2;
	int targetDiffX = 0;
	int targetDiffY = 0;
	int remainingTicks = 0;
	void reinitAnimation();


	Mat champImageColorTable;
	Mat* mask;

public:
	HSPassive();
	~HSPassive();
	int getType();
	void filterMat();
	Vec4b getCurrentColor(int index = 0);
	void updateKey();
	void initialize();
	int getMaxTick();

};


